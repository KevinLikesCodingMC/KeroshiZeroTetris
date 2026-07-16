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

			std :: cout << tetris.attack << ' ' << V << std :: endl;

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

void demo_p(const std :: string & model_path, int simu, const std :: string & output_path) {

	std :: ofstream ofs(output_path);

	int height = 10;
	ofs << height << std :: endl;

	Predictor predictor(
		model_path,
		true, true
	);

	Tetris tetris;
	tetris.rest_pieces = 12;

	while (tetris.rest_pieces > 0) {
		if (is_end(tetris)) break;

		std :: vector<std :: string> tb(height);

		for (int y = height - 1; y >= 0; y --) {
			for (int x = 0; x < 10; x ++) {
				int o = tetris.b[y] >> x & 1;
				ofs << "_X"[o];
				tb[y] += "_X"[o];
			}
			ofs << std :: endl;
		}

		MCTS<Tetris> mcts;
		mcts.C = 7;

		float fst;

		{
			Tetris g = tetris;
			mcts.root_game = tetris;
			mcts.root_game.depleted = true;
			mcts.root_game.record_seq = false;

			auto [V, P] = predictor.predict(g);

			fst = get_V(g) + V;
		}

		simu ++;
		for (int K = 0; K < simu; K ++) mcts_single(mcts, predictor, fst);

		int id = mcts.get_best();
		int u = mcts.root -> a[id];

		int piece = static_cast<int>(tetris.cur);
		tetris.step(u);

		int num = 3;
		ofs << num << std :: endl;

		int n = mcts.root -> n;
		std :: vector<int> idx(n);
		for (int i = 0; i < n; i ++) idx[i] = i;

		auto mcts_N = mcts.root -> N;
		std :: sort(idx.begin(), idx.end(), [&] (int x, int y) {
			return mcts_N[x] > mcts_N[y];
		});

		for (int k = 0; k < num; k ++) {
			int i = idx[k];
			double p = (double) mcts_N[i] / mcts.root -> Sum_N;
			ofs << (p * 100) << '%' << std :: endl;
			int U = mcts.root -> a[i];
			if (U == 0) {
				ofs << "Hold" << std :: endl;
			}
			else {
				std :: vector<std :: string> b(height);
				for (int y = height - 1; y >= 0; y --) {
					b[y] = tb[y];
				}

				auto [x, y, r] = Action :: decode(U);
				char pc = "_IJLOSTZ"[piece];

				for (int i = 0; i < 4; i ++) {
					int X = x + Mino :: minos[piece][r][i][0];
					int Y = y + Mino :: minos[piece][r][i][1];
					b[Y][X] = pc;
				}

				ofs << "board" << std :: endl;
				for (int y = height - 1; y >= 0; y --) {
					ofs << b[y] << std :: endl;
				}

			}
		}

	}

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

	std :: string output_path = "demo.out";
	if (argc > 3) output_path = argv[3];

	int simu = simu_number;

	demo_p(model_path, simu, output_path);

	return 0;
}
