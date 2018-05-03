#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "photomodel.h"
#include "colorpicker.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // this code to be covered in video 5.3
    qmlRegisterType<ColorPicker>("ColorPickers", 1, 0, "ColorPicker");

    PhotoModel photo_model;

    QQmlApplicationEngine engine;
    QQmlContext *qml_context = engine.rootContext();

    qml_context->setContextProperty("photo_model", &photo_model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
