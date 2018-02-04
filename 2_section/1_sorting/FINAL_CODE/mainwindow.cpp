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
    // NOTE: these can also be set in the `.ui` file
    ui->centralWidget->setIconSize(QSize(90, 90));
    ui->centralWidget->setViewMode(QListView::IconMode);
    ui->centralWidget->setResizeMode(QListView::Adjust);
}

MainWindow::~MainWindow()
{
    delete ui;
}
