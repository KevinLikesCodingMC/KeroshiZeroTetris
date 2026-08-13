//
// Created by keroshi on 2026/8/13.
//

#include "include/ai/net.hpp"
#include "include/utils/timer.hpp"

void usage(const char * prg) {
	std :: cout << "Usage:" << std :: endl;
	std :: cout << prg << " <model_path> <output_path>" << std :: endl;
}

int main(int argc, char * argv []) {
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}

	std :: string model_path = argv[1];
	std :: string output_path = argv[2];

	std :: ofstream ofs(output_path);
	if (! ofs) {
		Logger :: error("Failed to open ", output_path);
		throw std :: runtime_error("File Error.");
	}

	std :: vector batches = {
		1, 2, 4, 8, 16, 32, 64,
		128, 256, 512, 1024,
		2048, 4000, 10000,
	};

	ofs << "batch,elapse_us,sample_us" << std :: endl;

	for (int batch : batches) {
		TetrisValueNet net(model_path, true);

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

		ofs << batch << "," << elapsed << "," << elapsed / batch << std :: endl;

	}

	return 0;
}