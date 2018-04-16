#include <QMediaPlayer>
#include <QVideoWidget>


QMediaPlayer *player = new QMediaPlayer;
QMediaPlaylist = new QMediaPlaylist(player);

QVideoWidget *view = new QVideoWidget;

player->setVideoOutput(view);

view.show();

playlist.setCurrentIndex(0);
player->play();


