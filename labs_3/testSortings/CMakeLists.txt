project (testSort)

set(CUSTOM_LIBRARY_PATH /home/vpert/gitproj/labs_3/sorting/)
find_library(CUSTOM_LIBRARY_PATH
    sorting_lib
)

include_directories(
    "${CMAKE_SOURCE_DIR}"
)

add_executable(Sortings TestSorting.cpp)
target_link_libraries(
    Sortings
    sorting_lib
)

add_test(Bubble Sortings)
