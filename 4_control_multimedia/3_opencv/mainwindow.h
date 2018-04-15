#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QGraphicsView * _graphics_view;
    QGraphicsScene *_graphics_scene;
    QMediaPlayer * _media_player;
    QGraphicsVideoItem *_graphics_video_item;
};

#endif // MAINWINDOW_H
