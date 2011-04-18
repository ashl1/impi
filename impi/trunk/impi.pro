TEMPLATE = app
TARGET = impi
QT += core \
    gui \
    sql
HEADERS += plugin-interface.h \
    error.h \
    objects.h \
    impi.h
SOURCES += ui_impi.cpp \
    error.cpp \
    main.cpp \
    impi.cpp
FORMS +=
RESOURCES +=
