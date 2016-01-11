#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24T22:36:42
#
#-------------------------------------------------

QT       += core gui widgets network xml webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bestsoft-Space
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    highlighter.cpp \
    start.cpp \
    new.cpp \
    httpdownload.cpp \
    settings.cpp \
    textedit.cpp \
    newsource.cpp

HEADERS  += mainwindow.h \
    highlighter.h \
    start.h \
    new.h \
    httpdownload.h \
    settings.h \
    textedit.h \
    newsource.h

FORMS    += mainwindow.ui \
    start.ui \
    new.ui \
    codehelp.ui \
    httpdownload.ui \
    settings.ui \
    newsource.ui
