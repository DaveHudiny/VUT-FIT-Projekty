QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++1z

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = BlockEditor
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/blockitem.cpp \
    src/classes.cpp \
    src/helpwindow.cpp \
    src/in.cpp \
    src/lineconnection.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/scene.cpp \
    src/auxilaryfunctions.cpp

HEADERS += \
    src/blockitem.h \
    src/classes.h \
    src/helpwindow.h \
    src/in.h \
    src/lineconnection.h \
    src/mainwindow.h \
    src/scene.h \
    src/auxilaryfunctions.h

FORMS += \
    src/helpwindow.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
