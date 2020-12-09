cmake_minimum_required(VERSION 3.17)
project(TempsReaction C)

set(CMAKE_C_STANDARD 99)

add_executable(TempsReaction main.c Definitions.h Definitions.c GlobalVariables.c GlobalVariables.h)