#-------------------------------------------------
#
# Project created by QtCreator 2018-04-14T20:59:21
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3_opencv
TEMPLATE = app
INCLUDEPATH += \
    /usr/include/opencv2
    /usr/include/opencv

!contains(QT_CONFIG, no-pkg-config) {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
} else {
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_objdetect
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    videowidget.cpp \
    videosurface.cpp

HEADERS += \
        mainwindow.h \
    videowidget.h \
    videosurface.h

RESOURCES += \
    resource.qrc
