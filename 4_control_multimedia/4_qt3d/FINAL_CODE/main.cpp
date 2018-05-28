#include <QApplication>
#include <QWidget>
#include <QScreen>

#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/QCamera>

#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/QSphereMesh>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include <QPropertyAnimation>

#include "rotatetransform.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Qt3DExtras::Qt3DWindow *window = new Qt3DExtras::Qt3DWindow();
    Qt3DExtras::QForwardRenderer *forward_render = window->defaultFrameGraph();

    forward_render->setClearColor(QColor("white"));
    QWidget *window_container = QWidget::createWindowContainer(window);


    // NOTE: Grab window size out, set `window_container` min/maximums
    QSize screen_size = window->screen()->size();
    window_container->setMinimumSize(QSize(200, 100));
    window_container->setMaximumSize(screen_size);

    // TODO: Create root entity & set on window
    Qt3DCore::QEntity *root_entity = new Qt3DCore::QEntity();
    window->setRootEntity(root_entity);

    Qt3DRender::QCamera *camera_entity = window->camera();
    camera_entity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera_entity->setPosition(QVector3D(0, 0, 20.0f));
    camera_entity->setUpVector(QVector3D(0, 1, 0));
    camera_entity->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *light_entity = new Qt3DCore::QEntity(root_entity);
    // `QPointLight` is a component and will need to be added
    // to the `light_entity`
    Qt3DRender::QPointLight *point_light_component = new Qt3DRender::QPointLight();
    point_light_component->setColor("white");
    point_light_component->setIntensity(1);
    light_entity->addComponent(point_light_component);

    Qt3DCore::QTransform *light_transform = new Qt3DCore::QTransform();
    light_transform->setTranslation(camera_entity->position());
    light_entity->addComponent(light_transform);


    // TODO: Create First person Camera Controller
    Qt3DExtras::QFirstPersonCameraController *cam_controller = new Qt3DExtras::QFirstPersonCameraController(root_entity);
    cam_controller->setCamera(camera_entity);

    Qt3DCore::QEntity *sphere_entity = new Qt3DCore::QEntity(root_entity);

    Qt3DExtras::QSphereMesh *mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(30);
    mesh->setSlices(30);
    mesh->setRadius(2);

    sphere_entity->addComponent(mesh);

    Qt3DExtras::QPhongMaterial *sphere_material = new Qt3DExtras::QPhongMaterial();
    sphere_material->setDiffuse(QColor(QRgb(0xa69929)));

    sphere_entity->addComponent(sphere_material);

    Qt3DCore::QEntity *rotating_sphere = new Qt3DCore::QEntity(root_entity);
    rotating_sphere->addComponent(mesh);
    rotating_sphere->addComponent(sphere_material);

    RotateTransform *rotate_transform = new RotateTransform();
    QPropertyAnimation *rotation_animation = new QPropertyAnimation(rotate_transform, "user_angle");
    rotation_animation->setStartValue(0);
    rotation_animation->setEndValue(360);
    rotation_animation->setDuration(10000);
    rotation_animation->setLoopCount(-1); // infinite loops
    rotation_animation->start();          // start looping

    rotating_sphere->addComponent(rotate_transform);

    window_container->show();
    return a.exec();
}
