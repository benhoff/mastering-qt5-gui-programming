#include "videosurface.h"
#include <QPainter>
#include <QDir>
#include <QCoreApplication>
#include <QResource>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QDebug>
#include <QGraphicsItem>


VideoSurface::VideoSurface(QWidget *parent_widget, QObject *parent) : QAbstractVideoSurface(parent), _widget(parent_widget)
{
    QDir exeDir(QCoreApplication::applicationDirPath());
    bool loaded = _face_classifier.load(exeDir.filePath("haarcascade_frontalface_default.xml").toStdString().c_str());
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
    /*
    if (handleType == QAbstractVideoBuffer::NoHandle)
    {
        return QList<QVideoFrame::PixelFormat>()
            << QVideoFrame::Format_RGB32
            << QVideoFrame::Format_BGR32
            << QVideoFrame::Format_ARGB32
            << QVideoFrame::Format_RGB24
            << QVideoFrame::Format_BGR24
            << QVideoFrame::Format_RGB565
            << QVideoFrame::Format_AYUV444
            << QVideoFrame::Format_YUV444
            << QVideoFrame::Format_YV12
            << QVideoFrame::Format_YUV420P;
       */
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
	//  glTexImage2D
	// https://github.com/qt/qtmultimedia/blob/5.11/src/multimediawidgets/qpaintervideosurface.cpp#L400
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
     if (format.handleType() != QAbstractVideoBuffer::NoHandle)
	     qFatal("This won't work");
        case QVideoFrame::Format_RGB32:
            initRgbTextureInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_xrgbShaderProgram;
            break;
        case QVideoFrame::Format_BGR32:
            initRgbTextureInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_rgbShaderProgram;
            break;
        case QVideoFrame::Format_ARGB32:
            initRgbTextureInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_argbShaderProgram;
            break;
        case QVideoFrame::Format_RGB24:
            initRgbTextureInfo(GL_RGB8, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_rgbShaderProgram;
            break;
        case QVideoFrame::Format_BGR24:
            initRgbTextureInfo(GL_RGB8, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_xrgbShaderProgram;
            break;
        case QVideoFrame::Format_RGB565:
            initRgbTextureInfo(GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, format.frameSize());
            program = qt_arbfp_rgbShaderProgram;
            break;
        case QVideoFrame::Format_YUV444:
            initRgbTextureInfo(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_xyuvShaderProgram;
            m_yuv = true;
            break;
        case QVideoFrame::Format_AYUV444:
            initRgbTextureInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_ayuvShaderProgram;
            m_yuv = true;
            break;
        case QVideoFrame::Format_YV12:
            initYv12TextureInfo(format.frameSize());
            program = qt_arbfp_yuvPlanarShaderProgram;
            break;
        case QVideoFrame::Format_YUV420P:
            initYuv420PTextureInfo(format.frameSize());
            program = qt_arbfp_yuvPlanarShaderProgram;
            break;
        default:
            break;
        }   
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

        painter.drawImage(_target_rectangle, image, _source_rectangle);
        painter.setTransform(old_transform);
        qDebug() << "Size: " << faces.size();



        _current_video_frame.unmap();
    }
}
