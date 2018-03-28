#include "mainwindow.h"
#include <iostream>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QUrl>
#include <QFile>
#include "websocketclientwrapper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _view = new QWebEngineView();
    _view->load(QUrl("qrc:///index.html"));
    setCentralWidget(_view);

    if (!_start_websocket_server())
        qFatal("Failed to start web socket server on port 12345.");

    _webchannel = new QWebChannel();
    _webchannel->registerObject("signaler", &_my_signaler);

    connect(&_my_signaler, &Signaler::launch_new_window,
            this, &MainWindow::launch_new_window);

    // method connects relevant signal to the `QWebChannel::connectTo` slot.
    _setup_webchannel_transport();
    _inject_javascript_into_page();
}

void MainWindow::launch_new_window()
{

}

bool MainWindow::_start_websocket_server()
{
    // Note that the server is unsecured
    _websocket_server = new QWebSocketServer(QStringLiteral("Example Server for QWebChannel"),
                                   QWebSocketServer::NonSecureMode);

    // we're listening on port 12345
    return _websocket_server->listen(QHostAddress::LocalHost, 12345);
}

void MainWindow::_inject_javascript_into_page()
{
}

void MainWindow::_setup_webchannel_transport()
{
    // `WebSocketClientWrapper` wraps incoming WebSocket clients in QWebChannelAbstractTransport objects
    // See `handleNewConnection` method in `WebSocketClientWrapper` for relevant code
    _client_wrapper = new WebSocketClientWrapper(_websocket_server);
    // Note that every websocket client that connects to our websocket server gets
    // access to every published QObject.
    QObject::connect(_client_wrapper, &WebSocketClientWrapper::clientConnected,
                     _webchannel, &QWebChannel::connectTo);
}


QWebEngineScript MainWindow::_get_custom_javascript()
{
    QWebEngineScript script;

    // Read in `qwebchannel.js`
    QFile web_channel(":/qwebchannel.js");
    web_channel.open(QIODevice::ReadOnly);
    QByteArray javascript = web_channel.readAll();
    // Read in `script.js`
    QFile custom_javascript(":/script.js");
    custom_javascript.open((QIODevice::ReadOnly));

    // Note that the custom `script.js` is appended below `webchannel.js`
    javascript.append(custom_javascript.readAll());

    script.setSourceCode(javascript);
    script.setName("qwebchannel.js");
    script.setRunsOnSubFrames(false);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    return script;
}
