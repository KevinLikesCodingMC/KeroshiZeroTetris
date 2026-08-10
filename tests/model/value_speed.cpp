//
// Created by keroshi on 2026/8/9.
//

#include "include/ai/net.hpp"
#include "include/utils/timer.hpp"

void usage(const char * prg) {
	std :: cout << "Usage:" << std :: endl;
	std :: cout << prg << " <model_path>" << std :: endl;
}

int main(int argc, char * argv []) {
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	std :: string model_path = argv[1];

	TetrisValueNet net(model_path, true);

	TetrisEnv tetris;

	float sum = 0;
	for (int i = 0; i < 10; i ++) {
		sum += net.predict(tetris);
	}
	Logger :: info("Warmup ", sum);

	int n = 10;
	Timer timer;
	for (int i = 0; i < n; i ++) {
		sum += net.predict(tetris);
	}
	double elapsed = timer.elapsed_us() / n;
	Logger :: info("Elapsed ", elapsed, " us");

	return 0;
}