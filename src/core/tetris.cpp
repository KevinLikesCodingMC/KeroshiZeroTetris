//
// Created by keroshi on 2026/7/24.
//

#include "include/core/tetris.hpp"
#include "include/core/mino.hpp"

#include <queue>
#include <cmath>


bool Placement :: b2b() const {
	return clear == 4 || spin != Spin :: None;
}


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
	is_actions_cache = false;

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

std :: vector<int> TetrisEnv :: get_player_actions() {
	if (! is_actions_cache) {
		calc_actions();
		is_actions_cache = true;
	}

	return actions_cache;
}

std :: vector<int> TetrisEnv :: get_actions() {
	return is_player() ? get_player_actions()
		: get_gen_actions();
}

void TetrisEnv :: calc_actions() {
	actions_cache.clear();

	if (game_over) return;
	if (can_hold) actions_cache.push_back(Action :: hold());

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
			actions_cache.push_back(Action :: place(x, y, r));
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

std :: vector<int> TetrisEnv :: get_gen_actions() {
	std :: vector<int> res;
	for (int i = 0; i < 7; i ++) {
		if (! (bag_use >> i & 1)) {
			res.push_back(i);
		}
	}
	return res;
}

bool TetrisEnv :: step_gen(int action) {
	if (action < 0 || action >= 7) return false;
	if (bag_use >> action & 1) return false;

	bag_piece --;
	bag_use |= 1 << action;

	if (bag_piece == 0) {
		bag_piece = 7;
		bag_use = 0;
	}

	auto piece = static_cast<Piece>(action + 1);

	if (cur == Piece :: EMPTY) {
		cur = piece;
	}
	else {
		for (int i = 0; i < Board :: NEXT; i ++) {
			if (nxt[i] == Piece :: EMPTY) {
				nxt[i] = piece;
				break;
			}
		}
	}

	check_game_over();

	return true;
}

bool TetrisEnv :: step_player(int action) {
	if (action < 0 || action >= Action :: SIZE) return false;

	bool success = false;
	if (action == Action :: hold()) {
		success = use_hold();
	}
	else {
		auto [x, y, r] = Action :: decode(action);
		success = place(x, y, r);
	}
	return success;
}

bool TetrisEnv :: step(int action) {
	return is_player() ? step_player(action)
		: step_gen(action);
}

bool TetrisEnv :: use_hold() {
	if (! can_hold) return false;
	can_hold = false;
	is_actions_cache = false;

	if (hold == Piece :: EMPTY) {
		hold = cur;
		roll();
	}
	else {
		std :: swap(cur, hold);
	}

	return true;
}

void TetrisEnv :: roll() {
	cur = nxt[0];
	for (int i = 0; i < Board :: NEXT - 1; i ++) {
		nxt[i] = nxt[i + 1];
	}
	nxt[Board :: NEXT - 1] = Piece :: EMPTY;
}

bool TetrisEnv :: place(int x, int y, int r) {
	if (! is_legal(x, y, r)) return false;
	if (! is_grounded(x, y, r)) return false;

	auto last_placement = placement;
	placement = {cur, x, y, r};

	placement.spin = check_spin(x, y, r);

	is_actions_cache = false;

	for (int i = 0; i < 4; i ++) {
		auto [X, Y] = Mino :: get(cur, i, r, x, y);
		set_board(X, Y, true);
	}

	placement.clear = 0;
	int write_pos = 0;
	for (int row = 0; row < Board :: HEIGHT; row ++) {
		if (board[row] == Board :: BOARD_ROW_FULL) {
			placement.clear ++;
			continue;
		}
		board[write_pos ++] = board[row];
	}
	while (write_pos < Board :: HEIGHT) {
		board[write_pos ++] = 0;
	}

	if (last_placement.clear && placement.clear) {
		combo ++;
	}
	if (placement.clear == 0) {
		combo = 0;
	}

	bool b2b_l = last_placement.b2b();
	bool b2b_r = placement.b2b();
	if (b2b_l && b2b_r) {
		b2b ++;
	}
	if (! b2b_r) {
		b2b = 0;
	}

	roll();
	check_game_over();

	can_hold = true;

	pieces ++;
	rest_pieces --;

	return true;
}

void TetrisEnv :: check_game_over() {
	if (game_over) return;
	if (placement.clear > 0) return; // Clutch Clear
	if (cur == Piece :: EMPTY) return;

	if (! is_legal(Board :: SPAWN_X, Board :: SPAWN_Y, 0)) {
		game_over = true;
	}
}

Spin TetrisEnv :: check_spin(int x, int y, int r) {
	if (cur == Piece :: EMPTY || cur == Piece :: O) {
		return Spin :: None;
	}

	if (cur == Piece :: T) {
		return check_tspin(x, y, r);
	}

	/*
		Immobile
		Reference: https://tetrio.wiki.gg/wiki/Spins
	*/

	if (is_legal(x - 1, y, r)) return Spin :: None;
	if (is_legal(x + 1, y, r)) return Spin :: None;
	if (is_legal(x, y - 1, r)) return Spin :: None;
	if (is_legal(x, y + 1, r)) return Spin :: None;

	return Spin :: Mini;
}

Spin TetrisEnv :: check_tspin(int x, int y, int r) {

	/*
		2-corner and 3-corner
		Reference: https://tetrio.wiki.gg/wiki/Spins
	*/

	int corner_cnt = 0;

	// T mid (- 1, - 1)

	int mx = x - 1;
	int my = y - 1;

	for (int dx : {- 1, 1}) {
		for (int dy : {- 1, 1}) {
			corner_cnt += is_occupied(mx + dx, my + dy);
		}
	}

	// 3-corner
	if (corner_cnt == 4) {
		return Spin :: Full;
	}
	if (corner_cnt == 3) {
		// 2-corner
		int facing_cnt = 0;

		static constexpr int dx[4][2] = {{1, - 1}, {1, 1}, {1, - 1}, {- 1, - 1}};
		static constexpr int dy[4][2] = {{1, 1}, {1, - 1}, {- 1, - 1}, {1, - 1}};

		facing_cnt += is_occupied(mx + dx[r][0], my + dy[r][0]);
		facing_cnt += is_occupied(mx + dx[r][1], my + dy[r][1]);

		if (facing_cnt == 2) {
			return Spin :: Full;
		}

		uint8_t spin_kick = check_tspin_kicks(x, y, r);

		// Unless the 5th kick was used, in which case it is still a "full Spin".
		if (spin_kick >> 5 & 1) {
			return Spin :: Full;
		}

		if (spin_kick) {
			return Spin :: Mini;
		}
	}

	// Immobile
	if (is_legal(x - 1, y, r)) return Spin :: None;
	if (is_legal(x + 1, y, r)) return Spin :: None;
	if (is_legal(x, y - 1, r)) return Spin :: None;
	if (is_legal(x, y + 1, r)) return Spin :: None;

	return Spin :: Mini;
}

uint8_t TetrisEnv :: check_tspin_kicks(int x, int y, int r) {

	auto actions = get_player_actions();
	uint8_t res = 0;

	for (int action : actions) {
		if (action == Action :: hold()) continue;
		auto [nx, ny, nr] = Action :: decode(action);

		int dist = std :: abs(x - nx) + std :: abs(y - ny);
		if (dist > 3) continue;

		for (SRSP :: Rotation rotation : {
			SRSP :: Rotation :: CW,
			SRSP :: Rotation :: CCW,
			SRSP :: Rotation :: R180,
		}) {
			auto [success, X, Y, R, id] = get_kick_id(nx, ny, nr, rotation);
			if (! success) continue;
			if (X != x || Y != y || R != r) continue;

			res |= 1u << id;
		}
	}

	return res;
}

SRSP :: kick_decode_id TetrisEnv :: get_kick_id(
	int x, int y, int r, SRSP :: Rotation rotation
) {
	auto kicks = SRSP :: get_kicks(cur, rotation, r);

	int nr = SRSP :: get_r(r, rotation);

	int id = 0;

	for (auto [dx, dy] : kicks) {
		int nx = x + dx;
		int ny = y + dy;

		id ++;

		if (is_legal(nx, ny, nr)) {
			return {true, nx, ny, nr, id};
		}
	}

	return {false};
}

bool TetrisEnv :: is_over() {
	return game_over;
}

bool TetrisEnv :: is_player() {
	if (cur == Piece :: EMPTY) {
		return false;
	}

	for (int i = 0; i < Board :: NEXT; i ++) {
		if (nxt[i] == Piece :: EMPTY) {
			return false;
		}
	}

	return true;
}
