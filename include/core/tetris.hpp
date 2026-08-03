//
// Created by keroshi on 2026/7/24.
//

#ifndef KEROSHIZEROTETRIS_TETRIS_HPP
#define KEROSHIZEROTETRIS_TETRIS_HPP

#include "def.hpp"
#include "kick.hpp"
#include <utility>
#include <vector>

struct Placement {
	Piece piece;
	int x, y, r;
	Spin spin;
	int clear;
	bool pc;

	bool b2b() const;
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

	static constexpr int hold() {
		return 0;
	}
	static constexpr int place(int x, int y, int r) {
		return (x << 7 | y << 2 | r) + 1;
	}
	static constexpr ActionDecode decode(int u) {
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
	void set_board_row(int y, uint16_t x);

	Piece get_cur();
	void set_cur(Piece piece);
	Piece get_hold();
	Piece get_next(int x);

	std :: vector<int> get_gen_actions();
	std :: vector<int> get_player_actions();
	std :: vector<int> get_actions();

	bool step_player(int action);
	bool step_gen(int action);
	bool step(int action);

	bool is_over();
	bool is_player();

private:

	Piece cur, nxt[5]{}, hold;

	uint16_t board[30]{};

	Placement placement{};
	int combo, b2b;
	bool game_over, can_hold;
	int pieces; int rest_pieces;
	float attack;

	int bag_piece; uint8_t bag_use;

	bool is_occupied(int x, int y);
	bool is_legal(int x, int y, int r);
	bool is_grounded(int x, int y, int r);

	bool is_actions_cache;
	std :: vector<int> actions_cache;

	std :: pair<int, int> get_spawn();

	SRSP :: kick_decode get_kick(int x, int y, int r, SRSP :: Rotation rotation);
	SRSP :: kick_decode_id get_kick_id(int x, int y, int r, SRSP :: Rotation rotation);

	std :: pair<int, int> das_d(int x, int y, int r);

	void calc_actions();

	void roll();
	bool use_hold();
	bool place(int x, int y, int r);

	void check_game_over();
	Spin check_spin(int x, int y, int r);
	Spin check_tspin(int x, int y, int r);
	uint8_t check_tspin_kicks(int x, int y, int r);
};


#endif //KEROSHIZEROTETRIS_TETRIS_HPP
