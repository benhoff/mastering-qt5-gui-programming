#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _media_player = new QMediaPlayer;
    _media_playlist = new QMediaPlaylist(_media_player);
    _video_widget = new QVideoWidget();

    // _media_playlist->setCurrentIndex(0);

    _media_player->setVideoOutput(_video_widget);
    setCentralWidget(_video_widget);

    // play on media player?
}

MainWindow::~MainWindow()
{

}
