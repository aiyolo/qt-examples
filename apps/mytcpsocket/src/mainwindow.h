#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include "ui_mainwindow.h"
#include <qtcpsocket.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
private:
    Ui::MainWindow ui;
    QTcpSocket* m_socket;
};
