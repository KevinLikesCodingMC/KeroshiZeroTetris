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

	float train(const std :: vector<TetrisTrainData> & data);

	std :: pair<std :: vector<float>, std :: vector<std :: vector<float>>>
	predict_batch
	(std :: vector<Tetris> & t);
};

#endif //KEROSHIZEROTETRIS_TRAIN_H
