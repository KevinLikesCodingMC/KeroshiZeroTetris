//
// Created by keroshi on 2026/8/11.
//

#include "include/ai/buffer.hpp"
#include "include/ai/converter.hpp"

int main() {

	ValueBuffer buffer("tests/buffer/0");

	using Data = Dataset :: player_value;
	std :: vector<Data> dataset;

	TetrisEnv tetris;
	for (int i = 0; i < 5; i ++) {
		auto data = Converter :: to_dataset_value(tetris);
		data.V = i;
		dataset.push_back(data);
	}

	buffer.add(dataset);

	for (int i = 0; i < 5; i ++) {
		auto data = buffer.get(i);
		std :: cout << data.V << " ";
	}
	std :: cout << std :: endl;

	return 0;
}
