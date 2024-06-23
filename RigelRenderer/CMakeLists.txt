cmake_minimum_required(VERSION 3.19)

project(RigelRenderer)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/RigelRenderer/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_library(RigelRenderer STATIC ${SOURCES})

target_compile_definitions(RigelRenderer PRIVATE "GLEW_STATIC")
target_compile_definitions(RigelRenderer PRIVATE "INTERNAL_LIB")

include_directories(
    src
    include
    include/publicUtility
    include/components
    include/input
    include/objects
    src/internal
    src/internal/utility
    src/built-in

    ${CMAKE_SOURCE_DIR}/dependencies/glfw/include/GLFW
    ${CMAKE_SOURCE_DIR}/dependencies/glew/include/GL
    ${CMAKE_SOURCE_DIR}/dependencies/glm
    ${CMAKE_SOURCE_DIR}/dependencies/obj_loader
    ${CMAKE_SOURCE_DIR}/dependencies/stb_image
    ${CMAKE_SOURCE_DIR}/dependencies/freetype
    ${CMAKE_SOURCE_DIR}/dependencies/gltext
    ${CMAKE_SOURCE_DIR}/dependencies
)