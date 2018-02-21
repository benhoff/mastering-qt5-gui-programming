#include "socket_manager.h"

#include <iostream>
#include <QTimer>
#include <QHostAddress>


SocketManager::SocketManager(int number_sockets, QObject *parent) :
    QObject(parent),
    _total_sockets(number_sockets)
    {
        setup_sockets();
    }

void SocketManager::start()
{
    std::cout << "Start" << std::endl << std::endl;
    _elasped_time.start();
    for (int i = 0; i < _total_sockets; i++)
    {
        QTcpSocket *socket = _sockets[i];
        socket->connectToHost(QHostAddress::LocalHost, 5000);
    }
}

void SocketManager::setup_sockets()
{
    for (int sock_num =0; sock_num < _total_sockets; sock_num++)
    {
        QTcpSocket *socket = new QTcpSocket();
        QDataStream *in = new QDataStream();
        in->setDevice(socket);

        // --- start connection method ---
        connect(socket, &QIODevice::readyRead, [this, sock_num, socket, in](){
            in->startTransaction();
            QString work;
            *in >> work;

            if (!in->commitTransaction())
                return;

            do_work(work, sock_num);

            int simulated_work_time = _random.bounded(500);

            QTimer::singleShot(simulated_work_time, [this, sock_num](){reset_socket(sock_num);});
        });
        // --- stop connection method ---

        connect(socket,
                QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                [this, sock_num](QAbstractSocket::SocketError error){error_handler(error, sock_num);});

        _sockets.append(socket);
        _data_streams.append(in);

    }

    for (int i =0; i < _total_sockets; i++)
    {
        QTcpSocket *socket = new QTcpSocket();
        _sockets.append(socket);
        socket->connectToHost(QHostAddress::LocalHost, 5001);
    }
}

void SocketManager::reset_socket(int socket_number)
{
    QTcpSocket *socket = _sockets[socket_number];
    socket->abort();
    socket->connectToHost(QHostAddress::LocalHost, 5000);
}

void SocketManager::do_work(QString work, int socket_number)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << work;

    std::cout << "Did work: " << work.toStdString() << " with socket # " << socket_number << std::endl;

    if (work.toInt() == 100)
    {
        std::cout << "Time elasped: " << (float) _elasped_time.elapsed() / 1000 << " seconds" << std::endl;
        emit finished();
    }

    int simulated_work_time = _random.bounded(500);

    QTimer::singleShot(simulated_work_time, [this, block, work, socket_number](){
        QTcpSocket *socket = _sockets[socket_number + _total_sockets];
        if (socket->state() == QAbstractSocket::ConnectedState)
            socket->write(block);
    });
}

void SocketManager::error_handler(QAbstractSocket::SocketError error, int socket_number)
{
    if (error == QAbstractSocket::ConnectionRefusedError)
    {
        QTimer::singleShot(1000, [this, socket_number](){reset_socket(socket_number);});
        if (_random.bounded(_total_sockets) == 0)
            std::cout << "No host found, is the server running?" << std::endl;
    }
}
