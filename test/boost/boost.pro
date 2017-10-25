#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_boost
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_boost.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:

#-------------------------------------------------
# Boost
#-------------------------------------------------
include($$PWD/../../FastenerPattern_config.pri)

INCLUDEPATH += $$Boost_INCLUDE_DIR
LIBS        += -L$$Boost_LIB_DIR
DEFINES     += BOOST_MATH_DISABLE_FLOAT128
