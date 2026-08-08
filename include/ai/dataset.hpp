//
// Created by keroshi on 2026/8/5.
//

#ifndef KEROSHIZEROTETRIS_SAMPLE_HPP
#define KEROSHIZEROTETRIS_SAMPLE_HPP

#include "include/core/tetris.hpp"

namespace Dataset {

	inline constexpr int PIECE_CAPACITY = 10;
	inline constexpr int INFO_CAPACITY = 10;

	#pragma pack(push, 1)
	struct player_value {
		uint16_t board[Board :: HEIGHT];
		uint8_t piece[PIECE_CAPACITY];
		float info[INFO_CAPACITY];
		float V;
	};
	#pragma pack(pop)

	static_assert(
		sizeof(player_value) ==
		sizeof(uint16_t) * Board :: HEIGHT +
		sizeof(uint8_t) * PIECE_CAPACITY +
		sizeof(float) * INFO_CAPACITY +
		sizeof(float)
	);

}

#endif //KEROSHIZEROTETRIS_SAMPLE_HPP
