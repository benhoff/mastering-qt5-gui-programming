#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QActionGroup>
#include <QMenuBar>

Q_DECLARE_METATYPE(QCameraInfo)


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    // FIXME
    _central_widget = QWidget::createWindowContainer(view);
    
    setCentralWidget(_central_widget);
    Qt3DCore::QEntity *root_entity = new Qt3DCore::QEntity();
    Qt3DRender::QCamera *camera_entity = view->camera();
    // set camera perspective, position, up vector, view center
    
    // QTexture2D is the winner. Should probably let the VideoSurface hold onto it.

    view->setRootEntity(root_entity);
    _setup_camera_devices();

    set_camera(QCameraInfo::defaultCamera());

    if (_camera.data()->isCaptureModeSupported(QCamera::CaptureVideo))
        _camera.data()->setCaptureMode(QCamera::CaptureVideo);
    else
        qFatal("Capture Video not supported by this camera!");

}

MainWindow::~MainWindow()
{

}

void MainWindow::_setup_camera_devices()
{
    QActionGroup *camera_group = new QActionGroup(this);
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QMenuBar* menu_bar = menuBar();
    QMenu *device_menu = menu_bar->addMenu("Devices");

    for (const QCameraInfo &camera_info: cameras)
    {
        QAction *camera_action = new QAction(camera_info.description(), camera_group);
        camera_action->setCheckable(true);
        camera_action->setData(QVariant::fromValue(camera_info));
        if (camera_info == QCameraInfo::defaultCamera())
            camera_action->setChecked(true);
        device_menu->addAction(camera_action);
    }

    connect(camera_group, &QActionGroup::triggered, this, &MainWindow::set_camera_action);
}

void MainWindow::set_camera_action(QAction *camera_action)
{
    set_camera(qvariant_cast<QCameraInfo>(camera_action->data()));
}

void MainWindow::set_camera(const QCameraInfo &camera_info)
{
    _camera.reset(new QCamera(camera_info));
    _camera.data()->setViewfinder(_video_widget->get_videosurface());
    _camera.data()->start();
}
