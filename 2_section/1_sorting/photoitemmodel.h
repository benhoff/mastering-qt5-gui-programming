#ifndef PHOTOITEMMODEL_H
#define PHOTOITEMMODEL_H

#include <QIcon>
#include <QImage>
#include <QObject>
#include <QVariant>
#include <QStandardItem>
#include <QStandardItemModel>


class PhotoItem : public QStandardItem
{
public:
    bool operator<(const QStandardItem &other) const;

};


class PhotoItemModel : public QStandardItemModel
{
public:
    PhotoItemModel(QObject *parent=0);
};



#endif // PHOTOITEMMODEL_H
