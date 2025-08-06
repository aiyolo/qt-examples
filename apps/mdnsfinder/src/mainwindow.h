#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include "ui_mainwindow.h"

class SimpleImageViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void openImage();
    void onImageDoubleClicked(const QPointF& pos);
    void onImageRightClicked(const QPointF& pos);
    void onImageMouseMoved(const QPointF& pos);

private:
    Ui::MainWindow ui;
    SimpleImageViewer* m_imageViewer;
};
