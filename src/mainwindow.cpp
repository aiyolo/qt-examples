#include "mainwindow.h"
#include <QDebug>

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

    // 连接 ImageViewer 的信号
    connect(m_imageViewer, &ImageViewer::doubleClicked, this, &MainWindow::onImageDoubleClicked);
    connect(m_imageViewer, &ImageViewer::rightClicked, this, &MainWindow::onImageRightClicked);
    connect(m_imageViewer, &ImageViewer::mouseMoved, this, &MainWindow::onImageMouseMoved);
}

MainWindow::~MainWindow()
{}

void MainWindow::onImageDoubleClicked(const QPointF& pos)
{
    qDebug() << "双击坐标：" << pos;
}

void MainWindow::onImageRightClicked(const QPointF& pos)
{
    qDebug() << "右键坐标：" << pos;
}

void MainWindow::onImageMouseMoved(const QPointF& pos)
{
    qDebug() << "鼠标位置：" << pos;
}

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

