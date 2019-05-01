cmake_minimum_required(VERSION 3.13.4)
project(c-format-checker)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++17 -Wall -Wextra -Wno-unused-function")

file(GLOB_RECURSE UTIL_SOURCES "src/*.cpp")
file(GLOB_RECURSE UTIL_HEADERS "src/*.h")
add_executable(${PROJECT_NAME} ${UTIL_SOURCES} ${UTIL_HEADERS})
