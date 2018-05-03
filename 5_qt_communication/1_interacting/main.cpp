#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "photomodel.h"
#include "colorpicker.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // NOTE: we use `QGuiApplication` instead of `QApplication` as
    // we're not using widgets
    QGuiApplication app(argc, argv);

    // this registration code to be covered in video 5.3
    // qmlRegisterType<ColorPicker>("ColorPickers", 1, 0, "ColorPicker");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
