#include <iostream>
#include <QStringList>
#include <QCoreApplication>
#include <QTcpServer>
#include <QDataStream>
#include <QTcpSocket>
#include "clientmanager.h"

class ServerManager : public QObject
{
public:
    explicit ServerManager(QObject *parent=nullptr) : QObject(parent)
    {
        std::cout << "Start server" << std::endl;
        bool push_sucess, pull_sucess;
        push_sucess = _push.listen(QHostAddress::LocalHost, 5000);
        pull_sucess = _pull.listen(QHostAddress::LocalHost, 5001);

        if (!push_sucess && !pull_sucess)
            std::cout << "oh man. Oh boy" << std::endl;

        _operations << "1" << "2" << "3" << "4" << "5";
        connect(&_push, &QTcpServer::newConnection, this, &ServerManager::send_work);
        connect(&_pull, &QTcpServer::newConnection, this, &ServerManager::recieve_work);
    }

private slots:
    void send_work()
    {
        QTcpSocket *client_connection = _push.nextPendingConnection();
        connect(client_connection, &QAbstractSocket::disconnected, client_connection, &QObject::deleteLater);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        if (_operations.isEmpty())
            return;

        out << _operations.takeFirst();
        std::cout << "send work: " << block.toStdString() << std::endl;

        client_connection->write(block);
        client_connection->disconnectFromHost();
    }

    void recieve_work()
    {
        QTcpSocket *client_connection = _pull.nextPendingConnection();
        connect(client_connection, &QAbstractSocket::disconnected, client_connection, &QObject::deleteLater);
        QDataStream in;
        in.setDevice(client_connection);
        in.startTransaction();
        QString result;
        in >> result;

        if (in.commitTransaction())
            return;

        std::cout << "Result is: " << result.toStdString() << std::endl;

        connect(client_connection, &QAbstractSocket::disconnected, client_connection, &QObject::deleteLater);
    }

private:
    QTcpServer _push;
    QTcpServer _pull;
    QStringList _operations;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerManager manager;
    return a.exec();
}


