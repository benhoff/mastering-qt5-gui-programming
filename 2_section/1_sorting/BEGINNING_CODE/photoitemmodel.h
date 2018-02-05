#ifndef PHOTOITEMMODEL_H
#define PHOTOITEMMODEL_H

#include <QIcon>
#include <QImage>
#include <QObject>
#include <QVariant>
#include <QStandardItem>
#include <QStandardItemModel>
#include "picturesort.h"


class PhotoItemModel : public QStandardItemModel
{
public:
    PhotoItemModel(QObject *parent=0);
};

#endif // PHOTOITEMMODEL_H
