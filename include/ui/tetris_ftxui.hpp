//
// Created by keroshi on 2026/7/26.
//

#ifndef KEROSHIZEROTETRIS_TETRIS_FTXUI_HPP
#define KEROSHIZEROTETRIS_TETRIS_FTXUI_HPP

#include "include/core/tetris.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

class TetrisEnvView {
public:
	explicit TetrisEnvView(TetrisEnv & tetris)
		: tetris(tetris) {}

	void set_height(int height);
	int get_height();

	ftxui :: Element render_board();

	static ftxui :: Element render_mino(Piece piece);

	ftxui :: Element render_cur();
	ftxui :: Element render_hold();
	ftxui :: Element render_next();

	ftxui :: Element render();

private:
	TetrisEnv & tetris;
	int view_height = 24;
};

#endif //KEROSHIZEROTETRIS_TETRIS_FTXUI_HPP
