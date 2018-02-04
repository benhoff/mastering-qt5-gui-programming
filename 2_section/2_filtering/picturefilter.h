#ifndef PICTURESORT_H
#define PICTURESORT_H

#include <QIcon>
#include <QSize>
#include <QImage>
#include <QColor>
#include <QtMath>
#include <QPixmap>
#include <QSortFilterProxyModel>


class PictureFilter: public QSortFilterProxyModel
{
public:
    PictureFilter(QObject *parent=0);

    qreal get_lab(const QColor color) const;
    void set_min_filter_value(const qreal value);
    void set_max_filter_value(const qreal value);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QImage get_image(const QModelIndex &source) const;

    qreal min_filter_value;
    qreal max_filter_value;
};

#endif // PICTURESORT_H
