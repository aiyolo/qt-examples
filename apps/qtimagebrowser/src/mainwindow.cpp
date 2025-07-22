#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDialog>
#include <QPixmap>
#include <QDir>
#include <QRandomGenerator>
#include <QImage>
#include <QVector>
#include <QLabel>
#include <QMouseEvent>
#include <QGridLayout>
#include <QScrollArea>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , currentRow(0)
  , currentCol(0)
{
    setupUI();
    generateTestImages();
    for (int i = 0; i < m_images.size(); ++i) {
        addThumbnail(m_images[i], i);
    }
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // 创建主窗口部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建垂直布局
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建滚动区域
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    // 创建容器部件
    containerWidget = new QWidget(scrollArea);
    scrollArea->setWidget(containerWidget);

    // 创建网格布局
    gridLayout = new QGridLayout(containerWidget);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    // 设置窗口标题和大小
    setWindowTitle("图片浏览器");
    resize(800, 600);
}

// loadImage method implementation removed as it's not needed

void MainWindow::generateTestImages()
{
    m_images.clear();
    for (int i = 0; i < 100; ++i) {
        QImage image(512, 512, QImage::Format_RGB32);
        QRgb* data = (QRgb*)image.bits();
        int hue = (i * 360) / 100;
        QRgb color = QColor::fromHsv(hue, 255, 255).rgb();
        for (int j = 0; j < 128 * 128; ++j) {
            data[j] = color;
        }
        m_images.append(QPixmap::fromImage(image));
    }
}

void MainWindow::addThumbnail(const QPixmap& pixmap, int index)
{
    // 创建SimpleImageViewer实例
    SimpleImageViewer* viewer = new SimpleImageViewer(containerWidget);
    viewer->setFixedSize(128, 128);
    viewer->setScaledContents(true);
    viewer->setPixmap(pixmap);

    // 连接点击事件
    connect(viewer, &SimpleImageViewer::clicked, [=]() { showDetail(index); });

    // 添加到网格布局
    gridLayout->addWidget(viewer, currentRow, currentCol);

    // 更新网格位置
    currentCol++;
    if (currentCol >= MAX_COLUMNS) {
        currentCol = 0;
        currentRow++;
    }
}

void MainWindow::onThumbnailClicked()
{
    // 可以添加点击效果或状态更新
    // 例如：改变边框颜色或显示加载指示器
}

void MainWindow::showDetail(int index)
{
    if (index < 0 || index >= m_images.size()) {
        return;
    }

    QDialog* previewDialog = new QDialog(this);
    previewDialog->setWindowTitle("图片详情");
    previewDialog->resize(800, 600);

    QVBoxLayout* dialogLayout = new QVBoxLayout(previewDialog);

    SimpleImageViewer* previewViewer = new SimpleImageViewer(previewDialog);
    previewViewer->setPixmap(m_images[index]);
    dialogLayout->addWidget(previewViewer);

    previewDialog->exec();
    previewDialog->deleteLater();
}
