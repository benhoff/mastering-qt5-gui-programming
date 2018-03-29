#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QMessageBox>
#include <QTimer>

class Interactor : public QObject
{
    Q_OBJECT
public:

    explicit Interactor(QObject *parent = nullptr) :
        QObject(parent)
    {
        who_changed_this_string = "C++ Application";
        QTimer::singleShot(1000, [this](){emit fired();});
        QTimer::singleShot(3000, [this](){this_string_changes_in_time = "Changed after 3 seconds!";});
    }

    void call_me_from_website()
    {
        qDebug("Method Called!");
    }

    QString call_me_returns_string()
    {
        return "This is a neat string";
    }

    void launch_new_window()
    {
        QMessageBox message_box;
        message_box.setText("The button has been pushed on the website");
        message_box.exec();
    }

    QString who_changed_this_string;
    QString this_string_changes_in_time;

signals:
    void fired();
};

#endif // INTERACTOR_H
