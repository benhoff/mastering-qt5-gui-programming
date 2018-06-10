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

    QQmlApplicationEngine engine;

    // TODO: add the `imports` absolute path to engine
    // using the `addImportPath` method
    engine.addImportPath("/home/hoff/recording/mastering_qt/5_qt_communication/3_qml_types/imports");

    // TODO: use the `qmlRegisterType` method to add the C++ class
    // `ColorPickers` as a useable QML type.
    qmlRegisterType<ColorPicker>("ColorPickers", 1, 0, "ColorPicker");




    PhotoModel photo_model;

    QQmlContext *root_context = engine.rootContext();
    root_context->setContextProperty("photo_model", &photo_model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
