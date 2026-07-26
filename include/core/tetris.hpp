//
// Created by keroshi on 2026/7/24.
//

#ifndef KEROSHIZEROTETRIS_TETRIS_HPP
#define KEROSHIZEROTETRIS_TETRIS_HPP

#include "def.hpp"
#include <utility>

struct Placement {
	Piece piece;
	int x, y, r;
	Spin spin;
	int clear;
	bool pc;
};

struct Action {
	/*
		Hold: 0
		Place: (x << 7 | y << 2 | r) + 1
		y [0, 32)
		x [0, 12)
		Action [0, 1537)
	*/
	static constexpr int SIZE = 1537;

	struct ActionDecode {
		int x, y, r;
	};

	static int hold() {
		return 0;
	}
	static int place(int x, int y, int r) {
		return (x << 7 | y << 2 | r) + 1;
	}
	static ActionDecode decode(int u) {
		if (u <= 0 || u >= SIZE) return {0, 0, 0};
		u --;
		return {
			u >> 7,
			u >> 2 & 31,
			u & 3
		};
	}
};

class TetrisEnv {
public:
	TetrisEnv();
	void reset();

	void set_board(int x, int y, bool o);
	bool get_board(int x, int y);

	int get_cur();
	bool is_over();
	float get_V();

private:

	Piece cur, nxt[5], hold;

	uint16_t board[30];

	Placement placement;
	int combo, b2b;
	bool game_over, can_hold;
	int pieces; int rest_pieces;
	float attack;

	int bag_piece; uint8_t bag_use;

	bool is_occupied(int x, int y);
	bool is_legal(int x, int y, int r);
	bool is_grounded(int x, int y, int r);
	std :: pair<int, int> get_spawn();

};


#endif //KEROSHIZEROTETRIS_TETRIS_HPP
