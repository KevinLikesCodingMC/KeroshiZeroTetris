//
// Created by keroshi on 2026/8/3.
//

#include "include/core/tetris.hpp"
#include "include/ui/tetris_ftxui.hpp"

#include <random>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>


int main() {
	ftxui :: Elements boards;

	std :: mt19937 rnd(std :: random_device {} ());

	TetrisEnv tetris;
	TetrisEnvView view(tetris);

	auto step = [&] () {
		if (tetris.is_over()) return;

		auto actions = tetris.get_actions();
		int u = rnd() % actions.size();
		bool success = tetris.step(actions[u]);

		if (! success) {
			std :: abort();
		}
	};

	auto last = std :: chrono :: steady_clock :: now();
	auto renderer = ftxui :: Renderer([&] {

		auto now = std :: chrono :: steady_clock :: now();
		if (now - last >= std :: chrono :: milliseconds(200)) {
			step();
			last = now;
		}

		return view.render();
	});

	auto screen = ftxui :: ScreenInteractive :: TerminalOutput();
	screen.Loop(renderer);

	return 0;
}