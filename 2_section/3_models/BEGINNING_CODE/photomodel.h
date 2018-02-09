#ifndef PHOTOMODEL_H
#define PHOTOMODEL_H

#include <QAbstractListModel>

class PhotoModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PhotoModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
};

#endif // PHOTOMODEL_H
