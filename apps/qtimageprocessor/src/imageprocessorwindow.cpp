#include "imageprocessorwindow.h"
#include "services/ImageProcessingService.h"
#include "factories/ProcessorFactory.h"
#include "interfaces/IParams.h"
#include "interfaces/IProcessorFactory.h"
#include "simpleimageviewer/SimpleImageViewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

ImageProcessorWindow::ImageProcessorWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_originalViewer(new SimpleImageViewer(this))
  , m_processedViewer(new SimpleImageViewer(this))
  , m_processorCombo(new QComboBox(this))
  , m_parameterWidget(new QWidget(this))
  , m_parameterLayout(new QVBoxLayout(m_parameterWidget))
  , m_openButton(new QPushButton("打开图像", this))
  , m_saveButton(new QPushButton("保存结果", this))
{
    setWindowTitle("Qt Image Processor (SOLID架构)");

    // 使用依赖注入创建服务 - 遵循依赖倒置原则
    auto factory = std::make_unique<ProcessorFactory>();
    m_processingService = std::make_unique<ImageProcessingService>(std::move(factory));

    setupUI();

    // 连接信号槽
    connect(m_openButton, &QPushButton::clicked, this, &ImageProcessorWindow::openImage);
    connect(m_saveButton, &QPushButton::clicked, this, &ImageProcessorWindow::saveProcessedImage);
    connect(m_processorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImageProcessorWindow::onProcessorChanged);

    // 加载处理器列表
    m_processorCombo->addItems(m_processingService->getAvailableProcessors());

    // 初始化处理器
    onProcessorChanged(0);
}

ImageProcessorWindow::~ImageProcessorWindow() = default;

void ImageProcessorWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 图像显示区域
    QWidget* imageWidget = new QWidget(this);
    QHBoxLayout* imageLayout = new QHBoxLayout(imageWidget);
    imageLayout->addWidget(m_originalViewer);
    imageLayout->addWidget(m_processedViewer);
    mainLayout->addWidget(imageWidget, 2);

    // 控制面板
    QWidget* controlWidget = new QWidget(this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlWidget);

    controlLayout->addWidget(new QLabel("算法选择:", this));
    controlLayout->addWidget(m_processorCombo);

    setupParameterArea();
    controlLayout->addWidget(m_parameterWidget);
    controlLayout->addWidget(m_openButton);
    controlLayout->addWidget(m_saveButton);
    controlLayout->addStretch();

    mainLayout->addWidget(controlWidget, 1);
}

void ImageProcessorWindow::setupParameterArea()
{
    m_parameterLayout->setAlignment(Qt::AlignTop);
    m_parameterWidget->setLayout(m_parameterLayout);
    m_parameterWidget->setMinimumHeight(200);
}

void ImageProcessorWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(
      this, "打开图像", "", "图像文件 (*.png *.jpg *.jpeg *.bmp *.tiff *.tif)");

    if (fileName.isEmpty()) {
        return;
    }

    QImage newImage(fileName);
    if (newImage.isNull()) {
        QMessageBox::warning(this, "加载错误", "无法加载图像文件！");
        return;
    }

    m_originalImage = newImage;
    m_originalViewer->setImage(m_originalImage);
    processAndDisplay();
}

void ImageProcessorWindow::saveProcessedImage()
{
    QImage imageToSave = m_processedViewer->getImage();
    if (imageToSave.isNull()) {
        QMessageBox::warning(this, "保存警告", "没有可保存的处理后图像！");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
      this,
      "保存处理后的图像",
      "",
      "PNG文件 (*.png);;JPEG文件 (*.jpg);;BMP文件 (*.bmp)");

    if (!fileName.isEmpty() && !imageToSave.save(fileName)) {
        QMessageBox::warning(this, "保存失败", "无法保存图像文件！");
    }
}

void ImageProcessorWindow::onProcessorChanged(int index)
{
    if (index < 0) return;

    clearParameterWidgets();

    QString processorName = m_processorCombo->itemText(index);
    ProcessorType type = m_processingService->getProcessorType(processorName);

    // 设置新的处理器类型
    m_processingService->setProcessorType(type);

    // 创建参数控件
    IParams* paramProvider = m_processingService->getParameterProvider();
    if (paramProvider) {
        QWidget* paramWidget = paramProvider->createWidget(m_parameterWidget);
        if (paramWidget) {
            m_parameterLayout->addWidget(paramWidget);
            connect(paramProvider, &IParams::parametersChanged,
                    this, &ImageProcessorWindow::onParametersChanged);
        }
    }

    m_parameterLayout->addStretch();
    processAndDisplay();
}

void ImageProcessorWindow::processAndDisplay()
{
    if (m_originalImage.isNull()) {
        return;
    }

    QImage processed = m_processingService->processImage(m_originalImage);
    m_processedViewer->setImage(processed.isNull() ? m_originalImage : processed);
}

void ImageProcessorWindow::clearParameterWidgets()
{
    // 断开所有信号连接
    IParams* paramProvider = m_processingService->getParameterProvider();
    if (paramProvider) {
        disconnect(paramProvider, &IParams::parametersChanged,
                   this, &ImageProcessorWindow::onParametersChanged);
    }

    // 清理所有子控件
    QLayoutItem* child;
    while ((child = m_parameterLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
            delete child->widget();
        }
        delete child;
    }
}

void ImageProcessorWindow::onParametersChanged()
{
    processAndDisplay();
}
