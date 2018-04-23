#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCamera>
#include <QCameraInfo>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/qt3dwindow.h>
#include "videosurface.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void set_camera(const QCameraInfo &camera_info);
    void set_camera_action(QAction *camera_action);
    void _setup_camera_devices();

    QScopedPointer<QCamera> _camera;

    QWidget *_central_widget;


    QPushButton *_record_button;
    QPushButton *_stop_record_button;
    Qt3DExtras::Qt3DWindow *_view;
    Qt3DCore::QEntity *_root_entity;
    VideoSurface *_video_surfaces
};

#endif // MAINWINDOW_H
