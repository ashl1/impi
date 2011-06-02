TEMPLATE = lib
TARGET = impi-skype-linux
CONFIG += plugin release
VERSION = 0.1
QT += core
QT -= gui
HEADERS += skype-linux.h \
    ../impi/plugin-interface.h
SOURCES += skype-linux.cpp
DESTDIR = ../impi/plugins
