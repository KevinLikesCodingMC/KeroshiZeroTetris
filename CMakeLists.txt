cmake_minimum_required(VERSION 4.0)
project(KeroshiZeroTetris)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
	add_compile_options(-O3)
elseif(MSVC)
	add_compile_options(/utf-8)
endif()

include_directories(${CMAKE_SOURCE_DIR})

find_package(ftxui REQUIRED)
add_library(ftxui_lib INTERFACE)
target_link_libraries(
		ftxui_lib
		INTERFACE
		ftxui::screen
		ftxui::dom
		ftxui::component
)

find_package(OpenMP REQUIRED)
find_package(yaml-cpp REQUIRED)

if(MSVC)
	set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")
endif()

find_package(Torch REQUIRED)

add_library(tetris_core STATIC
		src/core/scoring.cpp
		src/core/tetris.cpp
)

add_library(tetris_ui STATIC
		src/ui/tetris_ftxui.cpp
)
target_link_libraries(
		tetris_ui PRIVATE
		tetris_core
		ftxui_lib
)

add_library(tetris_ai  STATIC
		src/ai/mcts.cpp
		src/ai/converter.cpp
		src/ai/net.cpp
		src/ai/buffer.cpp
)
target_link_libraries(
		tetris_ai PRIVATE
		tetris_core
		"${TORCH_LIBRARIES}"
)

add_library(tetris_train STATIC
		src/train/selfplay.cpp
		src/train/config.cpp
)
target_link_libraries(
		tetris_train PRIVATE
		tetris_ai
		"${TORCH_LIBRARIES}"
)

add_subdirectory(tests)
add_subdirectory(benchmark)


add_executable(
		value_train
		src/train/value_train.cpp
)
target_link_libraries(
		value_train
		PRIVATE
		tetris_train
)
