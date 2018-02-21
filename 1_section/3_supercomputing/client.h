#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = nullptr);

private:
    QTcpSocket *_tcp_socket;
    QDataStream in;
};

#endif // CLIENT_H
