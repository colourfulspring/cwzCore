project(fs)

file(GLOB_RECURSE fs_src "include/*.h" "./*.c")

add_library(${PROJECT_NAME} OBJECT ${fs_src})

target_include_directories(${PROJECT_NAME} PUBLIC include)