cmake_minimum_required(VERSION 3.20)

project(Sandbox)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_executable(Sandbox ${SOURCES})

# For now, change that according to the build variant
# P.S. I know that's retarted but it is the only way everything works...
#set(CMAKE_BUILD_TYPE "Debug")

set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Sandbox/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Sandbox/Release"
)

target_compile_definitions(Sandbox PRIVATE "GLEW_STATIC")

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

target_include_directories(Sandbox PUBLIC ../RigelRenderer/src/include)
target_link_libraries(Sandbox RigelRenderer)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_definitions(Sandbox PRIVATE "DEBUG")
#    target_compile_definitions(Sandbox PRIVATE "T_DEBUG")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od")
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#    target_compile_definitions(Sandbox PRIVATE "RELEASE")
#    target_compile_definitions(Sandbox PRIVATE "T_RELEASE")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /MD")
#endif()

# Post build 

#set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
#set(DEST_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Oh God, this abomination needs to be rewritten...eventually,
# when I finally figure out how build types work in CMake
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    set(DEST_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/Debug)
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#    set(DEST_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/Release)
#endif()

#message(${SOURCE_DIR})
#message(${DEST_DIR})

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_SOURCE_DIR}/bin/Sandbox/Debug/resources
)
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_SOURCE_DIR}/bin/Sandbox/Release/resources
)
