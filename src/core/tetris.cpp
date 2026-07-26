//
// Created by keroshi on 2026/7/24.
//

#include "include/core/tetris.hpp"
#include "include/core/mino.hpp"


TetrisEnv :: TetrisEnv() {

	// Piece Initialization
	cur = hold = Piece :: EMPTY;
	for (int i = 0; i < Board :: NEXT; i ++) {
		nxt[i] = Piece :: EMPTY;
	}
	bag_piece = 7;
	bag_use = 0;

	// Board Initialization
	for (int i = 0; i < Board :: HEIGHT; i ++) {
		board[i] = 0;
	}

	// Meta Initialization
	placement = {
		Piece :: EMPTY,
		0, 0, 0,
		Spin :: None,
		0,
		false
	};
	combo = b2b = 0;
	game_over = false;
	can_hold = true;
	pieces = rest_pieces = 0;
	attack = 0;

}

void TetrisEnv :: reset() {
	* this = TetrisEnv();
}

void TetrisEnv :: set_board(int x, int y, bool o) {
	if (x < 0 || x >= Board :: WIDTH) return;
	if (y < 0 || y >= Board :: HEIGHT) return;
	if (o) {
		board[y] |= 1 << x;
	}
	else {
		board[y] &= ~ (1 << x);
	}
}

bool TetrisEnv :: get_board(int x, int y) {
	if (x < 0 || x >= Board :: WIDTH || y < 0) return true;
	if (y >= Board :: HEIGHT) return false;
	return board[y] >> x & 1;
}

bool TetrisEnv :: is_occupied(int x, int y) {
	if (x < 0 || x >= Board :: WIDTH) return true;
	if (y < 0 || y >= Board :: HEIGHT) return true;
	return get_board(x, y);
}

bool TetrisEnv :: is_grounded(int x, int y, int r) {
	return ! is_legal(x, y - 1, r);
}

bool TetrisEnv :: is_legal(int x, int y, int r) {
	for (int i = 0; i < 4; i ++) {
		auto [X, Y] = Mino :: get(cur, i, r, x, y);
		if (is_occupied(X, Y)) return false;
	}
	return true;
}

std :: pair<int, int> TetrisEnv :: get_spawn() {
	int x = Board :: SPAWN_X;
	int y = Board :: SPAWN_Y;

	/*
		Clutch Clear
		Reference: https://tetrio.wiki.gg/wiki/Mechanics
	*/
	if (placement.clear > 0) {
		for (; y < Board :: HEIGHT; y ++) {
			if (is_legal(x, y, 0)) break;
		}
	}

	return {x, y};
}
