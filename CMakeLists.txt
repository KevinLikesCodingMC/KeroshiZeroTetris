cmake_minimum_required(VERSION 4.2)
project(KeroshiZeroTetris)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(
		test_mino
		test/test_mino.cpp
)
