# cmake_minimum_required(VERSION <specify CMake version here>)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp hashtable.cpp AST.cpp lexer.cpp list.cpp listChild.cpp listToken.cpp parser.cpp semantics.cpp)