//
// Created by keroshi on 2026/8/9.
//

#include "include/ai/net.hpp"
#include "include/utils/logger.hpp"
#include "include/ai/converter.hpp"

#include <filesystem>


TetrisNet :: TetrisNet(const std :: string & path, bool cuda)
	: device(torch :: kCPU) {

	if (! std :: filesystem :: exists(path)) {
		Logger :: error("model <", path, "> loading failed.");
		throw std :: runtime_error("File Not Found.");
	}

	try {
		module = torch :: jit :: load(path);

		if (cuda && ! torch :: cuda :: is_available()) {
			Logger :: warning("CUDA is not available.");
			cuda = false;
		}

		if (cuda) {
			Logger :: info("Device set to CUDA.");
			device = torch :: kCUDA;
		}
		else {
			Logger :: info("Device set to CPU.");
			device = torch :: kCPU;
		}

		module.to(device);
		module.eval();
	}
	catch (const c10 :: Error & e) {
		Logger :: error("model <", path, "> loading failed.");
		throw std :: runtime_error(e.what());
	}
}

torch :: IValue TetrisNet :: predict(
	const std :: vector<torch :: IValue> & inputs
) {
	module.eval();
	torch :: NoGradGuard no_grad;
	return module.forward(inputs);
}

TetrisValueNet :: TetrisValueNet(const std :: string & path, bool cuda)
	: TetrisNet(path, cuda) {

}

torch :: Tensor TetrisValueNet :: predict(
	torch :: Tensor board,
	torch :: Tensor piece,
	torch :: Tensor info
) {
	auto output = TetrisNet :: predict({
		board, piece, info
	});
	return output.toTensor();
}

float TetrisValueNet :: predict(TetrisEnv & tetris) {
	auto board = Converter :: to_tensor_board(tetris);
	auto piece = Converter :: to_tensor_piece(tetris);
	auto info = Converter :: to_tensor_info(tetris);

	auto output = predict(board, piece, info);

	return output.cpu().item<float>();
}