#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QVector>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qdrawutil.h>
#include "color_utils.h"

class QColorLuminancePicker : public QWidget
{
    Q_OBJECT

public:
    QColorLuminancePicker(QColor color, QWidget* parent=0);
    ~QColorLuminancePicker();

    QColor color() const { return current_color; }

    void set_color(QColor color);

    QColor current_color;

signals:
    void new_color(QColor val);

protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    QSize sizeHint() const;

private:
    enum { foff = 3, coff = 4 }; //frame and contents offset

    QColor y_to_color(int y);
    int color_to_y(QColor color);

    QPixmap *pix;
    QVector<QColor> virdis_values;
};
#endif // COLORPICKER_H
