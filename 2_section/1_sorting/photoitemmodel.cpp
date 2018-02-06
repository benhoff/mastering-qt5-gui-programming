#include "photoitemmodel.h"
#include "luminosity_calc.h"
#include <iostream>

PhotoItemModel::PhotoItemModel(QObject *parent):
    QStandardItemModel(parent)
{
    QIcon last_icon(QString(":/pics/99.png"));

    // For this purpose, I've just hardcoded in the number of pictures
    for (int i = 0; i < 100; i++)
    {
        // Dynamically generate our filepath
        QString filepath(":/pics/" + QString::number(i) + ".png");
        // Create an icon based on the filepath
        QIcon icon(filepath);
        // Create a new QStandaradItem
        PhotoItem *item = new PhotoItem();
        // set the QStandardItem's icon
        item->setIcon(icon);
        // set the data for the new QStandard item to be the image that we just loaded
        // row, column
        setItem(i, 0, item);
        // set a second row, reusing some of our code from before.
        PhotoItem *second_item = new PhotoItem();
        second_item->setIcon(last_icon);
        setItem(i, 1, second_item);
        last_icon = icon;
    }
}

bool PhotoItem::operator <(const QStandardItem &other) const
{
    QVariant left =  data(Qt::DecorationRole);
    QVariant right = other.data(Qt::DecorationRole);
    return luminosity_less_than(left, right);
}

