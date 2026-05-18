//
// Created by keroshi on 2026/4/27.
//

#ifndef KEROSHIZEROTETRIS_TETRIS_H
#define KEROSHIZEROTETRIS_TETRIS_H

#include "combo.h"
#include "def.h"
#include "mino.h"
#include "kick.h"

#include <random>
#include <queue>

struct Placement {
	Piece piece;
	int x, y, r;
	Spin spin;
	int clear;
	bool pc;
};

struct Action {
	struct ActionDecode {
		int x, y, r;
	};
	static int hold() {
		return 0;
	}
	static int place(int x, int y, int r) {
		return (y << 6 | x << 2 | r) + 1;
	}
	static ActionDecode decode(int u) {
		if (u <= 0) return {0, 0, 0};
		u --;
		return {
			u >> 2 & 15,
			u >> 6,
			u & 3
		};
	}
};

struct Tetris {
	// 7 bag
	Piece bag[7]; int bag_pos;
	Piece cur, nxt[5], hold;

	uint16_t b[30]{};

	int combo, b2b;

	bool game_over, can_hold;

	int pieces; float attack;

	Placement last_placement;

	ComboFunc combo_func;

	Tetris() {
		bag_pos = 0;
		for (int i = 0; i < 7; i ++) {
			bag[i] = static_cast<Piece>(i + 1);
		}

		hold = Piece :: EMPTY;
		cur = get_next();
		for (int i = 0; i < 5; i ++) {
			nxt[i] = get_next();
		}

		combo = 0;
		b2b = 0;
		game_over = false;
		can_hold = true;
		pieces = 0;
		attack = 0;

		last_placement = {
			Piece :: EMPTY, 0, 0, 0,
			Spin :: None, 0, false
		};

		// default
		combo_func = KeroshiCombo :: stage1;
	}

	Piece get_next() {
		static std :: mt19937 rnd(std :: random_device{}());
		if (bag_pos == 0) {
			bag_pos = 7;
			std :: shuffle(bag, bag + 7, rnd);
		}
		return bag[-- bag_pos];
	}

	void roll() {
		cur = nxt[0];
		for (int i = 0; i < 4; i ++) {
			nxt[i] = nxt[i + 1];
		}
		nxt[4] = get_next();
	}

	void use_hold() {
		if (! can_hold) return;
		can_hold = false;
		if (hold == Piece :: EMPTY) {
			hold = cur;
			roll();
		}
		else {
			std :: swap(cur, hold);
		}
	}

	int is_occupied(int x, int y) {
		if (x < 0 || x > 9) return 1;
		if (y < 0 || y > 29) return 1;
		return b[y] >> x & 1;
	}

	bool tspin_check_srs5(int x, int y, int r) {
		// Piece 6 is T
		auto kicks_cw = SRSP :: kicks_cw[6];
		auto kicks_ccw = SRSP :: kicks_ccw[6];

		for (int dx = - 2; dx <= 2; dx ++)
		for (int dy = - 2; dy <= 2; dy ++)
		for (int pr = 0; pr < 4; pr ++)
		{
			int px = x + dx;
			int py = y + dy;

			int kick = 1;
			for (const auto & h : kicks_cw[pr]) {
				int R = (pr + 1) & 3;
				int X = px + h[0];
				int Y = py + h[1];
				if (is_legal(X, Y, R)) {
					if (kick == 5 && X == x && Y == y && R == r) {
						return true;
					}
					break;
				}
				kick ++;
			}

			kick = 1;
			for (const auto & h : kicks_ccw[pr]) {
				int R = (pr + 3) & 3;
				int X = px + h[0];
				int Y = py + h[1];
				if (is_legal(X, Y, R)) {
					if (kick == 5 && X == x && Y == y && R == r) {
						return true;
					}
					break;
				}
				kick ++;
			}
		}
		return false;
	}

	Spin spin_check(int x, int y, int r) {
		if (cur == Piece :: T) {
			// T mid (- 1, - 1)
			int mx = x - 1;
			int my = y - 1;

			int cnt = 0;
			for (int dx : {- 1, 1}) for (int dy : {- 1, 1})
				cnt += is_occupied(mx + dx, my + dy);

			if (cnt == 4) {
				return Spin :: Full;
			}

			if (cnt == 3) {
				int cnt2 = 0;

				static constexpr int dx[4][2] = {{1, - 1}, {1, 1}, {1, - 1}, {- 1, - 1}};
				static constexpr int dy[4][2] = {{1, 1}, {1, - 1}, {- 1, - 1}, {1, - 1}};
				cnt2 += is_occupied(mx + dx[r][0], my + dy[r][0]);
				cnt2 += is_occupied(mx + dx[r][1], my + dy[r][1]);

				if (cnt2 == 2) {
					return Spin :: Full;
				}

				if (tspin_check_srs5(x, y, r)) {
					return Spin :: Full;
				}

				return Spin :: Mini;
			}
		}
		if (
			! is_legal(x - 1, y, r) &&
			! is_legal(x + 1, y, r) &&
			! is_legal(x, y - 1, r) &&
			! is_legal(x, y + 1, r)
		) return Spin :: Mini;
		return Spin :: None;
	}

	void place(int x, int y, int r) {
		Placement placement{};
		placement.piece = cur;
		pieces ++;

		placement.spin = spin_check(x, y, r);

		int piece = static_cast<int>(cur);
		for (int i = 0; i < 4; i ++) {
			int X = x + Mino :: minos[piece][r][i][0];
			int Y = y + Mino :: minos[piece][r][i][1];
			b[Y] |= 1 << X;
		}

		placement.clear = 0;
		for (int row = 0; row < 30; row ++) {
			if (b[row] == 1023) {
				placement.clear ++;
				for (int i = row; i < 29; i ++) {
					b[i] = b[i + 1];
				}
				b[29] = 0;
				row --;
			}
		}

		placement.pc = true;
		for (int row = 0; row < 30; row ++) {
			if (b[row]) {
				placement.pc = false;
				break;
			}
		}

		can_hold = true;

		if (last_placement.clear && placement.clear) {
			combo ++;
		}
		if (placement.clear == 0) {
			combo = 0;
		}

		attack += combo_func(
			placement.clear,
			placement.spin,
			combo,
			b2b,
			placement.pc
		);

		bool b2b_r = placement.clear == 4 || placement.spin != Spin :: None;
		bool b2b_l = last_placement.clear == 4 || last_placement.spin != Spin :: None;
		if (b2b_l && b2b_r) {
			b2b ++;
		}
		if (! b2b_r) {
			b2b = 0;
		}

		last_placement = placement;
		roll();

		if (placement.clear == 0) {
			piece = static_cast<int>(cur);
			for (int i = 0; i < 4; i ++) {
				int X = 6 + Mino :: minos[piece][0][i][0];
				int Y = 21 + Mino :: minos[piece][0][i][1];
				if (is_occupied(X, Y)) game_over = true;
			}
		}
	}

	void step(int action) {
		if (action == 0) {
			use_hold();
		}
		else {
			auto [x, y, r] = Action :: decode(action);
			place(x, y, r);
		}
	}

	bool is_legal(int x, int y, int r) {
		int piece = static_cast<int>(cur);
		for (int i = 0; i < 4; i ++) {
			int X = x + Mino :: minos[piece][r][i][0];
			int Y = y + Mino :: minos[piece][r][i][1];
			if (is_occupied(X, Y)) return false;
		}
		return true;
	}

	bool is_grounded(int x, int y, int r) {
		return ! is_legal(x, y - 1, r);
	}

	std :: pair<int, int> get_spawn() {
		if (last_placement.clear > 0) {
			int x = 5, y = 21;
			while (! is_legal(x, y, 0)) y ++;
			return {x, y};
		}
		return {5, 21};
	}

	std :: vector<int> legal() {
		std :: vector<int> res;
		if (game_over) return res;
		if (can_hold) res.push_back(Action :: hold());

		std :: vector<bool> vis(1 << 11);
		std :: queue<std :: tuple<int, int, int>> q;

		auto [sx, sy] = get_spawn();
		q.emplace(sx, sy, 0);

		constexpr int dx[] = {0, - 1, 1};
		constexpr int dy[] = {- 1, 0, 0};

		int piece = static_cast<int>(cur);

		auto kicks_cw = SRSP :: kicks_cw[piece];
		auto kicks_ccw = SRSP :: kicks_ccw[piece];
		auto kicks_180 = SRSP :: kicks_180[piece];

		while (! q.empty()) {
			auto [x, y, r] = q.front();
			q.pop();

			int u = y << 6 | x << 2 | r;
			if (vis[u]) continue;
			vis[u] = true;
			if (is_grounded(x, y, r)) {
				res.push_back(Action :: place(x, y, r));
			}

			for (int d = 0; d < 3; d ++) {
				int X = x + dx[d];
				int Y = y + dy[d];
				if (! is_legal(X, Y, r)) continue;
				q.emplace(X, Y, r);
			}

			if (cur == Piece :: O) continue;

			for (const auto & h : kicks_cw[r]) {
				int R = (r + 1) & 3;
				int X = x + h[0];
				int Y = y + h[1];
				if (is_legal(X, Y, R)) {
					q.emplace(X, Y, R);
					break;
				}
			}

			for (const auto & h : kicks_ccw[r]) {
				int R = (r + 3) & 3;
				int X = x + h[0];
				int Y = y + h[1];
				if (is_legal(X, Y, R)) {
					q.emplace(X, Y, R);
					break;
				}
			}

			for (const auto & h : kicks_180[r]) {
				int R = (r + 2) & 3;
				int X = x + h[0];
				int Y = y + h[1];
				if (is_legal(X, Y, R)) {
					q.emplace(X, Y, R);
					break;
				}
			}
		}
		return res;
	}
};

#endif //KEROSHIZEROTETRIS_TETRIS_H
