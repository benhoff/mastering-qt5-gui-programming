#ifndef PICTURESORT_H
#define PICTURESORT_H

#include <QSortFilterProxyModel>


class SortProxyModel: public QSortFilterProxyModel
{
public:
    PictureSort(QObject *parent=0);

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
};

#endif // PICTURESORT_H
