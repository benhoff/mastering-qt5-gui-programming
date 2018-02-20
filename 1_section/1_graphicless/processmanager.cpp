#include "processmanager.h"
#include <iostream>

ProcessManager::ProcessManager(int max_processes, QObject *parent) :
    QObject(parent),
    _max_processes(max_processes),
    _num_process(0)
{
    _timer = new QTimer(this);
    _timer->setInterval(2000);
    connect(_timer, &QTimer::timeout, this, &ProcessManager::start_process);
}

void ProcessManager::start()
{
    std::cout << "starting timer" << std::endl;
    _timer->start();
}

void ProcessManager::start_process()
{
    std::cout << "starting process # " << _num_process << std::endl;

    QProcess *process = new QProcess(this);
    _processes.append(process);
    process->start("less");

    // process->setProcessEnvironment();

    connect(process, &QProcess::finished, this, &ProcessManager::process_finished);
    connect(process, &QProcess::errorOccurred, this, &ProcessManager::error_handler);

    _num_process += 1;
}

void ProcessManager::process_finished(int exit_code, QProcess::ExitStatus status)
{
    std::cout << "Made it here!" << std::endl;

}

void ProcessManager::error_handler(QProcess::ProcessError error)
{
    std::cout << "error!" << std::endl;
}
