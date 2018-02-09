#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include "photomodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    // void setup_default_editor();
    void setup_list_view_ui();

    QListView *view;
    PhotoModel *model;
    QWidget *main_widget;

};

#endif // MAINWINDOW_H
