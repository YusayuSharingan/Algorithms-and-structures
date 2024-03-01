cmake_minimum_required(VERSION 2.6)

set(SOURCES src/arch.cpp src/huffman.hpp src/lz77.hpp)

add_executable(arch ${SOURCES})