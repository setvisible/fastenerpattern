# CMakeLists.txt

# Copyright (C) 2017 Sebastien Vavassori

# This code is released under the LGPL license.
# For conditions of distribution and use, see the disclaimer
# and license in LICENSE

cmake_minimum_required(VERSION 2.8)
#cmake_policy(VERSION 2.8)

project(FastenerPattern)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

##############################################################################
## Versioning
##############################################################################

# read 'version' file into a variable (stripping any newlines or spaces)
file(READ version versionFile)
if (NOT versionFile)
    message(FATAL_ERROR "Unable to determine FastenerPattern version. Version file is missing.")
endif()
string(STRIP "${versionFile}" FASTENER_PATTERN_VERSION)
# add a dependency on the versino file
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS version)

# split version string into components, note CMAKE_MATCH_0 is the entire regexp match
string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" APP_VERSION ${FASTENER_PATTERN_VERSION} )
set(APP_VERSION_MAJOR ${CMAKE_MATCH_1})
set(APP_VERSION_MINOR ${CMAKE_MATCH_2})
set(APP_VERSION_PATCH ${CMAKE_MATCH_3})


configure_file( ./src/config.h.cmake ./src/config.h )


##############################################################################
# Rename the executable for DEBUG and RELEASE
##############################################################################

set(FastenerPattern_NAME "fastenerpattern")

# Change the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# to distinguish between debug and release
set(CMAKE_DEBUG_POSTFIX "d")



##############################################################################
# build options
##############################################################################
#TODO: enable_testing()
option(ENABLE_TESTS   "Set to ON to build test applications (default)" ON)


##############################################################################
## Qt5
##############################################################################
# https://cmake.org/cmake/help/v3.0/manual/cmake-qt.7.html
# find_package(Qt5Core    REQUIRED)
# find_package(Qt5Gui     REQUIRED)
# find_package(Qt5Widgets REQUIRED)

find_package(Qt5 5.1 COMPONENTS Core)
find_package(Qt5 5.1 COMPONENTS Gui)
find_package(Qt5 5.1 COMPONENTS Widgets)
if(ENABLE_TESTS)
    find_package(Qt5 5.1 COMPONENTS Test)
endif(ENABLE_TESTS)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


##############################################################################
## 3RD-PARTY DEPENDENCIES
##############################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/3rd/CMakeLists.txt)


##############################################################################
### Include
##############################################################################
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(./include/)

##############################################################################
### Sources
##############################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/editor/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/math/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/CMakeLists.txt)



##############################################################################
# Qt -- Run the MOC, resource, etc.
##############################################################################
qt5_wrap_cpp(       fastenerpattern_HEADERS_MOC    ${MY_HEADERS}   )
qt5_wrap_ui(        fastenerpattern_FORMS_HEADERS  ${MY_FORMS}     )

add_library(configwin
    ${fastenerpattern_HEADERS_MOC}
    ${fastenerpattern_FORMS_HEADERS}
    )

qt5_use_modules(configwin Widgets)




##############################################################################
# Linking the executable
##############################################################################
add_executable(${FastenerPattern_NAME}
    WIN32
    ${MY_SOURCES}
    ${configwin}
    ${MY_RESOURCES}
    )


# Qt5
qt5_use_modules(${FastenerPattern_NAME} Core Gui Widgets )




##############################################################################
# Unit Tests
##############################################################################

#TODO: add_test()

if(ENABLE_TESTS)

    include(${CMAKE_CURRENT_SOURCE_DIR}/test/boost/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/delaunay/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/math/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/optimisationsolver/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/rigidbodysolver/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/splicecalculator/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/tensor/CMakeLists.txt)

    # install(TARGETS test1 test2 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

endif(ENABLE_TESTS)


##############################################################################
## Deploying documentation and files
##############################################################################
set(MY_RELEASE_DOCS
    ./user-manual/User_Manual.pdf
    ./LICENSE
    )
install (
    FILES ${MY_RELEASE_DOCS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    COMPONENT release_docs
    )

## Example files
set(MY_RELEASE_EXAMPLES
    ./src/examples/4BoltJoint.splice
    ./src/examples/PatternJoint.splice
    ./src/examples/RandomJoint.splice
    ./src/examples/Optimize_4BoltJoint.splice
    )
install (
    FILES ${MY_RELEASE_EXAMPLES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/examples
    COMPONENT release_examples
    )


## 3rd-party libraries
if (MSVC OR MSYS OR MINGW) # for detecting Windows compilers

    ## Qt Libraries
    get_target_property(QtCore_LOCATION Qt5::Core LOCATION)
    get_filename_component(QT_DLL_DIR ${QtCore_LOCATION} PATH)

    install(FILES
        ${QT_DLL_DIR}/libgcc_s_dw2-1.dll
        ${QT_DLL_DIR}/libstdc++-6.dll
        ${QT_DLL_DIR}/libwinpthread-1.dll
        ${QT_DLL_DIR}/Qt5Core.dll
        ${QT_DLL_DIR}/Qt5Gui.dll
        ${QT_DLL_DIR}/Qt5Widgets.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

    ## Qt Platform Plugin
    install(FILES
        ${QT_DLL_DIR}/../plugins/platforms/qwindows.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/platforms
    )

endif(MSVC OR MSYS OR MINGW)


##############################################################################
# Deploying executable
##############################################################################
install(TARGETS ${FastenerPattern_NAME}
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )


