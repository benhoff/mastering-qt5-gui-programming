#include "videosurface.h"
#include <QPainter>
#include <QDir>
#include <QCoreApplication>
#include <QResource>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QDebug>
#include <QGraphicsItem>
#include <Qt3DRender/QShaderProgram>

VideoSurface::VideoSurface(QObject *parent)
    : QAbstractVideoSurface(parent)
    ,_scan_line_direction(QVideoSurfaceFormat::TopToBottom)
    , _mirrored(false)
    , _color_space(QVideoSurfaceFormat::YCbCr_BT601)
    , _texture_format(0)
    , _texture_type(0)
    , _texture_count(0)
    , _yuv(false)
{
    QDir exeDir(QCoreApplication::applicationDirPath());
    bool loaded = _face_classifier.load(exeDir.filePath("haarcascade_frontalface_default.xml").toStdString().c_str());
    Q_UNUSED(loaded)
}


QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
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
        // FIXME: implementation this is being based on uses helper method `setCurrentFrame`
        // see: https://github.com/qt/qtmultimedia/blob/5.11/src/multimediawidgets/qpaintervideosurface.cpp#L384
        set_current_frame(frame);
        return true;
    }
}

void VideoSurface::stop()
{
    // TODO: delete all texutres and remove all structures
    QAbstractVideoSurface::stop();
}

static const char *qt_glsl_vertexShaderProgram =
        "attribute highp vec4 vertexCoordArray;\n"
        "attribute highp vec2 textureCoordArray;\n"
        "uniform highp mat4 positionMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = positionMatrix * vertexCoordArray;\n"
        "   textureCoord = textureCoordArray;\n"
        "}\n";

// Paints an RGB32 frame
static const char *qt_glsl_xrgbShaderProgram =
        "uniform sampler2D texRgb;\n"
        "uniform mediump mat4 colorMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec4 color = vec4(texture2D(texRgb, textureCoord.st).bgr, 1.0);\n"
        "    gl_FragColor = colorMatrix * color;\n"
        "}\n";

// Paints an ARGB frame.
static const char *qt_glsl_argbShaderProgram =
        "uniform sampler2D texRgb;\n"
        "uniform mediump mat4 colorMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec4 color = vec4(texture2D(texRgb, textureCoord.st).bgr, 1.0);\n"
        "    color = colorMatrix * color;\n"
        "    gl_FragColor = vec4(color.rgb, texture2D(texRgb, textureCoord.st).a);\n"
        "}\n";

// Paints an RGB(A) frame.
static const char *qt_glsl_rgbShaderProgram =
        "uniform sampler2D texRgb;\n"
        "uniform mediump mat4 colorMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec4 color = vec4(texture2D(texRgb, textureCoord.st).rgb, 1.0);\n"
        "    color = colorMatrix * color;\n"
        "    gl_FragColor = vec4(color.rgb, texture2D(texRgb, textureCoord.st).a);\n"
        "}\n";

// Paints a YUV420P or YV12 frame.
static const char *qt_glsl_yuvPlanarShaderProgram =
        "uniform sampler2D texY;\n"
        "uniform sampler2D texU;\n"
        "uniform sampler2D texV;\n"
        "uniform mediump mat4 colorMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec4 color = vec4(\n"
        "           texture2D(texY, textureCoord.st).r,\n"
        "           texture2D(texU, textureCoord.st).r,\n"
        "           texture2D(texV, textureCoord.st).r,\n"
        "           1.0);\n"
        "    gl_FragColor = colorMatrix * color;\n"
        "}\n";

// Paints a YUV444 frame.
static const char *qt_glsl_xyuvShaderProgram =
        "uniform sampler2D texRgb;\n"
        "uniform mediump mat4 colorMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec4 color = vec4(texture2D(texRgb, textureCoord.st).gba, 1.0);\n"
        "    gl_FragColor = colorMatrix * color;\n"
        "}\n";

// Paints a AYUV444 frame.
static const char *qt_glsl_ayuvShaderProgram =
        "uniform sampler2D texRgb;\n"
        "uniform mediump mat4 colorMatrix;\n"
        "varying highp vec2 textureCoord;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec4 color = vec4(texture2D(texRgb, textureCoord.st).gba, 1.0);\n"
        "    color = colorMatrix * color;\n"
        "    gl_FragColor = vec4(color.rgb, texture2D(texRgb, textureCoord.st).r);\n"
        "}\n";

bool VideoSurface::start(const QVideoSurfaceFormat &format)
{
     const char *program = 0;

     // check to make sure we're only getting the no handle version
     if (format.handleType() != QAbstractVideoBuffer::NoHandle) {
         qFatal("This won't work");
         return false;
     }

     _texture = new Texture2D();

     switch (format.pixelFormat()) {
     case QVideoFrame::Format_RGB32:
         fragmentProgram = qt_glsl_xrgbShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGBAFormat);
         _texture->set_pixel_format(GL_RGBA);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         break;
     case QVideoFrame::Format_BGR32:
         fragmentProgram = qt_glsl_rgbShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGBFormat);
         _texture->set_pixel_format(GL_RGBA);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         break;
     case QVideoFrame::Format_ARGB32:
         fragmentProgram = qt_glsl_argbShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGBAFormat);
         _texture->set_pixel_format(GL_RGBA);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         break;
     case QVideoFrame::Format_RGB24:
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGB8_UNorm);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         _texture->set_pixel_format(GL_RGB);
         fragmentProgram = qt_glsl_rgbShaderProgram;

         break;
     case QVideoFrame::Format_BGR24:
         fragmentProgram = qt_glsl_argbShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGB8_UNorm);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         _texture->set_pixel_format(GL_RGB);
         break;
     case QVideoFrame::Format_RGB565:
         fragmentProgram = qt_glsl_rgbShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGBFormat);
         _texture->set_pixel_format(GL_RGB);
         _texture->set_pixel_type(GL_UNSIGNED_SHORT_5_6_5);
         break;
     case QVideoFrame::Format_YUV444:
         fragmentProgram = qt_glsl_xyuvShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGBFormat);
         _texture->set_pixel_format(GL_RGB);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         m_yuv = true;
         break;
     case QVideoFrame::Format_AYUV444:
         fragmentProgram = qt_glsl_ayuvShaderProgram;
         m_yuv = true;
         _texture.setFormat(Qt3DRender::QAbstractTexture::RGBAFormat);
         _texture->set_pixel_format(GL_RGBA);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         break;
     case QVideoFrame::Format_YV12:
         _texture.setFormat(Qt3DRender::QAbstractTexture::LuminanceFormat);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         _texture->set_pixel_format(GL_LUMINANCE);
         // TODO: Check if this works.
         _texture->setDepth(3);
         // offsets are a little weird?
         // see: https://github.com/enthought/qt-mobility/blob/master/src/multimedia/qpaintervideosurface.cpp#L518
        // int bytesPerLine = (size.width() + 3) & ~3;
        // int bytesPerLine2 = (size.width() / 2 + 3) & ~3;
         fragmentProgram = qt_glsl_yuvPlanarShaderProgram;
         break;
     case QVideoFrame::Format_YUV420P:
         fragmentProgram = qt_glsl_yuvPlanarShaderProgram;
         _texture.setFormat(Qt3DRender::QAbstractTexture::LuminanceFormat);
         _texture->set_pixel_type(GL_UNSIGNED_BYTE);
         _texture->set_pixel_format(GL_LUMINANCE);

         // see: https://github.com/enthought/qt-mobility/blob/master/src/multimedia/qpaintervideosurface.cpp#L497
         // int bytesPerLine = (size.width() + 3) & ~3;
         // int bytesPerLine2 = (size.width() / 2 + 3) & ~3;
         break;
     default:
         break;
     }
     // FIXME: looks like the textures are not consistent, IE Yuv420, etc are different
     // might need to set these in the `setData` command
     _texture.setHeight(format.frameHeight());
     _texture.setWidth(format.frameWidth());

     // set linear magnificatnion filter
     _texture.setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
     // Create wrap mode instance and set wrap to clamp to edge
     Qt3DRender::QTextureWrapMode wrap_mode;
     wrap_mode.setX(Qt3DRender::QTextureWrapMode::ClampToEdge);
     wrap_mode.setY(Qt3DRender::QTextureWrapMode::ClampToEdge);

     _texture.setWrapMode(wrap_mode);


     // TODO: bind program, and compile shader
     QAbstractVideoSurface::start(format);
     // NOTE: https://github.com/qt/qtmultimedia/blob/5.11/src/multimediawidgets/qpaintervideosurface.cpp#L845S
     // FIXME missing frame size
     _scan_line_direction = format.scanLineDirection();
     _mirrored = format.property("mirrored").toBool();
     _color_space = format.yCbCrColorSpace();
     _frame_size = format.frameSize();

    // FIXME
    Qt3DRender::QShaderProgram program(Qt3DRender::QShaderProgram::Fragment, program);


         return true;
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

void VideoSurface::set_current_frame(const QVideoFrame &frame)
{
    // https://www.kdab.com/writing-custom-qt-3d-aspect-part-2/
}
/*
void VideoSurface::paint(QPainter &painter)
{
    if (_current_video_frame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform old_transform = painter.transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter.scale(1, -1);
           painter.translate(0, -_widget->height());
        }


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
*/

QVideoSurfaceFormat VideoSurface::nearestFormat(const QVideoSurfaceFormat &format) const
{
    Q_UNUSED(format)
    /* NOTE: this method is not used internally in Qt's programming, yet is required to be overriden
     * per the abstract base class.
     * see https://github.com/qt/qtmultimedia/blob/5.11/src/multimediawidgets/qpaintervideosurface_p.h#L71
     * Not sure what the purpose of this method is.
    */
}
