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
    PhotoModel(QObject *parent=0);
};

#endif // PHOTOITEMMODEL_H
