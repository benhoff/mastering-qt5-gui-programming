#ifndef PICTURESORT_H
#define PICTURESORT_H

#include <QImage>
#include <QColor>
#include <QSortFilterProxyModel>


class PictureSort : public QSortFilterProxyModel
{
public:
    PictureSort(QObject *parent=0);

    qreal get_lab(const QColor color) const;

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

private:
    QImage get_image(const QModelIndex &source) const;
};

#endif // PICTURESORT_H
