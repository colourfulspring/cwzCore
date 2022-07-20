cmake_minimum_required(VERSION 3.16)

project(cwzCore C ASM)

# 设置参数
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-m32")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

# 检查工具
if (EXISTS ${CMAKE_LINKER})
    message(STATUS "Check for linker: ${CMAKE_LINKER}")
else()
    message(FATAL_ERROR "Linker not found!")
endif()
if (EXISTS ${CMAKE_OBJCOPY})
    message(STATUS "Check for objcopy: ${CMAKE_OBJCOPY}")
    else()
    message(FATAL_ERROR "ObjCopy not found!")
endif()

# 特殊目标
set(KERNEL_TARGET kernel.img)

# 添加子目录
add_subdirectory(tools)

add_subdirectory(kernel)