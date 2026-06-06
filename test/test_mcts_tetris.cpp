//
// Created by keroshi on 2026/5/20.
//

#include "include/mcts.h"
#include "include/tetris.h"
#include "include/render.h"

float score(Tetris & g) {
	float V = 0;
	V += g.attack * 10;
	int top = 30;
	for (int y = 29; y >= 0; y --) {
		if (g.b[y]) break;
		top = y;
	}
	V += (20 - top) * 0.3f;
	return V;
}

int main() {
	Tetris tetris;

	for (int step = 0; step < 100; step ++) {
		float fst = score(tetris);
		MCTS<Tetris> mcts;
		mcts.root_game = tetris;
		mcts.root_game.depleted = true;

		for (int _ = 0; _ < 1000; _ ++) {
			Tetris g = mcts.playout_select();
			float V = score(g) - fst;

			if (g.is_leaf()) {
				mcts.playout_back(V, {});
			}
			else {
				int n = g.legal().size();
				std :: vector P(n, 1.f / n);
				mcts.playout_back(V, P);
			}
		}

		int id = mcts.get_best();
		int u = mcts.root -> a[id];
		tetris.step(u);
		TetrisRender :: render(tetris);
		std :: cout << tetris.attack << std :: endl;
		if (tetris.is_over()) break;
	}

	return 0;
}
