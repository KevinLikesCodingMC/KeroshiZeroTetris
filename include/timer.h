//
// Created by keroshi on 2026/5/30.
//

#ifndef KEROSHIZEROTETRIS_TIMER_H
#define KEROSHIZEROTETRIS_TIMER_H

#include <chrono>

#include "message.h"

struct Timer {
	std :: chrono :: steady_clock :: time_point start;

	Timer() : start(std :: chrono :: steady_clock :: now()) {}

	void reset() {
		start = std :: chrono :: steady_clock :: now();
	}

	void elapsed(const std :: string & info = "") const {
		auto end = std :: chrono :: steady_clock :: now();
		double elapsed = std :: chrono :: duration<double, std :: nano>(end - start).count();

		if (elapsed < 1e3) {
			Message :: log(Message :: INFO, true,
				info, " ", elapsed, " ns"
			);
		}
		else if (elapsed < 1e6) {
			Message :: log(Message :: INFO, true,
				info, " ", elapsed / 1e3, " us"
			);
		}
		else if (elapsed < 1e9) {
			Message :: log(Message :: INFO, true,
				info, " ", elapsed / 1e6, " ms"
			);
		}
		else {
			Message :: log(Message :: INFO, true,
				info, " ", elapsed / 1e9, " s"
			);
		}

	}
};

#endif //KEROSHIZEROTETRIS_TIMER_H
