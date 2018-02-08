#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QVector>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qdrawutil.h>

namespace{
    inline QColor get_color(float r, float g, float b)
    {
        int red = (int)((r)*256.0);
        int green = (int)((g)*256.0);
        int blue = (int)((b)*256.0);
        return QColor(red, green, blue);
    }
}

class QColorLuminancePicker : public QWidget
{
    Q_OBJECT
public:
    QColorLuminancePicker(QColor color, QWidget* parent=0);
    ~QColorLuminancePicker();

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
    void set_color(QColor color);

    QColor y_to_color(int y);
    int color_to_y(QColor color);

    QPixmap *pix;
    QVector<QColor> virdis_values;
};
#endif // COLORPICKER_H
