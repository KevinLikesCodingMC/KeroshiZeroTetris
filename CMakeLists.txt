cmake_minimum_required(VERSION 4.0)
project(KeroshiZeroTetris)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O3)

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

if(UNIX)
	list(APPEND CMAKE_PREFIX_PATH "/opt/libtorch")
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
)
target_link_libraries(
		tetris_ai PRIVATE
		tetris_core
		"${TORCH_LIBRARIES}"
)

add_subdirectory(tests)
