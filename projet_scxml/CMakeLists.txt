cmake_minimum_required(VERSION 3.6)
project(parser_scxml)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SOURCE_FILES main.cpp State.cpp Transition.cpp Xmlparser.cpp codeGenerator.cpp AST.cpp Parallel.cpp TransitionParallele.cpp StateParallele.cpp)

add_executable(parser_scxml ${SOURCE_FILES})