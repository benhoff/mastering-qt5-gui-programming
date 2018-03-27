#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineScript>
#include <QWebSocketServer>
#include "websocketclientwrapper.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    bool _start_webserver();
    void _setup_webchannel_transport();
    QWebEngineScript _get_webchannel_javascript();
    void _inject_webchannel_javascript();

private:
    QWebEngineView *_view;
    QWebChannel *_channel;
    QWebSocketServer *_server;
    WebSocketClientWrapper *_client_wrapper;
};

#endif // MAINWINDOW_H
