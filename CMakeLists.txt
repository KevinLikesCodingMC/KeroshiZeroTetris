cmake_minimum_required(VERSION 4.2)
project(KeroshiZeroTetris)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(
		test_mino
		test/test_mino.cpp
)

if(MSVC)
	add_compile_options(/O2 /arch:AVX2 /fp:fast /Ot /GL /Qpar)
else()
	add_compile_options(-O3 -mavx2 -ffast-math -flto)
endif()
