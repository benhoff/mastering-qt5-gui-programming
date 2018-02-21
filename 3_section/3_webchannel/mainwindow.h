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
    QWebEngineScript get_script();

signals:

public slots:
private:
    QWebEngineView *_view;
    QWebChannel *_channel;
    QWebSocketServer *_server;
    WebSocketClientWrapper *_client_wrapper;
};

#endif // MAINWINDOW_H
