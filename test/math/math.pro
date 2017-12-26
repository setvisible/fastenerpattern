#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_utils
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_utils.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS  += $$PWD/../../src/math/utils.h
