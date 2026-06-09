//
// Created by keroshi on 2026/5/23.
//

#include "include/converter.h"
#include "include/message.h"

#include <ATen/ATen.h>

namespace Converter {

	at :: Tensor to_board(const Tetris & t) {
		auto tensor = at :: zeros({1, 1, 30, 10}, at :: kFloat);
		if (t.is_over()) return tensor;

		auto acc = tensor.accessor<float, 4>();
		for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
			if (t.b[i] >> j & 1) acc[0][0][i][j] = 1.f;
		}
		return tensor;
	}

	at :: Tensor to_seq(const Tetris & t) {
		auto tensor = at :: zeros({1, 10}, at :: kLong);
		if (t.is_over()) return tensor;

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
		if (t.is_over()) return tensor;

		auto acc = tensor.accessor<float, 2>();
		acc[0][0] = t.can_hold ? 1.f : 0.f;
		acc[0][1] = static_cast<float>(t.combo) / 10.f;
		acc[0][2] = static_cast<float>(std :: log(t.b2b + 1));
		acc[0][3] = static_cast<float>(t.pieces);
		acc[0][4] = t.attack;
		return tensor;
	}

	at :: Tensor to_pos(Tetris & t) {
		auto tensor = at :: zeros({1, 128, 5}, at :: kLong);
		if (t.is_leaf()) return tensor;

		auto actions = t.legal();
		int n = static_cast<int>(actions.size());

		if (n > 128) return tensor;

		int cur = static_cast<int>(t.cur);

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

	at :: Tensor to_board(const std :: vector<Tetris> & t) {
		int batch = t.size();
		auto tensor = at :: zeros({batch, 1, 30, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 4>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_over()) continue;

			for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
				if (t[I].b[i] >> j & 1) acc[I][0][i][j] = 1.f;
			}
		}
		return tensor;
	}

	at :: Tensor to_seq(const std :: vector<Tetris> & t) {
		int batch = t.size();
		auto tensor = at :: zeros({batch, 10}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 2>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_over()) continue;

			acc[I][0] = static_cast<int64_t>(t[I].cur);
			acc[I][1] = static_cast<int64_t>(t[I].hold);
			for (int i = 0; i < 5; i ++) {
				acc[I][i + 2] = static_cast<int64_t>(t[I].nxt[i]);
			}
		}
		return tensor;
	}

	at :: Tensor to_info(const std :: vector<Tetris> & t) {
		int batch = t.size();
		auto tensor = at :: zeros({batch, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_over()) continue;

			acc[I][0] = t[I].can_hold ? 1.f : 0.f;
			acc[I][1] = static_cast<float>(t[I].combo) / 10.f;
			acc[I][2] = static_cast<float>(std :: log(t[I].b2b + 1));
			acc[I][3] = static_cast<float>(t[I].pieces);
			acc[I][4] = t[I].attack;
		}
		return tensor;
	}

	at :: Tensor to_pos(std :: vector<Tetris> & t) {
		int batch = t.size();

		auto tensor = at :: zeros({batch, 128, 5}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 3>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_leaf()) continue;

			auto actions = t[I].legal();
			int n = static_cast<int>(actions.size());
			if (n > 128) continue;

			int cur = static_cast<int>(t[I].cur);

			for (int i = 0; i < n; i ++) {
				int u = actions[i];
				if (u == 0) {
					acc[I][i][0] = 0;
					acc[I][i][1] = 0;
					acc[I][i][2] = 0;
					acc[I][i][3] = 0;
					acc[I][i][4] = 1;
				}
				else {
					auto [x, y, r] = Action :: decode(u);
					acc[I][i][0] = x;
					acc[I][i][1] = y;
					acc[I][i][2] = r;
					acc[I][i][3] = cur;
					acc[I][i][4] = 1;
				}
			}
		}

		return tensor;
	}

	at :: Tensor to_board(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());
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
		int batch = static_cast<int>(data.size());
		auto tensor = at :: zeros({batch, 10}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 2>();
		for (int i = 0; i < batch; i ++) {
			for (int j = 0; j < 10; j ++)
				acc[i][j] = data[i].seq[j];
		}
		return tensor;
	}

	at :: Tensor to_info(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());
		auto tensor = at :: zeros({batch, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();
		for (int i = 0; i < batch; i ++) {
			for (int j = 0; j < 10; j ++)
				acc[i][j] = data[i].info[j];
		}
		return tensor;
	}

	at :: Tensor to_pos(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());

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
		int batch = static_cast<int>(data.size());

		auto tensor = at :: zeros({batch, 1}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int i = 0; i < batch; i ++) {
			acc[i][0] = data[i].V;
		}

		return tensor;
	}

	at :: Tensor to_P(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());

		auto tensor = at :: zeros({batch, 128}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int i = 0; i < batch; i ++) {
			int len = data[i].len;
			for (int j = 0; j < len; j ++)
				acc[i][j] = data[i].P[j];
		}

		return tensor;
	}

	TetrisTrainData to_train_data(Tetris & t, const std :: vector<float> & P) {
		TetrisTrainData data {};

		for (int i = 0; i < 30; i ++) data.b[i] = t.b[i];

		data.seq[0] = static_cast<int>(t.cur);
		data.seq[1] = static_cast<int>(t.hold);
		for (int i = 0; i < 5; i ++) data.seq[i + 2] = static_cast<int>(t.nxt[i]);

		data.info[0] = t.can_hold ? 1.f : 0.f;
		data.info[1] = static_cast<float>(t.combo) / 10.f;
		data.info[2] = static_cast<float>(std :: log(t.b2b + 1));
		data.info[3] = static_cast<float>(t.pieces);
		data.info[4] = t.attack;

		data.p = static_cast<int>(t.cur);

		auto actions = t.legal();
		data.len = static_cast<int>(actions.size());

		if (data.len <= 128) {

			data.P = P;
			data.x.resize(data.len);
			data.y.resize(data.len);
			data.r.resize(data.len);

			for (int i = 0; i < data.len; i ++) {
				int u = actions[i];
				if (u == 0) {
					data.x[i] = 0;
					data.y[i] = 0;
					data.r[i] = 0;
				}
				else {
					auto [x, y, r] = Action :: decode(u);
					data.x[i] = x;
					data.y[i] = y;
					data.r[i] = r;
				}
			}
		}
		else {
			data.len = 0;
		}

		return data;
	}
}
