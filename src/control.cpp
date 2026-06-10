//
// Created by Administrator on 2026/6/9.
//

#include <iostream>
#include "windows.h"

void send_key(char key) {
	WORD code {};
	if (key == 'A') code = 'A';
	if (key == 'C') code = 'C';
	if (key == 'Z') code = 'Z';
	if (key == 'd') code = VK_DOWN;
	if (key == 'u') code = VK_UP;
	if (key == 'l') code = VK_LEFT;
	if (key == 'r') code = VK_RIGHT;
	if (key == 's') code = VK_SPACE;

	INPUT inputs[2] = {};

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = code;
	inputs[0].ki.wScan = MapVirtualKeyA(code, MAPVK_VK_TO_VSC);

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = code;
	inputs[1].ki.wScan = inputs[0].ki.wScan;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, & inputs[0], sizeof(INPUT));
	Sleep(20);
	SendInput(1, & inputs[1], sizeof(INPUT));
	Sleep(20);
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