#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QPaintEvent>

#include <opencv2/opencv.hpp>


class MyVideoSurface : public QAbstractVideoSurface, public QWidget
{
    Q_OBJECT
public:
    MyVideoSurface();

    void paintEvent(QPaintEvent *event) override;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const override;

    bool present(const QVideoFrame &frame) override;
    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;

private:
    cv::CascadeClassifier _face_classifier;

    QImage::Format _image_format;
    QRect _target_rectangle;
    QSize _image_size;
    QRect _source_rectangle;
    QVideoFrame _current_video_frame;

};

#endif // MYVIDEOSURFACE_H
