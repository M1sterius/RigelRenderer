cmake_minimum_required(VERSION 3.12)

project(Sandbox)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_executable(Sandbox ${SOURCES})

set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Sandbox/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Sandbox/Release"
)

target_compile_definitions(RigelRenderer PRIVATE GLEW_STATIC)

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

if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(Sandbox PRIVATE DEBUG)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(Sandbox PRIVATE RELEASE)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /MD")
    endif()
endif()

