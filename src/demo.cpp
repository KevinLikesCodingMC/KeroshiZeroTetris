//
// Created by keroshi on 2026/6/6.
//

#include "include/predict.h"
#include "include/converter.h"
#include "include/mcts.h"
#include "include/render.h"
#include "include/tetris.h"

/*
demo
<model path>
[simu number]
*/

bool is_end(const Tetris & t) {
	return t.game_over || t.pieces >= 35;
}

float get_V(const Tetris & t) {
	return t.attack;
}

float get_ex_V(const Tetris & t) {
	return 0;
}

void mcts_single(MCTS<Tetris> & mcts, Predictor & predictor, float fst) {
	Tetris g = mcts.playout_select();

	auto [V, P] = predictor.predict(g);

	float Q = get_ex_V(g) - fst;
	if (is_end(g)) {
		Q += get_V(g);
	}
	else {
		Q += V;
	}

	if (g.is_leaf()) {
		mcts.playout_back(Q, {});
	}
	else {
		auto P_softmax = mcts.softmax(g, P);
		mcts.playout_back(Q, P_softmax);
	}
}

void demo_cycle(const std :: string & model_path, int simu) {
	Predictor predictor(
		model_path,
		true, true
	);

	Tetris tetris;
	TetrisRender :: render(tetris);

	tetris.record_seq = true;
	tetris.r_seq = std :: make_shared<std :: string>();
	tetris.refill();

	std :: string keys;

	int step = 100;
	while (step && ! is_end(tetris)) {
		step --;

		if (tetris.pieces == 7) {
			tetris.pieces = 0;
			tetris.attack = 0;
		}

		MCTS<Tetris> mcts(10);
		float fst;

		{
			Tetris g = tetris;
			mcts.root_game = tetris;
			mcts.root_game.depleted = true;
			mcts.root_game.record_seq = false;

			auto [V, P] = predictor.predict(g);

			fst = V + get_ex_V(tetris);
		}

		mcts_single(mcts, predictor, fst);
		mcts.noise();

		for (int K = 0; K < simu; K ++) mcts_single(mcts, predictor, fst);

		int id = mcts.get_best();
		int u = mcts.root -> a[id];

		keys += tetris.get_keys(u);
		tetris.step(u);

		TetrisRender :: render(tetris);
	}

	std :: cout << * tetris.r_seq << std :: endl;
	std :: cout << keys << std :: endl;
}

void demo_cycle_V(const std :: string & model_path, int simu) {
	Predictor predictor(
		model_path,
		true, true
	);

	Tetris tetris;
	TetrisRender :: render(tetris);

	tetris.record_seq = true;
	tetris.r_seq = std :: make_shared<std :: string>();
	tetris.refill();

	std :: string keys;

	int step = 100;
	while (step && ! is_end(tetris)) {
		step --;

		if (tetris.pieces == 7) {
			tetris.pieces = 0;
			tetris.attack = 0;
		}

		MCTSV<Tetris> mcts(10, 5, 100);
		float fst;

		{
			Tetris g = tetris;
			mcts.root_game = tetris;
			mcts.root_game.depleted = true;
			mcts.root_game.record_seq = false;

			auto [V, P] = predictor.predict(g);

			fst = V + get_ex_V(tetris);
		}

		float base = 1;
		for (int sum = simu; sum > 0;) {
			base = std :: min(256.f, base * 1.5f);
			int batch = static_cast<int>(base);
			sum -= batch;
			auto gs = mcts.playout_select(batch);

			auto [V, P] = predictor.predict_batch(gs);

			std :: vector<float> Q(batch);
			for (int I = 0; I < batch; I ++) {
				auto & g = gs[I];
				Q[I] = get_ex_V(g) - fst;
				if (is_end(g)) {
					Q[I] += get_V(g);
				}
				else {
					Q[I] += V[I];
				}
				if (! g.is_leaf()) {
					P[I] = mcts.softmax(g, P[I]);
				}
			}

			mcts.playout_back(Q, P);
		}

		int id = mcts.get_best();
		int u = mcts.root -> a[id];

		keys += tetris.get_keys(u);
		tetris.step(u);

		TetrisRender :: render(tetris);
	}

	std :: cout << * tetris.r_seq << std :: endl;
	std :: cout << keys << std :: endl;
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

	demo_cycle_V(model_path, simu);

	return 0;
}
