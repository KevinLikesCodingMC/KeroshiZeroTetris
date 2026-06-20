//
// Created by keroshi on 2026/5/23.
//

#include "include/render.h"

namespace TetrisRender {
	void render(const Tetris & tetris) {
		std :: vector screen(30, std :: string(50, ' '));

		for (int i = 0; i < 25; i ++) {
			int y = 27 - i;
			screen[y][1] = screen[y][12] = '#';
			for (int x = 0; x < 10; x ++) {
				int o = tetris.b[i] >> x & 1;
				if (o) screen[y][x + 2] = 'o';
			}
		}
		for (int x = 1; x <= 12; x ++) {
			screen[28][x] = '#';
		}

		if (tetris.last_placement.piece != Piece :: EMPTY
			&& tetris.last_placement.clear == 0) {
			int piece = static_cast<int>(tetris.last_placement.piece);
			int x = tetris.last_placement.x;
			int y = tetris.last_placement.y;
			int r = tetris.last_placement.r;
			for (int i = 0; i < 4; i ++) {
				int X = x + Mino :: minos[piece][r][i][0];
				int Y = y + Mino :: minos[piece][r][i][1];
				screen[27 - Y][X + 2] = '*';
			}
		}

		std :: string output = "\033[2J\033[H";
		for (auto & line : screen) {
			output += line + '\n';
		}
		std :: cout << output << std :: endl;
	}
}
