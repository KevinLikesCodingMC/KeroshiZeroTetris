//
// Created by keroshi on 2026/5/10.
//

#include "include/tetris.h"
#include "include/converter.h"
#include "include/model_loader.h"
#include "include/timer.h"

int main(int argc, char * argv []) {
	if (argc < 2) {
		std :: cout << "Usage: "
					<< argv[0]
					<< " <model.pt>"
					<< "[ output / speed ]"
					<< std :: endl;
		return 1;
	}

	std :: string model_path = argv[1];

	std :: string mode = "output";

	if (argc > 2) mode = argv[2];

	if (mode == "output") {
		Timer timer;

		auto model = ModelLoader :: load(model_path, true);

		timer.elapsed("Load Model:");

		Tetris tetris;

		auto device = (* model.parameters().begin()).device();

		std :: cout << device << std :: endl;

		timer.reset();

		at :: Tensor board_t = Converter :: to_board(tetris).to(device);
		at :: Tensor seq_t = Converter :: to_seq(tetris).to(device);
		at :: Tensor info_t = Converter :: to_info(tetris).to(device);
		at :: Tensor pos_t = Converter :: to_pos(tetris).to(device);

		timer.elapsed("Converter:");

		std :: cout << board_t << std :: endl;
		std :: cout << seq_t << std :: endl;
		std :: cout << info_t << std :: endl;
		std :: cout << pos_t << std :: endl;

		timer.reset();

		auto output = model.forward({
			board_t,
			seq_t,
			info_t,
			pos_t,
		}).toTuple();

		at :: Tensor value_t = output -> elements()[0].toTensor();
		at :: Tensor policy_t = output -> elements()[1].toTensor();

		timer.elapsed("Model Forward:");

		std :: cout << value_t << std :: endl;
		std :: cout << policy_t << std :: endl;
	}

	if (mode == "speed") {
		Timer timer;

		auto model = ModelLoader :: load(model_path, true);

		timer.elapsed("Load Model:");

		Tetris tetris;

		auto device = (* model.parameters().begin()).device();

		std :: cout << device << std :: endl;

		timer.reset();

		at :: Tensor board_t = Converter :: to_board(tetris).to(device);
		at :: Tensor seq_t = Converter :: to_seq(tetris).to(device);
		at :: Tensor info_t = Converter :: to_info(tetris).to(device);
		at :: Tensor pos_t = Converter :: to_pos(tetris).to(device);

		timer.elapsed("Converter:");

		auto predict = [&] () {
			auto output = model.forward({
				board_t,
				seq_t,
				info_t,
				pos_t,
			}).toTuple();

			at :: Tensor value_t = output -> elements()[0].toTensor();

			return value_t[0].item<float>();
		};

		// warm up
		for (int i = 0; i < 5; i ++) predict();

		timer.reset();

		for (int i = 0; i < 10; i ++) {
			std :: cout << predict() << " ";
		}
		std :: cout << std :: endl;

		timer.elapsed("Model Forward:");

	}

	return 0;
}
