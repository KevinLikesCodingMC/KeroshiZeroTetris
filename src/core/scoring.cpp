//
// Created by keroshi on 2026/8/4.
//

#include "include/core/scoring.hpp"

namespace Scoring {
	float line_sq :: calc(const Context & ctx) const {
		return ctx.clear * ctx.clear;
	}
}
