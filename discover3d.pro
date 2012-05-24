#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T20:40:27
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = discover3d
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    aboutdialog.cpp \
    settingsdialog.cpp \
    highscoresdialog.cpp \
    newgamedialog.cpp \
    level.cpp \
    arcball.cpp \
    editordialog.cpp \
    texturemanager.cpp \
    highscores.cpp \
    util.cpp \
    levelrenderer.cpp \
    levelserializer.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    aboutdialog.h \
    settingsdialog.h \
    highscoresdialog.h \
    newgamedialog.h \
    level.h \
    arcball.h \
    editordialog.h \
    texturemanager.h \
    highscores.h \
    global.h \
    util.h \
    levelrenderer.h \
    levelserializer.h

RESOURCES += \
    discover3d.qrc

win32:RC_FILE += discover3d.rc

FORMS += \
    aboutdialog.ui \
    helpdialog.ui \
    settingsdialog.ui \
    highscoresdialog.ui \
    newgamedialog.ui \
    loadgamedialog.ui \
    savegamedialog.ui \
    editordialog.ui
