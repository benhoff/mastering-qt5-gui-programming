#include "mainwindow.h"

#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    web_view = new QWebEngineView();
    central_widget = new QWidget();
    edit_bar = new QLineEdit();
    QString temp("http://images.google.com");
    edit_bar->setText(temp);

    web_view->load(QUrl(temp));
    setup_central_widget();
    setCentralWidget(central_widget);

    connect(edit_bar, &QLineEdit::editingFinished, this, &MainWindow::set_url);
}

void MainWindow::setup_central_widget()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(edit_bar);
    layout->addWidget(web_view);

    central_widget->setLayout(layout);
}

void MainWindow::set_url()
{
    QString url = edit_bar->text();
    web_view->load(QUrl::fromUserInput(url));
}

MainWindow::~MainWindow()
{

}
