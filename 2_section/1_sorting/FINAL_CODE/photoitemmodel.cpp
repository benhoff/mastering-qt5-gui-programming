#include "photoitemmodel.h"
#include <iostream>

PhotoItemModel::PhotoItemModel(QObject *parent):
    QStandardItemModel(parent)
{
    // NOTE: This block of text loads in our pictures
    // For this purpose, I've just hardcoded in the number of pictures
    for (int i = 0; i < 100; i++)
    {
        // Dynamically generate our filepath
        QString filepath(":/pics/" + QString::number(i) + ".png");
        // Create an icon based on the filepath
        QIcon icon(filepath);
        // Create a new QStandaradItem
        QStandardItem *item = new QStandardItem();
        // set the QStandardItem's icon
        item->setIcon(icon);
        // set the data for the new QStandard item to be the image that we just loaded
        this->setItem(i, 0, item);
    }
}
