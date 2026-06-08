//
// Created by keroshi on 2026/6/6.
//

#include "include/train.h"
#include "include/buffer.h"
#include "include/converter.h"
#include "include/mcts.h"
#include "include/tetris.h"

/*
trainer
<model path>
<optimizer path | empty>
<data path>
[game number]
[simu number]
[thread number]
[sample number]
*/

bool is_end(const Tetris & t) {
	return t.game_over || t.pieces >= 35;
}

float get_V(const Tetris & t) {
	return t.attack;
}

float get_ex_V(const Tetris & t) {
	int top = 30;
	for (int y = 29; y >= 0; y --) {
		if (t.b[y]) break;
		top = y;
	}
	return (20 - top) * 0.7f;
}

int main(int argc, char * argv []) {
	if (argc < 4) {
		std :: cout << "Usage: " << argv[0]
					<< " <model path>"
					<< " <optimizer path | empty>"
					<< " <data path>"
					<< " [game number: 10]"
					<< " [simu number: 100]"
					<< " [thread number: 1]"
					<< " [sample number: 32]"
					<< std :: endl;
		return 1;
	}

	std :: string model_path = argv[1];
	std :: string optimizer_path = argv[2];
	if (optimizer_path == "empty") {
		optimizer_path = "";
	}
	std :: string data_path = argv[3];

	int game_number = 10;
	int simu_number = 100;
	int thread_number = 1;
	int sample_number = 32;

	try {
		if (argc > 4) game_number = std :: stoi(argv[4]);
		if (argc > 5) simu_number = std :: stoi(argv[5]);
		if (argc > 6) thread_number = std :: stoi(argv[6]);
		if (argc > 7) sample_number = std :: stoi(argv[7]);
	}
	catch (const std :: exception & e) {
		Message :: log(Message :: ERROR, true,
			e.what()
		);
		std :: exit(EXIT_FAILURE);
	}

	int batch = thread_number;
	int target = game_number;
	int sample = sample_number;
	int simu = simu_number;

	TrainContext trainer(
		model_path,
		optimizer_path,
		true, true
	);

	TetrisBuffer buffer(data_path, 10000, 100000);

	std :: vector<Tetris> tetris(batch);
	std :: vector<std :: vector<TetrisTrainData>> train_data(batch);

	int games = 0;
	while (games < target) {

		std :: vector<MCTS<Tetris>> mcts(batch);
		std :: vector<float> fst(batch);
		{
			std :: vector<Tetris> g(batch);
			for (int I = 0; I < batch; I ++) {
				g[I] = tetris[I];
				mcts[I].root_game = tetris[I];
				mcts[I].root_game.depleted = true;
			}

			auto [V, P] = trainer.predict_batch(g);

			fst = V;
			for (int I = 0; I < batch; I ++) {
				fst[I] += get_ex_V(g[I]);
			}
		}

		for (int _ = 0; _ < simu; _ ++) {
			std :: vector<Tetris> g(batch);
			for (int I = 0; I < batch; I ++) {
				g[I] = mcts[I].playout_select();
			}
			auto [V, P_t] = trainer.predict_batch(g);
			for (int I = 0; I < batch; I ++) {
				float Q = get_ex_V(g[I]) - fst[I];
				if (is_end(g[I])) {
					Q += get_V(g[I]);
				}
				else {
					Q += V[I];
				}

				if (g[I].is_leaf()) {
					mcts[I].playout_back(Q, {});
				}
				else {
					int n = g[I].legal().size();

					if (n > 128) {
						mcts[I].playout_back(Q, std :: vector<float>(1.f / n));
					}
					else {
						auto P = P_t[I];
						P.resize(n);
						mcts[I].playout_back(Q, P);
					}
				}
			}
		}

		for (int I = 0; I < batch; I ++) {
			int id = mcts[I].get_best();
			int u = mcts[I].root -> a[id];

			auto data = Converter :: to_train_data(tetris[I], mcts[I].get_P());
			train_data[I].push_back(data);

			tetris[I].step(u);

			if (is_end(tetris[I])) {

				double V = get_V(tetris[I]);
				for (auto & h : train_data[I]) h.V = V;

				buffer.add_game(train_data[I]);

				train_data[I].clear();

				if (buffer.tot >= 50) {
					std :: vector<TetrisTrainData> samples;
					for (int _ = 0; _ < sample; _ ++) {
						samples.push_back(buffer.sample());
					}

					float loss = trainer.train(samples);
					Message :: log(Message :: INFO, true,
						"Loss: ", loss, " | Attack: ", tetris[I].attack
					);
				}
				else {
					Message :: log(Message :: INFO, true,
						"Attack: ", tetris[I].attack
					);
				}

				tetris[I] = Tetris();
				games ++;
			}

			if (games == target) break;
		}
	}

	trainer.save();

	return 0;
}
