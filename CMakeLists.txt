cmake_minimum_required(VERSION 3.16)

project(cwzCore C ASM)

set(CMAKE_C_STANDARD 99)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

# 特殊目标
set(KERNEL_TARGET kernel.img)

# 添加子目录
add_subdirectory(tools)

add_subdirectory(kernel)