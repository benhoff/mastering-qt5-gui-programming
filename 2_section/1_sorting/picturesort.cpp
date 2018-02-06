#include "picturesort.h"
#include "luminosity_calc.h"

PictureSort::PictureSort(QObject *parent):
    QSortFilterProxyModel(parent)
{
}

bool PictureSort::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant left_item = sourceModel()->data(source_left, Qt::DecorationRole);
    QVariant right_item = sourceModel()->data(source_right, Qt::DecorationRole);
    return luminosity_less_than(left_item, right_item);


}

