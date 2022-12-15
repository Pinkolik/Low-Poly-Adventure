cmake_minimum_required(VERSION 3.24)

project(Game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)

# file(COPY resources/shaders resources/textures DESTINATION resources)

add_executable(Game
        src/main.cpp
        src/glad.c
        src/helpers/stb_image.h
        src/helpers/stb_image_write.h
        src/helpers/json.hpp
        src/helpers/tiny_gltf.h
        src/shader/shader.h
        src/shader/shader.cpp
        src/engine/model/mesh.h
        src/engine/model/mesh.cpp
        src/engine/map/map.cpp
        src/engine/map/map.h
        src/engine/model/model.cpp
        src/engine/model/model.h
        src/engine/model/node.h
        src/engine/model/node.cpp
        src/engine/model/primitive.h
        src/engine/model/primitive.cpp
        src/engine/model/position_struct.h
        src/engine/player/player.h
        src/engine/player/player.cpp
        src/engine/intersection/intersection_util.h
        src/engine/intersection/intersection_util.cpp
        src/engine/intersection/projection.h
        src/engine/intersection/projection.cpp
        )
add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources)
target_link_libraries(Game PUBLIC glfw glm)
target_include_directories(Game PUBLIC include)
