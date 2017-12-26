
set(MY_TEST_TARGET tst_delaunay)

set(MY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/delaunay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/triangle/triangle.c
    )

add_executable(${MY_TEST_TARGET} WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/test/delaunay/tst_delaunay.cpp
    ${MY_TEST_SOURCES}
    )

# Qt
qt5_use_modules(${MY_TEST_TARGET} Core Test )

