cmake_minimum_required(VERSION 3.20)

project(RigelRenderer)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_library(RigelRenderer STATIC ${SOURCES})

set_target_properties(RigelRenderer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/RigelRenderer/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/RigelRenderer/Release"
)

target_compile_definitions(RigelRenderer PRIVATE "GLEW_STATIC")
target_compile_definitions(RigelRenderer PRIVATE "INTERNAL_LIB")

include_directories(
    src
    include
    include/components
    include/input
    include/objects
    src/internal
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