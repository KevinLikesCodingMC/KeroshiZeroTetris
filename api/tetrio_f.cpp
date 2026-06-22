//
// Created by Administrator on 2026/6/22.
//

#include "include/control.h"
#include <fstream>
#include <iostream>
#include "windows.h"

int main() {
	std :: ifstream ifs("api/tetrio_f_input.txt");
	std :: ofstream ofs_i("api/tetrio_f_socket.txt");
	ofs_i.close();

	std :: cout << "Focus on window in 3 seconds" << std :: endl;
	Sleep(3000);

	int n = 0; ifs >> n;

	for (int T = 0; T < n; T ++) {
		std :: ofstream ofs("api/tetrio_f_socket.txt");

		std :: string seq; ifs >> seq;
		std :: string keys; ifs >> keys;

		Controller :: control("c");
		ofs << T << ' ' << seq << std :: endl;
		ofs.flush();

		Sleep(1000);

		Controller :: control(keys);
	}

	return 0;
}
