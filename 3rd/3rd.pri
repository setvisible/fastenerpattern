#-------------------------------------------------
# 3RD-PARTY LIBRARIES
#-------------------------------------------------

# BOOST
INCLUDEPATH += $$Boost_INCLUDE_DIR
LIBS        += -L$$Boost_LIB_DIR
DEFINES     += BOOST_MATH_DISABLE_FLOAT128


# TRIANGLE
INCLUDEPATH += $$PWD/triangle
LIBS        += -lm
DEFINES     += TRILIBRARY ANSI_DECLARATORS NO_TIMER REDUCED CDT_ONLY

HEADERS += $$PWD/triangle/triangle.h
SOURCES += $$PWD/triangle/triangle.c

