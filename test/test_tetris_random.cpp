//
// Created by keroshi on 2026/5/3.
//

#include "include/tetris.h"
#include "include/render.h"

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

int main() {

	test(true); return 0;

	int cnt = 0, sum = 1e3;
	for (int i = 0; i < sum; i ++) {
		int c = test();
		if (c) cnt ++;
	}
	std :: cout << cnt << " / " << sum << std :: endl;

	return 0;
}