set(SRCS
        std_vector.cpp
        std.cpp
        alloc.cpp
        utils.cpp
)

add_library(utils ${SRCS})
target_include_directories(utils PRIVATE ${CMAKE_BINARY_DIR})