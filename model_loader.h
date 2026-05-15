//
// Created by keroshi on 2026/5/10.
//

#ifndef KEROSHIZEROTETRIS_MODEL_LOADER_H
#define KEROSHIZEROTETRIS_MODEL_LOADER_H

#include "message.h"
#include <torch/torch.h>
#include <torch/script.h>

struct ModelLoader {
	static torch :: jit :: script :: Module load(
		const std :: string & path,
		bool cuda = false,
		bool output = true
	) {
		torch :: jit :: script :: Module module;
		try {
			module = torch :: jit :: load(path);

			if (cuda) {
				if (torch :: cuda :: is_available()) { // cuda
					module.to(torch :: kCUDA);
					if (output) {
						Message :: log(Message :: INFO, true,
						"[ModelLoader] Device set to CUDA.");
					}
				}
				else { // cpu
					module.to(torch :: kCPU);
					if (output) {
						Message :: log(Message :: WARNING, true,
						"[ModelLoader] CUDA is not available.");
						Message :: log(Message :: INFO, true,
						"[ModelLoader] Device set to CPU.");
					}
				}
			}
			else { // cpu
				module.to(torch :: kCPU);
				if (output) {
					Message :: log(Message :: INFO, true,
					"[ModelLoader] Device set to CPU.");
				}
			}

			module.eval();

			if (output) {
				Message :: log(Message :: INFO, true,
					"[ModelLoader] ",
					"model<", path, "> loaded.");
			}
		}
		catch (const c10 :: Error & e) {
			if (output) {
				Message :: log(Message :: ERROR, true,
					"[ModelLoader] ",
					"model<", path, "> loading failed.");
				Message :: log(Message :: ERROR, true,
					"Error: ", e.what());
			}
			std :: exit(EXIT_FAILURE);
		}
		return module;
	}
};

#endif //KEROSHIZEROTETRIS_MODEL_LOADER_H
