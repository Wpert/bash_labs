cmake_minimum_required (VERSION 3.10)
project (LABS)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

enable_testing ()

add_subdirectory (sequence)
add_subdirectory (sorting)
add_subdirectory (pointers)

add_subdirectory (testSequences)
add_subdirectory (testSortings)
add_subdirectory (testPointers)

add_executable (main main.cpp)
# cmake itmo !!!
target_link_libraries (main sequence_lib)
