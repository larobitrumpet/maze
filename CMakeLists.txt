cmake_minimum_required(VERSION 3.28)

project(maze)

include_directories(include)
file(GLOB SRC_FILES src/*.c)
add_executable(maze ${SRC_FILES})
target_link_libraries(maze -lallegro_image -lallegro)
file(CREATE_LINK "${PROJECT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)
