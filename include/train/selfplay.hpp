//
// Created by keroshi on 2026/8/13.
//

#ifndef KEROSHIZEROTETRIS_SELFPLAY_HPP
#define KEROSHIZEROTETRIS_SELFPLAY_HPP

#include "include/ai/dataset.hpp"
#include "include/ai/net.hpp"

namespace Training {
	namespace Value {
		using Data = Dataset :: player_value;

		struct selfplay_result {
			double total_elapse_ms;
			double net_elapse_ms;
			double mcts_elapse_ms;

			std :: vector<float> V;
			std :: vector<Dataset :: player_value> dataset;
		};

		selfplay_result selfplay_single(TetrisValueTrainNet & net);
	}
}

#endif //KEROSHIZEROTETRIS_SELFPLAY_HPP
