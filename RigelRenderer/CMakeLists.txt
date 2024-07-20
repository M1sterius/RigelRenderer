cmake_minimum_required(VERSION 3.28)

project(RigelRenderer)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/RigelRenderer/${CMAKE_BUILD_TYPE})
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/Dependencies)

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.hpp")
add_library(RigelRenderer SHARED ${SOURCES}
        ${DEPENDENCIES_DIR}/glad/src/glad.c
        source/modules/Core.cpp
        include/modules/Core.hpp
        source/modules/Time.cpp
)

target_compile_definitions(RigelRenderer PRIVATE "INTERNAL_LIB")
set_target_properties(RigelRenderer PROPERTIES PREFIX "")

target_include_directories(RigelRenderer PRIVATE
    source
    include
    include/publicUtility
    include/components
    include/modules
    include/input
    include/objects
    source/internal
    source/internal/utility
    source/built-in

    ${DEPENDENCIES_DIR}/glad/include/glad
    ${DEPENDENCIES_DIR}/glad/include
    ${DEPENDENCIES_DIR}/glfw/include/GLFW
    ${DEPENDENCIES_DIR}/glm
    ${DEPENDENCIES_DIR}/obj_loader
    ${DEPENDENCIES_DIR}/stb_image
    ${DEPENDENCIES_DIR}/freetype
    ${DEPENDENCIES_DIR}/gltext
    ${DEPENDENCIES_DIR}/
)

add_subdirectory(${DEPENDENCIES_DIR}/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
add_subdirectory(${DEPENDENCIES_DIR}/freetype ${CMAKE_CURRENT_BINARY_DIR}/freetype)
add_subdirectory(${DEPENDENCIES_DIR}/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)

find_package(OpenGL REQUIRED)

target_link_libraries(RigelRenderer PRIVATE
        OpenGL::GL
        glfw
        freetype
        assimp
)

# copy .dll file
set(DLL_DIR ${CMAKE_SOURCE_DIR}/bin/RigelRenderer/${CMAKE_BUILD_TYPE}/RigelRenderer.dll)
set(DLL_COPY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE})
add_custom_command(TARGET RigelRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_DIR}
        ${DLL_COPY_OUT_DIR}
)

# Copy library's built-in resources (e.g shaders) to the output directory of the test project
set(COPY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE}/built-in-resources)
add_custom_command(TARGET RigelRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/built-in-resources/
        ${COPY_OUT_DIR}
)
