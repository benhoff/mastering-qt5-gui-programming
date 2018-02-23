#include "processmanager.h"
#include <QDebug>

ProcessManager::ProcessManager(int max_processes, QObject *parent) :
    QObject(parent),
    _num_process(0),
    _max_processes(max_processes)
{
    _timer = new QTimer(this);
    _timer->setInterval(1500);

    connect(_timer, &QTimer::timeout, this, &ProcessManager::start_process);
}

void ProcessManager::start()
{
    qDebug() << "starting timer";
    _timer->start();
}

void ProcessManager::start_process()
{
    qDebug() << "starting process # " << _num_process;

    QProcess *process = new QProcess(this);
    _processes.append(process);

    // NOTE: put your binary here!
    process->start("ls");

    // process->setProcessEnvironment();

    connect(process,
                  SIGNAL(finished(int, QProcess::ExitStatus)),
                  this,
                  SLOT(process_finished(int, QProcess::ExitStatus)));

    connect(process,
            SIGNAL(errorOccurred(QProcess::ProcessError)),
            this,
            SLOT(error_handler(QProcess::ProcessError)));

    // TODO: ready read standard output?

    _num_process += 1;

    if (_num_process == _max_processes)
    {
        qDebug() << "max processes reached!";
        _timer->stop();
    }
}

void ProcessManager::process_finished(int exit_code, QProcess::ExitStatus status)
{
    if (status == QProcess::NormalExit)
        qDebug() << "        Process finished! " << exit_code << " " << status;
    else if (status == QProcess::CrashExit)
    {
    }
}

void ProcessManager::error_handler(QProcess::ProcessError error)
{
    qDebug() << "Error!";
    switch (error) {
    case QProcess::Crashed:

        break;

    case QProcess::FailedToStart:

        break;

    case QProcess::Timedout:

        break;

    case QProcess::WriteError:

        break;

    case QProcess::ReadError:

        break;

    case QProcess::UnknownError:

        break;
    }
}
