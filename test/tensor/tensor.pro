#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_tensor
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_tensor.cpp

QMAKE_CXXFLAGS += -std=c++11

# 3rd libraries:
include($$PWD/../../3rd/boost.pri)

# Include:
INCLUDEPATH += $$PWD/../../include

# Dependancies:
HEADERS += $$PWD/../../src/core/units/unit_system.h
HEADERS += $$PWD/../../src/core/global.h
HEADERS += $$PWD/../../src/core/tensor.h

SOURCES += $$PWD/../../src/core/tensor.cpp

