#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include "push_server.h"
#include "pull_server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // NOTE: You can put the Push/Pull servers on separate comptuers if desired.
    PullServer pull_server;

    int number_of_operations = 101;
    PushServer push_server(number_of_operations);

    if (!push_server.started)
    {
        qFatal("Server already started or ports used by other application! Exiting...");
        return 1;
    }

    QObject::connect(&push_server, &PushServer::finished, &a, &QCoreApplication::quit);

    return a.exec();
}


