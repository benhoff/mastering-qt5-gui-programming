#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "photomodel.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    PhotoModel photo_model;
    QQmlApplicationEngine engine;
    QQmlContext *qml_context = engine.rootContext();
    qml_context->setContextProperty("photo_model", &photo_model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;



    return app.exec();
}
