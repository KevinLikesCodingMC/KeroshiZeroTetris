//
// Created by keroshi on 2026/7/26.
//

#ifndef KEROSHIZEROTETRIS_KICK_HPP
#define KEROSHIZEROTETRIS_KICK_HPP

#include "def.hpp"
#include <array>
#include <span>

/*
	SRS+ Kick Table (Matches TETR.IO)
	Reference: https://tetrio.wiki.gg/wiki/Mechanics
*/
namespace SRSP {
	struct offset {
		int8_t x = 0;
		int8_t y = 0;
	};

	using kick5 = std :: array<std :: array<offset, 5>, 4>;
	using kick6 = std :: array<std :: array<offset, 6>, 4>;

	constexpr kick5 KICK_CW_JLSTZ = {
		offset{0, 0}, {- 1, 0}, {- 1, + 1}, {0, - 2}, {- 1, - 2},
		offset{0, 0}, {+ 1, 0}, {+ 1, - 1}, {0, + 2}, {+ 1, + 2},
		offset{0, 0}, {+ 1, 0}, {+ 1, + 1}, {0, - 2}, {+ 1, - 2},
		offset{0, 0}, {- 1, 0}, {- 1, - 1}, {0, + 2}, {- 1, + 2},
	};
	constexpr kick5 KICK_CCW_JLSTZ = {
		offset{0, 0}, {+ 1, 0}, {+ 1, + 1}, {0, - 2}, {+ 1, - 2},
		offset{0, 0}, {+ 1, 0}, {+ 1, - 1}, {0, + 2}, {+ 1, + 2},
		offset{0, 0}, {- 1, 0}, {- 1, + 1}, {0, - 2}, {- 1, - 2},
		offset{0, 0}, {- 1, 0}, {- 1, - 1}, {0, + 2}, {- 1, + 2},
	};
	constexpr kick5 KICK_CW_I = {
		offset{0, 0}, {+ 1, 0}, {- 2, 0}, {- 2, - 1}, {+ 1, + 2},
		offset{0, 0}, {- 1, 0}, {+ 2, 0}, {- 1, + 2}, {+ 2, - 1},
		offset{0, 0}, {+ 2, 0}, {- 1, 0}, {+ 2, + 1}, {- 1, - 2},
		offset{0, 0}, {+ 1, 0}, {- 2, 0}, {+ 1, - 2}, {- 2, + 1},
	};
	constexpr kick5 KICK_CCW_I = {
		offset{0, 0}, {- 1, 0}, {+ 2, 0}, {+ 2, - 1}, {- 1, + 2},
		offset{0, 0}, {- 1, 0}, {+ 2, 0}, {- 1, - 2}, {+ 2, + 1},
		offset{0, 0}, {- 2, 0}, {+ 1, 0}, {- 2, + 1}, {+ 1, - 2},
		offset{0, 0}, {+ 1, 0}, {- 2, 0}, {+ 1, + 2}, {- 2, - 1},
	};
	constexpr kick6 KICK_180 = {
		offset{0, 0}, {0, + 1}, {+ 1, + 1}, {- 1, + 1}, {+ 1, 0}, {- 1, 0},
		offset{0, 0}, {+ 1, 0}, {+ 1, + 2}, {+ 1, + 1}, {0, + 2}, {0, + 1},
		offset{0, 0}, {0, - 1}, {- 1, - 1}, {+ 1, - 1}, {- 1, 0}, {+ 1, 0},
		offset{0, 0}, {- 1, 0}, {- 1, + 2}, {- 1, + 1}, {0, + 2}, {0, + 1},
	};

	enum class Rotation : uint8_t {
		CW, CCW, R180
	};

	constexpr std :: span<const offset> get_kicks(Piece piece, Rotation rotation, int r) {
		if (piece == Piece :: O || piece == Piece :: EMPTY) {
			return {};
		}
		if (rotation == Rotation :: R180) {
			return KICK_180[r];
		}
		if (rotation == Rotation :: CW) {
			return (piece == Piece :: I) ?
				KICK_CW_I[r] : KICK_CW_JLSTZ[r];
		}
		if (rotation == Rotation :: CCW) {
			return (piece == Piece :: I) ?
				KICK_CCW_I[r] : KICK_CCW_JLSTZ[r];
		}
		return {};
	}
}

#endif //KEROSHIZEROTETRIS_KICK_HPP
