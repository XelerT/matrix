set(SRCS
        test_lazy.cpp
)

add_library(tester ${SRCS})
target_include_directories(tester PRIVATE ${CMAKE_BINARY_DIR})