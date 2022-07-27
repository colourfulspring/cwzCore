cmake_minimum_required(VERSION 3.16)

project(scene C ASM)

# 设置参数
set(CMAKE_C_STANDARD 99)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置构建开关
set(BUILD_TEST on)

# 设置工程相关属性
set(PROJECT_SCRIPT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

# 设置CMake组件
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SCRIPT_PATH}/Modules)

# 检查工具
if (EXISTS ${CMAKE_LINKER})
    message(STATUS "Check for linker: ${CMAKE_LINKER}")
else()
    message(FATAL_ERROR "Linker not found!")
endif()

if (EXISTS ${CMAKE_OBJCOPY})
    message(STATUS "Check for objcopy: ${CMAKE_OBJCOPY}")
else()
    message(FATAL_ERROR "Command objcopy not found!")
endif()

find_program(CMAKE_DD dd)
if (EXISTS ${CMAKE_DD})
    message(STATUS "Check for dd: ${CMAKE_DD}")
else()
    message(FATAL_ERROR "Command dd not found!")
endif()

# 设置全局可见目标名称
set(TOOLS_TARGET tools)
set(BOOT_BLOCK_TARGET bootblock)
set(LOADER_TARGET loader)
set(KERNEL_TARGET kernel)
set(FINAL_TARGET scene)

add_subdirectory(src)

if (BUILD_TEST)
    add_subdirectory(test)
endif()