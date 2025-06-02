cmake_minimum_required(VERSION 3.31)
project(DSA_Revision)

find_package(Catch2 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 26)

set(MODULES
        Arrays-Hashing
        Two-Pointer
        Sliding-Window
        Stack
        Binary-Search
        Linked-List
        Trees
        Heap
        Backtracking
        Tries
        Graphs
        DP
)

set(SOURCES
        arrays-hashing.cpp
        two-pointer.cpp
        sliding-window.cpp
        stack.cpp
        binary-search.cpp
        linked-list.cpp
        trees.cpp
        heap.cpp
        backtracking.cpp
        tries.cpp
        graphs.cpp
        dp.cpp
)

add_executable(DSA_Revision main.cpp)

foreach (MODULE SOURCE IN ZIP_LISTS MODULES SOURCES)
    add_executable(${MODULE} ${SOURCE})
    target_link_libraries(${MODULE} PRIVATE Catch2::Catch2WithMain)
endforeach ()