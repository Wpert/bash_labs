project(testsSeq)

set(CUSTOM_LIBRARY_PATH /home/vpert/gitproj/labs_3/arrays/)
find_library(CUSTOM_LIBRARY_PATH
    sequence_lib
)

include_directories(
    "${CMAKE_SOURCE_DIR}"
)

add_executable(TestDynamic TestDynamic.cpp)
target_link_libraries(
    TestDynamic
    sequence_lib
)

add_executable(TestArray TestArraySeq.cpp)
target_link_libraries(
    TestArray
    sequence_lib
)

add_executable(TestLinked TestLinked.cpp)
target_link_libraries(
    TestLinked
    sequence_lib
)

add_test( DynamicArr TestDynamic )
add_test( ArrSequence TestArray )
add_test( LinkedList TestLinked )
