cmake_minimum_required(VERSION 2.8)
project( SIC_Disassembler )
add_executable( sic-disassemble main.cpp disassembler.cpp symtab.cpp optab.cpp util.cpp )
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -D DEBUG_MODE")
