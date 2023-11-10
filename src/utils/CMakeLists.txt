set(SRCS
        std_vector.cpp
        std.cpp
)

add_library(utils ${SRCS})
target_include_directories(avl PRIVATE ${CMAKE_BINARY_DIR})