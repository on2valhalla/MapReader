#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T10:03:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapReader
TEMPLATE = app


SOURCES += main.cpp\
        mapreader.cpp

HEADERS  += mapreader.h

FORMS    += mapreader.ui



INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/ \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_objdetect \
   -lopencv_calib3d