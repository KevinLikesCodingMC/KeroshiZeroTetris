//
// Created by keroshi on 2026/5/8.
//

#ifndef KEROSHIZEROTETRIS_CONVERTER_H
#define KEROSHIZEROTETRIS_CONVERTER_H

#include "tetris.h"

#include <ATen/ATen.h>

namespace InputConverter {

	at :: Tensor to_board(const Tetris & t);

	at :: Tensor to_seq(const Tetris & t);

	at :: Tensor to_info(const Tetris & t);

	at :: Tensor to_pos(const std :: vector<int> & actions, int cur);
	at :: Tensor to_pos(Tetris & t);

	at :: Tensor to_offset(const at :: Tensor & pos);
};

#endif //KEROSHIZEROTETRIS_CONVERTER_H
