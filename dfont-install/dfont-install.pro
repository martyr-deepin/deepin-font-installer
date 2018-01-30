QT += core
QT -= gui

CONFIG += c++11

TARGET = dfont-install
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/../libdeepin-font-installer
LIBS += -L$$OUT_PWD/../libdeepin-font-installer -ldeepin-font-installer

target.path = /usr/bin

INSTALLS += target
