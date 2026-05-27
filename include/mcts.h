//
// Created by keroshi on 2026/5/17.
//

#ifndef KEROSHIZEROTETRIS_MCTS_H
#define KEROSHIZEROTETRIS_MCTS_H

#include <memory>
#include <vector>
#include <cmath>

template < typename G >
concept GameState = requires ( G g , int action) {
	{ g.legal() } -> std :: same_as <std :: vector<int>>;
	{ g.step(action) } -> std :: same_as<void>;
	{ g.is_over() } -> std :: same_as<bool>;
	{ g.is_leaf() } -> std :: same_as<bool>;
};

template < GameState Game>
struct MCTS {
	float QR, C;
	struct Node {
		int n = 0;
		std :: vector<int> a;
		std :: vector<std :: unique_ptr<Node>> ch;
		int Sum_N = 0;
		std :: vector<int> N;
		std :: vector<float> P, Q;

		/*
			Score = Q / MaxQ + C * P * sqrt(sum N) / (N + 1)
		*/
		int select(float qr, float c) {
			int id = 0;
			float res = - 1e18;
			float val = c * std :: sqrt(Sum_N);
			for (int i = 0; i < n; i ++) {
				float score = Q[i] / qr + val * P[i] / (N[i] + 1.f);
				if (score > res) res = score, id = i;
			}
			return id;
		}
	};

	std :: unique_ptr<Node> root;
	Game root_game, game;

	std :: vector<std :: pair<Node *, int>> path;

	MCTS(float QR = 1.f, float C = 5.f)
		: QR(QR) , C(C) {
		root = nullptr;
	}

	Game playout_select() {
		game = root_game;
		path.clear();
		Node * cur = root.get();
		while (! game.is_leaf() && cur != nullptr) {
			int u = cur -> select(QR, C);
			path.emplace_back(cur, u);
			game.step(cur -> a[u]);
			cur = cur -> ch[u].get();
		}
		return game;
	}

	void expand(
		std :: unique_ptr<Node> & node,
		Game & g,
		std :: vector<float> P
	) {
		node = std :: make_unique<Node>();
		node -> a = g.legal();
		int n = node -> a.size();
		node -> n = n;
		node -> Sum_N = 0;
		node -> ch = std :: vector<std :: unique_ptr<Node>>(n);
		node -> P = P;
		node -> Q = std :: vector(n, 0.f);
		node -> N = std :: vector(n, 0);
	}

	void playout_back(float V, std :: vector<float> P) {
		if (! game.is_leaf()) {
			if (path.empty()) {
				expand(root, game, P);
			}
			else {
				auto [pos, a] = path.back();
				expand(pos -> ch[a], game, P);
			}
		}
		for (int i = int(path.size()) - 1; i >= 0; i --) {
			auto [pos, a] = path[i];
			pos -> N[a] ++;
			pos -> Q[a] += (V - pos -> Q[a]) / pos -> N[a];
			pos -> Sum_N ++;
		}
	}

	int get_best() {
		if (root_game.is_over()) return - 1;
		int id = 0, res = 0;
		for (int i = 0; i < root -> n; i ++) {
			if (root -> N[i] > res) {
				res = root -> N[i];
				id = i;
			}
		}
		return id;
	}
};

#endif //KEROSHIZEROTETRIS_MCTS_H
