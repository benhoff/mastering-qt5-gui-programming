#include "mainwindow.h"

#include <QPixmap>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "luminosity_calc.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QToolBar *toolbar = new QToolBar();
    min_button = new QToolButton();
    max_button = new QToolButton();

    toolbar->addWidget(min_button);
    toolbar->addWidget(max_button);

    connect(max_button, &QToolButton::clicked, this, &MainWindow::set_maximum);
    connect(min_button, &QToolButton::clicked, this, &MainWindow::set_minimum);

    addToolBar(toolbar);

    list_view = new QListView();
    photo_model = new PhotoModel();
    picture_filter = new PictureFilter();
    picture_filter->setSourceModel(photo_model);

    list_view->setModel(picture_filter);

    setup_list_view_ui();
    setup_tool_buttons_ui();

    setCentralWidget(list_view);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setup_list_view_ui()
{
    list_view->setIconSize(QSize(90, 90));
    list_view->setViewMode(QListView::IconMode);
    list_view->setResizeMode(QListView::Adjust);
}

void MainWindow::setup_tool_buttons_ui()
{
    QPixmap min_pix(20, 20);
    QPixmap max_pix(20, 20);

    min_pix.fill(QColor(68, 1, 84));
    max_pix.fill(QColor(254, 232, 37));

    QIcon min_icon(min_pix);
    QIcon max_icon(max_pix);

    max_button->setIcon(max_icon);
    max_button->setText("Set Maximum");
    max_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    min_button->setIcon(min_icon);
    min_button->setText("Set Minimum");
    min_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void MainWindow::set_minimum()
{
    color_min = VirdisDialog::get_color(color_min);
    QPixmap pix(20, 20);
    pix.fill(color_min);
    QIcon min_icon(pix);
    min_button->setIcon(min_icon);

    qreal luminosity = get_luminosity(color_min);
    picture_filter->set_min_filter_value(luminosity);
}

void MainWindow::set_maximum()
{
    color_max = VirdisDialog::get_color(color_max);
    QPixmap pix(20, 20);
    pix.fill(color_max);
    QIcon max_icon(pix);
    max_button->setIcon(max_icon);

    qreal luminosity = get_luminosity(color_max);
    picture_filter->set_max_filter_value(luminosity);
}
