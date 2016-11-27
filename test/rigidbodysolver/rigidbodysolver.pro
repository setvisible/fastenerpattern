#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_rigidbodysolver
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_rigidbodysolver.cpp

QMAKE_CXXFLAGS += -std=c++11

# 3rd libraries:
include($$PWD/../../3rd/boost.pri)

# Include:
INCLUDEPATH += $$PWD/../../include

# Dependancies:
include($$PWD/../../src/core/core.pri)


