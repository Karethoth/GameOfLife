#-------------------------------------------------
#
# Project created by QtCreator 2020-03-16T17:57:48
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameOfLife
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

CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/cellkernel.cpp \
    src/lifegridscene.cpp \
    src/lifegrid.cpp \
    src/ui/mainwindow.cpp \
    src/ui/resizedialog.cpp

HEADERS += \
    src/cellkernel.h \
    src/lifegridscene.h \
    src/lifegrid.h \
    src/ui/mainwindow.h \
    src/ui/resizedialog.h

FORMS += \
    ui/mainwindow.ui \
    ui/resizedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
