#include "videosurface.h"
#include <QPainter>
#include <QDir>
#include <QCoreApplication>
#include <QResource>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QDebug>
#include <QGraphicsItem>

// Paints an RGB32 frame
static const char *qt_arbfp_xrgbShaderProgram =
    "!!ARBfp1.0\n"
    "PARAM matrix[4] = { program.local[0..2],"
    "{ 0.0, 0.0, 0.0, 1.0 } };\n"
    "TEMP xrgb;\n"
    "TEX xrgb.xyz, fragment.texcoord[0], texture[0], 2D;\n"
    "MOV xrgb.w, matrix[3].w;\n"
    "DP4 result.color.x, xrgb.zyxw, matrix[0];\n"
    "DP4 result.color.y, xrgb.zyxw, matrix[1];\n"
    "DP4 result.color.z, xrgb.zyxw, matrix[2];\n"
    "END";

// Paints an ARGB frame.
static const char *qt_arbfp_argbShaderProgram =
    "!!ARBfp1.0\n"
    "PARAM matrix[4] = { program.local[0..2],"
    "{ 0.0, 0.0, 0.0, 1.0 } };\n"
    "TEMP argb;\n"
    "TEX argb, fragment.texcoord[0], texture[0], 2D;\n"
    "MOV argb.w, matrix[3].w;\n"
    "DP4 result.color.x, argb.zyxw, matrix[0];\n"
    "DP4 result.color.y, argb.zyxw, matrix[1];\n"
    "DP4 result.color.z, argb.zyxw, matrix[2];\n"
    "TEX result.color.w, fragment.texcoord[0], texture, 2D;\n"
    "END";

// Paints an RGB(A) frame.
static const char *qt_arbfp_rgbShaderProgram =
    "!!ARBfp1.0\n"
    "PARAM matrix[4] = { program.local[0..2],"
    "{ 0.0, 0.0, 0.0, 1.0 } };\n"
    "TEMP rgb;\n"
    "TEX rgb, fragment.texcoord[0], texture[0], 2D;\n"
    "MOV rgb.w, matrix[3].w;\n"
    "DP4 result.color.x, rgb, matrix[0];\n"
    "DP4 result.color.y, rgb, matrix[1];\n"
    "DP4 result.color.z, rgb, matrix[2];\n"
    "TEX result.color.w, fragment.texcoord[0], texture, 2D;\n"
    "END";

// Paints a YUV420P or YV12 frame.
static const char *qt_arbfp_yuvPlanarShaderProgram =
    "!!ARBfp1.0\n"
    "PARAM matrix[4] = { program.local[0..2],"
    "{ 0.0, 0.0, 0.0, 1.0 } };\n"
    "TEMP yuv;\n"
    "TEX yuv.x, fragment.texcoord[0], texture[0], 2D;\n"
    "TEX yuv.y, fragment.texcoord[0], texture[1], 2D;\n"
    "TEX yuv.z, fragment.texcoord[0], texture[2], 2D;\n"
    "MOV yuv.w, matrix[3].w;\n"
    "DP4 result.color.x, yuv, matrix[0];\n"
    "DP4 result.color.y, yuv, matrix[1];\n"
    "DP4 result.color.z, yuv, matrix[2];\n"
    "END";

// Paints a YUV444 frame.
static const char *qt_arbfp_xyuvShaderProgram =
    "!!ARBfp1.0\n"
    "PARAM matrix[4] = { program.local[0..2],"
    "{ 0.0, 0.0, 0.0, 1.0 } };\n"
    "TEMP ayuv;\n"
    "TEX ayuv, fragment.texcoord[0], texture[0], 2D;\n"
    "MOV ayuv.x, matrix[3].w;\n"
    "DP4 result.color.x, ayuv.yzwx, matrix[0];\n"
    "DP4 result.color.y, ayuv.yzwx, matrix[1];\n"
    "DP4 result.color.z, ayuv.yzwx, matrix[2];\n"
    "END";

// Paints a AYUV444 frame.
static const char *qt_arbfp_ayuvShaderProgram =
    "!!ARBfp1.0\n"
    "PARAM matrix[4] = { program.local[0..2],"
    "{ 0.0, 0.0, 0.0, 1.0 } };\n"
    "TEMP ayuv;\n"
    "TEX ayuv, fragment.texcoord[0], texture[0], 2D;\n"
    "MOV ayuv.x, matrix[3].w;\n"
    "DP4 result.color.x, ayuv.yzwx, matrix[0];\n"
    "DP4 result.color.y, ayuv.yzwx, matrix[1];\n"
    "DP4 result.color.z, ayuv.yzwx, matrix[2];\n"
    "TEX result.color.w, fragment.texcoord[0], texture, 2D;\n"
    "END";

VideoSurface::VideoSurface(QObject *parent)
    : QAbstractVideoSurface(parent)
    ,_scan_line_direction(QVideoSurfaceFormat::TopToBottom)
    , _mirrored(false)
    , _color_space(QVideoSurfaceFormat::YCbCr_BT601)
    , _texture_format(0)
    , _texture_internal_format(0)
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
     if (format.handleType() != QAbstractVideoBuffer::NoHandle) {
         qFatal("This won't work");
         return false;
     }
     switch (format.pixelFormat()){
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
            _yuv = true;
            break;
        case QVideoFrame::Format_AYUV444:
            initRgbTextureInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, format.frameSize());
            program = qt_arbfp_ayuvShaderProgram;
            _yuv = true;
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
         QAbstractVideoSurface::start(format);

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

void VideoSurface::initRgbTextureInfo(
        GLenum internalFormat, GLuint format, GLenum type, const QSize &size)
{
    _yuv = false;
    _texture_internal_format = internalFormat;
    _texture_format = format;
    _texture_type = type;
    _texture_count = 1; // Note: ensure this is always <= Max_Textures
    _texture_widths[0] = size.width();
    _texture_heights[0] = size.height();
    _texture_offsets[0] = 0;
}

void VideoSurface::initYuv420PTextureInfo(const QSize &size)
{
    int bytesPerLine = (size.width() + 3) & ~3;
    int bytesPerLine2 = (size.width() / 2 + 3) & ~3;

    _yuv = true;
    _texture_internal_format = GL_LUMINANCE;
    _texture_format = GL_LUMINANCE;
    _texture_type = GL_UNSIGNED_BYTE;
    _texture_count = 3; // Note: ensure this is always <= Max_Textures
    _texture_widths [0] = bytesPerLine;
    _texture_heights[0] = size.height();
    _texture_offsets[0] = 0;
    _texture_widths[1] = bytesPerLine2;
    _texture_heights[1] = size.height() / 2;
    _texture_offsets[1] = bytesPerLine * size.height();
    _texture_widths[2] = bytesPerLine2;
    _texture_heights[2] = size.height() / 2;
    _texture_offsets[2] = bytesPerLine * size.height() + bytesPerLine2 * size.height()/2;
}

void VideoSurface::initYv12TextureInfo(const QSize &size)
{
    int bytesPerLine = (size.width() + 3) & ~3;
    int bytesPerLine2 = (size.width() / 2 + 3) & ~3;

    _yuv = true;
    _texture_internal_format = GL_LUMINANCE;
    _texture_format = GL_LUMINANCE;
    _texture_type = GL_UNSIGNED_BYTE;
    _texture_count = 3; // Note: ensure this is always <= Max_Textures
    _texture_widths[0] = bytesPerLine;
    _texture_heights[0] = size.height();
    _texture_offsets[0] = 0;
    _texture_widths[1] = bytesPerLine2;
    _texture_heights[1] = size.height() / 2;
    _texture_offsets[1] = bytesPerLine * size.height() + bytesPerLine2 * size.height()/2;
    _texture_widths[2] = bytesPerLine2;
    _texture_heights[2] = size.height() / 2;
    _texture_offsets[2] = bytesPerLine * size.height();
}

QVideoSurfaceFormat VideoSurface::nearestFormat(const QVideoSurfaceFormat &format) const
{
    Q_UNUSED(format)
    /* NOTE: this method is not used internally in Qt's programming, yet is required to be overriden
     * per the abstract base class.
     * see https://github.com/qt/qtmultimedia/blob/5.11/src/multimediawidgets/qpaintervideosurface_p.h#L71
     * Not sure what the purpose of this method is.
    */
}
