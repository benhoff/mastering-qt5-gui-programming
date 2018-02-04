#include "virdisdialog.h"

VirdisDialog::VirdisDialog(int initial_index, QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *main_layout = new QVBoxLayout();

    picker = new QColorLuminancePicker(initial_index);
    main_layout->addWidget(picker);

    QHBoxLayout *box_layout = new QHBoxLayout();

    // TODO: some signal connections.
    QPushButton *ok =  new QPushButton("Ok");
    QPushButton *nope = new QPushButton("Nope");
    box_layout->addWidget(ok);
    box_layout->addWidget(nope);

    main_layout->addLayout(box_layout);
    setLayout(main_layout);
}

QColor VirdisDialog::get_current_color()
{
    return picker->current_color;
}

QColor VirdisDialog::get_color(int initial_index, QWidget *parent, const QString &title)
{
    VirdisDialog dialog(initial_index, parent);
    if (!title.isEmpty())
        dialog.setWindowTitle(title);
    dialog.exec();
    return dialog.get_current_color();
}
