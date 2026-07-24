//
// Created by keroshi on 2026/7/24.
//

#ifndef KEROSHIZEROTETRIS_MINO_HPP
#define KEROSHIZEROTETRIS_MINO_HPP

#include "def.hpp"
#include <array>

namespace Mino {
	struct offset {
		int8_t x = 0;
		int8_t y = 0;
	};

	using shape = std :: array<offset, 4>;
	using piece_shape = std :: array<shape, 4>;

	/*
		coordinates based on board position (5, 21) [0-index].
		Rotation state:
		- 0 : Spawn / 0
		- 1 : CW / 90
		- 2 : 180
		- 3 : CCW / 270
	*/

	constexpr piece_shape O = {
		offset{0, 0}, {0, - 1}, {- 1, 0}, {- 1, - 1},
		offset{0, 0}, {0, - 1}, {- 1, 0}, {- 1, - 1},
		offset{0, 0}, {0, - 1}, {- 1, 0}, {- 1, - 1},
		offset{0, 0}, {0, - 1}, {- 1, 0}, {- 1, - 1},
	};
	constexpr piece_shape I = {
		offset{- 2, - 1}, {- 1, - 1}, {0, - 1}, {+ 1, - 1},
		offset{0, 0}, {0, - 1}, {0, - 2}, {0, - 3},
		offset{- 2, - 2}, {- 1, - 2}, {0, - 2}, {+ 1, - 2},
		offset{- 1, 0}, {- 1, - 1}, {- 1, - 2}, {- 1, - 3},
	};
	constexpr piece_shape T = {
		offset{- 1, - 1}, {0, - 1}, {- 1, 0}, {- 2, - 1},
		offset{- 1, - 1}, {0, - 1}, {- 1, 0}, {- 1, - 2},
		offset{- 1, - 1}, {0, - 1}, {- 1, - 2}, {- 2, - 1},
		offset{- 1, - 1}, {- 1, 0}, {- 1, - 2}, {- 2, - 1},
	};
	constexpr piece_shape J = {
		offset{- 1, - 1}, {0, - 1}, {- 2, - 1}, {- 2, 0},
		offset{- 1, - 1}, {0, 0}, {- 1, 0}, {- 1, - 2},
		offset{- 1, - 1}, {0, - 1}, {- 2, - 1}, {0, - 2},
		offset{- 1, - 1}, {- 2, - 2}, {- 1, 0}, {- 1, - 2},
	};
	constexpr piece_shape L = {
		offset{- 1, - 1}, {0, - 1}, {- 2, - 1}, {0, 0},
		offset{- 1, - 1}, {0, - 2}, {- 1, 0}, {- 1, - 2},
		offset{- 1, - 1}, {0, - 1}, {- 2, - 1}, {- 2, - 2},
		offset{- 1, - 1}, {- 2, 0}, {- 1, 0}, {- 1, - 2},
	};
	constexpr piece_shape S = {
		offset{- 1, - 1}, {0, 0}, {- 1, 0}, {- 2, - 1},
		offset{- 1, - 1}, {0, - 1}, {- 1, 0}, {0, - 2},
		offset{- 1, - 1}, {0, - 1}, {- 1, - 2}, {- 2, - 2},
		offset{- 1, - 1}, {- 2, - 1}, {- 1, - 2}, {- 2, 0},
	};
	constexpr piece_shape Z = {
		offset{- 1, - 1}, {0, - 1}, {- 1, 0}, {- 2, 0},
		offset{- 1, - 1}, {0, 0}, {0, - 1}, {- 1, - 2},
		offset{- 1, - 1}, {- 2, - 1}, {- 1, - 2}, {0, - 2},
		offset{- 1, - 1}, {- 2, - 1}, {- 1, 0}, {- 2, - 2},
	};

	constexpr std :: array MINOS = {
		piece_shape{},
		I, J, L, O, S, T, Z
	};

	constexpr std :: pair<int, int> get(Piece piece, int i, int r, int x = 0, int y = 0) {
		int p = static_cast<int>(piece);
		return {
			x + MINOS[p][r][i].x,
			y + MINOS[p][r][i].y
		};
	}
}

#endif //KEROSHIZEROTETRIS_MINO_HPP
