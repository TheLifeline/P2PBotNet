cmake_minimum_required(VERSION 3.16.5)
project(P2PBotNet C)

set(CMAKE_C_STANDARD 99)

add_executable(P2PBotNet main.c peer.c peer.h global.h)
find_package(Threads)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
