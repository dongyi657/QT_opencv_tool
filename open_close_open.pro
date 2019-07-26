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
        opencv_draw.cpp \
        opencv_edge_detection.cpp \
        opencv_filter.cpp \
        opencv_morphology.cpp \
        opencv_remap.cpp \
        opencv_template_match.cpp

HEADERS += \
        lib4opencvtool.h \
        lib4opencvtoolmanager.h \
        mainwindow.h \
        opencv_draw.h \
        opencv_edge_detection.h \
        opencv_filter.h \
        opencv_morphology.h \
        opencv_remap.h \
        opencv_template_match.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += D:/workcache/OpenCV/includelibbin/include\
               D:/workcache/OpenCV/includelibbin/include/opencv\
               D:/workcache/OpenCV/includelibbin/include/opencv2

#unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin -llibopencv_calib3d346

unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_calib3d346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_core346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_dnn346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_features2d346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_flann346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_highgui346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_imgcodecs346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_imgproc346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_ml346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_objdetect346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_photo346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_shape346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_stitching346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_superres346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_video346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_videoio346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -llibopencv_videostab346
unix|win32: LIBS += -LD:/workcache/OpenCV/includelibbin/bin/ -lopencv_ffmpeg346
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
