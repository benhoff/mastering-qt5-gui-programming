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

#include <Qt3DCore/QTransform>
#include <QPropertyAnimation>

class MyTransform : public QTransform
{
    Q_OBJECT
    Q_PROPERTY(float user_angle READ get_user_angle WRITE set_user_angle NOTIFY notify_angle_changed)
public:
    MyTransform(){
        conect(this, &MyTransform::notify_angle_changed, this, &MyTransform::rotate);
    }

    float get_user_angle() { return _user_angle;}
    void set_user_angle(float new angle)
    {
        _user_angle = new_angle;
    }

public signals:
    void notify_angle_changed();
private slots:
    void rotate()
    {
       QMatrix4x4 matrix;
       matrix.rotate(_user_angle, QVector3D(0, 1, 0));
       matrix.translate(QVector3D(20, 0, 0));
       setMatrix(matrix);
    }

private:
    float _user_angle;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor("white"));

    QWidget *container = QWidget::createWindowContainer(view);
    QSize screen_size = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screen_size);

    Qt3DCore::QEntity *root_entity = new Qt3DCore::QEntity();

   Qt3DRender::QCamera *camera_entity = view->camera();
   camera_entity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
   camera_entity->setPosition(QVector3D(0, 0, 20.0f));
   camera_entity->setUpVector(QVector3D(0, 1, 0));
   camera_entity->setViewCenter(QVector3D(0, 0, 0));

   Qt3DCore::QEntity *light_entity = new Qt3DCore::QEntity(root_entity);
   Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(light_entity);
   light->setColor("white");
   light->setIntensity(1);
   light_entity->addComponent(light);

   Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(light_entity);
   lightTransform->setTranslation(camera_entity->position());
   light_entity->addComponent(lightTransform);


   Qt3DExtras::QFirstPersonCameraController *cam_controller = new Qt3DExtras::QFirstPersonCameraController(root_entity);
   cam_controller->setCamera(camera_entity);

   Qt3DCore::QEntity *sphere_entity = new Qt3DCore::QEntity(root_entity);
   Qt3DExtras::QSphereMesh *sphere_mesh = new Qt3DExtras::QSphereMesh();
   sphere_mesh->setRings(30);
   sphere_mesh->setSlices(30);
   sphere_mesh->setRadius(2);

   Qt3DCore::QTransform *sphere_transform = new Qt3DCore::QTransform();
   sphere_transform->setScale(1.2f);
   sphere_transform->setTranslation(QVector3D(0.0f, 0.0f, 1.0f));

   Qt3DExtras::QPhongMaterial *sphere_material = new Qt3DExtras::QPhongMaterial();
   sphere_material->setDiffuse(QColor(QRgb(0xa69929)));


   sphere_entity->addComponent(sphere_mesh);
   sphere_entity->addComponent(sphere_material);
   sphere_entity->addComponent(sphere_transform);
   sphere_entity->setEnabled(true);

   MyTransform *my_transform = new MyTransform();
   Qt3DCore::QTransform *rotate_transform = new Qt3DCore::QTransform();

   QPropertyAnimation *my_animation = new QPropertyAnimation(my_transform, "user_angle");
   my_animation->setDuration(10000);
   my_animation->setStartValue(0);
   my_animation->setEndValue(360);
   // infinite
   my_animation->setLoopCount(-1);

   Qt3DCore::QEntity *rotating_sphere = new Qt3DCore::QEntity(root_entity);
   rotating_sphere->addComponent(sphere_mesh);
   rotating_sphere->addComponent(sphere_material);
   rotating_sphere->addComponent(my_transform);
   view->setRootEntity(root_entity);

    container->show();
    container->resize(1200, 800);

    return a.exec();
}
