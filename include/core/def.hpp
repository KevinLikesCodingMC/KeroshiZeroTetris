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

#endif //KEROSHIZEROTETRIS_DEF_HPP
