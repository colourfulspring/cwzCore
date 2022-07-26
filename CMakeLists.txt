cmake_minimum_required(VERSION 3.16)

project(scene C ASM)

# 设置参数
set(CMAKE_C_STANDARD 99)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_SCRIPT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SCRIPT_PATH}/Modules)

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

# 添加子目录
add_subdirectory(tools)

add_subdirectory(kernel)

# 最终目标
get_target_property(BOOT_BLOCK_FILE ${BOOT_BLOCK_TARGET} OUTPUT_FILE)
set(FINAL_OUTPUT ${CMAKE_BINARY_DIR}/scene.img)
# add_custom_command(OUTPUT ${FINAL_OUTPUT}
#     COMMAND ${CMAKE_DD} if=/dev/zero of=${FINAL_OUTPUT} count=10000
#     COMMAND ${CMAKE_DD} if=${BOOT_BLOCK_FILE} of=${FINAL_OUTPUT} conv=notrunc
# 	  COMMAND ${CMAKE_DD} if=$<TARGET_FILE:${KERNEL_TARGET}> of=${FINAL_OUTPUT} seek=1 conv=notrunc
#     DEPENDS ${BOOT_BLOCK_TARGET} ${KERNEL_TARGET}
# )
add_custom_command(OUTPUT ${FINAL_OUTPUT}
    COMMAND ${CMAKE_COMMAND}
            -D check_size=true
            -D bootblock=${BOOT_BLOCK_FILE}
            -D loader=$<TARGET_FILE:${LOADER_TARGET}>
            -D kernel=$<TARGET_FILE:${KERNEL_TARGET}>
            -D mkfat32=$<TARGET_FILE:mkfat32>
            -D dd=${CMAKE_DD}
            -D output=${FINAL_OUTPUT}
            -P ${CMAKE_SCRIPT_PATH}/BuildRootfs.cmake
    DEPENDS ${TOOLS_TARGET} ${BOOT_BLOCK_TARGET} ${LOADER_TARGET} ${KERNEL_TARGET}
)
add_custom_target(${FINAL_TARGET} ALL DEPENDS ${FINAL_OUTPUT})
set_target_properties(${FINAL_TARGET} PROPERTIES OUTPUT_FILE ${FINAL_OUTPUT})