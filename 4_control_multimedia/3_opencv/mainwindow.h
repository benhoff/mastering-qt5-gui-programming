#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include "myvideosurface.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMediaPlayer * _media_player;
    MyVideoSurface *_video_surface;
};

#endif // MAINWINDOW_H
