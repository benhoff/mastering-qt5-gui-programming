#include "videosurface.h"
#include <QPainter>
#include <QFile>
#include <opencv2/imgproc/imgproc.hpp>


VideoSurface::VideoSurface(QWidget *parent_widget, QObject *parent) : QAbstractVideoSurface(parent), _widget(parent_widget)
{
    _face_classifier.load(QFile("://haarcascade_frontalface_default.xml").fileName().toStdString().c_str());
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
    QSize frame_size = format.frameSize();
    // get current frame size from widget

}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
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

cv::Mat VideoSurface::_get_mat(QImage image)
{
    switch (image.format()) {
    case QImage::Format_RGB888:{
        // FIXME
        cv::Mat result = qimage_to_mat_ref(image, CV_8UC3);
        cv::cvtColor(result, result, CV_RGB2BGR);
        return result;
    }
    case QImage::Format_Indexed8:{
        return qimage_to_mat_ref(image, CV_8U);
    }
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:{
        return qimage_to_mat_ref(image, CV_8UC4);
    }
    default:
        break;
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

        cv::Mat frame = _get_mat(image);
        cv::Mat grey_image;

        cv::cvtColor(frame, grey_image, CV_BGR2GRAY);
        cv::equalizeHist(grey_image, grey_image);
        std::vector<cv::Rect> faces;
        _face_classifier.detectMultiScale(grey_image, faces, 1.1, 2,  0|CV_HAAR_SCALE_IMAGE,
                                          cv::Size(frame.cols/4, frame.rows/4)); // Minimum size of obj);

        painter.drawImage(_target_rectangle, image, _source_rectangle);

        painter.setTransform(old_transform);

        _current_video_frame.unmap();
    }
}
