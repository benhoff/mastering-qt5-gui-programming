#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _media_player = new QMediaPlayer();
    _video_surface = new MyVideoSurface();
    _media_player->setVideoOutput(_video_surface);
    setCentralWidget(_video_surface);


}

MainWindow::~MainWindow()
{

}
