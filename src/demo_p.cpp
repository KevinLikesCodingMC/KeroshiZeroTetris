//
// Created by Administrator on 2026/7/13.
//

#include "include/predict.h"
#include "include/converter.h"
#include "include/mcts.h"
#include "include/render.h"
#include "include/tetris.h"

bool is_end(const Tetris & t) {
	return t.game_over || t.pieces >= 35;
}

float get_V(const Tetris & t) {
	return t.attack;
}

void mcts_single(MCTS<Tetris> & mcts, Predictor & predictor, float fst) {
	Tetris g = mcts.playout_select();

	auto [V, P] = predictor.predict(g);

	float Q = - fst;
	if (is_end(g)) {
		Q += get_V(g);
	}
	else {
		Q += get_V(g) + V;
	}

	if (g.is_leaf()) {
		mcts.playout_back(Q, {});
	}
	else {
		auto P_softmax = mcts.softmax(g, P);
		mcts.playout_back(Q, P_softmax);
	}
}

void demo_p(const std :: string & model_path, int simu) {
	Predictor predictor(
		model_path,
		true, true
	);

	Tetris tetris;
	tetris.rest_pieces = 35;

	while (tetris.rest_pieces > 0) {
		if (is_end(tetris)) break;

		MCTS<Tetris> mcts;
		mcts.C = 5;

		float fst;

		{
			Tetris g = tetris;
			mcts.root_game = tetris;
			mcts.root_game.depleted = true;
			mcts.root_game.record_seq = false;

			auto [V, P] = predictor.predict(g);

			std :: cout << V << std :: endl;

			fst = get_V(g) + V;
		}

		for (int K = 0; K < simu; K ++) mcts_single(mcts, predictor, fst);

		int id = mcts.get_best();
		int u = mcts.root -> a[id];

		tetris.step(u);

		auto mcts_N = mcts.root -> N;
		std :: sort(mcts_N.begin(), mcts_N.end());

		for (int x : mcts_N) std :: cout << x << ' ';
		std :: cout << std :: endl;

		auto mcts_Q = mcts.root -> Q;
		std :: sort(mcts_Q.begin(), mcts_Q.end());

		for (float x : mcts_Q) std :: cout << x << ' ';
		std :: cout << std :: endl;

		std :: cout << std :: endl;
	}

	std :: cout << tetris.attack << std :: endl;
}

int main(int argc, char * argv []) {
	if (argc < 2) {
		std :: cout << "Usage: " << argv[0]
					<< " <model path>"
					<< " [simu number: 100]"
					<< std :: endl;
		return 1;
	}

	std :: string model_path = argv[1];

	int simu_number = 100;

	try {
		if (argc > 2) simu_number = std :: stoi(argv[2]);
	}
	catch (const std :: exception & e) {
		Message :: log(Message :: ERROR, true,
			e.what()
		);
		std :: exit(EXIT_FAILURE);
	}

	int simu = simu_number;

	demo_p(model_path, simu);

	return 0;
}
