//
// Created by keroshi on 2026/8/12.
//

#include "include/ai/net.hpp"
#include "include/utils/timer.hpp"

void usage(const char * prg) {
	std :: cout << "Usage:" << std :: endl;
	std :: cout << prg << " <model_path> <cuda> <batch>" << std :: endl;
}

int main(int argc, char * argv []) {
	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

	std :: string model_path = argv[1];
	bool cuda = std :: stoi(argv[2]) != 0;
	int batch = std :: stoi(argv[3]);

	TetrisValueNet net(model_path, cuda);

	std :: vector<TetrisEnv> tetris(batch);
	for (int i = 0; i < batch; i ++) {
		tetris[i].set_rest_pieces(i);
	}

	float sum = 0;
	for (int i = 0; i < 10; i ++) {
		auto output = net.predict(tetris);
		sum += output[0];
	}
	Logger :: info("Warmup ", sum);

	int n = 10;
	Timer timer;
	for (int i = 0; i < n; i ++) {
		auto output = net.predict(tetris);
		sum += output[0];
	}
	double elapsed = timer.elapsed_us() / n;
	Logger :: info("Elapsed ", elapsed, " us");
	Logger :: info("Sample ", elapsed / batch, " us");

	return 0;
}