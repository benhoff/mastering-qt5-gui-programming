#include "picturemodel.h"

PictureModel::PictureModel()
{
    setup_data();
}

void PictureModel::setup_data()
{
    // NOTE: This block of text loads in our pictures
    // For this purpose, I've just hardcoded in the number of pictures
    for (int i = 0; i < 100; i++)
    {
        // Dynamically generate our filepath
        QString filepath(":/pics/" + QString::number(i) + ".png");
        // Create an icon based on the filepath
        QIcon icon(filepath);

        // FIXME: this stuff is not needed.

        // Create a new QStandaradItem
        QStandardItem *item = new QStandardItem();
        // set the QStandardItem's icon
        item->setIcon(icon);
        // set the data for the new QStandard item to be the image that we just loaded
        this->setItem(i, 0, item);
    }
}


QModelIndex PictureModel::parent(const QModelIndex &child) const override
{
    if (!child.isValid())
        return QModelIndex();
}

QVariant PictureModel::data(const QModelIndex &index, int role) const override
{
}

int PictureModel::rowCount(const QModelIndex &parent) const override
{
}

int PictureModel::columnCount(const QModelIndex &parent) const override
{
    // FIXME
    return 2;
}

QModelIndex PictureModel::index(int row, int column, const QModelIndex &parent) const override
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
}
