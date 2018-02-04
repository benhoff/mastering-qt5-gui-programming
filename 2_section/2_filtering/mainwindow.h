#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QIcon>
#include <QString>
#include <QPixmap>
#include <QColor>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QListView>
#include <QColorDialog>
#include "photomodel.h"
#include "picturefilter.h"
#include "virdisdialog.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void set_minimum();
    void set_maximum();
private:
    QToolButton *min_button;
    QToolButton *max_button;

    QListView *list_view;
    PhotoModel *photo_model;
    PictureFilter *picture_filter;
    QColor color_min;
    QColor color_max;
};

#endif // MAINWINDOW_H
