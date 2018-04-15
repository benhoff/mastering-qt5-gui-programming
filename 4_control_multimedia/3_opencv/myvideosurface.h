#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QPaintEvent>


class MyVideoSurface : public QAbstractVideoSurface, public QWidget
{
public:
    MyVideoSurface();

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.end();
    }

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override
    {
        Q_UNUSED(handleType);

        // Return the formats you will support
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB565;
    }

    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const override
    {

    }

    bool present(const QVideoFrame &frame) override
    {
        Q_UNUSED(frame);
        // Handle the frame and do your processing

        return true;
    }

    bool start(const QVideoSurfaceFormat &format) override
    {
        return QAbstractVideoSurface::start(format);

    }

    void stop() override
    {
        QAbstractVideoSurface::stop();
    }

};

#endif // MYVIDEOSURFACE_H
