
set(MY_TEST_TARGET tst_optimisationsolver)

set(MY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/optimizer/maxminload.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/optimizer/optimisationsolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/solvers/parameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/solvers/rigidbodysolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/designspace.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/fastener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/splice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tensor.cpp
    )

add_executable(${MY_TEST_TARGET} WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/test/optimisationsolver/dummysolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/optimisationsolver/tst_optimisationsolver.cpp
    ${MY_TEST_SOURCES}
    )

# Qt
qt5_use_modules(${MY_TEST_TARGET} Core Gui Test )

