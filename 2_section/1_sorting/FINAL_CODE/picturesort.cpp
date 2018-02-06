#include "picturesort.h"

PictureSort::PictureSort(QObject *parent):
    QSortFilterProxyModel(parent)
{
}

QImage PictureSort::get_image(const QModelIndex &source) const
{
    QVariant data = sourceModel()->data(source, Qt::DecorationRole);

    QIcon icon = data.value<QIcon>();
    // NOTE: We just loaded the images directly without any sizing information...
    // But we do know that our pics are 50 px X 50 px...
    // have to pass in the size of pixmap we want...
    return icon.pixmap(50, 50).toImage();
}

bool PictureSort::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // Use item from index?
    QImage left = get_image(source_left);
    QImage right = get_image(source_right);

    QStandardItem* left = sourceModel()->itemFromIndex(source_left);
    QStandardItem* right = sourceModel()->itemFromIndex(source_right);


}

qreal PictureSort::get_lab(const QColor color) const
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

    std::array<qreal, 3> xyz{x, y, z};

    qreal one_third = 1./3.;

    // FIXME: These calcs all end with the values being 1, 1, 1
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
