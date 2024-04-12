cmake_minimum_required (VERSION 3.10)

project (sortlib)

set(SORTING_SOURCES
    bubble_sort.cpp
)

set(SORTING_HEADERS
    sort_utilities.hpp
    bubble_merge.hpp
    quick_shell.hpp
)

add_library(sorting_lib ${SORTING_SOURCES} ${SORTING_HEADERS})

target_include_directories(sorting_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message("___SORT_LIB COMPILED")
