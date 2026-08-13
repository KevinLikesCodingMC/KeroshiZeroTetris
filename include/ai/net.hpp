//
// Created by keroshi on 2026/8/9.
//

#ifndef KEROSHIZEROTETRIS_NET_HPP
#define KEROSHIZEROTETRIS_NET_HPP

#include "include/core/tetris.hpp"
#include "dataset.hpp"

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
	std :: vector<float> predict(std :: vector<TetrisEnv> & tetris);

};

class TetrisValueTrainNet : public TetrisValueNet {
public:
	TetrisValueTrainNet(
		const std :: string & model_path,
		const std :: string & opt_path,
		bool cuda
	);

	float train(const std :: vector<Dataset :: player_value> & data);

	void save(const std :: string & path);

private:
	std :: unique_ptr<torch :: optim :: Adam> optimizer;
};

#endif //KEROSHIZEROTETRIS_NET_HPP
