#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QImage>
#include <QWidget>


class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QWidget *parent_widget, QObject *parent=nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const override;

    bool present(const QVideoFrame &frame) override;
    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;
    void resize();
    void paint(QPainter &painter);


private:
    QImage::Format _image_format;
    QRect _target_rectangle;
    QSize _image_size;
    QRect _source_rectangle;
    QVideoFrame _current_video_frame;
    QWidget *_widget;

};

#endif // VIDEOSURFACE_H
