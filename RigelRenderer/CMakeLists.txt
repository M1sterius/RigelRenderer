cmake_minimum_required(VERSION 3.28)

project(RigelRenderer)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/RigelRenderer/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_library(RigelRenderer STATIC ${SOURCES})

target_compile_definitions(RigelRenderer PRIVATE "GLEW_STATIC" "INTERNAL_LIB")

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

# Copy library's built-in resources (e.g shaders) to the output directory of the test project
set(COPY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE}/built-in-resources)
add_custom_command(TARGET RigelRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/built-in-resources/
        ${COPY_OUT_DIR}
)
