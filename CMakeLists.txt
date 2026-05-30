cmake_minimum_required(VERSION 4.0)
project(KeroshiZeroTetris)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
	add_compile_options(/O2 /arch:AVX2 /fp:fast /Ot /GL /Qpar)
else()
	add_compile_options("$<$<CONFIG:Release>:-O3;-mavx2;-ffast-math;-flto>")
	add_compile_options("$<$<CONFIG:Debug>:-O0;-g>")
endif()

include_directories(${CMAKE_SOURCE_DIR})

if(UNIX)
	list(APPEND CMAKE_PREFIX_PATH "/opt/libtorch")
endif()

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_library(torch_pch INTERFACE)
target_link_libraries(torch_pch INTERFACE "${TORCH_LIBRARIES}")
target_include_directories(torch_pch INTERFACE ${TORCH_INCLUDE_DIRS})
target_precompile_headers(
		torch_pch INTERFACE
		<torch/torch.h>
		<torch/script.h>
)

add_executable(
		test_mino
		test/test_mino.cpp
)

add_executable(
		test_tetris_legal
		test/test_tetris_legal.cpp
)

add_executable(
		test_spin_check
		test/test_spin_check.cpp
)

add_executable(
		test_tetris_random
		test/test_tetris_random.cpp
		src/render.cpp
)

add_executable(
		test_libtorch
		test/test_libtorch.cpp
)
target_link_libraries(test_libtorch "${TORCH_LIBRARIES}")

add_executable(
		test_model_input
		test/test_model_input.cpp
		src/converter.cpp
		src/model_loader.cpp
)
target_link_libraries(test_model_input "${TORCH_LIBRARIES}")

add_executable(
		test_mcts_tetris
		test/test_mcts_tetris.cpp
		src/render.cpp
)

install(
		TARGETS
		test_mino
		test_tetris_legal
		test_spin_check
		test_tetris_random
		test_libtorch
		test_model_input
		DESTINATION
		bin
)
