//
// Created by keroshi on 2026/5/31.
//

#include "include/buffer.h"
#include <iostream>

int main() {
	TetrisBuffer buffer("test/buffer", 3, 10);

	buffer.load_sort_idx();

	for (int i = 0; i < 10; i ++) {
		TetrisTrainData data {};
		data.len = i + 5;
		data.V = 100 - i;
		data.P.resize(data.len);
		data.u.resize(data.len);
		data.u[0] = i;

		buffer.add_game({data});
	}

	{
		auto data = buffer.sample_recent();
		std :: cout << data.len << ' ' << int(data.u[0]) << std :: endl;
	}

	std :: cout << std :: endl;

	for (int i = 0; i < buffer.tot; i ++) {
		auto data = buffer.sample_by_id(i);
		std :: cout << data.len << ' ' << int(data.u[0]) << std :: endl;
	}

	std :: cout << std :: endl;

	for (int i = 0; i < buffer.tot; i ++) {
		auto data = buffer.sample_by_sort_id(i);
		std :: cout << data.len << ' ' << int(data.u[0]) << std :: endl;
	}

	buffer.save_sort_idx();

	return 0;
}
