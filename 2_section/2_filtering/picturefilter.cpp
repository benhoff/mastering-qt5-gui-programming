#include "picturefilter.h"
#include <iostream>

PictureFilter::PictureFilter(QObject *parent):
    QSortFilterProxyModel(parent)
{
    // NOTE: Virdis actual low is like, 14.95
    min_filter_value = 0.;
    // NOTE: Viridis actual high is like 90.899
    max_filter_value = 100.;

}

bool PictureFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    int column = 0;
    QModelIndex index = sourceModel()->index(source_row, column, source_parent);
    QImage image = get_image(index);
    qreal value = get_lab(image.pixelColor(0, 0));
    // std::cout << value << " " << min_filter_value << " " << max_filter_value << std::endl;
    return value > min_filter_value && value < max_filter_value;
}

QImage PictureFilter::get_image(const QModelIndex &source) const
{
    QVariant data = sourceModel()->data(source, Qt::DecorationRole);

    QIcon icon = data.value<QIcon>();
    // NOTE: We just loaded the images directly without any sizing information...
    // But we do know that our pics are 50 px X 50 px...
    // have to pass in the size of pixmap we want...
    return icon.pixmap(50, 50).toImage();
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

qreal PictureFilter::get_lab(const QColor color) const
{
    // Ref: http://www.easyrgb.com/en/math.php

    // RGB to XYZ
    std::array<qreal, 3> rgb{color.redF(), color.greenF(), color.blueF()};
    for (auto it = rgb.begin(); it != rgb.end(); it++)
    {
        if (*it > 0.04045)
            *it = qPow(((*it + 0.055) / 1.055), 2.4) * 100;
        else
            *it = *it / 12.92 * 100;
    }
    // We'll declare the coordinates
    qreal x, y, z;
    x = rgb[0] * 0.4124 + rgb[1] * 0.3576 + rgb[2] * 0.1805;
    y = rgb[0] * 0.2126 + rgb[1] * 0.7152 + rgb[2] * 0.0722;
    z = rgb[0] * 0.0193 + rgb[1] * 0.1192 + rgb[2] * 0.9505;
    // NOTE: These are the real xyz values at this before this next calc
    x = x/100.;
    y = y/100.;
    z = z/100.;

    std::array<qreal, 3> xyz{x, y, z};

    qreal one_third = 1./3.;

    for (auto it = xyz.begin(); it != xyz.end(); it++)
    {
        if (*it > 0.008856)
        {
            *it = qPow(*it, one_third);
        }
        else
        {
            qreal value = *it;
            *it = (7.787 * value) + (16 / 116);
        }
    }

    qreal l, a, b;
    l = (116 * xyz[1]) - 16;
    a = 500 * (xyz[0] - xyz[1]);
    b = 200 * (xyz[1] - xyz[2]);
    return l;
}
