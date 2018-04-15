#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _graphics_view = new QGraphicsView();
    _media_player = new QMediaPlayer();
    _graphics_video_item = new QGraphicsVideoItem();
    _media_player->setVideoOutput(_graphics_video_item);

    _graphics_scene = _graphics_view->scene();
    _graphics_scene->addItem(_graphics_video_item);
    // _graphics_scene->addRect()

    // _media_player->setMedia();
    // _media_player->play();
    _graphics_video_item->
}

MainWindow::~MainWindow()
{

}
