cmake_minimum_required(VERSION 3.16)
project(readfat C)

set(CMAKE_C_STANDARD 99)

add_executable(readfat main.c)
target_link_libraries(readfat PRIVATE m)