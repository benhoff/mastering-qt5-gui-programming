#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    view = new QTreeView();
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{

}
