//
// Created by keroshi on 2026/6/6.
//

#ifndef KEROSHIZEROTETRIS_TRAIN_H
#define KEROSHIZEROTETRIS_TRAIN_H

#include "model_loader.h"
#include "record.h"
#include "tetris.h"

struct TrainContext {
	torch :: jit :: Module model;
	torch :: Device device = torch :: kCPU;
	std :: unique_ptr<torch :: optim :: Adam> optimizer;

	TrainContext(
		const std :: string & model_path,
		const std :: string & opt_path,
		bool cuda = false,
		bool output = true
	);

	std :: pair<float, float>
	train(const std :: vector<TetrisTrainData> & data);

	std :: pair<float, std :: vector<float>>
	predict
	(Tetris & t);

	std :: pair<std :: vector<float>, std :: vector<std :: vector<float>>>
	predict_batch
	(std :: vector<Tetris> & t);

	void save(const std :: string & name);
	void save();
};

#endif //KEROSHIZEROTETRIS_TRAIN_H
