//
// Created by keroshi on 2026/8/5.
//

#ifndef KEROSHIZEROTETRIS_CONVERTER_HPP
#define KEROSHIZEROTETRIS_CONVERTER_HPP

#include "include/core/tetris.hpp"
#include "dataset.hpp"

#include <ATen/ATen.h>

namespace Converter {

	// TetrisEnv to Dataset
	Dataset :: player_value to_dataset_player_value(TetrisEnv & tetris);

	// TetrisEnv to Tensor
	at :: Tensor to_tensor_board(TetrisEnv & tetris);
	at :: Tensor to_tensor_piece(TetrisEnv & tetris);
	at :: Tensor to_tensor_info(TetrisEnv & tetris);

};


#endif //KEROSHIZEROTETRIS_CONVERTER_HPP
