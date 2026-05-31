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

struct TetrisBuffer {
	std :: filesystem :: path path;

	int g;   // data per file
	int w;   // buffer size
	int tot; // data size
	int lst_file; // last file id
	int lst_game; // last game id

	TetrisBuffer(
		const std :: string & path_str,
		int g = 1000, int w = 10000,
		bool output = true
	);

	void add_game(const TetrisTrainData & data);
	void add_game(const std :: vector<TetrisTrainData> & data);

};

#endif //KEROSHIZEROTETRIS_BUFFER_H
