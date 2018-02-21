#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QVector>
#include <QProcess>
#include <QTimer>

class ProcessManager : public QObject
{
    Q_OBJECT
public:
    explicit ProcessManager(int max_processes=5, QObject *parent = nullptr);
    void start();

signals:

private slots:
    void start_process();
    void process_finished(int exit_code, QProcess::ExitStatus status);
    void error_handler(QProcess::ProcessError error);


private:
    QTimer *_timer;
    int _num_process;
    int _max_processes;
    QVector<QProcess*> _processes;
};

#endif // CLIENTMANAGER_H
