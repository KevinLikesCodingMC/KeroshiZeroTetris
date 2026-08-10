//
// Created by keroshi on 2026/8/10.
//

#ifndef KEROSHIZEROTETRIS_TIMER_HPP
#define KEROSHIZEROTETRIS_TIMER_HPP

#include "logger.hpp"

#include <chrono>

class Timer {
public:
	using clock = std :: chrono :: steady_clock;
	using time_point = clock :: time_point;

	Timer() {
		reset();
	}

	void reset() {
		start = clock :: now();
	}

	double elapsed_us() const {
		auto end = clock :: now();
		return std :: chrono :: duration<double, std :: micro>
			(end - start).count();
	}

	double elapsed_ms() const {
		return elapsed_us() / 1e3;
	}

	double elapsed_s() const {
		return elapsed_us() / 1e6;
	}

	void log_elapsed(const std :: string & msg = "") const {
		double t = elapsed_us();

		if (t < 1e3) {
			Logger :: info(msg, " ", t, " us");
		}
		else if (t < 1e6) {
			Logger :: info(msg, " ", t / 1e3, " ms");
		}
		else {
			Logger :: info(msg, " ", t / 1e6, " s");
		}
	}

private:
	time_point start;
};


#endif //KEROSHIZEROTETRIS_TIMER_HPP
