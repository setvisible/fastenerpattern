#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_optimisationsolver
CONFIG      += testcase
QT           = core gui testlib
SOURCES     += tst_optimisationsolver.cpp


HEADERS     += $$PWD/dummysolver.h
SOURCES     += $$PWD/dummysolver.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += $$PWD/../../src/core/optimizer/controller.h
SOURCES += $$PWD/../../src/core/optimizer/controller.cpp

HEADERS += $$PWD/../../src/core/optimizer/maxminload.h
SOURCES += $$PWD/../../src/core/optimizer/maxminload.cpp

HEADERS += $$PWD/../../src/core/optimizer/optimisationsolver.h
SOURCES += $$PWD/../../src/core/optimizer/optimisationsolver.cpp

HEADERS += $$PWD/../../src/core/designspace.h
SOURCES += $$PWD/../../src/core/designspace.cpp

HEADERS += $$PWD/../../src/core/fastener.h
SOURCES += $$PWD/../../src/core/fastener.cpp

HEADERS += $$PWD/../../src/core/splice.h
SOURCES += $$PWD/../../src/core/splice.cpp

HEADERS += $$PWD/../../src/core/tensor.h
SOURCES += $$PWD/../../src/core/tensor.cpp

HEADERS += $$PWD/../../src/core/solvers/isolver.h
HEADERS += $$PWD/../../src/core/units/unit_system.h
HEADERS += $$PWD/../../src/math/utils.h

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
