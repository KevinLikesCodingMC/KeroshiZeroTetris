//
// Created by keroshi on 2026/5/23.
//

#include "include/converter.h"
#include "include/message.h"

#include <ATen/ATen.h>

namespace Converter {

	at :: Tensor to_board(const Tetris & t) {
		auto tensor = at :: zeros({1, 1, 30, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 4>();
		for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
			if (t.b[i] >> j & 1) acc[0][0][i][j] = 1.f;
		}
		return tensor;
	}

	at :: Tensor to_seq(const Tetris & t) {
		auto tensor = at :: zeros({1, 10}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 2>();
		acc[0][0] = static_cast<int64_t>(t.cur);
		acc[0][1] = static_cast<int64_t>(t.hold);
		for (int i = 0; i < 5; i ++) {
			acc[0][i + 2] = static_cast<int64_t>(t.nxt[i]);
		}
		return tensor;
	}

	at :: Tensor to_info(const Tetris & t) {
		auto tensor = at :: zeros({1, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();
		acc[0][0] = t.can_hold ? 1.f : 0.f;
		acc[0][1] = static_cast<float>(t.combo) / 10.f;
		acc[0][2] = static_cast<float>(std :: log(t.b2b + 1));
		acc[0][3] = static_cast<float>(t.pieces);
		acc[0][4] = static_cast<float>(t.attack);
		return tensor;
	}

	at :: Tensor to_pos(const std :: vector<int> & actions, int cur) {
		int n = static_cast<int>(actions.size());

		if (n > 128) {
			Message :: log(Message :: ERROR, true,
				"Action size is more than 128."
			);
			std :: exit(EXIT_FAILURE);
		}

		auto tensor = at :: zeros({1, 128, 5}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 3>();
		for (int i = 0; i < n; i ++) {
			int u = actions[i];
			if (u == 0) {
				acc[0][i][0] = 0;
				acc[0][i][1] = 0;
				acc[0][i][2] = 0;
				acc[0][i][3] = 0;
				acc[0][i][4] = 1;
			}
			else {
				auto [x, y, r] = Action :: decode(u);
				acc[0][i][0] = x;
				acc[0][i][1] = y;
				acc[0][i][2] = r;
				acc[0][i][3] = cur;
				acc[0][i][4] = 1;
			}
		}
		return tensor;
	}
	at :: Tensor to_pos(Tetris & t) {
		return to_pos(t.legal(), static_cast<int>(t.cur));
	}


	at :: Tensor to_board(const std :: vector<TetrisTrainData> & data) {
		int batch = data.size();
		auto tensor = at :: zeros({batch, 1, 30, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 4>();
		for (int i = 0; i < batch; i ++) {
			for (int y = 0; y < 30; y ++) for (int x = 0; x < 10; x ++) {
				if (data[i].b[y] >> x & 1) acc[i][0][y][x] = 1.f;
			}
		}
		return tensor;
	}

	at :: Tensor to_seq(const std :: vector<TetrisTrainData> & data) {
		int batch = data.size();
		auto tensor = at :: zeros({batch, 10}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 2>();
		for (int i = 0; i < batch; i ++) {
			for (int j = 0; j < 10; j ++)
				acc[i][j] = data[i].seq[j];
		}
		return tensor;
	}

	at :: Tensor to_info(const std :: vector<TetrisTrainData> & data) {
		int batch = data.size();
		auto tensor = at :: zeros({batch, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();
		for (int i = 0; i < batch; i ++) {
			for (int j = 0; j < 10; j ++)
				acc[i][j] = data[i].info[j];
		}
		return tensor;
	}

	at :: Tensor to_pos(const std :: vector<TetrisTrainData> & data) {
		int batch = data.size();

		auto tensor = at :: zeros({batch, 128, 5}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 3>();

		for (int i = 0; i < batch; i ++) {
			int len = data[i].len;
			for (int j = 0; j < len; j ++) {
				acc[i][j][0] = data[i].x[j];
				acc[i][j][1] = data[i].y[j];
				acc[i][j][2] = data[i].r[j];
				acc[i][j][3] = data[i].p;
				acc[i][j][4] = 1;
			}
		}
		return tensor;
	}

	at :: Tensor to_V(const std :: vector<TetrisTrainData> & data) {
		int batch = data.size();

		auto tensor = at :: zeros({batch, 1}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int i = 0; i < batch; i ++) {
			acc[i][0] = data[i].V;
		}

		return tensor;
	}

	at :: Tensor to_P(const std :: vector<TetrisTrainData> & data) {
		int batch = data.size();

		auto tensor = at :: zeros({batch, 128}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int i = 0; i < batch; i ++) {
			int len = data[i].len;
			for (int j = 0; j < len; j ++)
				acc[i][j] = data[i].P[j];
		}

		return tensor;
	}
}
