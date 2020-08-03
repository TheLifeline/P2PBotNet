cmake_minimum_required(VERSION 3.17)
project(P2PBotNet C)

set(CMAKE_C_STANDARD 99)

add_executable(P2PBotNet main.c tool.c tool.h)