cmake_minimum_required(VERSION 3.28)

project(RigelRenderer)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/RigelRenderer/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.hpp")
add_library(RigelRenderer SHARED ${SOURCES} ${CMAKE_SOURCE_DIR}/Dependencies/glad/src/glad.c)

target_compile_definitions(RigelRenderer PRIVATE "INTERNAL_LIB")

target_include_directories(RigelRenderer PRIVATE
    source
    include
    include/publicUtility
    include/components
    include/input
    include/objects
    source/internal
    source/internal/utility
    source/built-in

    ${CMAKE_SOURCE_DIR}/Dependencies/glad/include/glad
    ${CMAKE_SOURCE_DIR}/Dependencies/glad/include
    ${CMAKE_SOURCE_DIR}/Dependencies/glfw/include/GLFW
    ${CMAKE_SOURCE_DIR}/Dependencies/glm
    ${CMAKE_SOURCE_DIR}/Dependencies/obj_loader
    ${CMAKE_SOURCE_DIR}/Dependencies/stb_image
    ${CMAKE_SOURCE_DIR}/Dependencies/freetype
    ${CMAKE_SOURCE_DIR}/Dependencies/gltext
    ${CMAKE_SOURCE_DIR}/Dependencies
)

target_link_directories(RigelRenderer PUBLIC
        ${CMAKE_SOURCE_DIR}/Dependencies/glfw/lib-vc2022
        ${CMAKE_SOURCE_DIR}/Dependencies/freetype/lib
)

target_link_libraries(RigelRenderer PUBLIC
        opengl32
        glfw3
        freetype
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
