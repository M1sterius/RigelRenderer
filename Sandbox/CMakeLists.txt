cmake_minimum_required(VERSION 3.28)

project(Sandbox)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_executable(Sandbox ${SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/materials
    ${CMAKE_SOURCE_DIR}/dependencies/glm
)

target_link_directories(Sandbox PRIVATE
    ../dependencies/glfw/lib-vc2022
    ../dependencies/glew/lib/Release/x64
    ../dependencies/freetype/lib
)

target_link_libraries(Sandbox 
    opengl32
    glew32s
    glfw3
    freetype
)

target_include_directories(Sandbox PUBLIC ../RigelRenderer/include)
target_link_libraries(Sandbox RigelRenderer)

set(COPY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE}/resources)
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${COPY_OUT_DIR}
)
