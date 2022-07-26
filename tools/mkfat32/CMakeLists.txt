set(CMAKE_INCLUDE_CURRENT_DIR on)

file(GLOB src ./*.c fatfs/*.h fatfs/*.c)

add_executable(mkfat32 ${src})