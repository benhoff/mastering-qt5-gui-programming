#include "videosurface.h"
#include <QPainter>
#include <QDir>
#include <QCoreApplication>
#include <QResource>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QDebug>
#include <QGraphicsItem>


VideoSurface::VideoSurface(QWidget *video_widget, QObject *parent)
       : QAbstractVideoSurface(parent)
	, _video_widget(video_widget)
{
    QDir exeDir(QCoreApplication::applicationDirPath());
    bool loaded = _face_classifier.load(exeDir.filePath("haarcascade_frontalface_default.xml").toStdString().c_str());
}

void VideoSurface::paint(QPainter &painter)
{
    if (_current_video_frame.map(QAbstractVideoBuffer::ReadOnly)) {
        QImage image(
                _current_video_frame.bits(),
                _current_video_frame.width(),
                _current_video_frame.height(),
                _current_video_frame.bytesPerLine(),
                _image_format);

        cv::Mat frame = _get_mat(image);

        std::vector<cv::Rect> faces;
        _face_classifier.detectMultiScale(frame, faces, 1.1, 2,  0|CV_HAAR_SCALE_IMAGE,
                                          cv::Size(frame.cols/4, frame.rows/4)); // Minimum size of obj);

         QPainter image_painter(&image);
         QPen pen;
         pen.setColor(Qt::red);
         pen.setWidth(10);
         image_painter.setPen(pen);
        for (cv::Rect rectangle: faces)
        {
            QPoint top_left(rectangle.tl().x, rectangle.tl().y);

            QPoint bottom_right(rectangle.br().x, rectangle.br().y);
            QRect my_rectangle(top_left, bottom_right);
            image_painter.drawRect(my_rectangle);
        }

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           const QTransform old_transform = painter.transform();
           painter.scale(1, -1);
           painter.translate(0, -_video_widget->height());
	   painter.drawImage(_target_rectangle, image, _source_rectangle);
	   painter.setTransform(old_transform);
        }
	else
		painter.drawImage(_target_rectangle, image, _source_rectangle);

        qDebug() << "Size: " << faces.size();

        _current_video_frame.unmap();
    }
}

void VideoSurface::resize()
{
    QSize size = surfaceFormat().sizeHint();
    // scale the size of things
    size.scale(_video_widget->size().boundedTo(size), Qt::KeepAspectRatio);

    _target_rectangle = QRect(QPoint(0, 0), size);
    // align the rectangle in the center
    _target_rectangle.moveCenter(_video_widget->rect().center());
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

        _video_widget->repaint(_target_rectangle);

        return true;
    }
}

void VideoSurface::stop()
{
    _current_video_frame = QVideoFrame();
    _target_rectangle = QRect();
    QAbstractVideoSurface::stop();
    _video_widget->update();
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

         _video_widget->updateGeometry();
         resize();

         return true;
     } else {
         return false;
     }
}

cv::Mat VideoSurface::_get_mat(QImage image)
{
    cv::Mat result;
    switch (image.format())
    {
    case QImage::Format_RGB888:{
        result = qimage_to_mat_ref(image, CV_8UC3);
        cv::cvtColor(result, result, CV_RGB2BGR);
        break;
    }
    case QImage::Format_Indexed8:
    {
        result = qimage_to_mat_ref(image, CV_8U);
        break;
    }
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        result = qimage_to_mat_ref(image, CV_8UC4);
        break;
    }
    case QImage::Format_RGB32:
    {
        result = qimage_to_mat_ref(image, CV_8UC4);
        cv::Mat new_result;
        cv::cvtColor(result, new_result, cv::COLOR_BGRA2BGR);
        cv::cvtColor(new_result, new_result, CV_BGR2GRAY);
        cv::equalizeHist(new_result, new_result);
        return new_result;
    }
    default:
        qWarning() << "QImage format not handled in switch: " << image.format();
        break;
    }

    cv::Mat new_result;
    cv::cvtColor(result, new_result, CV_BGR2GRAY);
    cv::equalizeHist(new_result, new_result);
    return new_result;
}
