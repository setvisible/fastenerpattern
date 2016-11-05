set(MY_SOURCES ${MY_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/solvers/rigidbodysolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/solvers/parameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/splice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/fastener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/splicecalculator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/abstractsplicemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tensor.cpp
    )

# Rem: set here the headers related to the Qt MOC (i.e., with associated *.ui)
set(MY_HEADERS ${MY_HEADERS}
    )

