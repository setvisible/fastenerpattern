#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_boost
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_boost.cpp

# 3rd libraries:
include($$PWD/../../3rd/boost.pri)


