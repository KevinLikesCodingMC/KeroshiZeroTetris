//
// Created by keroshi on 2026/5/20.
//

#include "include/mcts.h"
#include "include/tetris.h"
#include "include/render.h"

int main() {
	Tetris tetris;

	for (int step = 0; step < 100; step ++) {
		MCTS<Tetris, 20.f> mcts;
		mcts.root_game = tetris;
		mcts.root_game.depleted = true;

		for (int _ = 0; _ < 10000; _ ++) {
			Tetris g = mcts.playout_select();
			float V = g.attack * 100;

			if (g.is_over()) V -= 10 - g.pieces + tetris.pieces;

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
