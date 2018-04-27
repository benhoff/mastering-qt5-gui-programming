#ifndef MYPAINTEDITEM_H
#define MYPAINTEDITEM_H

#include <QQuickPaintedItem>
#include <QVector>
#include <QColor>
#include <qdrawutil.h>


class MyPaintedItem : public QQuickPaintedItem
{
public:
    MyPaintedItem();

    void paint(QPainter *p);

private:
    void update_rectangle();


    QVector<QColor> viridis_values;
    QPixmap *pix;
};

#endif // MYPAINTEDITEM_H
