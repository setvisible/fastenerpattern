
set(MY_TEST_TARGET tst_splicecalculator)

set(MY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/solvers/parameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/solvers/rigidbodysolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/abstractsplicemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/splicecalculator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/fastener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tensor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/splice.cpp
    )

add_executable(${MY_TEST_TARGET} WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/test/splicecalculator/tst_splicecalculator.cpp
    ${MY_TEST_SOURCES}
    )

# Qt
qt5_use_modules(${MY_TEST_TARGET} Core Test )

# Gecode
target_link_libraries(${MY_TEST_TARGET} ${GECODE_LIBRARIES})
