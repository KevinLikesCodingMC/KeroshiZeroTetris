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
	int top = 30;
	for (int y = 29; y >= 0; y --) {
		if (t.b[y]) break;
		top = y;
	}
	return (20 - top) * 0.7f;
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

	Predictor predictor(
		model_path,
		true, true
	);

	Tetris tetris;
	TetrisRender :: render(tetris);

	tetris.record_seq = true;
	tetris.r_seq = std :: make_shared<std :: string>();
	tetris.refill();

	std :: string keys = "";

	while (! is_end(tetris)) {
		MCTS<Tetris> mcts;
		float fst;

		{
			Tetris g = tetris;
			mcts.root_game = tetris;
			mcts.root_game.depleted = true;
			mcts.root_game.record_seq = false;

			auto [V, P] = predictor.predict(g);

			fst = V + get_ex_V(tetris);
		}

		for (int K = 0; K < simu; K ++) {
			if (K == 1) {
				mcts.noise();
			}

			Tetris g = mcts.playout_select();

			auto [V, P_t] = predictor.predict(g);

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
				int n = g.legal().size();

				if (n > 128) {
					mcts.playout_back(Q, std :: vector<float>(1.f / n));
				}
				else {
					auto P = P_t;
					P.resize(n);
					mcts.playout_back(Q, P);
				}
			}
		}

		int id = mcts.get_best();
		int u = mcts.root -> a[id];

		keys += tetris.get_keys(u);
		tetris.step(u);

		TetrisRender :: render(tetris);
	}

	std :: cout << * tetris.r_seq << std :: endl;
	std :: cout << keys << std :: endl;

	return 0;
}
