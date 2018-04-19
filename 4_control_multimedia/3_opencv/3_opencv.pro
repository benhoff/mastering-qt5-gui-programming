#-------------------------------------------------
#
# Project created by QtCreator 2018-04-14T20:59:21
#
#-------------------------------------------------

QT       += core gui multimedia widgets

TARGET = opencv
TEMPLATE = app

# https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows

# NOTE: for windows need to add the correct include path!
# INCLUDEPATH += D:\opencv\build\include
INCLUDEPATH += \
    /usr/include/opencv2


 # NOTE: see here for windows http://doc.qt.io/qt-5/qmake-variable-reference.html#libs
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

# https://stackoverflow.com/questions/19066593/copy-a-file-to-build-directory-after-compiling-project-with-qt

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    videowidget.cpp \
    videosurface.cpp

HEADERS += \
        mainwindow.h \
    videowidget.h \
    videosurface.h

# https://stackoverflow.com/a/39234363/2701402
copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\haarcascade_frontalface_default.xml)\" \"$$shell_path($$OUT_PWD)\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
