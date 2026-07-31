//
// Created by keroshi on 2026/7/24.
//

#include "include/core/tetris.hpp"
#include "include/core/mino.hpp"

#include <queue>

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

	// Actions Cache
	actions_cache = false;

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
void TetrisEnv :: set_board_row(int y, uint16_t x) {
	board[y] = x;
}

Piece TetrisEnv :: get_cur() {
	return cur;
}
void TetrisEnv :: set_cur(Piece piece) {
	cur = piece;
}
Piece TetrisEnv :: get_hold() {
	return hold;
}
Piece TetrisEnv :: get_next(int x) {
	if (x < 0 || x >= Board :: NEXT) {
		return Piece :: EMPTY;
	}
	return nxt[x];
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

SRSP :: kick_decode TetrisEnv :: get_kick(
	int x, int y, int r, SRSP :: Rotation rotation
) {
	auto kicks = SRSP :: get_kicks(cur, rotation, r);

	int nr = SRSP :: get_r(r, rotation);

	for (auto [dx, dy] : kicks) {
		int nx = x + dx;
		int ny = y + dy;

		if (is_legal(nx, ny, nr)) {
			return {true, nx, ny, nr};
		}
	}

	return {false};
}

std :: pair<int, int> TetrisEnv :: das_d(int x, int y, int r) {
	while (is_legal(x, y - 1, r)) y --;
	return {x, y};
}

std :: vector<int> TetrisEnv :: get_actions() {
	if (! actions_cache) {
		calc_actions();
		actions_cache = true;
	}

	return actions;
}

void TetrisEnv :: calc_actions() {
	actions.clear();

	if (game_over) return;
	if (can_hold) actions.push_back(Action :: hold());

	std :: vector<bool> vis(Action :: SIZE);

	struct pos {
		int x, y, r;
	};

	std :: queue<pos> q;

	auto [sx, sy] = get_spawn();
	vis[Action :: place(sx, sy, 0)] = true;
	q.emplace(sx, sy, 0);

	while (! q.empty()) {
		auto [x, y, r] = q.front();
		q.pop();

		if (is_grounded(x, y, r)) {
			actions.push_back(Action :: place(x, y, r));
		}

		for (int dx : {- 1, 1}) {
			int X = x + dx;
			if (! is_legal(X, y, r)) continue;
			int v = Action :: place(X, y, r);
			if (! vis[v]) {
				vis[v] = true;
				q.emplace(X, y, r);
			}
		}

		{
			auto [X, Y] = das_d(x, y, r);
			int v = Action :: place(X, Y, r);
			if (! vis[v]) {
				vis[v] = true;
				q.emplace(X, Y, r);
			}
		}

		if (cur == Piece :: O) continue;

		for (SRSP :: Rotation rotation : {
			SRSP :: Rotation :: CW,
			SRSP :: Rotation :: CCW,
			SRSP :: Rotation :: R180,
		}) {
			auto [success, X, Y, R] = get_kick(x, y, r, rotation);
			if (! success) continue;

			int v = Action :: place(X, Y, R);
			if (! vis[v]) {
				vis[v] = true;
				q.emplace(X, Y, R);
			}
		}
	}
}
