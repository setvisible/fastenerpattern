#cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)
cmake_minimum_required(VERSION 3.0)

# if(COMMAND cmake_policy)
#    if(POLICY CMP0054)
#        cmake_policy(SET CMP0054 NEW)
#    endif()
#    # Mac RPATH policy
#    if(POLICY CMP0042)
#        cmake_policy(SET CMP0042 NEW)
#    endif()
# endif()

# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE "Release" CACHE STRING
#             "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
#             FORCE)
# endif(NOT CMAKE_BUILD_TYPE)

# let's use & require C++11 - note these are only functional with CMake 3.1
# we do manual fallbacks for CMake 3.0 in the compilers section
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED YES)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


project(FastenerPattern)

# We have some custom .cmake scripts not in the official distribution.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")


# message(STATUS "Tips: Building with all the CPU cores: > cmake --build . --target '' -j")


##############################################################################
# Rename the executable for DEBUG and RELEASE
##############################################################################

set(FastenerPattern_NAME "fastenerpattern")

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(FastenerPattern_NAME ${FastenerPattern_NAME}d)
endif()

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()


##############################################################################
# build options
##############################################################################
option(ENABLE_GECODE  "Set to ON to build FastenerPattern with GeCOde support (default)" ON)
option(ENABLE_TESTS   "Set to ON to build test applications (default)" ON)



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
## Boost
##############################################################################
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_MATH_DISABLE_FLOAT128)

##############################################################################
# Gecode
##############################################################################
if(ENABLE_GECODE)
    find_package(Gecode REQUIRED
     #   COMPONENTS
     #   kernel
     #   support
     #   int
     #   set
     #   driver
     #   flatzinc
     #   gist
     #   minimodel
     #   search
        )
    include_directories(${GECODE_INCLUDE_DIR})
    add_definitions(-DWITH_GECODE)
   # find_package(GeCode REQUIRED)
   # add_definitions(-DWITH_GECODE)
   # include_directories( ${GECODE_INCLUDE_DIR} )
    message(STATUS "Gecode enabled")

else()
    message(WARNING "Gecode disabled")
endif()


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

# Gecode
target_link_libraries(${FastenerPattern_NAME} ${GECODE_LIBRARIES})


##############################################################################
# Unit Tests
##############################################################################
if(ENABLE_TESTS)

    include(${CMAKE_CURRENT_SOURCE_DIR}/test/boost/CMakeLists.txt)
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

## Examples
set(MY_RELEASE_EXAMPLES
    ./src/examples/4BoltJoint.splice
    ./src/examples/PatternJoint.splice
    ./src/examples/RandomJoint.splice
    )
install (
    FILES ${MY_RELEASE_EXAMPLES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/examples
    COMPONENT release_examples
    )

## Qt
find_path( QT5CORE_DLL_DIR     Qt5Core.dll    DOC "Location of Qt5Core.dll")
find_path( QT5GUI_DLL_DIR      Qt5Gui.dll     DOC "Location of Qt5Gui.dll")
find_path( QT5WIDGETS_DLL_DIR  Qt5Widgets.dll DOC "Location of Qt5Widgets.dll")

set(MY_QT_MSVC_DLLS
    ${QT5CORE_DLL_DIR}/Qt5Core.dll
    ${QT5GUI_DLL_DIR}/Qt5Gui.dll
    ${QT5WIDGETS_DLL_DIR}/Qt5Widgets.dll
    )
install(
    FILES ${MY_QT_MSVC_DLLS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    COMPONENT qt_dependancies
    )

set(MY_QT_MSVC_WINDOWS_DLLS
    ${CMAKE_PREFIX_PATH}/plugins/platforms/qwindows.dll
    )
install(
    FILES ${MY_QT_MSVC_WINDOWS_DLLS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/platforms
    COMPONENT qt_dependancies2
    )



##############################################################################
# Deploying executable
##############################################################################
install(TARGETS ${FastenerPattern_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})


