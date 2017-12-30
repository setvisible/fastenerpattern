#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_tensor
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_tensor.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS  += \
    $$PWD/../../src/core/units/area_moment_of_inertia.h \
    $$PWD/../../src/core/units/unit_system.h \
    $$PWD/../../src/core/global.h \
    $$PWD/../../src/core/tensor.h \
    $$PWD/../../src/math/utils.h

SOURCES += \
    $$PWD/../../src/core/tensor.cpp

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
