//
// Created by keroshi on 2026/5/9.
//

#ifndef KEROSHIZEROTETRIS_RECORD_H
#define KEROSHIZEROTETRIS_RECORD_H

#include <cstdint>
#include <vector>

struct TetrisTrainData {
	uint16_t b[30];
	uint8_t seq[10];
	float info[10];
	float V;
	int len;
	std :: vector<uint16_t> u;
	std :: vector<float> P;
	float PW = 1; // Policy Loss Weight
};

#endif //KEROSHIZEROTETRIS_RECORD_H
