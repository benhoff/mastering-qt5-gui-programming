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

    Qt3DExtras::Qt3DWindow *window = ;
    Qt3DExtras::QForwardRenderer *forward_render = ;
    QWidget *widget_container = ;






    // NOTE: Grab window size out, set `window_container` min/maximums
    /*
    QSize screen_size = window->screen()->size();
    window_container->setMinimumSize(QSize(200, 100));
    window_container->setMaximumSize(screen_size);
    */

    // TODO: Create root entity & set on window
    Qt3DCore::QEntity *root_entity = ;

    Qt3DRender::QCamera *camera_entity = ;
    /*
    camera_entity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera_entity->setPosition(QVector3D(0, 0, 20.0f));
    camera_entity->setUpVector(QVector3D(0, 1, 0));
    camera_entity->setViewCenter(QVector3D(0, 0, 0));
   */

    Qt3DCore::QEntity *light_entity = ;
    // `QPointLight` is a component and will need to be added
    // to the `light_entity`
    Qt3DRender::QPointLight *point_light_component = ;

    /*
    point_light_component->setColor("white");
    point_light_component->setIntensity(1);
    */

    Qt3DCore::QTransform *light_transform = ;


    // TODO: Create First person Camera Controller
    Qt3DExtras::QFirstPersonCameraController *cam_controller = ;


    Qt3DCore::QEntity *sphere_entity = ;

    Qt3DExtras::QSphereMesh *mesh = ;

    Qt3DExtras::QPhongMaterial *sphere_material = ;

    // sphere_material->setDiffuse(QColor(QRgb(0xa69929)));

    Qt3DCore::QEntity *rotating_sphere = ;

    RotateTransform *rotate_transform = new RotateTransform();
    QPropertyAnimation *rotation_animation = ;

    /*
    rotation_animation->setLoopCount(-1) // infinite loops
    rotation_animation->start()          // start looping
    */


    container.show();
    return a.exec();
}
