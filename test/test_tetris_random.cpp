//
// Created by keroshi on 2026/5/3.
//

#include "include/tetris.h"
#include "include/render.h"
#include "include/timer.h"

#include <thread>
#include <chrono>

Tetris tetris;

int test(bool if_print = false) {
	static std :: mt19937 rnd(std :: random_device{}());

	tetris = Tetris();
	int clears = 0;

	auto print = [&] () {
		TetrisRender :: render(tetris);
		std :: this_thread :: sleep_for(std :: chrono :: milliseconds(1000));
	};

	if (if_print) print();
	while (! tetris.game_over) {
		std :: vector<int> actions = tetris.legal();
		int u = rnd() % size(actions);
		tetris.step(actions[u]);
		clears += tetris.last_placement.clear;
		if (if_print) print();
	}

	return clears;
}

int main(int argc, char * argv []) {
	if (argc < 2) {
		std :: cout << "Usage: "
					<< argv[0]
					<< " [ render | speed ]"
					<< std :: endl;
		return 1;
	}

	std :: string arg(argv[1]);

	if (arg == "render") {
		test(true);
	}
	if (arg == "speed") {
		int cnt = 0, sum = 1e3;
		Timer timer;
		for (int i = 0; i < sum; i ++) {
			int c = test();
			if (c) cnt ++;
		}
		timer.elapsed();
		std :: cout << cnt << " / " << sum << std :: endl;
	}

	return 0;
}