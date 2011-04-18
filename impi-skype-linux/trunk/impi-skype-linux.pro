TEMPLATE = lib
TARGET = impi-skype-linux
CONFIG += plugin
QT += core
QT -= gui
HEADERS += skype-linux.h \
	../impi/plugin-interface.h
SOURCES += skype-linux.cpp
DESTDIR = ../impi/plugins