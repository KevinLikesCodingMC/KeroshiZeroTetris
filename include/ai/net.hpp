//
// Created by keroshi on 2026/8/9.
//

#ifndef KEROSHIZEROTETRIS_NET_HPP
#define KEROSHIZEROTETRIS_NET_HPP

#include "include/core/tetris.hpp"

#include <torch/torch.h>
#include <torch/script.h>

class TetrisNet {
public:
	explicit TetrisNet(const std :: string & path, bool cuda);

	torch :: IValue predict
	(const std :: vector<torch :: IValue> & inputs);

protected:
	torch :: jit :: script :: Module module;
	torch :: Device device;
};

class TetrisValueNet : public TetrisNet {
public:
	explicit TetrisValueNet(const std :: string & path, bool cuda);

	torch :: Tensor predict(
		torch :: Tensor board,
		torch :: Tensor piece,
		torch :: Tensor info
	);

	float predict(TetrisEnv & tetris);
};

#endif //KEROSHIZEROTETRIS_NET_HPP
