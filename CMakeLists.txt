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
        src/shader/Shader.h
        src/shader/Shader.cpp
        src/engine/model/Mesh.h
        src/engine/model/Mesh.cpp
        src/engine/map/Map.cpp
        src/engine/map/Map.h
        src/engine/model/Model.cpp
        src/engine/model/Model.h
        src/engine/model/Node.h
        src/engine/model/Node.cpp
        src/engine/model/Primitive.h
        src/engine/model/Primitive.cpp
        src/engine/model/PositionStruct.h
        src/engine/player/Player.h
        src/engine/player/Player.cpp
        src/engine/intersection/IntersectionUtil.h
        src/engine/intersection/IntersectionUtil.cpp
        src/engine/intersection/Projection.h
        src/engine/intersection/Projection.cpp
        src/engine/game/GameInstance.cpp src/engine/game/GameInstance.h src/engine/intersection/AABB.cpp src/engine/intersection/AABB.h)
add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources)
target_link_libraries(Game PUBLIC glfw glm)
target_include_directories(Game PUBLIC include)
