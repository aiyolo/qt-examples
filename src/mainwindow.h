#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include "ui_mainwindow.h"
#include "imageviewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void openImage();

private:
    Ui::MainWindow ui;
    ImageViewer* m_imageViewer;
};
