//
// Created by keroshi on 2026/8/15.
//

#include "include/train/config.hpp"

namespace Training {
	training_config load_config(std :: string config_path) {
		training_config config;
		YAML :: Node root;

		try {
			root = YAML :: LoadFile(config_path);
		} catch (const YAML :: Exception & e) {
			Logger :: error("Failed to load ", config_path);
			Logger :: error(e.what());
			return config;
		}

		if (root["model"]) {
			yaml :: get_val(root["model"]["path"], config.model_path);
			yaml :: get_val(root["model"]["cuda"], config.cuda);
			yaml :: get_val(root["model"]["optimizer"], config.optimizer_path);
		}

		if (root["mcts"]) {
			yaml :: get_val(root["mcts"]["C"], config.mcts_C);
			yaml :: get_val(root["mcts"]["simu"], config.mcts_simu);
		}

		if (root["buffer"]) {
			yaml :: get_val(root["buffer"]["path"], config.buffer_path);
			yaml :: get_val(root["buffer"]["sample"], config.sample_per_game);
			yaml :: get_val(root["buffer"]["window"], config.window_size);
		}

		if (root["train"]) {
			yaml :: get_val(root["train"]["game"], config.total_games);
			yaml :: get_val(root["train"]["output"], config.output_path);
		}

		return config;
	}
}
