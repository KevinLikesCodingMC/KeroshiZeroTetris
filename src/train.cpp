//
// Created by keroshi on 2026/6/6.
//

#include <include/train.h>

#include <filesystem>

#include "include/converter.h"

TrainContext :: TrainContext(
	const std :: string & model_path,
	const std :: string & opt_path,
	bool cuda, bool output
) {
	model = ModelLoader :: load(model_path, cuda, output);

	std :: vector<torch :: Tensor> params;
	for (at :: Tensor p : model.parameters()) {
		(void) p.set_requires_grad(true);
		params.push_back(p);
	}

	device = params.begin() -> device();

	optimizer = std :: make_unique<torch :: optim :: Adam>(
		params,
		torch :: optim :: AdamOptions(1e-4)
	);

	if (opt_path.empty() || ! std :: filesystem :: exists(opt_path)) {
		if (output) {
			Message :: log(Message :: INFO, true,
				"Initialized default optimizer.");
		}
	}
	else {
		try {
			torch :: load(* optimizer, opt_path);

			if (output) {
				Message :: log(Message :: INFO, true,
					"optimizer<", opt_path, "> loaded.");
			}
		}
		catch (const c10 :: Error & e) {
			if (output) {
				Message :: log(Message :: ERROR, true,
					"optimizer<", opt_path, "> loading failed.");
				Message :: log(Message :: ERROR, true,
					"Error: ", e.what());
				Message :: log(Message :: WARNING, true,
					"Initializing default optimizer.");
			}

			optimizer = std :: make_unique<torch :: optim :: Adam>(
				params,
				torch :: optim :: AdamOptions(1e-4)
			);
		}
	}

	model.eval();
}

float TrainContext :: train(const std :: vector<TetrisTrainData> & data) {

	auto board_t = Converter :: to_board(data);
	auto seq_t = Converter :: to_seq(data);
	auto info_t = Converter :: to_info(data);
	auto mask_t = Converter :: to_mask(data);
	auto V = Converter :: to_V(data);
	auto P = Converter :: to_P(data);

	board_t = board_t.to(device);
	seq_t = seq_t.to(device);
	info_t = info_t.to(device);
	mask_t = mask_t.to(device);
	V = V.to(device);
	P = P.to(device);

	model.train();
	optimizer -> zero_grad();

	auto output = model.forward({
		board_t,
		seq_t,
		info_t,
		mask_t
	}).toTuple();

	auto V_out = output -> elements()[0].toTensor();
	auto P_out = output -> elements()[1].toTensor();

	auto V_loss = torch :: mse_loss(V_out, V);

	auto P_log = torch :: log_softmax(P_out, 1);
	auto P_loss = - (P * P_log).sum(1).mean();

	auto loss = V_loss + P_loss;
	loss.backward();
	optimizer -> step();

	return loss.item<float>();
}

std :: pair<std :: vector<float>, std :: vector<std :: vector<float>>>
TrainContext :: predict_batch
(std :: vector<Tetris> & t) {
	auto board_t = Converter :: to_board(t);
	auto seq_t = Converter :: to_seq(t);
	auto info_t = Converter :: to_info(t);
	auto mask_t = Converter :: to_mask(t);

	board_t = board_t.to(device);
	seq_t = seq_t.to(device);
	info_t = info_t.to(device);
	mask_t = mask_t.to(device);

	model.eval();
	torch :: NoGradGuard no_grad;

	auto output = model.forward({
		board_t,
		seq_t,
		info_t,
		mask_t
	}).toTuple();

	auto V_out = output -> elements()[0].toTensor();
	auto P_out = torch :: softmax(output -> elements()[1].toTensor(), 1);

	auto V_cpu = V_out.to(torch :: kCPU).contiguous();
	auto P_cpu = P_out.to(torch :: kCPU).contiguous();

	int batch = static_cast<int>(t.size());

	std :: vector<float> V(batch);
	std :: vector P(batch, std :: vector(1537, 0.f));

	auto V_acc = V_cpu.accessor<float, 2>();
	for (int I = 0; I < batch; I ++) {
		V[I] = V_acc[I][0];
	}

	auto P_acc = P_cpu.accessor<float, 2>();
	for (int I = 0; I < batch; I ++) {
		for (int i = 0; i < 1537; i ++)
			P[I][i] = P_acc[I][i];
	}

	return {V, P};
}

void TrainContext :: save(const std :: string & name) {
	std :: string model_path = name + ".pt";
	std :: string opt_path = name + ".opt";

	try {
		torch :: jit :: Module model_s = model;
		model_s.to(torch :: kCPU);

		model_s.save(model_path);
		torch :: save(* optimizer, opt_path);

		Message :: log(Message :: INFO, true,
			"Saved!");
	}
	catch(const c10 :: Error & e) {
		Message :: log(Message :: ERROR, true,
			"Saving Failed.");
		Message :: log(Message :: ERROR, true,
			"Error: ", e.what());
	}
}

void TrainContext :: save() {
	save(std :: to_string(std :: time(nullptr)));
}
