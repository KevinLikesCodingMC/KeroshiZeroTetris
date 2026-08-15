//
// Created by keroshi on 2026/8/15.
//

#ifndef KEROSHIZEROTETRIS_CONFIG_HPP
#define KEROSHIZEROTETRIS_CONFIG_HPP

#include <string>
#include <yaml-cpp/yaml.h>
#include "include/utils/logger.hpp"

namespace Training {

	struct training_config {
		std :: string model_path = "";
		std :: string optimizer_path = "";
		bool cuda = false;

		float mcts_C = 5;
		int mcts_simu = 100;

		std :: string buffer_path = "";
		int window_size = 100000;
		int sample_per_game = 64;

		int total_games = 1;

		std :: string output_path = "";
	};

	training_config load_config(std :: string config_path);

	namespace yaml {
		template < typename T >
		static void get_val(const YAML :: Node & node, T & x) {
			if (! node || ! node.IsDefined() || node.IsNull()) {
				return;
			}

			try {
				x = node.as<T>();
			} catch (const YAML :: Exception & e) {
				Logger :: error(e.what());
			}
		}
	}

}


#endif //KEROSHIZEROTETRIS_CONFIG_HPP
