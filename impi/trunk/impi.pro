TEMPLATE = app
CONFIG += qt \
    thread
TARGET = impi
QT += core \
    gui \
    sql
HEADERS += db-temp.h \
    limit.h \
    objects.h \
    dialogs/chooseAccount.h \
    plugins.h \
    plugin-interface.h \
    ui_impi.h \
    error.h \
    impi.h
SOURCES += db-temp.cpp \
    objects.cpp \
    dialogs/chooseAccount.cpp \
    plugins.cpp \
    ui_impi.cpp \
    error.cpp \
    impi.cpp \
    main.cpp
FORMS +=
RESOURCES +=
