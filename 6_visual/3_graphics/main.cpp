#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Qt3DQuickExtras/qt3dquickwindow.h>
#include <Qt3DQuick/QQmlAspectEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    Qt3DExtras::Quick::Qt3DQuickWindow view;
    view.resize(1600, 800);

    view.engine()->qmlEngine()->rootContext()->setContextProperty("_window", &view);

    /*
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
     */

    view.setSource(QStringLiteral("qrd:/main.qml"));
    view.show();

    return app.exec();
}
