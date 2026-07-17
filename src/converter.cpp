//
// Created by keroshi on 2026/5/23.
//

#include "include/converter.h"

#include <ATen/ATen.h>

namespace Converter {

	at :: Tensor to_board(const Tetris & t) {
		auto tensor = at :: zeros({1, 4, 30, 10}, at :: kFloat);
		if (t.is_over()) return tensor;

		auto acc = tensor.accessor<float, 4>();
		for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
			if (t.b[i] >> j & 1) acc[0][0][i][j] = 1.f;
		}
		for (int k = 0; k < 3; k ++) {
			for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
				if (t.his[k][i] >> j & 1) acc[0][k + 1][i][j] = 1.f;
			}
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
		acc[0][3] = static_cast<float>(t.rest_pieces);
		return tensor;
	}

	at :: Tensor to_mask(Tetris & t) {
		auto tensor = at :: zeros({1, 1537}, at :: kByte);
		if (t.is_leaf()) return tensor;

		auto acc = tensor.accessor<uint8_t, 2>();

		auto actions = t.legal();

		for (int u : actions) {
			acc[0][u] = 1;
		}

		return tensor;
	}

	at :: Tensor to_board(const std :: vector<Tetris> & t) {
		int batch = static_cast<int>(t.size());
		auto tensor = at :: zeros({batch, 4, 30, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 4>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_over()) continue;

			for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
				if (t[I].b[i] >> j & 1) acc[I][0][i][j] = 1.f;
			}
			for (int k = 0; k < 3; k ++) {
				for (int i = 0; i < 30; i ++) for (int j = 0; j < 10; j ++) {
					if (t[I].his[k][i] >> j & 1) acc[I][k + 1][i][j] = 1.f;
				}
			}
		}
		return tensor;
	}

	at :: Tensor to_seq(const std :: vector<Tetris> & t) {
		int batch = static_cast<int>(t.size());
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
		int batch = static_cast<int>(t.size());
		auto tensor = at :: zeros({batch, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_over()) continue;

			acc[I][0] = t[I].can_hold ? 1.f : 0.f;
			acc[I][1] = static_cast<float>(t[I].combo) / 10.f;
			acc[I][2] = static_cast<float>(std :: log(t[I].b2b + 1));
			acc[I][3] = static_cast<float>(t[I].rest_pieces);
		}
		return tensor;
	}

	at :: Tensor to_mask(std :: vector<Tetris> & t) {
		int batch = static_cast<int>(t.size());

		auto tensor = at :: zeros({batch, 1537}, at :: kByte);
		auto acc = tensor.accessor<uint8_t, 2>();

		for (int I = 0; I < batch; I ++) {
			if (t[I].is_leaf()) continue;

			auto actions = t[I].legal();
			for (int u : actions) {
				acc[I][u] = 1;
			}
		}

		return tensor;
	}

	at :: Tensor to_board(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());
		auto tensor = at :: zeros({batch, 4, 30, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 4>();
		for (int I = 0; I < batch; I ++) {
			for (int k = 0; k < 4; k ++) {
				for (int y = 0; y < 30; y ++) for (int x = 0; x < 10; x ++) {
					if (data[I].b[k][y] >> x & 1) acc[I][k][y][x] = 1.f;
				}
			}
		}
		return tensor;
	}

	at :: Tensor to_seq(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());
		auto tensor = at :: zeros({batch, 10}, at :: kLong);
		auto acc = tensor.accessor<int64_t, 2>();
		for (int I = 0; I < batch; I ++) {
			for (int i = 0; i < 10; i ++)
				acc[I][i] = data[I].seq[i];
		}
		return tensor;
	}

	at :: Tensor to_info(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());
		auto tensor = at :: zeros({batch, 10}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();
		for (int I = 0; I < batch; I ++) {
			for (int i = 0; i < 10; i ++)
				acc[I][i] = data[I].info[i];
		}
		return tensor;
	}

	at :: Tensor to_mask(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());

		auto tensor = at :: zeros({batch, 1537}, at :: kByte);
		auto acc = tensor.accessor<uint8_t, 2>();

		for (int I = 0; I < batch; I ++) {
			for (int u : data[I].u) {
				acc[I][u] = 1;
			}
		}
		return tensor;
	}

	at :: Tensor to_V(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());

		auto tensor = at :: zeros({batch, 1}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int I = 0; I < batch; I ++) {
			acc[I][0] = data[I].V;
		}

		return tensor;
	}

	at :: Tensor to_P(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());

		auto tensor = at :: zeros({batch, 1537}, at :: kFloat);
		auto acc = tensor.accessor<float, 2>();

		for (int I = 0; I < batch; I ++) {
			int len = data[I].len;
			for (int i = 0; i < len; i ++) {
				int u = data[I].u[i];
				acc[I][u] = data[I].P[i];
			}
		}

		return tensor;
	}

	at :: Tensor to_PW(const std :: vector<TetrisTrainData> & data) {
		int batch = static_cast<int>(data.size());

		auto tensor = at :: zeros({batch}, at :: kFloat);
		auto acc = tensor.accessor<float, 1>();

		for (int I = 0; I < batch; I ++) {
			acc[I] = data[I].PW;
		}

		return tensor;
	}

	TetrisTrainData to_train_data(Tetris & t, const std :: vector<float> & P) {
		TetrisTrainData data {};

		for (int i = 0; i < 30; i ++) data.b[0][i] = t.b[i];
		for (int k = 0; k < 3; k ++) {
			for (int i = 0; i < 30; i ++) data.b[k + 1][i] = t.his[k][i];
		}

		data.seq[0] = static_cast<uint8_t>(t.cur);
		data.seq[1] = static_cast<uint8_t>(t.hold);
		for (int i = 0; i < 5; i ++) data.seq[i + 2] = static_cast<uint8_t>(t.nxt[i]);

		data.info[0] = t.can_hold ? 1.f : 0.f;
		data.info[1] = static_cast<float>(t.combo) / 10.f;
		data.info[2] = static_cast<float>(std :: log(t.b2b + 1));
		data.info[3] = static_cast<float>(t.rest_pieces);

		auto actions = t.legal();
		data.len = static_cast<int>(actions.size());

		data.P = P;
		data.u.resize(data.len);

		for (int i = 0; i < data.len; i ++) {
			data.u[i] = actions[i];
		}

		return data;
	}
}
