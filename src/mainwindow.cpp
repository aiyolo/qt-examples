#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // 创建ImageViewer实例并设置为中心部件
    m_imageViewer = new ImageViewer(this);
    setCentralWidget(m_imageViewer);

    // 创建文件菜单
    QMenu* fileMenu = menuBar()->addMenu("文件");
    QAction* openAction = fileMenu->addAction("打开图片");
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);

    // 设置窗口标题
    setWindowTitle("图片查看器");

}

MainWindow::~MainWindow()
{}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "打开图片",
        QString(),
        "图片文件 (*.png *.jpg *.bmp);;所有文件 (*.*)");

    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            return;
        }
        m_imageViewer->setImage(image);
    }
}

