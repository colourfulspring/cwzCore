project(arch)

add_subdirectory(ia32)

add_library(${PROJECT_NAME} OBJECT
    $<TARGET_OBJECTS:boot>
)