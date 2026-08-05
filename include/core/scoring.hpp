//
// Created by keroshi on 2026/8/4.
//

#ifndef KEROSHIZEROTETRIS_SCORING_HPP
#define KEROSHIZEROTETRIS_SCORING_HPP

#include "def.hpp"

namespace Scoring {
	struct Context {
		int clear;
		Spin spin;
		int combo;
		int b2b;
		bool pc;
	};

	class Calculator {
	public:
		virtual ~ Calculator() = default;
		virtual float calc(const Context & ctx) const = 0;
	};

	class line_sq : public Calculator {
		float calc(const Context & ctx) const override;
	};

}


#endif //KEROSHIZEROTETRIS_SCORING_HPP
