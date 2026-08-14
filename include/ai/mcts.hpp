//
// Created by keroshi on 2026/8/3.
//

#ifndef KEROSHIZEROTETRIS_MCTS_HPP
#define KEROSHIZEROTETRIS_MCTS_HPP

#include "include/core/tetris.hpp"

#include <vector>
#include <memory>

class TetrisMCTS {
public:
	struct Node {
		bool player;

		int n = 0;
		std :: vector<int> actions;
		std :: vector<std :: unique_ptr<Node>> children;

		Node * parent = nullptr;
		int from_action = - 1;

		int sum_N = 0;
		std :: vector<int> N;
		std :: vector<float> P, Q;
	};

	TetrisMCTS() = default;

	void set_C(float c);
	void set_root(const TetrisEnv & tetris);
	Node * get_root();
	std :: vector<int> get_root_N();
	void set_V_base(float V);

	std :: pair<TetrisEnv, Node *> select();

	std :: vector<std :: pair<TetrisEnv, Node *>> select_layer();

	void back(Node * pos, float V);

	void noise(float alpha = 0.3f, float epsilon = 0.25f);

	std :: pair<int, int> best_action();
	std :: pair<int, int> temp_action(float temp = 1.0);

	void step(int u);

private:
	float C = 1;
	float V_base = 0;

	std :: unique_ptr<Node> root;
	TetrisEnv root_tetris;

	int node_select(const Node * node);

	void expand(Node * parent, int u, TetrisEnv & tetris);

};


#endif //KEROSHIZEROTETRIS_MCTS_HPP
