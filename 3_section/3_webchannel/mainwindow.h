#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineScript>
#include <QWebSocketServer>
#include "websocketclientwrapper.h"


class Signaler : public QObject
{
    Q_OBJECT
public:
    explicit Signaler(QObject *parent = nullptr) :
        QObject(parent),
        who_changed_this_string("C++ Application")
    {
    }

    QString who_changed_this_string;

    void call_me()
    {
        qDebug("Method Called!");
    }

    QString call_me_returns_string()
    {
        return "This is a neat string";
    }

signals:
    // NOTE: Need two signals for each side.
    void fired();
    void launch_new_window();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void launch_new_window();

    // Implementation methods
    bool _start_websocket_server();
    void _setup_webchannel_transport();
    QWebEngineScript _get_custom_javascript();
    void _inject_javascript_into_page();
    // Implementation methods

private:
    // Our Web Browser
    QWebEngineView *_view;

    // The Web Channel
    QWebChannel *_webchannel;

    // Web Socket Server
    QWebSocketServer *_websocket_server;
    // WebSocket->QWebChannel interface
    WebSocketClientWrapper *_client_wrapper;
    Signaler _my_signaler;
};

#endif // MAINWINDOW_H
