cmake_minimum_required(VERSION 3.13)
project(project3RT C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(project3RT
        entity.c
        entity.h
        simulator.c
        simulator.h)
