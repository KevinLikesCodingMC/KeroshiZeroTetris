//
// Created by keroshi on 2026/5/2.
//

#include "tetris.h"

Tetris tetris;
int x, y, r;

void board1() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[2] = 0b0000010000;
	tetris.b[1] = 0b1110001111;
	tetris.b[0] = 0b1111011111;
	x = 6; y = 2; r = 2;
}

void board2() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[2] = 0b0000010000;
	tetris.b[1] = 0b1110001111;
	tetris.b[0] = 0b1110011111;
	x = 6; y = 2; r = 2;
}

void board3() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[4] = 0b0001100000;
	tetris.b[3] = 0b0001000000;
	tetris.b[2] = 0b1111011111;
	tetris.b[1] = 0b1111001111;
	tetris.b[0] = 0b1111011111;
	x = 6; y = 2; r = 3;
}

void board4() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[4] = 0b0001100000;
	tetris.b[3] = 0b0001000000;
	tetris.b[2] = 0b1111011111;
	tetris.b[1] = 0b1111001111;
	tetris.b[0] = 0b1111001111;
	x = 6; y = 2; r = 3;
}
void board5() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[4] = 0b0001100000;
	tetris.b[3] = 0b0001000000;
	tetris.b[2] = 0b1111011111;
	tetris.b[1] = 0b1111001111;
	tetris.b[0] = 0b1110001111;
	x = 6; y = 2; r = 3;
}

void board6() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[2] = 0b0000000001;
	tetris.b[1] = 0b0000000001;
	tetris.b[0] = 0b1111111101;
	x = 2; y = 2; r = 1;
}

void board7() {
	tetris = Tetris();
	tetris.cur = Piece :: T;
	tetris.b[2] = 0b0000000000;
	tetris.b[1] = 0b0000000001;
	tetris.b[0] = 0b1111111101;
	x = 2; y = 2; r = 1;
}

void test() {
	if (! tetris.is_legal(x, y, r)) {
		std :: cout << "illegal" << std :: endl;
		return;
	}

	Spin spin = tetris.spin_check(x, y, r);
	if (spin == Spin :: None) {
		std :: cout << "None" << std :: endl;
	}
	if (spin == Spin :: Mini) {
		std :: cout << "Mini Spin" << std :: endl;
	}
	if (spin == Spin :: Full) {
		std :: cout << "Full Spin" << std :: endl;
	}
}

int main() {
	board6();
	test();
	return 0;
}