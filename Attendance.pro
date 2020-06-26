#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T13:18:43
#
#-------------------------------------------------

QT       += core gui widgets sql
QT += serialport
QT +=sql
QT +=multimedia multimediawidgets
//QT +=svg;



TARGET = Main
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    signature.cpp \
    allsignature.cpp \
    keyboard.cpp \
    list.cpp \
    about.cpp \
    delete.cpp \
    logs.cpp \
    blanker.cpp

HEADERS += \
        mainwindow.h \
    signature.h \
    allsignature.h \
    keyboard/keyboard.h \
    keyboard/ui_keyboard.h \
    ui_keyboard.h \
    keyboard.h \
    list.h \
    about.h \
    delete.h \
    logs.h \
    blanker.h

FORMS += \
        mainwindow.ui \
    signature.ui \
    allsignature.ui \
    keyboard.ui \
    list.ui \
    about.ui \
    delete.ui \
    logs.ui \
    blanker.ui

target.path = /home/pi
INSTALLS += target
LIBS += -L/usr/local/lib -lwiringPi
