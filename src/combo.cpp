//
// Created by Administrator on 2026/6/25.
//

#include "include/combo.h"

namespace KeroshiCombo {
	float stage1(int clear, Spin spin, int combo, int b2b, bool pc) {
		constexpr float table[5] = {
			0, 1, 3, 6, 12
		};
		return table[clear];
	}

	float stage2(int clear, Spin spin, int combo, int b2b, bool pc) {
		constexpr float table[5] = {
			0, - 1, - 1, - 1, 12
		};
		return table[clear];
	}
};