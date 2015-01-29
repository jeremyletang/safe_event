# min config
cmake_minimum_required (VERSION 2.8.1)

if(POLICY CMP0042)
cmake_policy(SET CMP0042 NEW)
endif()

project (safe_event CXX)

#include paths
set (SAFE_EVENT_INC ${CMAKE_SOURCE_DIR}/include)

# lib paths
set (SAFE_EVENT_LIB ${CMAKE_BINARY_DIR})

include_directories (${SAFE_EVENT_INC})

file (GLOB headers "*.h")
file (GLOB sources "*.c")

set (DIRS "src" "include")

foreach (dir ${DIRS})
   file (GLOB h_${dir} "${dir}/*.h")
   file (GLOB s_${dir} "${dir}/*.cpp")
   source_group (${dir} FILES ${s_${dir}} ${h_${dir}})
   set (sources ${sources} ${s_${dir}})
   set (header ${headers} ${h_${dir}})
endforeach ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

add_library (safe_event SHARED ${sources})
add_subdirectory(tests)