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
		std :: vector<Data> selfplay(TetrisValueTrainNet & net);
	}
}

#endif //KEROSHIZEROTETRIS_SELFPLAY_HPP
