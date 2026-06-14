//
// Created by Administrator on 2026/6/9.
//

#include "include/predict.h"
#include "include/converter.h"

Predictor :: Predictor(
	const std :: string & model_path,
	bool cuda, bool output
) {
	model = ModelLoader :: load(model_path, cuda, output);

	std :: vector<torch :: Tensor> params;
	for (at :: Tensor p : model.parameters()) {
		params.push_back(p);
	}
	device = params.begin() -> device();

	model.eval();
}

std :: pair<float, std :: vector<float>>
Predictor :: predict
(Tetris & t) {
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

	auto V_acc = V_cpu.accessor<float, 2>();
	float V = V_acc[0][0];

	auto P_acc = P_cpu.accessor<float, 2>();
	std :: vector P(1537, 0.f);
	for (int i = 0; i < 1537; i ++)
		P[i] = P_acc[0][i];

	return {V, P};
}
