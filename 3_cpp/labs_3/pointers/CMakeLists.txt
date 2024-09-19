cmake_minimum_required (VERSION 3.10)

project (pointerlib)

set(POINTERS_SOURCES
    unique.cpp
)

set(POINTERS_HEADERS
    unique.hpp
    shared.hpp
)

add_library(pointer_lib ${POINTERS_SOURCES} ${POINTERS_HEADERS})

target_include_directories(pointer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message("___POINTER_LIB COMPILED")
