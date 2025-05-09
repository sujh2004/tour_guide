
QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backpackpage.cpp \
    devicesearchwindow.cpp \
    edge.cpp \
    favoritewidget.cpp \
    introwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    migonggame.cpp \
    musicplayer.cpp \
    node.cpp \
    nodelist.cpp \
    playercontroller.cpp \
    puzzlewidget.cpp \
    routeplanner.cpp \
    taskpage.cpp

HEADERS += \
    backpackpage.h \
    devicesearchwindow.h \
    edge.h \
    favoritewidget.h \
    introwidget.h \
    mainwindow.h \
    migonggame.h \
    musicplayer.h \
    node.h \
    nodelist.h \
    playercontroller.h \
    puzzlewidget.h \
    routeplanner.h \
    taskpage.h

FORMS += \
    favoritewidget.ui \
    introwidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
