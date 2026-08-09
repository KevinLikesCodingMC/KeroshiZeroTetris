//
// Created by keroshi on 2026/8/5.
//

#include "include/ai/converter.hpp"

#include <cmath>

namespace Converter {
	Dataset :: player_value to_dataset_player_value(TetrisEnv & tetris) {
		Dataset :: player_value data {};

		for (int row = 0; row < Board :: HEIGHT; row ++) {
			data.board[row] = tetris.get_board_row(row);
		}

		data.piece[0] = static_cast<uint8_t>(tetris.get_cur());
		data.piece[1] = static_cast<uint8_t>(tetris.get_hold());
		for (int i = 0; i < Board :: NEXT; i ++) {
			data.piece[i + 2] = static_cast<uint8_t>(tetris.get_next(i));
		}

		data.info[0] = tetris.get_rest_pieces();
		data.info[1] = tetris.is_can_hold() ? 1.f : 0.f;
		data.info[2] = tetris.get_combo();
		data.info[3] = std :: logf(tetris.get_b2b() + 1);

		return data;
	}

	at :: Tensor to_tensor_board(TetrisEnv & tetris) {
		auto tensor = at :: zeros({1, 1, Board :: HEIGHT, Board :: WIDTH}, at :: kFloat);

		auto acc = tensor.accessor<float, 4>();
		for (int y = 0; y < Board :: HEIGHT; y ++) {
			for (int x = 0; x < Board :: WIDTH; x ++) {
				acc[0][0][y][x] = tetris.get_board(x, y) ? 1.f : 0.f;
			}
		}

		return tensor;
	}

	at :: Tensor to_tensor_piece(TetrisEnv & tetris) {
		auto tensor = at :: zeros({1, Dataset :: PIECE_CAPACITY}, at :: kLong);

		auto acc = tensor.accessor<int64_t, 2>();

		acc[0][0] = static_cast<int64_t>(tetris.get_cur());
		acc[0][1] = static_cast<int64_t>(tetris.get_hold());
		for (int i = 0; i < Board :: NEXT; i ++) {
			acc[0][i + 2] = static_cast<int64_t>(tetris.get_next(i));
		}

		return tensor;
	}

	at :: Tensor to_tensor_info(TetrisEnv & tetris) {
		auto tensor = at :: zeros({1, Dataset :: INFO_CAPACITY}, at :: kFloat);

		auto acc = tensor.accessor<float, 2>();

		acc[0][0] = tetris.get_rest_pieces();
		acc[0][1] = tetris.is_can_hold() ? 1.f : 0.f;
		acc[0][2] = tetris.get_combo();
		acc[0][3] = std :: logf(tetris.get_b2b() + 1);

		return tensor;
	}
}