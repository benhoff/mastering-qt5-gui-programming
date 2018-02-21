#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include "push_server.h"
#include "pull_server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // NOTE: You can put these servers on separate comptuers if desired.
    PullServer pull_server;
    PushServer push_server(101);
    if (!push_server.started)
    {
        std::cout << "Server already started or ports used by other application! Exiting..." << std::endl;
        return 1;
    }

    QObject::connect(&push_server, &PushServer::finished, &a, &QCoreApplication::quit);


    return a.exec();
}


