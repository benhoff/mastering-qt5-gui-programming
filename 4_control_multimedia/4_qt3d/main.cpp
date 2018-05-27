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






    // NOTE: Grab window size out, set `window_container` min/maximums
    /*
    QSize screen_size = window->screen()->size();
    window_container->setMinimumSize(QSize(200, 100));
    window_container->setMaximumSize(screen_size);
    */

    // TODO: Create root entity & set on window

    /*
   Qt3DRender::QCamera *camera_entity = window->camera();
   camera_entity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
   camera_entity->setPosition(QVector3D(0, 0, 20.0f));
   camera_entity->setUpVector(QVector3D(0, 1, 0));
   camera_entity->setViewCenter(QVector3D(0, 0, 0));
   */

   Qt3DCore::QEntity *light_entity = new Qt3DCore::QEntity(root_entity);
   Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(light_entity);

   /*
   light->setColor("white");
   light->setIntensity(1);
   */

    /*
   sphere_mesh->setRings(30);
   sphere_mesh->setSlices(30);
   sphere_mesh->setRadius(3);
   */

   // sphere_lighting->setDiffuse(QColor(QRgb(0xa69929)));


    /*
   sphere_transform->setScale(1.2f);
   sphere_transform->setTranslation(QVector3D(0.0f, 0.0f, 1.0f));
   */

   // FIXME: is this needed?
   sphere_entity->setEnabled(true);

   // NOTE: this code sets up a first person controller for our camera
   Qt3DExtras::QFirstPersonCameraController *cam_controller = new Qt3DExtras::QFirstPersonCameraController(root_entity);
   cam_controller->setCamera(camera_entity);


    // FIXME: do we need this?
    container->resize(1200, 800);

    return a.exec();
}
