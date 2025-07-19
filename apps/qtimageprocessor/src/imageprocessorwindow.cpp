#include "imageprocessorwindow.h"
#include "imageprocessor.h"
#include "algorithmparams.h"
#include "algorithmfactory.h"
#include "simpleimageviewer/SimpleImageViewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>

ImageProcessorWindow::ImageProcessorWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_originalViewer(new SimpleImageViewer(this))
    , m_processedViewer(new SimpleImageViewer(this))
    , m_algorithmParams(new AlgorithmParams(this))
    , m_imageProcessor(new ImageProcessor(m_algorithmParams))
    , m_originalImage()
    , m_algorithmCombo(new QComboBox(this))
    , m_parameterWidget(new QWidget(this))
    , m_parameterLayout(new QVBoxLayout(m_parameterWidget))
    , m_parameterScrollArea(new QScrollArea(this))
    , m_openButton(new QPushButton("打开图像", this))
    , m_saveButton(new QPushButton("保存结果", this))
    , m_resetButton(new QPushButton("重置", this))
{
    setWindowTitle("Qt Image Processor");

    // 创建主窗口部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 创建图像显示区域
    QGroupBox* imageGroup = new QGroupBox("图像显示", this);
    QHBoxLayout* imageLayout = new QHBoxLayout(imageGroup);

    QGroupBox* originalGroup = new QGroupBox("原图", this);
    QVBoxLayout* originalLayout = new QVBoxLayout(originalGroup);
    originalLayout->addWidget(m_originalViewer);

    QGroupBox* processedGroup = new QGroupBox("处理后", this);
    QVBoxLayout* processedLayout = new QVBoxLayout(processedGroup);
    processedLayout->addWidget(m_processedViewer);

    imageLayout->addWidget(originalGroup);
    imageLayout->addWidget(processedGroup);
    mainLayout->addWidget(imageGroup, 2);

    // 创建控制面板
    QGroupBox* controlGroup = new QGroupBox("控制面板", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

    // 算法选择
    QLabel* algorithmLabel = new QLabel("算法选择:", this);

    // 从算法工厂获取算法名称
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    QStringList algorithmNames = factory.algorithmNames();
    m_algorithmCombo->addItems(algorithmNames);

    // 参数区域设置
    m_parameterScrollArea->setWidget(m_parameterWidget);
    m_parameterScrollArea->setWidgetResizable(true);
    m_parameterScrollArea->setMinimumHeight(200);
    m_parameterWidget->setLayout(m_parameterLayout);

    // 添加控件到布局
    controlLayout->addWidget(algorithmLabel);
    controlLayout->addWidget(m_algorithmCombo);
    controlLayout->addWidget(m_parameterScrollArea);
    controlLayout->addWidget(m_openButton);
    controlLayout->addWidget(m_saveButton);
    controlLayout->addWidget(m_resetButton);
    controlLayout->addStretch();

    mainLayout->addWidget(controlGroup, 1);

    // 连接信号槽
    connect(m_openButton, &QPushButton::clicked, this, &ImageProcessorWindow::openImage);
    connect(m_saveButton, &QPushButton::clicked, this, &ImageProcessorWindow::saveProcessedImage);
    connect(m_resetButton, &QPushButton::clicked, this, &ImageProcessorWindow::resetToOriginal);

    connect(m_algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImageProcessorWindow::onAlgorithmChanged);

    connect(m_algorithmParams, &AlgorithmParams::parametersChanged,
            this, &ImageProcessorWindow::updateProcessedImage);

    // 初始化
    onAlgorithmChanged(0);
}

ImageProcessorWindow::~ImageProcessorWindow() = default;

void ImageProcessorWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开图像", "",
        "图像文件 (*.png *.jpg *.jpeg *.bmp *.tiff *.tif)");

    if (!fileName.isEmpty()) {
        m_originalImage = QImage(fileName);
        if (!m_originalImage.isNull()) {
            m_originalViewer->setImage(m_originalImage);
            processAndDisplay();
        } else {
            QMessageBox::warning(this, "错误", "无法加载图像文件！");
        }
    }
}

void ImageProcessorWindow::saveProcessedImage()
{
    if (m_processedViewer->getImage().isNull()) {
        QMessageBox::warning(this, "警告", "没有可保存的图像！");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "保存图像", "",
        "PNG文件 (*.png);;JPEG文件 (*.jpg);;BMP文件 (*.bmp)");

    if (!fileName.isEmpty()) {
        if (!m_processedViewer->getImage().save(fileName)) {
            QMessageBox::warning(this, "错误", "无法保存图像文件！");
        }
    }
}

void ImageProcessorWindow::resetToOriginal()
{
    if (!m_originalImage.isNull()) {
        m_processedViewer->setImage(m_originalImage);

        // 重置参数
        m_algorithmParams->setBlurKernelSize(5);
        m_algorithmParams->setEdgeThreshold(100);
        m_algorithmParams->setBrightness(0);
        m_algorithmParams->setContrast(1.0);
    }
}

void ImageProcessorWindow::updateProcessedImage()
{
    processAndDisplay();
}

void ImageProcessorWindow::onAlgorithmChanged(int index)
{
    if (index >= 0) {
        // 清空参数区域
        QLayoutItem* child;
        while ((child = m_parameterLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        // 获取对应的算法
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        QString algorithmName = m_algorithmCombo->itemText(index);
        AlgorithmParams::Algorithm algorithmType = factory.algorithmType(algorithmName);

        m_algorithmParams->setAlgorithm(algorithmType);

        ImageAlgorithm* algorithm = factory.createAlgorithm(algorithmType);

        if (algorithm) {
            // 创建参数控件
            QWidget* paramWidget = algorithm->createParameterWidget(m_parameterWidget);
            if (paramWidget) {
                m_parameterLayout->addWidget(paramWidget);
            }
        }

        processAndDisplay();
    }
}

void ImageProcessorWindow::processAndDisplay()
{
    if (!m_originalImage.isNull()) {
        QImage processed = m_imageProcessor->processImage(m_originalImage);
        m_processedViewer->setImage(processed);
    }
}
