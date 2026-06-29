//
// Created by Administrator on 2026/6/9.
//

#ifndef KEROSHIZEROTETRIS_PREDICT_H
#define KEROSHIZEROTETRIS_PREDICT_H

#include "model_loader.h"
#include "tetris.h"

struct Predictor {
	torch :: jit :: Module model;
	torch :: Device device = torch :: kCPU;

	Predictor(
		const std :: string & model_path,
		bool cuda = false,
		bool output = true
	);

	std :: pair<float, std :: vector<float>>
	predict(Tetris & t);

	std :: pair<std :: vector<float>, std :: vector<std :: vector<float>>>
	predict_batch
	(std :: vector<Tetris> & t);
};

#endif //KEROSHIZEROTETRIS_PREDICT_H