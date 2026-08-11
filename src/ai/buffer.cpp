//
// Created by keroshi on 2026/8/5.
//

#include "include/ai/buffer.hpp"
#include "include/utils/logger.hpp"

#include <fstream>

Buffer :: Buffer(const std :: string & path_str, uint32_t data_size)
	: path(path_str)
	, data_size(data_size)
	, size_per_file(100000) {

	if (! std :: filesystem :: exists(path)) {
		std :: filesystem :: create_directories(path);
		Logger :: info("Created Buffer directory.");
	}

	import();
}

void Buffer :: import() {
	siz = 0; last_file = - 1;

	int i = 0;
	while (std :: filesystem :: exists(get_path(i))) {
		last_file = i;
		i ++;
	}

	if (last_file == - 1) {
		return;
	}

	auto last_file_path = get_path(last_file);
	uint64_t last_size = std :: filesystem :: file_size(last_file_path);

	if (last_size % data_size != 0) {
		Logger :: error("File corrupted [0] : ", get_path(last_file));
		throw std :: runtime_error("File Error.");
	}

	siz = last_file * size_per_file + last_size / data_size;

}

std :: filesystem :: path Buffer :: get_path(uint32_t i) {
	return (path / std :: to_string(i))
			.replace_extension(".bin");
}

void Buffer :: add(const void * data, uint32_t count) {

	auto ptr = static_cast<const char *>(data);

	while (count > 0) {
		if (siz == (last_file + 1) * size_per_file) {
			last_file ++;
		}

		uint32_t max_count = (last_file + 1) * size_per_file - siz;
		uint32_t write_count = std :: min(max_count, count);

		std :: ofstream ofs(
			get_path(last_file),
			std :: ios :: binary | std :: ios :: app
		);

		if (! ofs) {
			Logger :: error("Failed to open ", get_path(last_file));
			throw std :: runtime_error("File Error.");
		}

		ofs.write(
			ptr,
			data_size * write_count
		);

		ofs.close();
		siz += write_count;
		ptr += write_count * data_size;
		count -= write_count;
	}
}

std :: vector<std :: byte> Buffer :: get(uint32_t u) {

	if (siz == 0 || u >= siz) return {};

	uint32_t file = u / size_per_file;
	uint32_t id = u % size_per_file;

	std :: ifstream ifs(get_path(file), std :: ios :: binary);

	if (! ifs) {
		Logger :: error("Failed to open ", get_path(file));
		throw std :: runtime_error("File Error.");
	}

	ifs.seekg(id * data_size, std :: ios :: beg);

	std :: vector<std :: byte> res (data_size);
	ifs.read(reinterpret_cast<char *>(res.data()), data_size);

	ifs.close();

	return res;
}

uint32_t Buffer :: get_size() {
	return siz;
}

ValueBuffer :: ValueBuffer(const std :: string & path_str)
	: Buffer(path_str, sizeof(Data))
{ }

void ValueBuffer :: add(const Data & data) {
	Buffer :: add(& data);
}
void ValueBuffer :: add(const std :: vector<Data> & data) {
	if (data.empty()) return;
	Buffer :: add(data.data(), data.size());
}

Dataset :: player_value ValueBuffer :: get(uint32_t u) {
	if (u >= get_size()) return {};

	auto bytes = Buffer :: get(u);
	return * reinterpret_cast<const Data *>(bytes.data());
}
