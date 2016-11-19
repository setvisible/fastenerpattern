project(FastenerPattern)

message(STATUS "Tips: Building with all the CPU cores: > cmake --build . --target '' -j")

### Versioning
set(APP_VERSION_MAJOR "0")
set(APP_VERSION_MINOR "1")
set(APP_VERSION_PATCH "2")
set(APP_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")

configure_file( ./src/config.h.cmake ./src/config.h )

# -------- Qt5 --------
# RTFM here:
# https://cmake.org/cmake/help/v3.0/manual/cmake-qt.7.html
find_package(Qt5Core    REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# -------- Boost --------
find_package(Boost REQUIRED)
configure_file( ./3rd/boost.pri.cmake ./3rd/boost.pri )
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_MATH_DISABLE_FLOAT128)

### Include
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(./include/)

### CMake Compiler Options
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

### Sources
include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/editor/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/CMakeLists.txt)

# Qt -- Run the MOC, resource, etc.
qt5_wrap_cpp(       fastenerpattern_HEADERS_MOC    ${MY_HEADERS}   )
qt5_wrap_ui(        fastenerpattern_FORMS_HEADERS  ${MY_FORMS}     )

add_library(configwin
    ${fastenerpattern_HEADERS_MOC}
    ${fastenerpattern_FORMS_HEADERS}
    )

qt5_use_modules(configwin Widgets)

# Linking the executable
add_executable(fastenerpattern WIN32 ${MY_SOURCES} ${configwin} ${MY_RESOURCES})
qt5_use_modules(fastenerpattern Core Gui Widgets)


#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------

# # Deploy MinGW *.DLL files for Windows
# # Note: This is a dirty 'hack' for MinGW
# if(WIN32 AND MINGW)
#     find_path( LIBGCC_S_DW2_DLL_DIR    libgcc_s_dw2-1.dll    DOC "Location of libgcc_s_dw2-1.dll")
#     find_path( LIBSTDC_6_DLL_DIR       libstdc++-6.dll       DOC "Location of libstdc++-6.dll")
#     find_path( LIBWINPTHREAD_DLL_DIR   libwinpthread-1.dll   DOC "Location of libwinpthread-1.dll")
#     set(MY_MINGW_DEPENDANCIES
#         ${LIBGCC_S_DW2_DLL_DIR}/libgcc_s_dw2-1.dll
#         ${LIBSTDC_6_DLL_DIR}/libstdc++-6.dll
#         ${LIBWINPTHREAD_DLL_DIR}/libwinpthread-1.dll
#     )
#     install (
#         FILES ${MY_MINGW_DEPENDANCIES}
#         DESTINATION ${CMAKE_INSTALL_PREFIX}
#         COMPONENT mingw_dependancies
#     )
#     message(STATUS "Found MINGW compiler: ${MINGW}")
# endif(WIN32 AND MINGW)


# Deploy documentation files
set(MY_RELEASE_DOCS
    ./src/examples/4BoltJoint.splice
    ./src/examples/PatternJoint.splice
    ./src/examples/RandomJoint.splice
    ./user-manual/User_Manual.pdf
    ./LICENSE
    )

install (
    FILES ${MY_RELEASE_DOCS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    COMPONENT release_docs
)

# Deploy the executable
install(TARGETS fastenerpattern RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

