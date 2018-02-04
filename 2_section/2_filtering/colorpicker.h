#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QVector>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qdrawutil.h>


class QColorLuminancePicker : public QWidget
{
    Q_OBJECT
public:
    QColorLuminancePicker(int index=0, QWidget* parent=0);
    ~QColorLuminancePicker();
    QColor current_color;

signals:
    void new_color(QColor val);

protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    enum { foff = 3, coff = 4 }; //frame and contents offset
    void set_color(QColor color);

    QColor y_to_color(int y);
    // TODO: Determine if needed
    int color_to_y(QColor color);

    QPixmap *pix;
    // Note: don't need the extensions that we have here actually.
    QVector<QColor> virdis_values;
};
#endif // COLORPICKER_H
