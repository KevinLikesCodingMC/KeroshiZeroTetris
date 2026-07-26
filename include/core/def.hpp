//
// Created by keroshi on 2026/7/24.
//

#ifndef KEROSHIZEROTETRIS_DEF_HPP
#define KEROSHIZEROTETRIS_DEF_HPP

#include <cstdint>

enum class Piece : uint8_t {
	EMPTY = 0,
	I, J, L, O, S, T, Z
};

enum class Spin : uint8_t {
	None, Mini, Full
};

namespace Board {
	inline constexpr int WIDTH = 10;
	inline constexpr int HEIGHT = 30;
	inline constexpr int NEXT = 5;
	inline constexpr int SPAWN_X = 5;
	inline constexpr int SPAWN_Y = 21;
}


#endif //KEROSHIZEROTETRIS_DEF_HPP
