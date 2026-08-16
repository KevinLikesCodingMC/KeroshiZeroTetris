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


	selfplay_result selfplay_batch(
		TetrisValueTrainNet & net,
		const training_config & config
	) {
		Timer total_timer;
		Timer timer;

		double net_elapse_ms = 0;
		double mcts_elapse_ms = 0;

		int batch = config.mcts_batch;

		std :: vector<TetrisEnv> tetris(batch);
		for (int I = 0; I < batch; I ++) {
			tetris[I].set_rest_pieces(20);
		}

		auto get_V = [&] (TetrisEnv & t) {
			float res = t.get_attack();
			if (t.is_game_over()) {
				res -= 20.f;
			}
			return res;
		};

		auto predict_batch_V = [&] (std :: vector<TetrisEnv> & t) {
			timer.reset();
			std :: vector<float> V = net.predict(t);
			net_elapse_ms += timer.elapsed_ms();

			for (int I = 0; I < batch; I ++) {
				if (t[I].is_over()) {
					V[I] = get_V(t[I]);
				}
				else {
					V[I] += t[I].get_attack();
				}
			}

			return V;
		};

		std :: vector<std :: vector<Data>> tetris_data(batch);
		std :: vector<TetrisMCTS> mcts(batch);
		for (int I = 0; I < batch; I ++) {
			mcts[I].set_C(config.mcts_C);
			mcts[I].set_root(tetris[I]);
		}
		int simu = config.mcts_simu;

		selfplay_result res {};

		auto add_dataset = [&] (int I) {
			float V = get_V(tetris[I]);
			res.V.push_back(V);
			for (auto & data : tetris_data[I]) {
				data.V = V - data.V;
				res.dataset.push_back(data);
			}
		};

		std :: vector<TetrisEnv> t_batch(batch);

		bool finished = false;
		while (! finished) {
			{
				auto V = predict_batch_V(tetris);
				for (int I = 0; I < batch; I ++) {
					mcts[I].set_V_base(V[I]);
				}
			}

			for (int k = 0; k < simu; k ++) {
				if (k == 1) {
					for (int I = 0; I < batch; I ++) {
						if (tetris[I].is_over()) continue;
						mcts[I].noise();
					}
				}

				timer.reset();
				std :: vector<TetrisMCTS :: Node *> nodes (batch, nullptr);
				for (int I = 0; I < batch; I ++) {
					if (tetris[I].is_over()) continue;
					auto [t, node] = mcts[I].select();
					t_batch[I] = t;
					nodes[I] = node;
				}
				mcts_elapse_ms += timer.elapsed_ms();

				auto V = predict_batch_V(t_batch);
				timer.reset();
				for (int I = 0; I < batch; I ++) {
					if (tetris[I].is_over()) continue;
					mcts[I].back(nodes[I], V[I]);
				}
				mcts_elapse_ms += timer.elapsed_ms();
			}

			for (int I = 0; I < batch; I ++) {
				if (tetris[I].is_over()) continue;

				auto data = Converter :: to_dataset_value(tetris[I]);
				data.V = tetris[I].get_attack();
				tetris_data[I].push_back(data);

				timer.reset();
				auto [id, action] = mcts[I].temp_action(0.2);
				mcts_elapse_ms += timer.elapsed_ms();

				tetris[I].step(action);
				mcts[I].step(id);

				if (tetris[I].is_over()) {
					add_dataset(I);
				}
			}

			finished = true;
			for (int I = 0; I < batch; I ++) {
				if (! tetris[I].is_over()) {
					finished = false;
					break;
				}
			}
		}

		res.total_elapse_ms = total_timer.elapsed_ms();
		res.net_elapse_ms = net_elapse_ms;
		res.mcts_elapse_ms = mcts_elapse_ms;

		return res;
	}
}
