//
// Created by keroshi on 2026/4/27.
//

#ifndef KEROSHIZEROTETRIS_DEF_H
#define KEROSHIZEROTETRIS_DEF_H

#include <cstdint>
#include <random>
#include <algorithm>
#include <iostream>
#include <queue>
#include <functional>

enum class Piece : uint8_t {
	EMPTY = 0,
	I, J, L, O, S, T, Z
};

enum class Spin : uint8_t {
	None, Mini, Full
};

#endif //KEROSHIZEROTETRIS_DEF_H
