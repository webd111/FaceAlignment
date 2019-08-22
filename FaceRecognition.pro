#-------------------------------------------------
#
# Project created by QtCreator 2019-07-14T15:28:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRecognition
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
        main.cpp \
        mainwindow.cpp \
    my_opencv.cpp \
    faceprocessor.cpp \
    facechecker.cpp

HEADERS += \
        mainwindow.h \
    my_opencv.h \
    faceprocessor.h \
    facechecker.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += "D:/OpenCV/opencv-4.0.0/install/include"

QMAKE_LIBDIR += "D:/OpenCV/opencv-4.0.0/install/x64/vc15/lib"

CONFIG(debug, debug|release){LIBS += opencv_world400d.lib}
else{LIBS += opencv_world400.lib}

