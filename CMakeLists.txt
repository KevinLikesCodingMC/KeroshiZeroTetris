cmake_minimum_required(VERSION 4.0)
project(KeroshiZeroTetris)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_library(core_lib STATIC
		src/core/scoring.cpp
		src/core/tetris.cpp
		src/ui/tetris_ftxui.cpp
		src/ai/mcts.cpp
)

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

add_subdirectory(tests)
