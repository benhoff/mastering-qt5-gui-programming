#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <iostream>
#include <QStringList>

#include <QTcpServer>
#include <QDataStream>
#include <QTcpSocket>
#include <QTimer>


class PushServer : public QObject
{
    Q_OBJECT
public:
    explicit PushServer(int number_operations = 100,  QObject *parent=nullptr)
        : QObject(parent)
    {
        std::cout << "Start Push server" << std::endl;
        started = _server.listen(QHostAddress::LocalHost, 5000);

        for (int i = 0; i < number_operations; i++)
            _operations << QString::number(i);

        connect(&_server, &QTcpServer::newConnection, this, &PushServer::send_work);
    }

private slots:
    void send_work()
    {
        // Check to see if we still have work to do first
        if (_operations.isEmpty())
        {
            // Need to give the client connection a second to write the
            // last instruction as `write` is non-blocking before finishing
            QTimer::singleShot(50, [this](){emit finished();});
            return;
        }

        QTcpSocket *client_connection = ;
        QString operation = ;

        std::cout << "send work: " << operation.toInt() << std::endl;

    }

signals:
    void finished();

private:
    QTcpServer _server;
    QStringList _operations;

public:
    bool started;
};
#endif // SERVERMANAGER_H
