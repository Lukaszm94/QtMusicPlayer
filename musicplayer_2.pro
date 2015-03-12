QT += widgets multimedia
CONFIG += c++11
HEADERS += \
    src/musicplayer.h \
    src/musicplayerGUI.h \
    src/musicplaylist.h \
    src/progressbar.h \
    src/ticker.h \
    src/settingsdialog.h \
    src/song.h

SOURCES += \
    src/main.cpp \
    src/musicplayer.cpp \
    src/musicplayerGUI.cpp \
    src/musicplaylist.cpp \
    src/progressbar.cpp \
    src/ticker.cpp \
    src/settingsdialog.cpp

RESOURCES += \
    icons.qrc
