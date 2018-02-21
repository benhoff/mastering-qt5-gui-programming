#include "mainwindow.h"
#include <iostream>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QUrl>
#include <QFile>
#include "websocketclientwrapper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _view(new QWebEngineView()),
      _server(new QWebSocketServer(QStringLiteral("QWebChannel Standalone Example Server"),
                                                          QWebSocketServer::NonSecureMode))
{
    setCentralWidget(_view);
    QWebEnginePage *page = _view->page();
    QWebEngineScript qwebchannel = get_script();
    page->profile()->scripts()->insert(qwebchannel);
    _view->load(QUrl("qrc:///index.html"));
    _channel = new QWebChannel();
    // page->scripts();
    if (!_server->listen(QHostAddress::LocalHost, 12345))
        qFatal("Failed to open web socket server.");

    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    _client_wrapper = new WebSocketClientWrapper(_server);

    QObject::connect(_client_wrapper, &WebSocketClientWrapper::clientConnected,
                                _channel, &QWebChannel::connectTo);

}


QWebEngineScript MainWindow::get_script()
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
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    script.setRunsOnSubFrames(false);
    return script;
}
