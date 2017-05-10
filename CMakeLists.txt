cmake_minimum_required(VERSION 3.7)
project(joystick)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/main.c src/controller.c src/controller.h)

include_directories(/usr/local/include/)
link_directories(/usr/local/lib)
add_executable(joystick ${SOURCE_FILES})
target_link_libraries(joystick SDL2 SDL2_image SDL2_ttf)