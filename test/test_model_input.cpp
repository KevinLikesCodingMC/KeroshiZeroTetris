//
// Created by keroshi on 2026/5/10.
//

#include "tetris.h"
#include "converter.h"
#include "model_loader.h"

int main(int argc, char * argv []) {
	if (argc < 2) {
		std :: cout << "Usage: "
					<< argv[0]
					<< " <model.pt>"
					<< std :: endl;
		return 1;
	}

	std :: string model_path = argv[1];

	auto model = ModelLoader :: load(model_path, true);

	Tetris tetris;

	auto device = (* model.parameters().begin()).device();

	std :: cout << device << std :: endl;

	at :: Tensor board_t = InputConverter :: to_board(tetris).to(device);
	at :: Tensor seq_t = InputConverter :: to_seq(tetris).to(device);
	at :: Tensor info_t = InputConverter :: to_info(tetris).to(device);
	at :: Tensor pos_t = InputConverter :: to_pos(tetris).to(device);
	at :: Tensor offset_t = InputConverter :: to_offset(pos_t).to(device);

	std :: cout << board_t << std :: endl;
	std :: cout << seq_t << std :: endl;
	std :: cout << info_t << std :: endl;
	std :: cout << pos_t << std :: endl;
	std :: cout << offset_t << std :: endl;

	auto output = model.forward({
		board_t,
		seq_t,
		info_t,
		pos_t,
		offset_t
	}).toTuple();

	at :: Tensor value_t = output -> elements()[0].toTensor();
	at :: Tensor policy_t = output -> elements()[1].toTensor();


	std :: cout << value_t << std :: endl;
	std :: cout << policy_t << std :: endl;

	return 0;
}
