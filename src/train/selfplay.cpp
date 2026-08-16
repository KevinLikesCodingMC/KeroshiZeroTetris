//
// Created by keroshi on 2026/8/13.
//

#include "include/train/selfplay.hpp"
#include "include/ai/converter.hpp"
#include "include/ai/mcts.hpp"
#include "include/utils/timer.hpp"

namespace Training :: Value {
	selfplay_result selfplay_single(
		TetrisValueTrainNet & net,
		const training_config & config
	) {

		Timer total_timer;
		Timer timer;
		double net_elapse_ms = 0;
		double mcts_elapse_ms = 0;

		TetrisEnv tetris;
		tetris.set_rest_pieces(20);

		auto get_V = [&] (TetrisEnv & t) {
			float res = t.get_attack();
			if (t.is_game_over()) {
				res -= 20.f;
			}
			return res;
		};

		auto predict_V = [&] (TetrisEnv & t) {
			if (t.is_over()) return get_V(t);

			timer.reset();
			float V = net.predict(t);
			net_elapse_ms += timer.elapsed_ms();
			return V + t.get_attack();
		};

		std :: vector<Data> tetris_data;
		TetrisMCTS mcts;
		mcts.set_C(config.mcts_C);
		mcts.set_root(tetris);

		int simu = config.mcts_simu;

		while (! tetris.is_over()) {

			mcts.set_V_base(predict_V(tetris));

			for (int k = 0; k < simu; k ++) {
				if (k == 1) mcts.noise();

				timer.reset();
				auto [t, node] = mcts.select();
				mcts_elapse_ms += timer.elapsed_ms();

				float V = predict_V(t);
				timer.reset();
				mcts.back(node, V);
				mcts_elapse_ms += timer.elapsed_ms();

			}

			auto data = Converter :: to_dataset_value(tetris);
			data.V = tetris.get_attack();
			tetris_data.push_back(data);

			timer.reset();
			auto [id, action] = mcts.temp_action(0.2);
			mcts_elapse_ms += timer.elapsed_ms();

			tetris.step(action);
			mcts.step(id);
		}

		float V = get_V(tetris);
		for (auto & data : tetris_data) {
			// net predict the value delta
			data.V = V - data.V;
		}

		selfplay_result res {};
		res.total_elapse_ms = total_timer.elapsed_ms();
		res.net_elapse_ms = net_elapse_ms;
		res.mcts_elapse_ms = mcts_elapse_ms;
		res.dataset = tetris_data;
		res.V = {V};

		return res;
	}
}
