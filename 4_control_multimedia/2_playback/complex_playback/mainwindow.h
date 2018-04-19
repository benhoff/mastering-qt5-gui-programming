#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "videowidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMediaPlayer * _media_player;
    QMediaPlaylist *_media_playlist;
    VideoWidget *_video_widget;
};

#endif // MAINWINDOW_H
