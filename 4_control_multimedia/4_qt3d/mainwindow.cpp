#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QActionGroup>
#include <QMenuBar>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QTextureMaterial>

Q_DECLARE_METATYPE(QCameraInfo)


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _view = new Qt3DExtras::Qt3DWindow();
    _central_widget = QWidget::createWindowContainer(_view);
    _video_surface = new VideoSurface();
    
    setCentralWidget(_central_widget);
    _root_entity = new Qt3DCore::QEntity();
    Qt3DRender::QCamera *camera_entity = _view->camera();
    // 2d scene
    // https://stackoverflow.com/questions/46821234/qt3d-qtquick-scene2d-using-c
    /*
    cameraEntity->setPosition(QVector3D(0, 0, 50.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    */

    // steps: https://doc.qt.io/qt-5.10/qt3drender-qmaterial.html

    // NOTE: probably want a 2D texture instead
    // texture material?
    // https://doc.qt.io/qt-5.10/qt3dextras-qtexturematerial.html#details

    // QTextureMaterial -> QTexture2D -> QTextureImageData (Gene'd by TextureGenerator)
    
    // QTexture2D is the winner. Should probably let the VideoSurface hold onto it.
    Qt3DExtras::QTextureMaterial *texture_material = new Qt3DExtras::QTextureMaterial();
    // FIXME: Implement
    // texture_material->setTexture();

    _view->setRootEntity(_root_entity);
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
    _camera.data()->setViewfinder(_video_surface);
    _camera.data()->start();
}
