#include "mainwindow.h"
#include <qdebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    qDebug() << "mainwindow";
    qInfo() << "mainwindow info ";
}

MainWindow::~MainWindow()
{}

void MainWindow::test_logger()
{
    qDebug() << "mainwindow";
    qInfo() << "mainwindow info ";
}

