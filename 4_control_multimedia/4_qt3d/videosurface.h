#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QImage>
#include <QWidget>

#include <QtGui/QOpenGLFunctions>

#include <opencv2/opencv.hpp>

inline cv::Mat qimage_to_mat_ref(QImage &img, int format)
{
    return cv::Mat(img.height(), img.width(), format, const_cast<uchar*>(img.bits()), static_cast<size_t>(img.bytesPerLine()));
}

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QObject *parent=nullptr);

    bool present(const QVideoFrame &frame) override;
    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;
    void set_current_frame(const QVideoFrame &frame);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const override;


private:
    cv::Mat _get_mat(QImage image);

    void initRgbTextureInfo(GLenum internalFormat, GLuint format, GLenum type, const QSize &size);
    void initYuv420PTextureInfo(const QSize &size);
    void initYv12TextureInfo(const QSize &size);

    cv::CascadeClassifier _face_classifier;

    QImage::Format _image_format;

    QVideoSurfaceFormat::Direction _scan_line_direction;
    bool _mirrored;
    QVideoSurfaceFormat::YCbCrColorSpace _color_space;
    GLenum _texture_format;
    GLuint _texture_internal_format;
    GLenum _texture_type;
    int _texture_count;

    static const uint Max_Textures = 3;
    GLuint _texture_ids[Max_Textures];
    int _texture_widths[Max_Textures];
    int _texture_heights[Max_Textures];
    int _texture_offsets[Max_Textures];
    bool _yuv;
};

#endif // VIDEOSURFACE_H
