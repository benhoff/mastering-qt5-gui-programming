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

    if (!_start_webserver())
        qFatal("Failed to start web socket server on socket.");

    _inject_webchannel_javascript();
    _setup_webchannel_transport();
}

bool MainWindow::_start_webserver()
{
    _server = new QWebSocketServer(QStringLiteral("QWebChannel Standalone Example Server"),
                                   QWebSocketServer::NonSecureMode);

    return _server->listen(QHostAddress::LocalHost, 12345);
}

void MainWindow::_inject_webchannel_javascript()
{
    // get page
    QWebEnginePage *page = _view->page();
    // get javascript
    QWebEngineScript qwebchannel = _get_webchannel_javascript();
    // put javascript into page
    page->profile()->scripts()->insert(qwebchannel);
}

void MainWindow::_setup_webchannel_transport()
{
    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    _client_wrapper = new WebSocketClientWrapper(_server);

    _channel = new QWebChannel();
    QObject::connect(_client_wrapper, &WebSocketClientWrapper::clientConnected,
                     _channel, &QWebChannel::connectTo);
}


QWebEngineScript MainWindow::_get_webchannel_javascript()
{
    QWebEngineScript script;

    QFile web_channel(":/qwebchannel.js");
    web_channel.open(QIODevice::ReadOnly);
    QByteArray javascript = web_channel.readAll();
    QFile custom_javascript(":/script.js");
    custom_javascript.open((QIODevice::ReadOnly));

    javascript.append(custom_javascript.readAll());

    script.setSourceCode(javascript);
    script.setName("qwebchannel.js");
    script.setRunsOnSubFrames(false);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    return script;
}
