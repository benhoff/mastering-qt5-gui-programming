#include "myvideosurface.h"

MyVideoSurface::MyVideoSurface()
{
    _face_classifier.load("://haarcascade_frontalface_default.xml");

}

void MyVideoSurface::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.end();
}

QVideoSurfaceFormat MyVideoSurface::nearestFormat(const QVideoSurfaceFormat &format) const
{
    QSize frame_size = format.frameSize();
    // get current frame size from widget

}

QList<QVideoFrame::PixelFormat> MyVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle)
    {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else
        return QList<QVideoFrame::PixelFormat>();
}

bool MyVideoSurface::present(const QVideoFrame &frame)
{
    // FIXME
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size())
    {
        setError(IncorrectFormatError);
        stop();

        return false;
    } else {
        _current_video_frame = frame;

        // FIXME
        widget->repaint(_target_rectangle);

        return true;
    }
}

void MyVideoSurface::stop()
{
    _current_video_frame = QVideoFrame();
    _target_rectangle = QRect();
    QAbstractVideoSurface::stop();
    // widget->update();
}

bool MyVideoSurface::start(const QVideoSurfaceFormat &format)
{
     const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
         _image_format = imageFormat;
         _image_size = size;
         _source_rectangle = format.viewport();

         QAbstractVideoSurface::start(format);

         widget->updateGeometry();
         updateVideoRect();

         return true;
     } else {
         return false;
     }
}
