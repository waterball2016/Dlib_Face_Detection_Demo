#-------------------------------------------------
#
# Project created by QtCreator 2017-07-26T10:49:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dlib_Face_Detection_Demo
TEMPLATE = app

include($$PWD/dlib/dlib.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

QMAKE_CXXFLAGS += -mfloat-abi=softfp -mfpu=neon -DDLIB_HAVE_NEON

android {
    RESOURCES = image.qrc
}

ios {
    deployment.files = $$PWD/test.bmp
    QMAKE_BUNDLE_DATA += deployment
}

