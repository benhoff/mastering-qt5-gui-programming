#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QActionGroup>
#include <QMenuBar>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QTextureMaterial>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QLayerFilter>
#include <Qt3DRender/QCameraSelector>

Q_DECLARE_METATYPE(QCameraInfo)


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _view = new Qt3DExtras::Qt3DWindow();
    _central_widget = QWidget::createWindowContainer(_view);
    _video_surface = new VideoSurface();
    
    setCentralWidget(_central_widget);
    _root_entity = new Qt3DCore::QEntity();

    Qt3DRender::QRenderSurfaceSelector *rendor_surface_selector = new Qt3DRender::QREnderSurfaceSelector();

    // First branch clear the buffers
    Qt3DRender::QClearBuffers *clear_buffers = new Qt3DRender::QClearBuffers(rendor_surface_selector);
    clear_buffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);


    // Second branch
    // Custom framegraph for background image
    Qt3DRender::QLayerFilter *background_layer_filter = new Qt3DRender::QLayerFilter(rendor_surface_selector);
    Qt3DRender::QLayer *background_layer = new Qt3DRender::QLayer(background_layer_filter);
    background_layer_filter->addLayer(background_layer);
    Qt3DRender::QCameraSelector *camera_selector = new Qt3DRender::QCameraSelector(background_layer);
    Qt3DRender::QCamera background_camera = new Qt3DRender::QCamera(camera_selector);
    background_camera->lens()->setOrthographicProjection(-1, 1, -1, 1, 0.1f, 1000.f);
    backgroun_camera->setPosition(QVector3D(0, 0, 1));
    background_camera->setViewCenter(QVector3D(0, 0, 0));
    background_camera->setUpVector(QVector3D(0, 1, 0));
    camera_selector->setCamera(backgroundCamera);

    // third branch
    // framegraph for objects
    // FIXME

    // https://github.com/qt/qt3d/blob/5.11/tests/manual/video-texture-qml/main.qml


    // set the new framegraph
    _view->setActiveFrameGraph(rendor_surface_selector);
    _view->renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);


    // root entity
    Qt3DCore::QEntity *plane_entity = new Qt3DCore::QEntity(_root_entity);
    Qt3DExtras::QPlaneMesh *plane_mesh = new Qt3DExtras::QPlaneMesh(plane_entity);

    // plane_mesh->setHeight(2);
    // plane_mesh->setWidth(2);

    Qt3DExtras::QTextureMaterial *plane_material = new Qt3DExtras::QTextureMaterial(plane_entity);
    Texture2D *plane_texture_image = new Texture2D(plane_material);

    // Add texture image?
    plane_material->setTexture(plane_texture_image);

    /*
    Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform(planeEntity);
    planeTransform->setRotationX(90);
    planeTransform->setTranslation(QVector3D(0, 0, 0));

    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeMaterial);
    planeEntity->addComponent(planeTransform);
    planeEntity->addComponent(backgroundLayer);
    */

// https://doc.qt.io/archives/4.6/opengl-framebufferobject.html
// http://interest.qt-project.narkive.com/cKrvD9nr/synchronous-update-of-qt3d-texture-data



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
