#include "mainwindow.h"

/*
#include <QItemEditorFactory>
#include <QItemEditorCreatorBase>
#include "colorpicker.h"
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // setup_default_editor();
    model = new PhotoModel();
    view = new QListView();
    view->setModel(model);
    setup_list_view_ui();

    setCentralWidget(view);

}

MainWindow::~MainWindow()
{

}
/*
void MainWindow::setup_default_editor()
{
    QItemEditorFactory *factory = new QItemEditorFactory;
    // FIXME: fix that name. Is brutal
    QItemEditorCreatorBase *color_picker = new QStandardItemEditorCreator<QColorLuminancePicker>();
    factory->registerEditor(QVariant::Color, color_picker);

    QItemEditorFactory::setDefaultFactory(factory);

}
*/
void MainWindow::setup_list_view_ui()
{
    view->setIconSize(QSize(90, 90));
    view->setViewMode(QListView::IconMode);
    view->setResizeMode(QListView::Adjust);
}
