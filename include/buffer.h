//
// Created by keroshi on 2026/5/25.
//

#ifndef KEROSHIZEROTETRIS_BUFFER_H
#define KEROSHIZEROTETRIS_BUFFER_H

#include <filesystem>

#include "record.h"

struct TetrisDataIndex {
	uint64_t off;
	uint32_t action_len;
	uint32_t game_id;
};

struct SortIndex {
	float key;
	uint32_t idx;

	bool operator < (const SortIndex & rhs) const {
		return key < rhs.key;
	}
	bool operator <= (const SortIndex & rhs) const {
		return key <= rhs.key;
	}
};

struct TetrisBuffer {
	std :: filesystem :: path path;

	int g;   // data per file
	int w;   // buffer size
	int tot; // data size
	int lst_file; // last file id
	int lst_game; // last game id

	std :: vector<SortIndex> sort_index;

	TetrisBuffer(
		const std :: string & path_str,
		int g = 1000, int w = 10000,
		bool output = true
	);

	TetrisTrainData sample_by_id(int u);
	TetrisTrainData sample_by_sort_id(int u);

	void save_sort_idx();
	void load_sort_idx();
	void rebuild_sort_idx();

	void add_game(const TetrisTrainData & data);
	void add_game(const std :: vector<TetrisTrainData> & data);

	TetrisTrainData sample_recent();

	TetrisTrainData sample_high_V(int len = - 1);
};

#endif //KEROSHIZEROTETRIS_BUFFER_H
