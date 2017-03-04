
set(MY_TEST_TARGET tst_tensor)

set(MY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tensor.cpp
    )

add_executable(${MY_TEST_TARGET} WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/test/tensor/tst_tensor.cpp
    ${MY_TEST_SOURCES}
    )

# Qt
qt5_use_modules(${MY_TEST_TARGET} Core Test )

