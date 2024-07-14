cmake_minimum_required(VERSION 3.28)

project(Sandbox)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.hpp")
add_executable(Sandbox ${SOURCES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/materials
    ${CMAKE_SOURCE_DIR}/Dependencies/glm
)

target_include_directories(Sandbox PUBLIC ../RigelRenderer/include)
target_link_libraries(Sandbox PRIVATE RigelRenderer)

set(COPY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE}/resources)
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${COPY_OUT_DIR}
)
