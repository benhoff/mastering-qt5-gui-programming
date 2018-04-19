#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _media_player = new QMediaPlayer;
    _media_playlist = new QMediaPlaylist(_media_player);
    _video_widget = new VideoWidget;
    _media_player->setVideoOutput(_video_widget->get_videosurface());
    setCentralWidget(_video_widget);
}

MainWindow::~MainWindow()
{

}
