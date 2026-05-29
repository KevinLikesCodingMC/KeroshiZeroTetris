//
// Created by keroshi on 2026/5/10.
//

#ifndef KEROSHIZEROTETRIS_MODEL_LOADER_H
#define KEROSHIZEROTETRIS_MODEL_LOADER_H

#include "message.h"
#include <torch/torch.h>
#include <torch/script.h>

namespace ModelLoader {
	torch :: jit :: script :: Module load(
		const std :: string & path,
		bool cuda = false,
		bool output = true
	);
};

#endif //KEROSHIZEROTETRIS_MODEL_LOADER_H
