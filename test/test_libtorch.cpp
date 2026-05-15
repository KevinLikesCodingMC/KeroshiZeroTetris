//
// Created by keroshi on 2026/5/7.
//

#include <iostream>
#include <torch/torch.h>

int main() {
	std :: cout << "cuda: ";
	std :: cout << torch :: cuda :: is_available();
	std :: cout << std :: endl;

	torch :: Tensor tensor = torch :: rand({3, 4});
	std :: cout << tensor << std :: endl;
	return 0;
}