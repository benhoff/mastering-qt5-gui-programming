#include "client.h"

client::client(QObject *parent) :
    QObject(parent),
    _tcp_socket(new QTcpSocket(this))
{
    in.setDevice(_tcp_socket);
    // connect(_tcp_socket, &QIODevice::readyRead, this, )

}
