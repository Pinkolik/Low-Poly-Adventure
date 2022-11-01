cmake_minimum_required(VERSION 3.24)

project(Game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(glfw)
add_subdirectory(glm)

file(COPY resources/shaders resources/textures DESTINATION resources)

add_executable(Game src/main.cpp src/glad.c src/helpers/shader.h src/helpers/shader.cpp)
target_link_libraries(Game PUBLIC glfw glm)
target_include_directories(Game PUBLIC include)