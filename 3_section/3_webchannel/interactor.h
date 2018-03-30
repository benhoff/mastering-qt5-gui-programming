#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QMessageBox>
#include <QDebug>
#include <QTimer>

class Interactor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString who_changed_this_string  READ get_string WRITE set_string NOTIFY string_changed)
public:

    explicit Interactor(QObject *parent = nullptr) :
        QObject(parent)
    {
        _who_changed_this_string = "C++ Application";
        QTimer::singleShot(1000, [this](){emit fired();});
    }

public slots:
    void call_me_from_website()
    {
        qDebug() << "Method Called!";
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


signals:
    void fired();
    void string_changed();
public:
    void set_string(QString new_string)
    {
        _who_changed_this_string = new_string;
    }

    QString get_string()
    {
        return _who_changed_this_string;
    }

private:
    QString _who_changed_this_string;
};

#endif // INTERACTOR_H
