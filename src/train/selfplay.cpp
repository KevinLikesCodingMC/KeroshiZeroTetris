//
// Created by keroshi on 2026/8/13.
//

#include "include/train/selfplay.hpp"

#include "include/ai/converter.hpp"
#include "include/ai/mcts.hpp"

namespace Training :: Value {
	std :: vector<Data> selfplay(TetrisValueTrainNet & net) {
		TetrisEnv tetris;
		tetris.set_rest_pieces(20);

		auto get_V = [&] (TetrisEnv & t) {
			float res = t.get_attack();
			if (t.is_game_over()) {
				res -= 20.f;
			}
			return res;
		};

		std :: vector<Data> tetris_data;
		TetrisMCTS mcts;
		mcts.set_C(5);
		mcts.set_root(tetris);

		while (! tetris.is_over()) {

			if (mcts.get_root() == nullptr) {
				auto [t, node] = mcts.select();
				float V = tetris.get_attack() + net.predict(t);
				mcts.back(node, V);
				mcts.set_V_base(V);
			}

			auto res = mcts.select_layer();
			std :: vector<TetrisEnv> tetris_predict;
			for (auto & [t, node] : res) {
				tetris_predict.push_back(t);
			}

			auto Vs = net.predict(tetris_predict);

			for (int i = 0; i < res.size(); i ++) {
				auto & [t, node] = res[i];

				float V = 0;
				if (t.is_over()) {
					V = get_V(t);
				}
				else {
					// net predict the value delta
					V = t.get_attack() + Vs[i];
				}

				mcts.back(node, V);
			}

			auto data = Converter :: to_dataset_value(tetris);
			data.V = tetris.get_attack();
			tetris_data.push_back(data);

			auto [id, action] = mcts.temp_action(0.2);

			tetris.step(action);
			mcts.step(id);
		}

		float V = get_V(tetris);
		for (auto & data : tetris_data) {
			// net predict the value delta
			data.V = V - data.V;
		}

		return tetris_data;
	}
}
