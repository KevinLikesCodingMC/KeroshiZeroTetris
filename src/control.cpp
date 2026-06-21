//
// Created by Administrator on 2026/6/9.
//

#include <iostream>
#include "windows.h"

void send_key(char key) {
	WORD code {};
	int push = 0;

	if (key == 'A') code = 'A', push = 10;
	if (key == 'C') code = 'C', push = 10;
	if (key == 'Z') code = 'Z', push = 10;
	if (key == 'u') code = VK_UP, push = 10;

	if (key == 'D') code = VK_DOWN, push = 120;
	if (key == 'L') code = VK_LEFT, push = 120;
	if (key == 'R') code = VK_RIGHT, push = 120;
	if (key == 'l') code = VK_LEFT, push = 10;
	if (key == 'r') code = VK_RIGHT, push = 10;

	if (key == 's') code = VK_SPACE, push = 10;

	INPUT inputs[2] = {};

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = code;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = code;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, & inputs[0], sizeof(INPUT));
	Sleep(push);
	SendInput(1, & inputs[1], sizeof(INPUT));
	Sleep(10);
}

int main() {
	std :: string keys;

	std :: cout << "Input Keys:";
	std :: cin >> keys;

	std :: cout << "Focus on window in 3 seconds" << std :: endl;

	Sleep(3000);

	for (char key : keys) send_key(key);

	return 0;
}