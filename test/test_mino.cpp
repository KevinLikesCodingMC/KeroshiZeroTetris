//
// Created by keroshi on 2026/4/30.
//

#include "include/mino.h"

#include <iostream>
#include <thread>
#include <chrono>

int a[5][5];

int main() {
	int r = 0;
	for (int piece = 1; piece <= 7; piece ++) {
		for (int _ = 0; _ < 8; _ ++) {
			std :: cout << "\033[2J\033[H" << std :: flush;
			for (int x = 0; x < 5; x ++)
				for (int y = 0; y < 5; y ++)
					a[x][y] = 0;
			for (int i = 0; i < 4; i ++) {
				int x = 3 + Mino :: minos[piece][r][i][0];
				int y = 3 + Mino :: minos[piece][r][i][1];
				a[x][y] = 1;
			}
			for (int y = 4; y >= 0; y --) {
				for (int x = 0; x < 5; x ++) {
					std :: cout << a[x][y];
				}
				std :: cout  << std :: endl;
			}
			std :: this_thread :: sleep_for(std :: chrono :: milliseconds(500));
			r = (r + 1) & 3;
		}
	}
	return 0;
}

