cmake_minimum_required(VERSION 3.24)

project(Game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(glfw)
add_subdirectory(glm)

# file(COPY resources/shaders resources/textures DESTINATION resources)

add_executable(Game
                src/main.cpp
                src/glad.c
                src/helpers/stb_image.h
                src/helpers/stb_image_write.h
                src/helpers/json.hpp
                src/helpers/tiny_gltf.h
                src/engine/shader.h
                src/engine/shader.cpp
                src/engine/mesh.h
                src/engine/map.cpp
                src/engine/map.h
                src/engine/node.h
                src/engine/primitive.h
                src/engine/player.h
                src/engine/player.cpp)
target_link_libraries(Game PUBLIC glfw glm)
target_include_directories(Game PUBLIC include)