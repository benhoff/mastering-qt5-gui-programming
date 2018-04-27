#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "photomodel.h"
#include "mypainteditem.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    // TODO: fix name
    qmlRegisterType<MyPaintedItem>("Colors", 1, 0, "MyPaintedItem");

    PhotoModel photo_model;

    QQmlApplicationEngine engine;
    QQmlContext *qml_context = engine.rootContext();

    qml_context->setContextProperty("photo_model", &photo_model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
