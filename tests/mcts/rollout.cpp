//
// Created by keroshi on 2026/8/3.
//

#include "include/ai/mcts.hpp"
#include "include/ui/tetris_ftxui.hpp"

#include <random>
#include <iostream>

float get_V(TetrisEnv & tetris) {
	float res = tetris.get_attack();
	if (tetris.is_game_over()) {
		res -= 20.f;
	}
	return res;
}

float rollout(TetrisEnv & tetris) {

	if (tetris.is_over()) {
		return get_V(tetris);
	}

	float Q_sum = 0;
	int n = 100;

	static std :: mt19937 rnd(std :: random_device{}());

	for (int i = 0; i < n; i ++) {
		TetrisEnv t = tetris;

		while (! t.is_over()) {
			auto actions = t.get_actions();
			int u = rnd() % actions.size();
			t.step(actions[u]);
		}

		Q_sum += get_V(t);
	}

	return Q_sum / n;
}


int main() {
	TetrisEnv tetris;
	tetris.set_rest_pieces(20);

	int simulation = 200;
	std :: vector<int> actions;

	TetrisMCTS mcts;
	mcts.set_C(5);
	mcts.set_root(tetris);

	while (! tetris.is_over()) {

		float V_base = rollout(tetris);
		mcts.set_V_base(V_base);
		std :: cout << V_base << std :: endl;

		for (int simu = 0; simu < simulation; simu ++ ) {
			auto [t, pos] = mcts.select();
			float V = rollout(t);
			mcts.back(pos, V);
		}

		auto N = mcts.get_root_N();
		for (int x : N) {
			std :: cout << x << ' ';
		}
		std :: cout << std :: endl;

		auto [id, action] = mcts.best_action();
		tetris.step(action);
		mcts.step(id);

		actions.push_back(action);
	}

	std :: cout << actions.size() << std :: endl;
	for (int action : actions) {
		std :: cout << action << ' ';
	}
	std :: cout << std :: endl;

	return 0;
}
