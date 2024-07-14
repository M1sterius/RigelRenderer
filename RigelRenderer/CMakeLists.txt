cmake_minimum_required(VERSION 3.28)

project(RigelRenderer)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/RigelRenderer/${CMAKE_BUILD_TYPE})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.hpp" )
add_library(RigelRenderer SHARED ${SOURCES}
        vendor/glad/src/glad.c
)

set(VCPKG_INSTALLED_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed)

if(WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
elseif (UNIX AND NOT APPLE)
    set(VCPKG_TARGET_TRIPLET x64-linux-static)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Freetype REQUIRED)

target_include_directories(RigelRenderer PRIVATE
    source
    include
    vendor/glad/include/glad
    vendor/glad/include
    include/publicUtility
    include/components
    include/input
    include/objects
    source/internal
    source/internal/utility
    source/built-in

    ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/GLFW
    ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/glm
    ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/freetype
)

target_link_directories(RigelRenderer PUBLIC ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
target_link_libraries(RigelRenderer PUBLIC glfw3 glm freetype)
target_compile_definitions(RigelRenderer PRIVATE "INTERNAL_LIB")

# Copy library's built-in resources (e.g shaders) to the output directory of the test project
set(COPY_OUT_DIR ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE}/built-in-resources)
add_custom_command(TARGET RigelRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/built-in-resources/
        ${COPY_OUT_DIR}
)
