#include "videosurface.h"
#include <QPainter>
#include <QDebug>


VideoSurface::VideoSurface(QWidget *parent_widget, QObject *parent) :
    QAbstractVideoSurface(parent),
    _widget(parent_widget)
{
}

void VideoSurface::resize()
{
    QSize size = surfaceFormat().sizeHint();
    // scale the size of things
    size.scale(_widget->size().boundedTo(size), Qt::KeepAspectRatio);

    _target_rectangle = QRect(QPoint(0, 0), size);
    // align the rectangle in the center
    _target_rectangle.moveCenter(_widget->rect().center());
}


QVideoSurfaceFormat VideoSurface::nearestFormat(const QVideoSurfaceFormat &format) const
{
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle)
    {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB24;
    } else
        return QList<QVideoFrame::PixelFormat>();
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size())
    {
        setError(IncorrectFormatError);
        stop();

        return false;
    }
    else {
        _current_video_frame = frame;

        _widget->repaint(_target_rectangle);

        return true;
    }
}

void VideoSurface::stop()
{
    _current_video_frame = QVideoFrame();
    _target_rectangle = QRect();
    QAbstractVideoSurface::stop();
    _widget->update();
}

bool VideoSurface::start(const QVideoSurfaceFormat &format)
{
     const QImage::Format image_format = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     if (image_format != QImage::Format_Invalid && !size.isEmpty()) {
         _image_format = image_format;
         _image_size = size;
         _source_rectangle = format.viewport();

         QAbstractVideoSurface::start(format);

         _widget->updateGeometry();
         resize();

         return true;
     } else {
         return false;
     }
}

void VideoSurface::paint(QPainter &painter)
{
    if (_current_video_frame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform old_transform = painter.transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter.scale(1, -1);
           painter.translate(0, -_widget->height());
        }

        QImage image(
                _current_video_frame.bits(),
                _current_video_frame.width(),
                _current_video_frame.height(),
                _current_video_frame.bytesPerLine(),
                _image_format);

        painter.drawImage(_target_rectangle, image, _source_rectangle);
        painter.setTransform(old_transform);

        _current_video_frame.unmap();
    }
}
