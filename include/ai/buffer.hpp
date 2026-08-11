//
// Created by keroshi on 2026/8/5.
//

#ifndef KEROSHIZEROTETRIS_BUFFER_HPP
#define KEROSHIZEROTETRIS_BUFFER_HPP

#include "dataset.hpp"

#include <filesystem>

class Buffer {
public:
	explicit Buffer(const std :: string & path_str, uint32_t data_size);

	uint32_t get_size();

	void import();
	void add(const void * data, uint32_t count = 1);
	std :: vector<std :: byte> get(uint32_t u);

private:
	std :: filesystem :: path path;

	uint32_t siz, data_size;
	uint32_t size_per_file;
	int32_t last_file;

	std :: filesystem :: path get_path(uint32_t i);
};

class ValueBuffer : public Buffer {
public:
	using Data = Dataset :: player_value;

	explicit ValueBuffer(const std :: string & path_str);

	void add(const Data & data);
	void add(const std :: vector<Data> & data);

	Data get(uint32_t u);
};

#endif //KEROSHIZEROTETRIS_BUFFER_HPP
