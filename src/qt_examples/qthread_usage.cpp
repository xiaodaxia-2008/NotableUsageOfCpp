#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

#include "worker.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow win;
    auto widget = new QWidget;
    auto layout = new QVBoxLayout;
    widget->setLayout(layout);
    win.setCentralWidget(widget);
    QProgressBar *bar = new QProgressBar;
    layout->addWidget(bar);
    auto btn_start = new QPushButton("Start");
    layout->addWidget(btn_start);
    auto btn_stop = new QPushButton("Stop");
    layout->addWidget(btn_stop);
    auto result_label = new QLabel("Result:");
    layout->addWidget(result_label);

    QThread worker_thread;
    Worker worker;
    worker.moveToThread(&worker_thread);

    QObject::connect(btn_start, &QPushButton::clicked, &worker, &Worker::long_time_compution);
    QObject::connect(btn_stop, &QPushButton::clicked, &worker, &Worker::stop, Qt::DirectConnection);
    QObject::connect(&worker, &Worker::progressValueUpdated, bar, &QProgressBar::setValue);
    QObject::connect(&worker, &Worker::resultReady, result_label, &QLabel::setText);

    worker_thread.start();

    win.show();
    app.exec();

    worker_thread.quit();
    worker_thread.wait();
}