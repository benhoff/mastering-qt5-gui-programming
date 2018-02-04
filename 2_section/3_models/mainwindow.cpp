#include <QVBoxLayout>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{



    model = new PictureModel();
    view = new QListView();
    view->setModel(model);

    main_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    main_widget->setLayout(layout);
    // TODO: add in some layouts here to mess with our model.
    // http://doc.qt.io/qt-5/model-view-programming.html#model-subclassing-reference
    setCentralWidget(main_widget);

}

MainWindow::~MainWindow()
{

}
