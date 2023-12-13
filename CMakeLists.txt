cmake_minimum_required(VERSION 3.26)
project(CASTR)

set(CMAKE_CXX_STANDARD 23)

# Set the path to the GLFW library files
set(GLFW_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a")
set(GLFW_DLL_PATH "${CMAKE_SOURCE_DIR}/lib/glfw3.dll")

# Add the directory containing GLFW headers to include path
include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(Google_tests)

add_library(GLFW SHARED IMPORTED)
set_target_properties(GLFW PROPERTIES IMPORTED_IMPLIB ${GLFW_LIBRARY_PATH})

add_executable(CASTR src/main.cpp
        src/Rendering/Renderer.cpp
        src/Rendering/Renderer.h
        src/Rendering/Geometry.cpp
        src/Rendering/Geometry.h
        include/glad/glad.h
        include/KHR/khrplatform.h
        include/glad/glad.c
        src/imports.h
        src/shaders.h
        src/Rendering/Shader.cpp
        src/Rendering/Shader.h
        src/GameState/Window.cpp
        src/GameState/Window.h
        src/Util/Vector.h
        src/Entities/Camera.cpp
        src/Entities/Entity.h
        src/Entities/Player.cpp
        src/Entities/Player.h
        src/GameState/InputReader.h
        src/Entities/Entity.cpp
        src/Util/Vector.cpp
        src/GameState/GameLogic.cpp
        src/GameState/GameLogic.h
        src/GameState/MapData.cpp
        src/GameState/MapData.h
)

# Link GLFW to your executable
target_link_libraries(CASTR PRIVATE GLFW)

# Copy the GLFW DLL to the output directory
add_custom_command(TARGET CASTR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_DLL_PATH}"
        $<TARGET_FILE_DIR:CASTR>)