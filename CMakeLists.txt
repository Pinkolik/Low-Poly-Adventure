cmake_minimum_required(VERSION 3.24)

project(Game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(glfw)
add_subdirectory(glm)

add_executable(Game src/main.cpp src/glad.c)
target_link_libraries(Game PUBLIC glfw glm)
target_include_directories(Game PUBLIC include)