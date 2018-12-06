#-------------------------------------------------
#
# Project created by QtCreator 2018-08-19T11:15:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = FFaudioConverter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/dirscan.cpp \
    src/ffmpegtask.cpp \
    src/settingsdialog.cpp \
    src/settings.cpp \
    src/util.cpp

HEADERS += \
    src/mainwindow.h \
    src/dirscan.h \
    src/ffmpegtask.h \
    src/settingsdialog.h \
    src/settings.h \
    src/util.h

FORMS += \
    src/mainwindow.ui \
    src/settingsdialog.ui

# Set default path for the ffmpeg binary
exists("/.flatpak-info") {
    DEFAULT_FFMPEG_BINARY = /app/bin/ffmpeg
} else {
    DEFAULT_FFMPEG_BINARY = /usr/bin/ffmpeg
}
DEFINES += DEFAULT_FFMPEG_BINARY

# Deployment
isEmpty(PREFIX) {
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
} else:unix {
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share

    target.path = $$BINDIR

    icon.files = ffaudioconverter.png
    icon.path = $$DATADIR/icons/hicolor/512x512/apps/

    desktop.files = ffaudioconverter.desktop
    desktop.path = $$DATADIR/applications/

    INSTALLS += target icon desktop
}
