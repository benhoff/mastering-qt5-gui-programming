#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new PhotoItemModel();

    sort = new PictureSort();
    sort->setSourceModel(model);
    sort->sort(0);

    ui->centralWidget->setModel(sort);
    ui->centralWidget->setViewMode(QListView::IconMode);
}

MainWindow::~MainWindow()
{
    delete ui;
}
