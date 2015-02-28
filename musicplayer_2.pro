QT += widgets multimedia
CONFIG += c++11
HEADERS += \
    src/musicplayer.h \
    src/musicplayerGUI.h \
    src/musicplaylist.h \
    src/progressbar.h \
    src/ticker.h

SOURCES += \
    src/main.cpp \
    src/musicplayer.cpp \
    src/musicplayerGUI.cpp \
    src/musicplaylist.cpp \
    src/progressbar.cpp \
    src/ticker.cpp

RESOURCES += \
    icons.qrc
