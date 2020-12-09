cmake_minimum_required(VERSION 3.16)
project(Enigma C)

set(CMAKE_C_STANDARD 11)

add_executable(Enigma main.c GlobalVariables.c Definitions.c libSl4sh.c)