//
// Created by keroshi on 2026/8/3.
//

#include "include/ai/mcts.hpp"

#include <cmath>


void TetrisMCTS :: set_C(float c) {
	C = c;
}

void TetrisMCTS :: set_root(const TetrisEnv & tetris) {
	root_tetris = tetris;
}

std :: vector<int> TetrisMCTS :: get_root_N() {
	if (root == nullptr) {
		return {};
	}
	return root -> N;
}

int TetrisMCTS :: node_select(const Node * node) {
	/*
		Score = Q + C * P * sqrt(sum N) / (N + 1)
	*/

	int id = 0; float res = - 1e18;
	float val = C * std :: sqrt(node -> sum_N);
	float o = node -> player ? 1.f : - 1.f;

	for (int i = 0; i < node -> n; i ++) {
		float score = node -> Q[i] * o
			+ val * node -> P[i] / (node -> N[i] + 1.f);

		if (score > res) {
			res = score;
			id = i;
		}
	}

	return id;
}

TetrisEnv TetrisMCTS :: select() {
	path.clear();
	leaf_tetris = root_tetris;
	Node * cur = root.get();

	while (! leaf_tetris.is_over() && cur != nullptr) {
		int u = node_select(cur);
		path.emplace_back(cur, u);
		leaf_tetris.step(cur -> actions[u]);
		cur = cur -> children[u].get();
	}

	return leaf_tetris;
}

void TetrisMCTS :: back(float V) {
	if (! leaf_tetris.is_over()) {
		Node * node;
		if (path.empty()) {
			root = std :: make_unique<Node>();
			node = root.get();
		}
		else {
			auto [pos, a] = path.back();
			pos -> children[a] = std :: make_unique<Node>();
			node = pos -> children[a].get();
		}

		node -> actions = leaf_tetris.get_actions();
		node -> n = node -> actions.size();
		node -> sum_N = 0;
		node -> children = std :: vector<std :: unique_ptr<Node>>(node -> n);
		node -> P = std :: vector(node -> n, 1.f / node -> n);
		node -> Q = std :: vector(node -> n, 0.f);
		node -> N = std :: vector(node -> n, 0);
	}

	for (int i = int(path.size()) - 1; i >= 0; i --) {
		auto [pos, a] = path[i];
		pos -> N[a] ++;
		pos -> sum_N ++;

		// Q -> (Q * N + V) / (N + 1) = Q + (V - Q) / (N + 1)
		pos -> Q[a] += (V - pos -> Q[a]) / pos -> N[a];
	}
}

std :: pair<int, int> TetrisMCTS :: best_action() {
	if (root == nullptr) {
		int id = 0;
		auto actions = root_tetris.get_actions();
		return {id, actions[id]};
	}

	int id = 0, res = 0;

	for (int i = 0; i < root -> n; i ++) {
		if (root -> N[i] > res) {
			res = root -> N[i];
			id = i;
		}
	}

	return {id, root -> actions[id]};
}