project(drivers)

include_directories("tui/include")

add_library(${PROJECT_NAME} OBJECT tui/port.c tui/tui.c io.c)

target_include_directories(${PROJECT_NAME} PUBLIC "tui/include")