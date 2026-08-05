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
		int n;
		std :: vector<int> actions;
		std :: vector<std :: unique_ptr<Node>> children;
		int sum_N;
		std :: vector<int> N;
		std :: vector<float> P, Q;
	};

	TetrisMCTS() = default;

	void set_C(float c);

	void set_root(const TetrisEnv & tetris);

	std :: vector<int> get_root_N();

	TetrisEnv select();
	void back(float V);

	std :: pair<int, int> best_action();

private:
	float C = 1;
	std :: unique_ptr<Node> root;
	TetrisEnv root_tetris, leaf_tetris;
	std :: vector<std :: pair<Node *, int>> path;

	int node_select(const Node * node);


};


#endif //KEROSHIZEROTETRIS_MCTS_HPP
