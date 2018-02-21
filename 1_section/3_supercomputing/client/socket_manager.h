#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QElapsedTimer>
#include <QRandomGenerator>


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
    void reset_socket(int socket_number);
    void do_work(QString work, int socket_number);

    QVector<QTcpSocket*> _sockets;
    QVector<QDataStream*> _data_streams;
    int _total_sockets;
    QRandomGenerator _random;
    QElapsedTimer _elasped_time;
};
#endif // SOCKET_MANAGER_H
