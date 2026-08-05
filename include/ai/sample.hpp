//
// Created by keroshi on 2026/8/5.
//

#ifndef KEROSHIZEROTETRIS_SAMPLE_HPP
#define KEROSHIZEROTETRIS_SAMPLE_HPP

#include "include/core/tetris.hpp"

#pragma pack(push, 1)
struct TetrisSample_player_value {
	uint16_t board[30];
	uint8_t piece[10];
	float info[10];
	float V;
};
#pragma pack(pop)

#endif //KEROSHIZEROTETRIS_SAMPLE_HPP
