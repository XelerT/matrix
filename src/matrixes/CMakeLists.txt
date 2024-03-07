set(SRCS
        matrix.cpp
        row.cpp
        sq_matrix.cpp
        chain.cpp
        lazy.cpp
        imatrix.cpp
)

add_library(matrixes ${SRCS})
target_include_directories(matrixes PRIVATE ${CMAKE_BINARY_DIR})