#include <QCoreApplication>
#include <QObject>
#include "socket_manager.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketManager manager(5);
    QObject::connect(&manager, &SocketManager::finished, &a, &QCoreApplication::quit);
    manager.start();

    return a.exec();
}
