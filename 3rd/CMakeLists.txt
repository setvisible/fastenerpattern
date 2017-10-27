##############################################################################
## Boost
##############################################################################
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_MATH_DISABLE_FLOAT128)


##############################################################################
## Triangle
##############################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/triangle/)
add_definitions(-DTRILIBRARY -DANSI_DECLARATORS -DNO_TIMER -DREDUCED -DCDT_ONLY)

#add_library(configwin -lm )

set(MY_SOURCES ${MY_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/triangle/triangle.c
    )
