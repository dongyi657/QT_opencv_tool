#-------------------------------------------------
#
# Project created by QtCreator 2019-06-04T18:53:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = open_close_open
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
        lib4opencvtool.cpp \
        lib4opencvtoolmanager.cpp \
        main.cpp \
        mainwindow.cpp \
        opencv_edge_detection.cpp \
        opencv_filter.cpp \
        opencv_morphology.cpp

HEADERS += \
        lib4opencvtool.h \
        lib4opencvtoolmanager.h \
        mainwindow.h \
        opencv_edge_detection.h \
        opencv_filter.h \
        opencv_morphology.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += D:/work/GitHub/opencv_srcdir/LibBinInclude/include\
               D:/work/GitHub/opencv_srcdir/LibBinInclude/include/opencv\
               D:/work/GitHub/opencv_srcdir/LibBinInclude/include/opencv2

#unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin -llibopencv_calib3d411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_calib3d411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_core411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_dnn411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_features2d411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_flann411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_highgui411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_imgcodecs411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_imgproc411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_ml411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_objdetect411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_photo411
#unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_shape411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_stitching411
#unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_superres411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_video411
unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_videoio411
#unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -llibopencv_videostab411
#unix|win32: LIBS += -LD:/work/GitHub/opencv_srcdir/LibBinInclude/bin/ -lopencv_ffmpeg411
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
