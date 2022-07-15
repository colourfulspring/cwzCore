project(libc)

file(GLOB_RECURSE libc_src "include/*.h" "src/*.c")

add_library(${PROJECT_NAME} OBJECT ${libc_src})

target_include_directories(${PROJECT_NAME} PUBLIC include)