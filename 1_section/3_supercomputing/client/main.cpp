#include <iostream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QTimer>


class SocketManager : public QObject
{
public:
    explicit SocketManager(int number_sockets = 5, QObject *parent=nullptr)
        : QObject(parent),
          _total_sockets(number_sockets),
          _timer(new QTimer()),
          _current_socket(0)
    {
        connect(_timer, &QTimer::timeout, this, &SocketManager::times_up);
        _timer->setInterval(500);
        setup_sockets();
    }

    void start()
    {
        std::cout << "Start Timer" << std::endl << std::endl;
        _timer->start();
    }

private slots:
    void times_up()
    {
        std::cout << "connect socket #" << _current_socket << std::endl;
        QTcpSocket *socket = _sockets[_current_socket];
        socket->connectToHost(QHostAddress::LocalHost, 5000);

        _current_socket ++;
        if (_current_socket >= _total_sockets)
            _timer->stop();
    }

private:
    void setup_sockets()
    {
        for (int i =0; i < _total_sockets; i++)
        {
            QTcpSocket *socket = new QTcpSocket();
            _sockets.append(socket);

            QDataStream *in = new QDataStream();
            _data_streams.append(in);

            in->setDevice(socket);

            connect(socket, &QIODevice::readyRead, [socket, in, i, this](){
                in->startTransaction();
                QString work;
                *in >> work;

                if (!in->commitTransaction())
                    return;

                this->do_work(work, i);
            });
        }

        for (int i =0; i < _total_sockets; i++)
        {
            QTcpSocket *socket = new QTcpSocket();
            _sockets.append(socket);
            socket->connectToHost(QHostAddress::LocalHost, 5001);
        }
    }
    void do_work(QString work, int socket_number)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << work;
        std::cout << "Did work: " << work.toStdString() << std::endl;

        QTimer::singleShot(500, [work, socket_number, this, block](){
            QTcpSocket *socket = _sockets[socket_number + _total_sockets];
            if (socket->state() == QAbstractSocket::ConnectedState)
                socket->write(block);
            else
                std::cout << "Not connected" << std::endl;
        });

    }

    QVector<QTcpSocket*> _sockets;
    QVector<QDataStream*> _data_streams;
    int _total_sockets;
    QTimer *_timer;
    int _current_socket;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketManager manager;
    manager.start();

    return a.exec();
}
