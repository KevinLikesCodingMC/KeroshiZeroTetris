//
// Created by keroshi on 2026/8/3.
//

#include "include/ai/mcts.hpp"

#include <cmath>
#include <random>


void TetrisMCTS :: set_C(float c) {
	C = c;
}

void TetrisMCTS :: set_root(const TetrisEnv & tetris) {
	root_tetris = tetris;
}

void TetrisMCTS :: set_V_base(float V) {
	V_base = V;
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
		float Exploration = val * node -> P[i] / (node -> N[i] + 1.f);
		float Exploitation = (node -> N[i] == 0 ? V_base : node -> Q[i]) * o;

		float score = Exploitation + Exploration;
		if (score > res) {
			res = score;
			id = i;
		}
	}

	return id;
}

void TetrisMCTS :: expand(Node * parent, int u, TetrisEnv & tetris) {

	Node * child;
	if (parent == nullptr) {
		root = std :: make_unique<Node>();
		child = root.get();
		child -> from_action = - 1;
		child -> parent = nullptr;
	}
	else {
		parent -> children[u] = std :: make_unique<Node>();
		child = parent -> children[u].get();
		child -> from_action = u;
		child -> parent = parent;
		tetris.step(parent -> actions[u]);
	}

	if (tetris.is_over()) {
		child -> n = 0;
	}
	else {
		child -> player = tetris.is_player();
		auto actions = tetris.get_actions();
		child -> n = actions.size();
		child -> actions = actions;
		child -> children = std :: vector<std :: unique_ptr<Node>>(child -> n);
		child -> sum_N = 0;
		child -> P = std :: vector(child -> n, 1.f / child -> n);
		child -> Q = std :: vector(child -> n, 0.f);
		child -> N = std :: vector(child -> n, 0);
	}
}

std :: pair<TetrisEnv, TetrisMCTS :: Node *> TetrisMCTS :: select() {

	Node * pos;
	auto tetris = root_tetris;

	if (root == nullptr) {
		expand(nullptr, - 1, tetris);
		pos = root.get();
	}
	else {
		pos = root.get();

		while (! tetris.is_over()) {
			int u = node_select(pos);

			if (pos -> children[u] != nullptr) {
				tetris.step(pos -> actions[u]);
				pos = pos -> children[u].get();
				continue;
			}

			expand(pos, u, tetris);
			pos = pos -> children[u].get();

			break;
		}
	}

	return {tetris, pos};
}

void TetrisMCTS :: back(Node * pos, float V) {
	while (pos -> parent != nullptr) {
		int u = pos -> from_action;
		pos = pos -> parent;
		pos -> N[u] ++;
		pos -> sum_N ++;
		pos -> Q[u] += (V - pos -> Q[u]) / pos -> N[u];
	}
}

void TetrisMCTS :: noise(float alpha, float epsilon) {
	if (root == nullptr) return;

	thread_local  std :: mt19937 rnd(std :: random_device{}());
	std :: gamma_distribution gamma(alpha, 1.f);

	int n = root -> n;
	if (n == 0) return;

	float sum = 0;
	std :: vector<float> p(n);

	for (int i = 0; i < n; i ++) {
		p[i] = gamma(rnd);
		sum += p[i];
	}

	for (int i = 0; i < n; i ++) {
		p[i] /= sum;
		root -> P[i] = (1.f - epsilon) * root -> P[i] + epsilon * p[i];
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

std :: pair<int, int> TetrisMCTS :: temp_action(float temp) {

	if (temp <= 1e-2f) return best_action();

	if (root == nullptr) {
		int id = 0;
		auto actions = root_tetris.get_actions();
		return {id, actions[id]};
	}

	// to softmax
	int n = root -> n;
	std :: vector<float> x(n);
	float x_max = 1e-9;
	for (int i = 0; i < n; i ++) {
		x[i] = std :: logf(root -> N[i] + 1) / temp;
		x_max = std :: max(x_max, x[i]);
	}

	float sum = 0;
	std :: vector<float> p(n);
	for (int i = 0; i < n; i ++) {
		x[i] -= x_max;
		p[i] = std :: expf(x[i]);
		sum += p[i];
	}
	for (int i = 0; i < n; i ++) {
		p[i] /= sum;
	}

	thread_local  std :: mt19937 rnd(std :: random_device{}());
	std :: discrete_distribution dist(p.begin(), p.end());

	int id = dist(rnd);
	return {id, root -> actions[id]};
}

void TetrisMCTS :: step(int u) {
	if (root == nullptr) {
		auto actions = root_tetris.get_actions();
		root_tetris.step(actions[u]);
		return;
	}

	root_tetris.step(root -> actions[u]);
	auto new_root = std :: move(root -> children[u]);
	root = std :: move(new_root);

	if (root != nullptr) {
		root -> from_action = - 1;
		root -> parent = nullptr;
	}
}

