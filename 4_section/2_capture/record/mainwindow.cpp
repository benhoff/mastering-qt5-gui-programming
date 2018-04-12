#include "mainwindow.h"

#include <QActionGroup>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>


Q_DECLARE_METATYPE(QCameraInfo)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _central_widget = new QWidget();
    _run_button = new QPushButton("Run");
    _pause_button = new QPushButton("Pause");
    _stop_button = new QPushButton("Stop");
    _camera_view = new QCameraViewfinder();

    _setup_ui();
    setCentralWidget(_central_widget);

    _setup_camera_devices();
    set_camera(QCameraInfo::defaultCamera());
    // update_recorder_state(_media_recorder.data()->state());
    connect(_media_recorder.data(), &QMediaRecorder::stateChanged, this, &MainWindow::update_recorder_state);
    connect(_run_button, &QPushButton::clicked, this, &MainWindow::run);
    connect(_pause_button, &QPushButton::clicked, this, &MainWindow::pause);
    connect(_stop_button, &QPushButton::clicked, this, &MainWindow::stop);
}

MainWindow::~MainWindow()
{

}

#include <iostream>

void MainWindow::update_recorder_state(QMediaRecorder::State media_state)
{
    switch (media_state){
    case QMediaRecorder::StoppedState:
        _run_button->setEnabled(true);
        _pause_button->setEnabled(true);
        _stop_button->setEnabled(false);
        std::cout << "Stopped!" << std::endl;
        break;
    case QMediaRecorder::PausedState:
        _run_button->setEnabled(true);
        _pause_button->setEnabled(false);
        _stop_button->setEnabled(true);
        std::cout << "Paused!" << std::endl;
        break;
    case QMediaRecorder::RecordingState:
        _run_button->setEnabled(false);
        _pause_button->setEnabled(true);
        _stop_button->setEnabled(true);
        std::cout << "Recording!" << std::endl;
        break;
    }

}

void MainWindow::_setup_ui()
{
    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(_run_button);
    layout->addWidget(_pause_button);
    layout->addWidget(_stop_button);

    QVBoxLayout *top_layout = new QVBoxLayout();
    top_layout->addWidget(_camera_view);
    top_layout->addLayout(layout);

    _central_widget->setLayout(top_layout);
}

void MainWindow::set_camera(const QCameraInfo &camera_info)
{
    _camera.reset(new QCamera(camera_info));
    _media_recorder.reset(new QMediaRecorder(_camera.data()));
    // state changed
    // error handeling
    _camera.data()->setViewfinder(_camera_view);
    _camera.data()->start();
}

void MainWindow::_setup_camera_devices()
{
    QActionGroup *camera_group = new QActionGroup(this);
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QMenuBar* menu_bar = menuBar();
    QMenu *device_menu = menu_bar->addMenu("Devices");

    for (const QCameraInfo &camera_info: cameras)
    {
        QAction *camera_action = new QAction(camera_info.description(), camera_group);
        camera_action->setCheckable(true);
        camera_action->setData(QVariant::fromValue(camera_info));
        if (camera_info == QCameraInfo::defaultCamera())
            camera_action->setChecked(true);
        device_menu->addAction(camera_action);
    }

    connect(camera_group, &QActionGroup::triggered, this, &MainWindow::set_camera_action);
}

void MainWindow::set_camera_action(QAction *camera_action)
{
    set_camera(qvariant_cast<QCameraInfo>(camera_action->data()));
}

void MainWindow::run()
{
    _media_recorder.data()->record();
}

void MainWindow::pause()
{
    _media_recorder.data()->pause();
}

void MainWindow::stop()
{
    _media_recorder.data()->stop();
}
