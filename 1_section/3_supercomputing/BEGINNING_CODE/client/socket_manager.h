#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QPair>


struct SocketPair{
    QTcpSocket *input;
    QTcpSocket *output;
}

class SocketManager : public QObject
{
    Q_OBJECT
public:
    explicit SocketManager(int number_sockets = 5, QObject *parent=nullptr);
    ~SocketManager(){}

    void start();

signals:
    void finished();

private slots:
    void error_handler(QAbstractSocket::SocketError error, int socket_number);

private:
    void setup_sockets();
    void setup_socket_connections(QTcpSocket* socket, QDataStream* data_stream, int socket_number);
    void _setup_error_handling(QTcpSocket *socket);

    void get_more_work(int socket_number);
    void do_work(QString work, SocketPair socket_pair, int socket_number);

    QVector<SocketPair> _sockets;
    QVector<QDataStream*> _data_streams;
    int _total_sockets;
    QRandomGenerator _random;
    QElapsedTimer _elasped_time;
};
#endif // SOCKET_MANAGER_H
