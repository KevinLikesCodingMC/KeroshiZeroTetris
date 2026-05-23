//
// Created by keroshi on 2026/5/8.
//

#ifndef KEROSHIZEROTETRIS_CONVERTER_H
#define KEROSHIZEROTETRIS_CONVERTER_H

#include "tetris.h"

#include <ATen/ATen.h>

struct InputConverter {

	static at :: Tensor to_board(const Tetris & t) {
		auto tensor = at :: zeros({1, 1, 30, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 4>();
		for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
			if (t.b[i] >> j & 1) acc[0][0][i][j] = 1.f;
		}
		return tensor;
	}

	static at :: Tensor to_seq(const Tetris & t) {
		auto tensor = at :: zeros({1, 10}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 2>();
		acc[0][0] = static_cast<int64_t>(t.cur);
		acc[0][1] = static_cast<int64_t>(t.hold);
		for (int i = 0; i < 5; i ++) {
			acc[0][i + 2] = static_cast<int64_t>(t.nxt[i]);
		}
		return tensor;
	}

	static at :: Tensor to_info(const Tetris & t) {
		auto tensor = at :: zeros({1, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();
		acc[0][0] = t.can_hold ? 1.f : 0.f;
		acc[0][1] = t.combo;
		acc[0][2] = t.b2b;
		acc[0][3] = t.pieces;
		acc[0][4] = t.attack;
		return tensor;
	}

	static at :: Tensor to_pos(const std :: vector<int> & actions, int cur) {
		int n = actions.size();
		auto tensor = at :: zeros({n, 4}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();
		for (int i = 0; i < n; i ++) {
			int u = actions[i];
			if (u == 0) {
				acc[i][0] = 0;
				acc[i][1] = 0;
				acc[i][2] = 0;
				acc[i][3] = 0;
			}
			else {
				auto [x, y, r] = Action :: decode(u);
				acc[i][0] = x;
				acc[i][1] = y;
				acc[i][2] = r;
				acc[i][3] = cur;
			}
		}
		return tensor;
	}
	static at :: Tensor to_pos(Tetris & t) {
		return to_pos(t.legal(), static_cast<int>(t.cur));
	}

	static at :: Tensor to_offset(const at :: Tensor & pos) {
		int64_t n = pos.size(0);
		return at :: tensor({n}, at :: kLong);
	}
};

#endif //KEROSHIZEROTETRIS_CONVERTER_H
