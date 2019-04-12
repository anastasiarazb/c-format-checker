cmake_minimum_required(VERSION 3.0.0)
project(format-checker)
#find_package(LLVM QUIET REQUIRED)

#set(Boost_ADDITIONAL_VERSIONS "1.49" "1.49.0" "1.50" "1.50.0")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
#include_directories(${LLVM_INCLUDE_DIRS})
#include_directories(${Boost_INCLUDE_DIR})
#add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++17 -Wall -Wextra -Wno-unused-function")

#llvm_map_components_to_libnames(llvm_libs all target passes analysis asmparser codegen executionengine mc linker irreader tablegen option object)

file(GLOB_RECURSE UTIL_SOURCES "src/*.cpp")
file(GLOB_RECURSE UTIL_HEADERS "src/*.h")
add_executable(${PROJECT_NAME} ${UTIL_SOURCES} ${UTIL_HEADERS})
#target_link_libraries(${PROJECT_NAME} ${llvm_libs} ${LLVM_LIBRARIES})
