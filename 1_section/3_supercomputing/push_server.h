#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <iostream>
#include <QStringList>

#include <QTcpServer>
#include <QDataStream>
#include <QTcpSocket>


class PushServer : public QObject
{
    Q_OBJECT
public:
    explicit PushServer(int number_operations = 10,  QObject *parent=nullptr)
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
        QTcpSocket *client_connection = _server.nextPendingConnection();
        connect(client_connection, &QAbstractSocket::disconnected, client_connection, &QObject::deleteLater);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);


        QString operation = _operations.takeFirst();

        out << operation;
        std::cout << "send work: " << operation.toInt() << std::endl;

        client_connection->write(block);
        client_connection->disconnectFromHost();

        if (_operations.isEmpty())
            emit finished();
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
