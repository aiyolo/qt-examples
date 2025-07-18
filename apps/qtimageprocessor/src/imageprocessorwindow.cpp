#include "imageprocessorwindow.h"
#include "imageprocessor.h"
#include "simpleimageviewer/SimpleImageViewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QWidget>

ImageProcessorWindow::ImageProcessorWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_originalViewer(new SimpleImageViewer(this))
    , m_processedViewer(new SimpleImageViewer(this))
    , m_imageProcessor(new ImageProcessor())
    , m_originalImage()
    , m_algorithmCombo(new QComboBox(this))
    , m_blurSlider(new QSlider(Qt::Horizontal, this))
    , m_blurSpin(new QSpinBox(this))
    , m_edgeSlider(new QSlider(Qt::Horizontal, this))
    , m_edgeSpin(new QSpinBox(this))
    , m_brightnessSlider(new QSlider(Qt::Horizontal, this))
    , m_brightnessSpin(new QSpinBox(this))
    , m_contrastSlider(new QSlider(Qt::Horizontal, this))
    , m_contrastSpin(new QDoubleSpinBox(this))
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
    m_algorithmCombo->addItem("模糊", static_cast<int>(ImageProcessor::Blur));
    m_algorithmCombo->addItem("边缘检测", static_cast<int>(ImageProcessor::EdgeDetection));
    m_algorithmCombo->addItem("亮度/对比度", static_cast<int>(ImageProcessor::BrightnessContrast));
    m_algorithmCombo->addItem("灰度化", static_cast<int>(ImageProcessor::Grayscale));

    // 模糊参数
    QLabel* blurLabel = new QLabel("模糊核大小:", this);
    m_blurSlider->setRange(1, 21);
    m_blurSlider->setValue(5);
    m_blurSlider->setSingleStep(2);
    m_blurSlider->setTickPosition(QSlider::TicksBelow);
    m_blurSpin->setRange(1, 21);
    m_blurSpin->setValue(5);
    m_blurSpin->setSingleStep(2);

    // 边缘检测参数
    QLabel* edgeLabel = new QLabel("边缘阈值:", this);
    m_edgeSlider->setRange(0, 255);
    m_edgeSlider->setValue(100);
    m_edgeSlider->setTickPosition(QSlider::TicksBelow);
    m_edgeSpin->setRange(0, 255);
    m_edgeSpin->setValue(100);

    // 亮度参数
    QLabel* brightnessLabel = new QLabel("亮度:", this);
    m_brightnessSlider->setRange(-100, 100);
    m_brightnessSlider->setValue(0);
    m_brightnessSlider->setTickPosition(QSlider::TicksBelow);
    m_brightnessSpin->setRange(-100, 100);
    m_brightnessSpin->setValue(0);

    // 对比度参数
    QLabel* contrastLabel = new QLabel("对比度:", this);
    m_contrastSlider->setRange(10, 300);
    m_contrastSlider->setValue(100);
    m_contrastSlider->setTickPosition(QSlider::TicksBelow);
    m_contrastSpin->setRange(0.1, 3.0);
    m_contrastSpin->setValue(1.0);
    m_contrastSpin->setSingleStep(0.1);

    // 添加控件到布局
    controlLayout->addWidget(algorithmLabel);
    controlLayout->addWidget(m_algorithmCombo);

    controlLayout->addWidget(blurLabel);
    QHBoxLayout* blurLayout = new QHBoxLayout();
    blurLayout->addWidget(m_blurSlider);
    blurLayout->addWidget(m_blurSpin);
    controlLayout->addLayout(blurLayout);

    controlLayout->addWidget(edgeLabel);
    QHBoxLayout* edgeLayout = new QHBoxLayout();
    edgeLayout->addWidget(m_edgeSlider);
    edgeLayout->addWidget(m_edgeSpin);
    controlLayout->addLayout(edgeLayout);

    controlLayout->addWidget(brightnessLabel);
    QHBoxLayout* brightnessLayout = new QHBoxLayout();
    brightnessLayout->addWidget(m_brightnessSlider);
    brightnessLayout->addWidget(m_brightnessSpin);
    controlLayout->addLayout(brightnessLayout);

    controlLayout->addWidget(contrastLabel);
    QHBoxLayout* contrastLayout = new QHBoxLayout();
    contrastLayout->addWidget(m_contrastSlider);
    contrastLayout->addWidget(m_contrastSpin);
    controlLayout->addLayout(contrastLayout);

    controlLayout->addWidget(m_openButton);
    controlLayout->addWidget(m_saveButton);
    controlLayout->addWidget(m_resetButton);
    controlLayout->addStretch();

    mainLayout->addWidget(controlGroup, 1);

    // 直接连接信号槽，避免使用findChild
    connect(m_openButton, &QPushButton::clicked, this, &ImageProcessorWindow::openImage);
    connect(m_saveButton, &QPushButton::clicked, this, &ImageProcessorWindow::saveProcessedImage);
    connect(m_resetButton, &QPushButton::clicked, this, &ImageProcessorWindow::resetToOriginal);

    connect(m_algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImageProcessorWindow::onAlgorithmChanged);

    connect(m_blurSlider, &QSlider::valueChanged, m_blurSpin, &QSpinBox::setValue);
    connect(m_blurSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            m_blurSlider, &QSlider::setValue);
    connect(m_blurSlider, &QSlider::valueChanged,
            [this](int value) { m_imageProcessor->setBlurKernelSize(value); updateProcessedImage(); });

    connect(m_edgeSlider, &QSlider::valueChanged, m_edgeSpin, &QSpinBox::setValue);
    connect(m_edgeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            m_edgeSlider, &QSlider::setValue);
    connect(m_edgeSlider, &QSlider::valueChanged,
            [this](int value) { m_imageProcessor->setEdgeThreshold(value); updateProcessedImage(); });

    connect(m_brightnessSlider, &QSlider::valueChanged, m_brightnessSpin, &QSpinBox::setValue);
    connect(m_brightnessSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            m_brightnessSlider, &QSlider::setValue);
    connect(m_brightnessSlider, &QSlider::valueChanged,
            [this](int value) { m_imageProcessor->setBrightness(value); updateProcessedImage(); });

    connect(m_contrastSlider, &QSlider::valueChanged,
            [this](int value) { m_contrastSpin->setValue(value / 100.0); });
    connect(m_contrastSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double value) { m_contrastSlider->setValue(static_cast<int>(value * 100)); });
    connect(m_contrastSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double value) { m_imageProcessor->setContrast(value); updateProcessedImage(); });

    // 初始化
    updateParameterControls();
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
        m_blurSlider->setValue(5);
        m_edgeSlider->setValue(100);
        m_brightnessSlider->setValue(0);
        m_contrastSpin->setValue(1.0);
    }
}

void ImageProcessorWindow::updateProcessedImage()
{
    processAndDisplay();
}

void ImageProcessorWindow::onAlgorithmChanged(int index)
{
    if (index >= 0) {
        ImageProcessor::Algorithm algorithm =
            static_cast<ImageProcessor::Algorithm>(m_algorithmCombo->itemData(index).toInt());
        m_imageProcessor->setAlgorithm(algorithm);
        updateParameterControls();
        processAndDisplay();
    }
}

void ImageProcessorWindow::onBlurKernelChanged(int value)
{
    m_imageProcessor->setBlurKernelSize(value);
    processAndDisplay();
}

void ImageProcessorWindow::onEdgeThresholdChanged(int value)
{
    m_imageProcessor->setEdgeThreshold(value);
    processAndDisplay();
}

void ImageProcessorWindow::onBrightnessChanged(int value)
{
    m_imageProcessor->setBrightness(value);
    processAndDisplay();
}

void ImageProcessorWindow::onContrastChanged(double value)
{
    m_imageProcessor->setContrast(value);
    processAndDisplay();
}

void ImageProcessorWindow::updateParameterControls()
{
    ImageProcessor::Algorithm algorithm = m_imageProcessor->getAlgorithm();

    bool showBlur = (algorithm == ImageProcessor::Blur);
    bool showEdge = (algorithm == ImageProcessor::EdgeDetection);
    bool showBrightnessContrast = (algorithm == ImageProcessor::BrightnessContrast);

    m_blurSlider->setVisible(showBlur);
    m_blurSpin->setVisible(showBlur);

    m_edgeSlider->setVisible(showEdge);
    m_edgeSpin->setVisible(showEdge);

    m_brightnessSlider->setVisible(showBrightnessContrast);
    m_brightnessSpin->setVisible(showBrightnessContrast);

    m_contrastSlider->setVisible(showBrightnessContrast);
    m_contrastSpin->setVisible(showBrightnessContrast);
}

void ImageProcessorWindow::processAndDisplay()
{
    if (!m_originalImage.isNull()) {
        QImage processed = m_imageProcessor->processImage(m_originalImage);
        m_processedViewer->setImage(processed);
    }
}
