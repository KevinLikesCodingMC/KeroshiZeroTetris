//
// Created by keroshi on 2026/7/3.
//

#include "include/train.h"
#include "include/buffer.h"

int main(int argc, char * argv []) {

	std :: string buffer_path = "test/model/attack/buffer";
	TetrisBuffer buffer(buffer_path, 10000, 100000);

	std :: string file_name = std :: to_string(std :: time(nullptr)) + ".log";
	if (argc >= 2) file_name = std :: string(argv[1]);

	TrainContext trainer("model/default.pt", "empty");

	std :: ofstream ofs(file_name);

	int epoch = 100;
	for (int i = 0; i < epoch; i ++) {
		std :: vector<TetrisTrainData> sample;
		for (int _ = 0; _ < 512; _ ++) {
			auto data = buffer.sample_recent();
			sample.push_back(data);
		}

		auto [V, P] = trainer.train(sample);
		ofs << V << ' ' << P << std :: endl;
	}

	return 0;
}

