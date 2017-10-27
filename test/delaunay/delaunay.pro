#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_delaunay
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_delaunay.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
include($$PWD/../../src/maths/maths.pri)

#-------------------------------------------------
# TRIANGLE
#-------------------------------------------------
include($$PWD/../../FastenerPattern_config.pri)
include($$PWD/../../3rd/3rd.pri)
