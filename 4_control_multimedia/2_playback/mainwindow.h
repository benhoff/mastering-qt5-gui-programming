#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMediaPlayer * _media_player;
    QMediaPlaylist *_media_playlist;
    QVideoWidget * _video_widget;
};

#endif // MAINWINDOW_H
