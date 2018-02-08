#ifndef PHOTOITEMMODEL_H
#define PHOTOITEMMODEL_H

#include <QIcon>
#include <QImage>
#include <QObject>
#include <QVariant>
#include <QStandardItem>
#include <QStandardItemModel>


class PhotoModel : public QStandardItemModel
{
public:
    PhotoModel(QObject *parent=0):
        QStandardItemModel(parent)
    {
        // NOTE: This block of text loads in our pictures
        for (int i = 0; i < 100; i++)
        {
            // Dynamically generate our filepath
            QString filepath(":/pics/" + QString::number(i) + ".png");
            // Create a new QStandaradItem
            QIcon icon(filepath);
            QStandardItem *item = new QStandardItem();
            item->setIcon(icon);
            // set the data for the new QStandard item to be the image that we just loaded
            this->setItem(i, 0, item);
        }

    }
};

#endif // PHOTOITEMMODEL_H
