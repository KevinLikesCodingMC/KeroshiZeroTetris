//
// Created by Administrator on 2026/7/8.
//

#include "include/tetris.h"

int main() {
	for (int _ = 0; _ < 3; _ ++) {
		Tetris tetris(114);
		for (int i = 0; i < 5; i ++) {
			int p = static_cast<int>(tetris.nxt[i]);
			std :: cout << p;
		}
		std :: cout << std :: endl;
	}
	std :: cout << std :: endl;

	for (int _ = 0; _ < 3; _ ++) {
		Tetris tetris;
		for (int i = 0; i < 5; i ++) {
			int p = static_cast<int>(tetris.nxt[i]);
			std :: cout << p;
		}
		std :: cout << std :: endl;
	}

	return 0;
}