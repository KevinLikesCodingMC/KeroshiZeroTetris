//
// Created by keroshi on 2026/5/25.
//

#include "include/buffer.h"

#include <fstream>
#include <random>
#include "include/message.h"
#include "include/record.h"

TetrisBuffer :: TetrisBuffer(
	std :: string const & path_str,
	int g, int w, bool output
) : path(path_str) , g(g) , w(w) {
	auto get_path = [this] (int i, const std :: string & ext) {
		return (path / std :: to_string(i))
				.replace_extension(ext);
	};

	tot = 0; lst_file = - 1;
	lst_game = 0;

	int i = 0;
	while (std :: filesystem :: exists(get_path(i, ".idx")) )
		lst_file = i, i ++;

	if (lst_file == - 1) {
		if (output) {
			Message :: log(Message :: INFO, true,
				"No existing data files detected."
			);
		}
		return;
	}

	auto lst_file_path = get_path(lst_file, ".idx");

	uint64_t lst_sz = std :: filesystem :: file_size(lst_file_path);
	tot = lst_file * g + lst_sz / sizeof(TetrisDataIndex);

	std :: ifstream ifs(lst_file_path, std :: ios :: binary);
	if (! ifs) {
		if (output) {
			Message :: log(Message :: ERROR, true,
				"Failed to open ", lst_file_path
			);
		}
		std :: exit(EXIT_FAILURE);
	}

	if (lst_sz < sizeof(TetrisDataIndex)) {
		if (output) {
			Message :: log(Message :: ERROR, true,
				"Idx corrupted [1]: ", lst_file_path
			);
		}
		std :: exit(EXIT_FAILURE);
	}

	ifs.seekg(- static_cast<int>(sizeof(TetrisDataIndex)), std :: ios :: end);

	TetrisDataIndex lst_idx {};
	if (ifs.read(reinterpret_cast<char *> (& lst_idx), sizeof(TetrisDataIndex))) {
		lst_game = lst_idx.game_id;
	}
	else {
		if (output) {
			Message :: log(Message :: ERROR, true,
				"Idx corrupted [2]: ", lst_file_path
			);
		}
		std :: exit(EXIT_FAILURE);
	}

	if (output) {
		Message :: log(Message :: INFO, true,
			"Dataset loaded",
			" | Files: ", lst_file + 1,
			" | Samples: ", tot,
			" | Games: ", lst_game
		);
	}

	ifs.close();
}

void TetrisBuffer :: add_game(const TetrisTrainData & data) {
	if (tot == (lst_file + 1) * g) lst_file ++;

	auto idx_path = (path / std :: to_string(lst_file)).replace_extension(".idx");
	auto bin_path = (path / std :: to_string(lst_file)).replace_extension(".bin");

	std :: ofstream idx_ofs(idx_path, std :: ios :: binary | std :: ios :: app);
	std :: ofstream bin_ofs(bin_path, std :: ios :: binary | std :: ios :: app);

	if (! idx_ofs || ! bin_ofs) {
		Message :: log(Message :: ERROR, true,
			"Failed to open files."
		);
		std :: exit(EXIT_FAILURE);
	}

	uint64_t offset = bin_ofs.tellp();

	bin_ofs.write(reinterpret_cast<const char *> (data.b), sizeof(data.b));
	bin_ofs.write(reinterpret_cast<const char *> (data.seq), sizeof(data.seq));
	bin_ofs.write(reinterpret_cast<const char *> (data.info), sizeof(data.info));
	bin_ofs.write(reinterpret_cast<const char *> (& data.V), sizeof(data.V));
	bin_ofs.write(reinterpret_cast<const char *> (& data.p), sizeof(data.p));

	uint32_t len = data.len;

	bin_ofs.write(reinterpret_cast<const char *> (data.x.data()), len * sizeof(uint8_t));
	bin_ofs.write(reinterpret_cast<const char *> (data.y.data()), len * sizeof(uint8_t));
	bin_ofs.write(reinterpret_cast<const char *> (data.r.data()), len * sizeof(uint8_t));
	bin_ofs.write(reinterpret_cast<const char *> (data.P.data()), len * sizeof(float));

	TetrisDataIndex idx {};
	idx.off = offset;
	idx.action_len = len;
	idx.game_id = lst_game;

	idx_ofs.write(reinterpret_cast<const char *> (& idx), sizeof(TetrisDataIndex));

	tot ++;

	idx_ofs.close();
	bin_ofs.close();
}

void TetrisBuffer :: add_game(const std :: vector<TetrisTrainData> & data) {
	lst_game ++;
	for (const auto & x : data)
		add_game(x);
}

int linear_distribution(int l, int r) {
	static std :: mt19937 rnd(std :: random_device {}());
	std :: uniform_int_distribution dist(l, r);
	int x = dist(rnd);
	int y = dist(rnd);
	return std :: max(x, y);
}

TetrisTrainData TetrisBuffer :: sample() {
	if (tot == 0) {
		Message :: log(Message :: ERROR, true,
			"Buffer is empty."
		);
		std :: exit(EXIT_FAILURE);
	}

	int l = std :: max(0, tot - w);
	int u = linear_distribution(l, tot - 1);
	int file = u / g, id = u % g;

	auto idx_path = (path / std :: to_string(file)).replace_extension(".idx");
	auto bin_path = (path / std :: to_string(file)).replace_extension(".bin");

	std :: ifstream idx_ifs(idx_path, std :: ios :: binary);
	std :: ifstream bin_ifs(bin_path, std :: ios :: binary);

	if (! idx_ifs || ! bin_ifs) {
		Message :: log(Message :: ERROR, true,
			"Failed to open files."
		);
		std :: exit(EXIT_FAILURE);
	}

	TetrisDataIndex idx {};

	idx_ifs.seekg(id * sizeof(TetrisDataIndex), std :: ios :: beg);
	idx_ifs.read(reinterpret_cast<char *>(& idx), sizeof(TetrisDataIndex));

	TetrisTrainData data {};
	data.len = idx.action_len;

	bin_ifs.seekg(idx.off, std :: ios :: beg);
	bin_ifs.read(reinterpret_cast<char *>(data.b), sizeof(data.b));
	bin_ifs.read(reinterpret_cast<char *>(data.seq), sizeof(data.seq));
	bin_ifs.read(reinterpret_cast<char *>(data.info), sizeof(data.info));
	bin_ifs.read(reinterpret_cast<char *>(& data.V), sizeof(data.V));
	bin_ifs.read(reinterpret_cast<char *>(& data.p), sizeof(data.p));

	data.x.resize(data.len);
	data.y.resize(data.len);
	data.r.resize(data.len);
	data.P.resize(data.len);

	bin_ifs.read(reinterpret_cast<char *>(data.x.data()), data.len * sizeof(uint8_t));
	bin_ifs.read(reinterpret_cast<char *>(data.y.data()), data.len * sizeof(uint8_t));
	bin_ifs.read(reinterpret_cast<char *>(data.r.data()), data.len * sizeof(uint8_t));
	bin_ifs.read(reinterpret_cast<char *>(data.P.data()), data.len * sizeof(float));

	idx_ifs.close();
	bin_ifs.close();

	return data;
}

