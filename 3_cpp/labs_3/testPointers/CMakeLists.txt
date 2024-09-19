project(testPtr)

set (CUSTOM_LIBRARY_PATH /home/vpert/gitproj/labs_3/arrays/)
find_library (CUSTOM_LIBRARY_PATH
    pointer_lib
)

include_directories(
    "${CMAKE_SOURCE_DIR}"
)

add_executable (TestPointers TestPointers.cpp)
target_link_libraries (
    TestPointers
    pointer_lib
)

add_test( Pointers TestPointers )
