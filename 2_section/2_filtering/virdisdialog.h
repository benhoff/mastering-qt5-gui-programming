#ifndef VIRDISDIALOG_H
#define VIRDISDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "colorpicker.h"


class VirdisDialog : public QDialog
{
public:
    VirdisDialog(int initial_index=0, QWidget *parent=0);
    static QColor get_color(int initial_index=0, QWidget* parent=0, const QString &title=QString());
private:
    QColor get_current_color();
    QColorLuminancePicker *picker;
};

#endif // VIRDISDIALOG_H
