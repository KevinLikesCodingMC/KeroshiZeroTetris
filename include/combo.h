//
// Created by keroshi on 2026/5/3.
//

#ifndef KEROSHIZEROTETRIS_COMBO_H
#define KEROSHIZEROTETRIS_COMBO_H

#include "def.h"

#include <functional>

using ComboFunc = std :: function <float(
	int clear, Spin spin, int combo, int b2b, bool pc
)>;

namespace KeroshiCombo {
	float stage1(int clear, Spin spin, int combo, int b2b, bool pc);
	float stage2(int clear, Spin spin, int combo, int b2b, bool pc);
};

#endif //KEROSHIZEROTETRIS_COMBO_H
