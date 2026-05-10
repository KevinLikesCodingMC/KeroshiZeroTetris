//
// Created by keroshi on 2026/5/1.
//

#include <tetris.h>

#include <thread>
#include <chrono>
#include <cstring>

int a[10][30], b[10][30];

void print() {
	std :: cout << "\033[2J\033[H" << std :: flush;
	for (int y = 24; y >= 0; y --) {
		for (int x = 0; x < 10; x ++) {
			std :: cout << b[x][y];
		}
		std :: cout  << std :: endl;
	}
}

int main() {
	Tetris tetris;
	a[5][5] = 2;
	tetris.b[5] |= 1 << 5;
	std :: vector actions = tetris.legal();
	for (int action : actions) {
		if (action == 0) continue;
		std :: memcpy(b, a, sizeof b);
		auto [x, y, r] = Action :: decode(action);
		int piece = static_cast<int>(tetris.cur);
		for (int i = 0; i < 4; i ++) {
			int X = x + Mino :: minos[piece][r][i][0];
			int Y = y + Mino :: minos[piece][r][i][1];
			b[X][Y] = 1;
		}
		print();
		std :: this_thread :: sleep_for(std :: chrono :: milliseconds(500));
	}
	return 0;
}
