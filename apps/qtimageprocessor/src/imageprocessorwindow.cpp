#include "imageprocessorwindow.h"
#include "imageprocessor.h"
#include "algorithmparameters.h"
#include "algorithmfactory.h"
#include "simpleimageviewer/SimpleImageViewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

ImageProcessorWindow::ImageProcessorWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_originalViewer(new SimpleImageViewer(this))
  , m_processedViewer(new SimpleImageViewer(this))
  , m_algorithmParams(nullptr)
  , m_imageProcessor(std::make_unique<ImageProcessor>())
  , m_originalImage()
  , m_algorithmCombo(new QComboBox(this))
  , m_parameterWidget(new QWidget(this))
  , m_parameterLayout(new QVBoxLayout(m_parameterWidget))
  , m_openButton(new QPushButton("打开图像", this))
  , m_saveButton(new QPushButton("保存结果", this))
{
    setWindowTitle("Qt Image Processor");
    setupUI();

    // 连接信号槽
    connect(m_openButton, &QPushButton::clicked, this, &ImageProcessorWindow::openImage);
    connect(m_saveButton, &QPushButton::clicked, this, &ImageProcessorWindow::saveProcessedImage);
    connect(m_algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImageProcessorWindow::onAlgorithmChanged);

    // 初始化算法
    onAlgorithmChanged(0);
}

ImageProcessorWindow::~ImageProcessorWindow() = default;

void ImageProcessorWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 图像显示区域
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

    // 控制面板
    QGroupBox* controlGroup = new QGroupBox("控制面板", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

    controlLayout->addWidget(new QLabel("算法选择:", this));

    // 获取算法列表
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    m_algorithmCombo->addItems(factory.algorithmNames());
    controlLayout->addWidget(m_algorithmCombo);

    // 参数区域
    setupParameterArea();
    controlLayout->addWidget(m_parameterWidget);

    controlLayout->addWidget(m_openButton);
    controlLayout->addWidget(m_saveButton);
    controlLayout->addStretch();

    mainLayout->addWidget(controlGroup, 1);
}

void ImageProcessorWindow::setupParameterArea()
{
    m_parameterLayout->setContentsMargins(10, 10, 10, 10);
    m_parameterLayout->setSpacing(8);
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
        this, "保存处理后的图像", "",
        "PNG文件 (*.png);;JPEG文件 (*.jpg);;BMP文件 (*.bmp)");

    if (!fileName.isEmpty() && !imageToSave.save(fileName)) {
        QMessageBox::warning(this, "保存失败", "无法保存图像文件！");
    }
}



void ImageProcessorWindow::onAlgorithmChanged(int index)
{
    if (index < 0) return;

    clearParameterWidgets();

    AlgorithmFactory& factory = AlgorithmFactory::instance();
    QString algorithmName = m_algorithmCombo->itemText(index);
    AlgorithmParams::Algorithm algorithmType = factory.algorithmType(algorithmName);

    // 创建算法和参数
    std::unique_ptr<ImageAlgorithm> algorithm(factory.createAlgorithm(algorithmType));
    if (!algorithm) return;

    std::unique_ptr<AlgorithmParameters> params(algorithm->createParameters());
    if (!params) return;

    // 创建参数控件
    QWidget* paramWidget = params->createParameterWidget(m_parameterWidget);
    if (paramWidget) {
        m_parameterLayout->addWidget(paramWidget);
        connect(params.get(), &AlgorithmParameters::parametersChanged,
                this, &ImageProcessorWindow::onParametersChanged);
    }

    m_parameterLayout->addStretch();
    m_algorithmParams = std::move(params);

    // 更新处理器参数
    if (m_algorithmParams) {
        std::unique_ptr<AlgorithmParameters> paramsCopy(m_algorithmParams->clone());
        m_imageProcessor->setParameters(std::move(paramsCopy));
    }

    processAndDisplay();
}

void ImageProcessorWindow::processAndDisplay()
{
    if (m_originalImage.isNull() || !m_algorithmParams) {
        return;
    }

    QImage processed = m_imageProcessor->processImage(m_originalImage);
    m_processedViewer->setImage(processed.isNull() ? m_originalImage : processed);
}

void ImageProcessorWindow::clearParameterWidgets()
{
    if (m_algorithmParams) {
        disconnect(m_algorithmParams.get(), &AlgorithmParameters::parametersChanged,
                   this, &ImageProcessorWindow::onParametersChanged);
    }

    QLayoutItem* child;
    while ((child = m_parameterLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
            delete child->widget();
        }
        delete child;
    }

    m_algorithmParams.reset();
}

void ImageProcessorWindow::onParametersChanged()
{
    if (!m_algorithmParams || m_originalImage.isNull()) {
        return;
    }

    // 更新处理器参数并重新处理
    std::unique_ptr<AlgorithmParameters> paramsCopy(m_algorithmParams->clone());
    m_imageProcessor->setParameters(std::move(paramsCopy));
    processAndDisplay();
}
