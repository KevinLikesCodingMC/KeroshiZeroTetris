//
// Created by keroshi on 2026/7/31.
//

#include "include/core/tetris.hpp"
#include "include/core/mino.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

ftxui :: Color get_color(Piece piece) {
	switch (piece) {
		case Piece :: I : return ftxui ::  Color :: Cyan;
		case Piece :: J : return ftxui ::  Color :: Blue;
		case Piece :: L : return ftxui ::  Color :: Orange1;
		case Piece :: O : return ftxui ::  Color :: Yellow;
		case Piece :: S : return ftxui ::  Color :: Green;
		case Piece :: T : return ftxui ::  Color :: Purple;
		case Piece :: Z : return ftxui ::  Color :: Red;
		default: return ftxui :: Color :: White;
	}
}

ftxui :: Element get_board(TetrisEnv & tetris, int x, int y, int r) {
	bool grid[10][30] = {false};

	auto piece = tetris.get_cur();
	auto color = get_color(piece);

	for (int i = 0; i < 4; i ++) {
		auto [X, Y] = Mino :: get(piece, i, r, x, y);
		grid[X][Y] = true;
	}

	ftxui :: Elements rows;

	for (int j = 10; j >= 0; j --) {
		ftxui :: Elements cols;
		for (int i = 0; i < Board :: WIDTH; i ++) {
			bool o = tetris.get_board(i, j);
			if (o && grid[i][j]) {
				std :: abort();
			}

			if (grid[i][j]) {
				cols.push_back(ftxui :: text("██") | ftxui :: color(color));
			}
			else {
				cols.push_back(ftxui :: text(o ? "██" : "  "));
			}
		}
		rows.push_back(ftxui :: hbox(cols));
	}

	return ftxui :: vbox(std :: move(rows)) | ftxui :: border;
}

int main() {

	TetrisEnv tetris;
	tetris.set_cur(Piece :: T);

	tetris.set_board_row(4, 0b0000000011);
	tetris.set_board_row(3, 0b0000000001);
	tetris.set_board_row(2, 0b1100001101);
	tetris.set_board_row(1, 0b1000111001);
	tetris.set_board_row(0, 0b1101111101);

	auto actions = tetris.get_actions();

	ftxui :: Elements boards;

	for (int action : actions) {
		if (action == Action :: hold()) continue;
		auto [x, y, r] = Action :: decode(action);

		boards.push_back(get_board(tetris, x, y, r));
	}

	ftxui :: Elements rows;
	int col = 12;

	for (int i = 0; i < boards.size(); i += col) {
		ftxui :: Elements row;
		for (int j = i; j < i + col && j < boards.size(); j ++) {
			row.push_back(boards[j]);
		}
		rows.push_back(ftxui :: hbox(row));
	}

	auto doc = ftxui :: vbox(rows) | ftxui :: border;

	auto screen = ftxui :: Screen :: Create(
		ftxui :: Dimension :: Fit(doc)
	);

	ftxui :: Render(screen, doc);
	screen.Print();

	std :: cout << std :: endl;

	return 0;
}
