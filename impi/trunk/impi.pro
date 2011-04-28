TEMPLATE = app
TARGET = impi
QT += core \
    gui \
    sql
HEADERS += dialogs/chooseAccount.h \
    plugins.h \
    plugin-interface.h \
    error.h \
    objects.h \
    impi.h
SOURCES += dialogs/chooseAccount.cpp \
    plugins.cpp \
    ui_impi.cpp \
    error.cpp \
    main.cpp \
    impi.cpp
FORMS += 
RESOURCES += 
