######################################################################
# Automatically generated by qmake
# > qmake.exe -project
######################################################################

TEMPLATE = app
TARGET   = FastenerPattern_Qt
QT       += core gui
# QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += no_keyword

# CONFIG += c++11
# LANGUAGE = C++
# QT += c++11
QMAKE_CXXFLAGS += -std=c++11

lessThan(QT_VERSION, 5.0) {
    warning("prefere to build it with Qt 5.0")
}

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


#-------------------------------------------------
# VERSION
#-------------------------------------------------
# Dummy version
APP_VERSION = 9.99.999
DEFINES += APP_VERSION=\\\"$$APP_VERSION\\\"


#-------------------------------------------------
# INCLUDE
#-------------------------------------------------
INCLUDEPATH += $$PWD/../include/


#-------------------------------------------------
# SOURCES
#-------------------------------------------------
include($$PWD/core/core.pri)
include($$PWD/dialogs/dialogs.pri)
include($$PWD/editor/editor.pri)
include($$PWD/widgets/widgets.pri)

HEADERS  += \
    $$PWD/about.h \
    $$PWD/version.h \
    $$PWD/mainwindow.h


SOURCES += \
    $$PWD/mainwindow.cpp \
    $$PWD/main.cpp

FORMS += \
    $$PWD/mainwindow.ui



#-------------------------------------------------
# RESOURCES
#-------------------------------------------------
RESOURCES += $$PWD/resources.qrc
RESOURCES += $$PWD/examples/examples.qrc

win32|unix {
    RC_FILE += $$PWD/resources_win.rc
}


#-------------------------------------------------
# BUILD OPTIONS
#-------------------------------------------------

# Rem: On Ubuntu, directories starting with '.' are hidden by default
win32{
    MOC_DIR =      ./.moc
    OBJECTS_DIR =  ./.obj
    UI_DIR =       ./.ui
}else{
    MOC_DIR =      ./_moc
    OBJECTS_DIR =  ./_obj
    UI_DIR =       ./_ui
}

#-------------------------------------------------
# OUTPUT
#-------------------------------------------------


#-------------------------------------------------
# TARGET DIRECTORY
#-------------------------------------------------
# Needs to define a subfolder, as the *.DLL
# cannot be copied in the root folder.
#DESTDIR = build
#
#build_pass:CONFIG(debug, debug|release) {
#    unix: TARGET = $$join(TARGET,,,_debug)
#    else: TARGET = $$join(TARGET,,,d)
#}
#