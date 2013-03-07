#-------------------------------------------------
#
# Project created by QtCreator 2012-09-23T11:35:20
#
#-------------------------------------------------

QT       += core gui

TARGET = qt_v4l2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    videodevice.cpp \
    detect.cpp \
    Watermark.cpp \
    Radon.cpp

HEADERS  += widget.h \
    videodevice.h \
    watermark.h \
    Radon.h \

INCLUDEPATH += /work/code/opencv/include/opencv

LIBS +=-lpthread \
-lrt \
-lm \
 /work/code/opencv/lib/libcv.so \
/work/code/opencv/lib/libcxcore.so \
/work/code/opencv/lib/libcvaux.so \
/work/code/opencv/lib/libml.so \
/work/code/opencv/lib/libhighgui.so \


FORMS    += widget.ui
