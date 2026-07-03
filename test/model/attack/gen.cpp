//
// Created by keroshi on 2026/7/3.
//

#include "include/buffer.h"
#include "include/converter.h"

int main(int argc, char * argv []) {

	std :: string buffer_path = "test/model/attack/buffer";
	TetrisBuffer buffer(buffer_path, 10000, 100000);

	int o = 0;
	if (argc >= 2) o = std :: atoi(argv[1]);

	int sample = 1e4;

	std :: mt19937 rnd(std :: random_device {} ());

	auto rd = [&] (int l, int r) {
		return static_cast<int64_t>(rnd()) % (r - l + 1) + l;
	};

	for (int _ = 0; _ < sample; _ ++) {
		Tetris t;

		t.attack = rd(0, 30);
		t.pieces = rd(0, 35);

		for (int y = 0; y < 10; y ++)
			for (int x = 0; x < 10; x ++)
				if (rd(0, 1)) {
					t.b[y] |= 1 << x;
				}

		int n = t.legal().size();
		std :: vector P(n, 1.f / n);
		auto data = Converter :: to_train_data(t, P);

		if (o == 0) {
			data.V = t.pieces + t.attack;
		}
		if (o == 1) {
			data.V = t.pieces;
		}

		buffer.add_game(data);
	}

	return 0;
}
