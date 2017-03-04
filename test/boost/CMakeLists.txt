
set(MY_TEST_TARGET tst_boost)

set(MY_TEST_SOURCES
    )

add_executable(${MY_TEST_TARGET} WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/test/boost/tst_boost.cpp
    ${MY_TEST_SOURCES}
    )

# Qt
qt5_use_modules(${MY_TEST_TARGET} Core Test )

