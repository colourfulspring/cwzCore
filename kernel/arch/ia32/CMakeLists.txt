# Add bootloader source files
add_subdirectory(boot)

set(init_objects $<TARGET_OBJECTS:boot>)

message(${init_objects})

message(STATUS "init_objects: ${init_objects}")
string(REGEX REPLACE ";" "\n" init_objects "${init_objects}")

# Prepare and set linker script
configure_file(linker.tpl.ld linker.ld.S)
add_custom_target(
    linker.ld ALL
    COMMAND
        ${CMAKE_C_COMPILER} -E -x c
        -I${CMAKE_CURRENT_LIST_DIR}/boot/include
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/linker.ld.S | grep -v "^#" >
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/linker.ld)
