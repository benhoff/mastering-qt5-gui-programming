#ifndef PICTURESORT_H
#define PICTURESORT_H

#include <QSortFilterProxyModel>


class PictureFilter: public QSortFilterProxyModel
{
public:
    PictureFilter(QObject *parent=0):
        QSortFilterProxyModel(parent),
        min_filter_value(0.),
        max_filter_value(100.)
    {
    }

    void set_min_filter_value(const qreal value)
    {

    }

    void set_max_filter_value(const qreal value)
    {

    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {

    }

private:
    qreal min_filter_value;
    qreal max_filter_value;
};

#endif // PICTURESORT_H
