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
	at :: Tensor to_tensor_board(std :: vector<TetrisEnv> & tetris);
	at :: Tensor to_tensor_piece(std :: vector<TetrisEnv> & tetris);
	at :: Tensor to_tensor_info(std :: vector<TetrisEnv> & tetris);

	// Dataset to Tensor
	at :: Tensor to_tensor_board(const std :: vector<Dataset :: player_value> & data);
	at :: Tensor to_tensor_piece(const std :: vector<Dataset :: player_value> & data);
	at :: Tensor to_tensor_info(const std :: vector<Dataset :: player_value> & data);
	at :: Tensor to_tensor_V(const std :: vector<Dataset :: player_value> & data);

};


#endif //KEROSHIZEROTETRIS_CONVERTER_HPP
