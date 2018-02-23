#include "processmanager.h"

ProcessManager::ProcessManager(int max_processes, QObject *parent) :
    QObject(parent),
    _max_processes(max_processes),
    _num_process(0)
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
    process->start("ls");

    // process->setProcessEnvironment();

    connect(process,
                  SIGNAL(finished(int,QProcess::ExitStatus)),
                  this,
                  SLOT(process_finished(int,QProcess::ExitStatus)));

    connect(process,
            SIGNAL(errorOccurred(QProcess::ProcessError)),
            this,
            SLOT(error_handler(QProcess::ProcessError)));

    _num_process += 1;

    if (_num_process == _max_processes)
    {
        qDebug() << "max processes reached!";
        _timer->stop();
    }
}

void ProcessManager::process_finished(int exit_code, QProcess::ExitStatus status)
{
    qDebug() << "        Process finished!";
}

void ProcessManager::error_handler(QProcess::ProcessError error)
{
    qDebug() << "Error!";
}
