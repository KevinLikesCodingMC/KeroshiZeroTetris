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

TetrisTrainData TetrisBuffer :: sample_by_id(int u) {
	if (u >= tot) {
		Message :: log(Message :: ERROR, true,
			"id out of range."
		);
		std :: exit(EXIT_FAILURE);
	}

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
	bin_ifs.read(reinterpret_cast<char *>(& data.PW), sizeof(data.PW));

	data.u.resize(data.len);
	data.P.resize(data.len);

	bin_ifs.read(reinterpret_cast<char *>(data.u.data()), data.len * sizeof(uint16_t));
	bin_ifs.read(reinterpret_cast<char *>(data.P.data()), data.len * sizeof(float));

	idx_ifs.close();
	bin_ifs.close();

	return data;
}

TetrisTrainData TetrisBuffer :: sample_by_sort_id(int u) {
	if (u >= tot) {
		Message :: log(Message :: ERROR, true,
			"id out of range."
		);
		std :: exit(EXIT_FAILURE);
	}

	if (sort_index.size() != tot) {
		Message :: log(Message :: WARNING, true,
			"sort.bin is corrupted. Rebuilding...");
		rebuild_sort_idx();
	}

	SortIndex sid = sort_index[u];

	if (sid.idx >= tot) {
		Message :: log(Message :: ERROR, true,
			"sort.bin is corrupted.");
		sid.idx = 0;
	}

	return sample_by_id(sid.idx);
}

void TetrisBuffer :: save_sort_idx() {
	auto sti_path = (path / "sort.bin");
	std :: ofstream sti_ofs(sti_path, std :: ios :: binary);

	if (! sti_ofs) {
		Message :: log(Message :: ERROR, true,
			"Failed to open sort.bin ."
		);
		std :: exit(EXIT_FAILURE);
	}

	uint32_t len = sort_index.size();

	sti_ofs.write(reinterpret_cast<const char *>(sort_index.data()), len * sizeof(SortIndex));

	sti_ofs.close();
}

void TetrisBuffer :: load_sort_idx() {

	if (tot == 0) {
		sort_index.clear();
		return;
	}

	auto sti_path = (path / "sort.bin");
	std :: ifstream sti_ifs(sti_path, std :: ios :: binary);

	if (! sti_ifs) {
		Message :: log(Message :: ERROR, true,
			"Failed to open sort.bin ."
		);
		Message :: log(Message :: WARNING, true,
			"sort.bin is corrupted. Rebuilding...");
		rebuild_sort_idx();
		return;
	}

	uint64_t tot_size = std :: filesystem :: file_size(sti_path);

	if (tot_size % sizeof(SortIndex)) {
		sti_ifs.close();
		Message :: log(Message :: WARNING, true,
			"sort.bin is corrupted. Rebuilding...");
		rebuild_sort_idx();
		return;
	}

	uint32_t len = tot_size / sizeof(SortIndex);

	sort_index.resize(len);

	sti_ifs.read(reinterpret_cast<char *>(sort_index.data()), len * sizeof(SortIndex));
	sti_ifs.close();

	if (! std :: is_sorted(
		sort_index.begin(),
		sort_index.end()
	)) {
		Message :: log(Message :: WARNING, true,
			"sort.bin is corrupted. Rebuilding...");
		rebuild_sort_idx();
	}
}

void TetrisBuffer :: rebuild_sort_idx() {
	sort_index.clear();

	for (int file_id = 0; file_id <= lst_file; file_id ++) {
		auto idx_path = (path / std :: to_string(file_id)).replace_extension(".idx");
		auto bin_path = (path / std :: to_string(file_id)).replace_extension(".bin");

		std :: ifstream idx_ifs(idx_path, std :: ios :: binary);
		std :: ifstream bin_ifs(bin_path, std :: ios :: binary);

		if (! idx_ifs || ! bin_ifs) {
			Message :: log(Message :: ERROR, true,
				"Failed to open files."
			);
			std :: exit(EXIT_FAILURE);
		}

		uint64_t tot_size = std :: filesystem :: file_size(idx_path);
		uint32_t len = tot_size / sizeof(TetrisDataIndex);

		for (int id = 0; id < len; id ++) {
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
			bin_ifs.read(reinterpret_cast<char *>(& data.PW), sizeof(data.PW));

			data.u.resize(data.len);
			data.P.resize(data.len);

			bin_ifs.read(reinterpret_cast<char *>(data.u.data()), data.len * sizeof(uint16_t));
			bin_ifs.read(reinterpret_cast<char *>(data.P.data()), data.len * sizeof(float));

			uint32_t tid = id + file_id * g;
			SortIndex sti {data.V, tid};
			sort_index.push_back(sti);
		}

		idx_ifs.close();
		bin_ifs.close();

	}

	sort(sort_index.begin(), sort_index.end());

	save_sort_idx();
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

	bin_ofs.seekp(0, std :: ios :: end);
	uint64_t offset = bin_ofs.tellp();

	bin_ofs.write(reinterpret_cast<const char *> (data.b), sizeof(data.b));
	bin_ofs.write(reinterpret_cast<const char *> (data.seq), sizeof(data.seq));
	bin_ofs.write(reinterpret_cast<const char *> (data.info), sizeof(data.info));
	bin_ofs.write(reinterpret_cast<const char *> (& data.V), sizeof(data.V));
	bin_ofs.write(reinterpret_cast<const char *> (& data.PW), sizeof(data.PW));

	uint32_t len = data.len;

	bin_ofs.write(reinterpret_cast<const char *> (data.u.data()), len * sizeof(uint16_t));
	bin_ofs.write(reinterpret_cast<const char *> (data.P.data()), len * sizeof(float));

	TetrisDataIndex idx {};
	idx.off = offset;
	idx.action_len = len;
	idx.game_id = lst_game;

	idx_ofs.write(reinterpret_cast<const char *> (& idx), sizeof(TetrisDataIndex));

	SortIndex sid {data.V, static_cast<uint32_t>(tot)};
	sort_index.push_back(sid);

	int siz = sort_index.size();

	for (int i = siz - 1; i >= 1; i --) {
		if (sort_index[i - 1] <= sort_index[i]) {
			break;
		}
		std :: swap(sort_index[i], sort_index[i - 1]);
	}

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

TetrisTrainData TetrisBuffer :: sample_recent() {
	if (tot == 0) {
		Message :: log(Message :: ERROR, true,
			"Buffer is empty."
		);
		std :: exit(EXIT_FAILURE);
	}

	int l = std :: max(0, tot - w);
	int u = linear_distribution(l, tot - 1);

	TetrisTrainData data = sample_by_id(u);

	return data;
}

TetrisTrainData TetrisBuffer :: sample_high_V() {
	if (tot == 0) {
		Message :: log(Message :: ERROR, true,
			"Buffer is empty."
		);
		std :: exit(EXIT_FAILURE);
	}

	int l = std :: max(0, tot - w);
	int u = linear_distribution(l, tot - 1);

	TetrisTrainData data = sample_by_sort_id(u);

	return data;
}