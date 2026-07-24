//
// Created by keroshi on 2026/7/24.
//

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
	ftxui :: Element doc = ftxui :: vbox({
		ftxui :: text(" Hello Keroshi ! ")
	}) | ftxui :: border;

	auto screen = ftxui :: Screen :: Create(
		ftxui :: Dimension :: Fit(doc)
	);

	ftxui :: Render(screen, doc);
	screen.Print();

	std :: cout << std :: endl;

	return 0;
}
