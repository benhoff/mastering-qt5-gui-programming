#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _central_widget(new QWidget()),
      _play_button(new QPushButton("Play")),
      _pause_button(new QPushButton("Pause")),
      _stop_button(new QPushButton("Stop")),
      _next_video(new QPushButton("Next Video")),
      _previous_video(new QPushButton("Previous Video"))

{
    _media_player = new QMediaPlayer;
    _media_playlist = new QMediaPlaylist;
    _media_player->setPlaylist(_media_playlist);

    _video_widget = new QVideoWidget();

    _media_player->setVideoOutput(_video_widget);
    _open_file = menuBar()->addAction("Open File");

    _setup_ui();
    setCentralWidget(_central_widget);

    connect(_play_button, &QPushButton::clicked, _media_player, &QMediaPlayer::play);
    connect(_pause_button, &QPushButton::clicked, _media_player, &QMediaPlayer::pause);
    connect(_stop_button, &QPushButton::clicked, _media_player, &QMediaPlayer::stop);
    connect(_next_video, &QPushButton::clicked, _media_playlist, &QMediaPlaylist::next);
    connect(_previous_video, &QPushButton::clicked, _media_playlist, &QMediaPlaylist::previous);
    connect(_open_file, &QAction::triggered, this, &MainWindow::trigger_media_file_dialog);
}

void MainWindow::trigger_media_file_dialog()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open");

    for (QString filepath: files)
    {
        _media_playlist->addMedia(QUrl::fromLocalFile(filepath));
    }

    if (_media_playlist->currentIndex() == -1)
        _media_playlist->setCurrentIndex(0);
}

void MainWindow::_setup_ui()
{
    QHBoxLayout *button_bar = new QHBoxLayout();
    button_bar->addWidget(_previous_video);
    button_bar->addWidget(_play_button);
    button_bar->addWidget(_pause_button);
    button_bar->addWidget(_stop_button);
    button_bar->addWidget(_next_video);

    QVBoxLayout *widget_stack = new QVBoxLayout();
    widget_stack->addWidget(_video_widget);
    widget_stack->addLayout(button_bar);

    _central_widget->setLayout(widget_stack);
}

MainWindow::~MainWindow()
{

}
