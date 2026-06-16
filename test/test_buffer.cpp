//
// Created by keroshi on 2026/5/31.
//

#include "include/buffer.h"
#include <iostream>

int main() {
	TetrisBuffer buffer("test/buffer");

	TetrisTrainData data {};

	data.len = 10;
	data.P.resize(data.len);
	data.u.resize(data.len);

	data.u[0] = 114;

	buffer.add_game({data, data});

	data = buffer.sample();

	std :: cout << data.len << std :: endl;
	std :: cout << int(data.u[0]) << std :: endl;

	return 0;
}
