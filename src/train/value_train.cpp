//
// Created by keroshi on 2026/8/14.
//

#include "include/ai/buffer.hpp"
#include "include/train/selfplay.hpp"
#include "include/utils/logger.hpp"

#include <random>

void usage(const char * prg) {
	std :: cout << "Usage:" << std :: endl;
	std :: cout << prg << " ";
	std :: cout << "<model_path> ";
	std :: cout << "<cuda> ";
	std :: cout << "<optimizer_path> ";
	std :: cout << "<buffer_path> ";
	std :: cout << "<output_path> ";
	std :: cout << "<game> ";
	std :: cout << "<sample> ";
	std :: cout << std :: endl;
}

int main(int argc, char * argv []) {
	if (argc < 8) {
		usage(argv[0]);
		return 1;
	}

	std :: string model_path = argv[1];
	bool cuda = std :: stoi(argv[2]) != 0;
	std :: string optimizer_path = argv[3];
	std :: string buffer_path = argv[4];
	std :: string output_path = argv[5];
	int game_num = std :: stoi(argv[6]);
	int sample_num = std :: stoi(argv[7]);

	if (optimizer_path == "empty") optimizer_path = "";

	TetrisValueTrainNet net(model_path, optimizer_path, cuda);
	ValueBuffer buffer(buffer_path);

	auto get_samples = [&] () {
		std :: vector<Dataset :: player_value> samples;
		if (buffer.get_size() == 0) {
			return samples;
		}

		static std :: mt19937 rnd(std :: random_device{} ());

		int siz = 100000;
		int l = std :: max(0, buffer.get_size() - siz);
		std :: uniform_int_distribution dist(l, buffer.get_size() - 1);

		for (int i = 0; i < sample_num; i ++) {
			int u = dist(rnd);
			samples.push_back(buffer.get(u));
		}
		return samples;
	};

	for (int game = 0; game < game_num; game ++) {
		auto data = Training :: Value :: selfplay(net);
		buffer.add(data);
		auto loss = net.train(get_samples());
		Logger :: info("Loss: ", loss);
	}

	net.save(output_path);

	return 0;
}