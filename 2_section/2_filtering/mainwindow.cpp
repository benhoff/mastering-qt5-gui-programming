#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QPixmap min_pix(20, 20);
    QPixmap max_pix(20, 20);
    //
    min_pix.fill(QColor(68, 1, 84));
    max_pix.fill(QColor(254, 232, 37));

    QIcon min_icon(min_pix);
    QIcon max_icon(max_pix);

    QToolBar *toolbar = new QToolBar();

    max_button = new QToolButton();
    // TODO: setIcon instead of trying to change pixmap
    max_button->setIcon(max_icon);
    max_button->setText("Set Maximum");
    max_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addWidget(max_button);

    connect(max_button, &QToolButton::clicked, this, &MainWindow::set_maximum);

    min_button = new QToolButton();
    min_button->setIcon(min_icon);
    min_button->setText("Set Minimum");
    min_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addWidget(min_button);

    connect(min_button, &QToolButton::clicked, this, &MainWindow::set_minimum);

    addToolBar(toolbar);

    list_view = new QListView();
    photo_model = new PhotoModel();
    picture_filter = new PictureFilter();
    picture_filter->setSourceModel(photo_model);

    list_view->setModel(picture_filter);
    list_view->setViewMode(QListView::IconMode);
    // FIXME: cleanup this class
    // central_widget = new CentralWidget();
    setCentralWidget(list_view);
}

MainWindow::~MainWindow()
{
}

void MainWindow::set_minimum()
{
    QColor new_color = VirdisDialog::get_color();
    QPixmap pix(20, 20);
    //
    pix.fill(new_color);
    QIcon min_icon(pix);
    min_button->setIcon(min_icon);
}

void MainWindow::set_maximum()
{
    QColor new_color = VirdisDialog::get_color();
    QPixmap pix(20, 20);
    //
    pix.fill(new_color);
    QIcon max_icon(pix);
    max_button->setIcon(max_icon);
}
