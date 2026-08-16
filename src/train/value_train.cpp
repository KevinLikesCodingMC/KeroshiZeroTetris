//
// Created by keroshi on 2026/8/14.
//

#include "include/ai/buffer.hpp"
#include "include/train/selfplay.hpp"
#include "include/train/config.hpp"
#include "include/utils/logger.hpp"
#include "include/utils/timer.hpp"

void usage(const char * prg) {
	std :: cout << "Usage:" << std :: endl;
	std :: cout << prg << " ";
	std :: cout << "<config_path> ";
	std :: cout << std :: endl;
}

int main(int argc, char * argv []) {
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	Timer total_timer;

	std :: string config_path = argv[1];
	auto config = Training :: load_config(config_path);

	if (config.optimizer_path == "empty") {
		config.optimizer_path = "";
	}

	TetrisValueTrainNet net(
		config.model_path,
		config.optimizer_path,
		config.cuda
	);
	ValueBuffer buffer(config.buffer_path);

	std :: filesystem :: path output_path = config.output_path;

	std :: vector<float> losses;
	std :: vector<float> V_list;

	double selfplay_elapse_s = 0;
	double train_elapse_s = 0;
	double buffer_elapse_s = 0;
	double net_elapse_s = 0;
	double mcts_elapse_s = 0;
	Timer timer;

	for (int game = 0; game < config.total_games; game ++) {
		auto res = Training :: Value :: selfplay_single(net, config);

		timer.reset();
		buffer.add(res.dataset);
		auto samples
			= buffer.sample_random(
			config.window_size,
			config.sample_per_game
			);
		buffer_elapse_s += timer.elapsed_s();

		timer.reset();
		float loss = net.train(samples);
		train_elapse_s += timer.elapsed_s();

		net_elapse_s += res.net_elapse_ms / 1000;
		mcts_elapse_s += res.mcts_elapse_ms / 1000;
		selfplay_elapse_s += res.total_elapse_ms / 1000;

		losses.push_back(loss);
		V_list.push_back(res.V[0]);
	}

	std :: filesystem :: create_directories(output_path);

	net.save(output_path / "model");

	double total_elapse_s = total_timer.elapsed_s();

	std :: ofstream report_ofs(output_path / "results.txt");
	report_ofs << "total: " << total_elapse_s << " s" << std :: endl;
	report_ofs << "selfplay: " << selfplay_elapse_s << " s" << std :: endl;
	report_ofs << "mcts: " << mcts_elapse_s << " s" << std :: endl;
	report_ofs << "net: " << net_elapse_s << " s" << std :: endl;
	report_ofs << "train: " << train_elapse_s << " s" << std :: endl;
	report_ofs << "buffer: " << buffer_elapse_s << " s" << std :: endl;
	report_ofs.close();

	std :: ofstream loss_ofs(output_path / "loss.txt");
	for (auto x : losses) loss_ofs << x << '\n';
	loss_ofs.close();

	std :: ofstream V_ofs(output_path / "V.txt");
	for (auto x : V_list) V_ofs << x << '\n';
	V_ofs.close();

	Logger :: info("Saved.");

	return 0;
}