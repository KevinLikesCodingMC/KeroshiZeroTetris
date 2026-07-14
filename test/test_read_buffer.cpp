//
// Created by keroshi on 2026/7/14.
//

#include "include/buffer.h"
#include <iostream>

int main() {
	TetrisBuffer buffer("buffer/1", 50000, 100000);

	buffer.load_sort_idx();

	for (int i = 0; i < 30; i ++) {
		auto data = buffer.sample_recent();
		std :: cout << data.V << ' ';
	}

	std :: cout << std :: endl;

	for (int i = 0; i < 30; i ++) {
		auto data = buffer.sample_high_V(100);
		std :: cout << data.V << ' ';
	}

	std :: cout << std :: endl;


	return 0;
}
