cmake_minimum_required(VERSION 3.10)
project(chess_engin)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess_engin main.cpp
        board.h
        board.cpp
        move.cpp
        move.h
        search.cpp
        search.h
)
