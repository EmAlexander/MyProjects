#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T20:24:11
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cuebicEngine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editorwidgetcl.cpp \
    buildoptions.cpp \
    wiki.cpp \
    gobjclass.cpp \
    engineoptions.cpp

HEADERS  += mainwindow.h \
    editorwidgetcl.h \
    buildoptions.h \
    wiki.h \
    gobjclass.h \
    engineoptions.h

FORMS    += mainwindow.ui \
    buildoptions.ui \
    wiki.ui \
    engineoptions.ui

RESOURCES += \
    Resources.qrc


unix|win32: LIBS += -L$$PWD/lua/ -llua52

INCLUDEPATH += $$PWD/lua/include
DEPENDPATH += $$PWD/lua/include
