//
// Created by keroshi on 2026/4/27.
//

#ifndef KEROSHIZEROTETRIS_KICK_H
#define KEROSHIZEROTETRIS_KICK_H

namespace SRSP {
	constexpr int KICK_CW_JLSZT[4][5][2] = {
		{{0, 0}, {- 1, 0}, {- 1, + 1}, {0, - 2}, {- 1, - 2}},
		{{0, 0}, {+ 1, 0}, {+ 1, - 1}, {0, + 2}, {+ 1, + 2}},
		{{0, 0}, {+ 1, 0}, {+ 1, + 1}, {0, - 2}, {+ 1, - 2}},
		{{0, 0}, {- 1, 0}, {- 1, - 1}, {0, + 2}, {- 1, + 2}},
	};
	constexpr int KICK_CCW_JLSZT[4][5][2] = {
		{{0, 0}, {+ 1, 0}, {+ 1, + 1}, {0, - 2}, {+ 1, - 2}},
		{{0, 0}, {+ 1, 0}, {+ 1, - 1}, {0, + 2}, {+ 1, + 2}},
		{{0, 0}, {- 1, 0}, {- 1, + 1}, {0, - 2}, {- 1, - 2}},
		{{0, 0}, {- 1, 0}, {- 1, - 1}, {0, + 2}, {- 1, + 2}},
	};
	constexpr int KICK_CW_I[4][5][2] = {
		{{0, 0}, {+ 1, 0}, {- 2, 0}, {- 2, - 1}, {+ 1, + 2}},
		{{0, 0}, {- 1, 0}, {+ 2, 0}, {- 1, + 2}, {+ 2, - 1}},
		{{0, 0}, {+ 2, 0}, {- 1, 0}, {+ 2, + 1}, {- 1, - 2}},
		{{0, 0}, {+ 1, 0}, {- 2, 0}, {+ 1, - 2}, {- 2, + 1}},
	};
	constexpr int KICK_CCW_I[4][5][2] = {
		{{0, 0}, {- 1, 0}, {+ 2, 0}, {+ 2, - 1}, {- 1, + 2}},
		{{0, 0}, {- 1, 0}, {+ 2, 0}, {- 1, - 2}, {+ 2, + 1}},
		{{0, 0}, {- 2, 0}, {+ 1, 0}, {- 2, + 1}, {+ 1, - 2}},
		{{0, 0}, {+ 1, 0}, {- 2, 0}, {+ 1, + 2}, {- 2, - 1}},
	};
	constexpr int KICK_180[4][6][2] = {
		{{0, 0}, {0, + 1}, {+ 1, + 1}, {- 1, + 1}, {+ 1, 0}, {- 1, 0}},
		{{0, 0}, {+ 1, 0}, {+ 1, + 2}, {+ 1, + 1}, {0, + 2}, {0, + 1}},
		{{0, 0}, {0, - 1}, {- 1, - 1}, {+ 1, - 1}, {- 1, 0}, {+ 1, 0}},
		{{0, 0}, {- 1, 0}, {- 1, + 2}, {- 1, + 1}, {0, + 2}, {0, + 1}},
	};

	// 0, I, J, L, O, S, T, Z
	inline const int (* kicks_cw[])[5][2] = {
		nullptr,
		KICK_CW_I,
		KICK_CW_JLSZT,
		KICK_CW_JLSZT,
		nullptr,
		KICK_CW_JLSZT,
		KICK_CW_JLSZT,
		KICK_CW_JLSZT,
	};
	inline const int (* kicks_ccw[])[5][2] = {
		nullptr,
		KICK_CCW_I,
		KICK_CCW_JLSZT,
		KICK_CCW_JLSZT,
		nullptr,
		KICK_CCW_JLSZT,
		KICK_CCW_JLSZT,
		KICK_CCW_JLSZT,
	};
	inline const int (* kicks_180[])[6][2] = {
		nullptr,
		KICK_180,
		KICK_180,
		KICK_180,
		nullptr,
		KICK_180,
		KICK_180,
		KICK_180,
	};
}

#endif //KEROSHIZEROTETRIS_KICK_H
