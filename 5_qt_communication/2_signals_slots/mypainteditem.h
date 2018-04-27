#ifndef MYPAINTEDITEM_H
#define MYPAINTEDITEM_H

#include <QQuickPaintedItem>
#include <QVector>
#include <QColor>
#include <QPainter>


class MyPaintedItem : public QQuickPaintedItem
{
    Q_OBJECT
    // See: https://wiki.qt.io/How_to_Bind_a_QML_Property_to_a_C%2B%2B_Function
    // Q_PROPERTY(QColor y_to_color READ name WRITE setName NOTIFY nameChanged)
public:
    explicit MyPaintedItem();

    void paint(QPainter *p);

private:
    QColor y_to_color(int y);
    int color_to_y(QColor color);

    QColor current_color;
    QVector<QColor> viridis_values;
    QPixmap *pix;
};

#endif // MYPAINTEDITEM_H
