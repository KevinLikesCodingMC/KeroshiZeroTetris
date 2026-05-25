//
// Created by keroshi on 2026/5/9.
//

#ifndef KEROSHIZEROTETRIS_RECORD_H
#define KEROSHIZEROTETRIS_RECORD_H

#include "tetris.h"

struct TetrisTrainData {
	uint16_t b[30];
	uint8_t seq[10];
	int info[10];
	float V;
	int len;
	std :: vector<uint8_t> x;
	std :: vector<uint8_t> y;
	std :: vector<uint8_t> r;
	std :: vector<uint8_t> p;
	std :: vector<float> P;
};

#endif //KEROSHIZEROTETRIS_RECORD_H
