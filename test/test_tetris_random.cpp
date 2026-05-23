//
// Created by keroshi on 2026/5/3.
//

#include "include/tetris.h"

#include <thread>
#include <chrono>

Tetris tetris;

void print() {
	std :: cout << "\033[2J\033[H" << std :: flush;
	for (int y = 24; y >= 0; y --) {
		for (int x = 0; x < 10; x ++) {
			int o = tetris.b[y] >> x & 1;
			std :: cout << ".#"[o];
		}
		std :: cout << std :: endl;
	}
	std :: cout << "Clear: " << tetris.last_placement.clear << std :: endl;
	std :: this_thread :: sleep_for(std :: chrono :: milliseconds(1000));
}

int test(bool if_print = false) {
	static std :: mt19937 rnd(std :: random_device{}());

	tetris = Tetris();
	int clears = 0;

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

	// test(true);

	int cnt = 0, sum = 1e3;
	for (int i = 0; i < sum; i ++) {
		int c = test();
		if (c) cnt ++;
	}
	std :: cout << cnt << " / " << sum << std :: endl;

	return 0;
}