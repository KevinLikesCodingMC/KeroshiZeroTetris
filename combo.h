//
// Created by keroshi on 2026/5/3.
//

#ifndef KEROSHIZEROTETRIS_COMBO_H
#define KEROSHIZEROTETRIS_COMBO_H

#include "def.h"

using ComboFunc = std :: function <int(
	int clear, Spin spin, int combo, int b2b, bool pc
)>;

namespace KeroshiCombo {
	inline int stage1(int clear, Spin spin, int combo, int b2b, bool pc) {
		constexpr int table[5] = {
			0, 1, 3, 6, 12
		};
		return table[clear];
	}
};

#endif //KEROSHIZEROTETRIS_COMBO_H
