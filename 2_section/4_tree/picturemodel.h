#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include "treeitem.h"


class PictureModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PictureModel();
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

private:
    void setup_data();

    TreeItem *root_item;


};

#endif // PICTUREMODEL_H
