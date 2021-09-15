cmake_minimum_required(VERSION 3.16.3)
project(P2PBotNet C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(P2PBotNet main.c global.h peer.c peer.h client.c client.h server.c server.h)

find_package(Threads)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
