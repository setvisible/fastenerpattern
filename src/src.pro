#-------------------------------------------------
#                FastenerPattern
#-------------------------------------------------
TEMPLATE = app
TARGET   = FastenerPattern
QT      += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += no_keyword

# QT += c++11
QMAKE_CXXFLAGS += -std=c++11

LANGUAGE = C++

lessThan(QT_VERSION, 5.0) {
    warning("prefere to build it with Qt 5.0")
}

#-------------------------------------------------
# INCLUDE
#-------------------------------------------------
INCLUDEPATH += $$PWD/../include/


#-------------------------------------------------
# DEPENDENCIES
#-------------------------------------------------
include($$PWD/../3rd/boost.pri)


#-------------------------------------------------
# VERSION
#-------------------------------------------------
include($$PWD/../version.pri)

#-------------------------------------------------
# SOURCES
#-------------------------------------------------
include($$PWD/core/core.pri)
include($$PWD/dialogs/dialogs.pri)
include($$PWD/editor/editor.pri)
include($$PWD/widgets/widgets.pri)

HEADERS  += \
    $$PWD/mainwindow.h \
    $$PWD/version.h \
    about.h

SOURCES += \
    $$PWD/mainwindow.cpp \
    $$PWD/main.cpp

FORMS    += \
    $$PWD/mainwindow.ui

OTHER_FILES += \
    $$PWD/../README.md

#-------------------------------------------------
# OUTPUT
#-------------------------------------------------

RESOURCES += \
    $$PWD/resources.qrc \
    $$PWD/examples/examples.qrc

win32{
    RC_FILE += $$PWD/resources_win.rc
}
