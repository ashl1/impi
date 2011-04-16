TEMPLATE = lib
TARGET = impi-skype-linux
CONFIG += plugin
QT += core
QT -= gui
HEADERS += skype-linux.h \
	../impi/client-interface.h
SOURCES += skype-linux.cpp	
DESTDIR = ../impi/plugins