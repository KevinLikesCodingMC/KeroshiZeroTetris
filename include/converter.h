//
// Created by keroshi on 2026/5/8.
//

#ifndef KEROSHIZEROTETRIS_CONVERTER_H
#define KEROSHIZEROTETRIS_CONVERTER_H

#include "tetris.h"
#include "record.h"

#include <ATen/ATen.h>

namespace Converter {

	at :: Tensor to_board(const Tetris & t);
	at :: Tensor to_seq(const Tetris & t);
	at :: Tensor to_info(const Tetris & t);
	at :: Tensor to_pos(const std :: vector<int> & actions, int cur);
	at :: Tensor to_pos(Tetris & t);

	at :: Tensor to_board(const std :: vector<TetrisTrainData> & data);
	at :: Tensor to_seq(const std :: vector<TetrisTrainData> & data);
	at :: Tensor to_info(const std :: vector<TetrisTrainData> & data);
	at :: Tensor to_pos(const std :: vector<TetrisTrainData> & data);
	at :: Tensor to_V(const std :: vector<TetrisTrainData> & data);
	at :: Tensor to_P(const std :: vector<TetrisTrainData> & data);

	TetrisTrainData to_train_data(Tetris & t, const std :: vector<float> & P);
};

#endif //KEROSHIZEROTETRIS_CONVERTER_H
