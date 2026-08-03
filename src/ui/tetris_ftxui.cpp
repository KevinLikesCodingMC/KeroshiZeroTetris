//
// Created by keroshi on 2026/7/26.
//

#include "include/ui/tetris_ftxui.hpp"
#include "include/core/mino.hpp"

#include <ftxui/component/event.hpp>

void TetrisEnvView :: set_height(int height) {
	view_height = height;
}
int TetrisEnvView :: get_height() {
	return view_height;
}

ftxui :: Element TetrisEnvView :: render_board() {
	ftxui :: Elements board;
	for (int y = view_height - 1; y >= 0; y --) {
		ftxui :: Elements cols;
		for (int x = 0; x < Board :: WIDTH; x ++) {
			if (tetris.get_board(x, y)) {
				cols.push_back(ftxui :: text("██"));
			}
			else {
				cols.push_back(ftxui :: text("  "));
			}
		}
		board.push_back(ftxui :: hbox(std :: move(cols)));
	}

	return ftxui :: vbox(std :: move(board))
		| ftxui :: border;
}

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

/*
	Width: 8
*/
ftxui :: Element TetrisEnvView :: render_mino(Piece piece) {
	if (piece == Piece :: EMPTY) {
		return ftxui :: text(" Empty  ");
	}

	auto color = get_color(piece);

	if (piece == Piece :: I) {
		return ftxui :: text(
			"████████"
		) | ftxui :: color(color);
	}
	if (piece == Piece :: O) {
		return ftxui :: text(
			"  ████  \n"
			"  ████  "
		) | ftxui :: color(color);
	}
	if (piece == Piece :: T) {
		return ftxui :: text(
			"   ██   \n"
			" ██████ "
		) | ftxui :: color(color);
	}
	if (piece == Piece :: J) {
		return ftxui :: text(
			" ██     \n"
			" ██████ "
		) | ftxui :: color(color);
	}
	if (piece == Piece :: L) {
		return ftxui :: text(
			"     ██ \n"
			" ██████ "
		) | ftxui :: color(color);
	}
	if (piece == Piece :: S) {
		return ftxui :: text(
			"   ████ \n"
			" ████   "
		) | ftxui :: color(color);
	}
	if (piece == Piece :: Z) {
		return ftxui :: text(
			" ████   \n"
			"   ████ "
		) | ftxui :: color(color);
	}

	return {};
}

ftxui :: Element TetrisEnvView :: render_cur() {
	Piece piece = tetris.get_cur();
	return ftxui :: vbox({
		ftxui :: text("Current"),
		ftxui :: separator(),
		render_mino(piece),
	}) | ftxui :: border;
}
ftxui :: Element TetrisEnvView :: render_hold() {
	Piece piece = tetris.get_hold();
	return ftxui :: vbox({
		ftxui :: text("  Hold  "),
		ftxui :: separator(),
		render_mino(piece),
	}) | ftxui :: border;
}
ftxui :: Element TetrisEnvView :: render_next() {
	ftxui :: Elements rows;
	rows.push_back(ftxui :: text("  Next  "));

	for (int i = 0; i < Board :: NEXT; i ++) {
		Piece piece = tetris.get_next(i);
		rows.push_back(ftxui :: separator());
		rows.push_back(render_mino(piece));
	}

	return ftxui :: vbox(std :: move(rows)) | ftxui :: border;
}

ftxui :: Element TetrisEnvView :: render() {
	return ftxui :: hbox({
		ftxui :: vbox({
			render_cur(),
			render_hold(),
		}),
		render_board(),
		ftxui :: vbox({
			render_next(),
		})
	});
}
