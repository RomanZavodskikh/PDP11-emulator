#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T14:53:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PDP-11
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    resource/decode.cpp \
    resource/emulator.cpp \
    resource/vcpu.cpp \
    resource/screen.cpp

HEADERS  += mainwindow.h \
    resource/decode.h \
    resource/emulator.h \
    resource/vcpu.h \
    resource/screen.h

FORMS    += mainwindow.ui
