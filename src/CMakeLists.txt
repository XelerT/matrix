set(SRC_LIST
        matrixes
        ui
        utils
        tester
)

foreach(SRC ${SRC_LIST})
        add_subdirectory(${SRC})
endforeach()

set(SRC_LIST "${SRC_LIST}" PARENT_SCOPE)