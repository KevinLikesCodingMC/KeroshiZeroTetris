//
// Created by keroshi on 2026/7/24.
//

#include "include/core/mino.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <chrono>

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

ftxui :: Element render_mino(Piece piece, int r) {
	bool grid[5][5] = {false};

	for (int i = 0; i < 4; i ++) {
		auto [x, y] = Mino :: get(piece, i, r, 3, 3);
		grid[y][x] = true;
	}

	auto color = get_color(piece);

	ftxui :: Elements rows;
	for (int i = 4; i >= 0; i --) {
		ftxui :: Elements cols;
		for (bool col : grid[i]) {
			if (col) {
				cols.push_back(ftxui :: text("██") | ftxui :: color(color));
			}
			else {
				cols.push_back(ftxui :: text("  "));
			}
		}
		rows.push_back(ftxui :: hbox(std :: move(cols)));
	}

	return ftxui :: vbox(std :: move(rows)) | ftxui :: border;
}

ftxui :: Element render_minos(int r) {
	ftxui :: Elements ele;

	for (int p = 1; p <= 7; p ++) {
		auto piece = static_cast<Piece>(p);
		ele.push_back(render_mino(piece, r));
	}

	return ftxui :: hbox(std :: move(ele));
}

int main() {
	auto screen = ftxui :: ScreenInteractive :: TerminalOutput();

	auto st = std :: chrono :: steady_clock :: now();

	auto renderer = ftxui :: Renderer([&] {
		auto now = std :: chrono :: steady_clock :: now();
		int ms = std :: chrono :: duration_cast<std :: chrono :: milliseconds>(now - st).count();
		int r = (ms / 500) % 4;

		return ftxui :: vbox({
			ftxui :: text("Tetris Minos") | ftxui :: bold | ftxui :: center,
			ftxui :: separator(),
			ftxui :: text("Rotation: " + std :: to_string(r * 90)) | ftxui :: center,
			ftxui :: separator(),
			render_minos(r) | ftxui :: center,
		}) | ftxui :: border;
	});

	screen.Loop(renderer);

	return 0;
}
