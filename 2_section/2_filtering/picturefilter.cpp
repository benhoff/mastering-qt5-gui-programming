#include "picturefilter.h"
#include "luminosity_calc.h"
#include <iostream>

PictureFilter::PictureFilter(QObject *parent):
    QSortFilterProxyModel(parent)
{
    min_filter_value = 0.;
    max_filter_value = 100.;

}

bool PictureFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    QVariant data = sourceModel()->data(index, Qt::DecorationRole);
    QIcon icon = data.value<QIcon>();
    qreal luminosity = icon_to_lumonsity(icon);
    return luminosity > min_filter_value && luminosity < max_filter_value;
}

void PictureFilter::set_min_filter_value(const qreal value)
{
    min_filter_value = value;
    invalidateFilter();
}

void PictureFilter::set_max_filter_value(const qreal value)
{
    max_filter_value = value;
    invalidateFilter();
}
