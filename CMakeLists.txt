cmake_minimum_required(VERSION 3.10)
project(cgwind)
set(TARGET_NAME run)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
# Setup executable target
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/*.h)
add_executable(${TARGET_NAME} WIN32 ${SOURCE_FILES})