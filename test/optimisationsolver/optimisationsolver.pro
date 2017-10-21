#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_optimisationsolver
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_optimisationsolver.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
include($$PWD/../../src/core/core.pri)

#-------------------------------------------------
# Boost
#-------------------------------------------------

# INSTRUCTION: Replace the following path by your path:
Boost_INCLUDE_DIR  = "C:/Qt/Boost/boost_1_61_0/"
Boost_LIB_DIR      = "C:/Qt/Boost/boost_1_61_0/stage/lib/"
#


INCLUDEPATH += $$Boost_INCLUDE_DIR
LIBS        += -L$$Boost_LIB_DIR
DEFINES     += BOOST_MATH_DISABLE_FLOAT128
