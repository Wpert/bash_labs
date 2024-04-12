cmake_minimum_required(VERSION 3.10)

project (seqlib)

set(SOURCES
    DynamicArray.cpp
    ArraySequence.cpp
)
set(HEADERS
    Sequence.hpp
    DynamicArray.hpp
    ArraySequence.hpp
    LinkedList.hpp
)

add_library(sequence_lib ${SOURCES} ${HEADERS})

target_include_directories(sequence_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message("___SEQ_LIB COMPILED")

