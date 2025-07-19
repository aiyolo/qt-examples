#include "algorithmparameters.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QtGlobal>
#include <stdexcept>

// BlurParameters implementation
BlurParameters::BlurParameters(QObject* parent)
  : AlgorithmParameters(parent)
  , m_kernelSize(5)
{}

int BlurParameters::kernelSize() const
{
    return m_kernelSize;
}

void BlurParameters::setKernelSize(int size)
{
    if (m_kernelSize != size) {
        m_kernelSize = size;
        emit parametersChanged();
    }
}

QWidget* BlurParameters::createParameterWidget(QWidget* parent)
{
    try {
        QWidget* widget = new QWidget(parent);
        if (!widget) {
            throw std::runtime_error("Failed to create widget container");
        }

        QFormLayout* layout = new QFormLayout(widget);
        if (!layout) {
            delete widget;
            throw std::runtime_error("Failed to create form layout");
        }

        QSpinBox* kernelSpin = new QSpinBox(widget);
        if (!kernelSpin) {
            delete widget;
            throw std::runtime_error("Failed to create kernel size spinbox");
        }

        // Validate current kernel size before setting
        int safeKernelSize = qBound(1, m_kernelSize, 21);
        if (safeKernelSize != m_kernelSize) {
            m_kernelSize = safeKernelSize;
        }

        kernelSpin->setRange(1, 21);
        kernelSpin->setSingleStep(2);
        kernelSpin->setValue(m_kernelSize);
        kernelSpin->setToolTip("设置模糊核的大小 (1-21)");

        // Test signal connection
        bool connected = connect(kernelSpin,
                                 QOverload<int>::of(&QSpinBox::valueChanged),
                                 this,
                                 &BlurParameters::setKernelSize);

        if (!connected) {
            delete widget;
            throw std::runtime_error("Failed to connect kernel size signal");
        }

        layout->addRow("核大小:", kernelSpin);

        return widget;

    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("BlurParameters widget creation failed: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "BlurParameters widget creation failed with unknown error");
    }
}

void BlurParameters::resetToDefaults()
{
    try {
        setKernelSize(5);
    } catch (const std::exception& e) {
        throw std::runtime_error(QString("Failed to reset BlurParameters: %1")
                                   .arg(e.what())
                                   .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to reset BlurParameters with unknown error");
    }
}

BlurParameters* BlurParameters::clone() const
{
    try {
        BlurParameters* clone = new BlurParameters();
        if (!clone) {
            throw std::runtime_error("Failed to allocate BlurParameters clone");
        }
        clone->setKernelSize(m_kernelSize);
        return clone;
    } catch (const std::exception& e) {
        throw std::runtime_error(QString("Failed to clone BlurParameters: %1")
                                   .arg(e.what())
                                   .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to clone BlurParameters with unknown error");
    }
}

AlgorithmParams::Algorithm BlurParameters::algorithmType() const
{
    return AlgorithmParams::Blur;
}

QVariantMap BlurParameters::toVariantMap() const
{
    QVariantMap map;
    map["kernelSize"] = m_kernelSize;
    return map;
}

void BlurParameters::fromVariantMap(const QVariantMap& map)
{
    if (map.contains("kernelSize")) {
        setKernelSize(map["kernelSize"].toInt());
    }
}

// EdgeDetectionParameters implementation
EdgeDetectionParameters::EdgeDetectionParameters(QObject* parent)
  : AlgorithmParameters(parent)
  , m_threshold(100)
{}

int EdgeDetectionParameters::threshold() const
{
    return m_threshold;
}

void EdgeDetectionParameters::setThreshold(int threshold)
{
    if (m_threshold != threshold) {
        m_threshold = threshold;
        emit parametersChanged();
    }
}

QWidget* EdgeDetectionParameters::createParameterWidget(QWidget* parent)
{
    try {
        QWidget* widget = new QWidget(parent);
        if (!widget) {
            throw std::runtime_error("Failed to create widget container");
        }

        QFormLayout* layout = new QFormLayout(widget);
        if (!layout) {
            delete widget;
            throw std::runtime_error("Failed to create form layout");
        }

        QSlider* thresholdSlider = new QSlider(Qt::Horizontal, widget);
        if (!thresholdSlider) {
            delete widget;
            throw std::runtime_error("Failed to create threshold slider");
        }

        // Validate current threshold before setting
        int safeThreshold = qBound(0, m_threshold, 255);
        if (safeThreshold != m_threshold) {
            m_threshold = safeThreshold;
        }

        thresholdSlider->setRange(0, 255);
        thresholdSlider->setValue(m_threshold);
        thresholdSlider->setToolTip("设置边缘检测阈值 (0-255)");

        // Add a label to show current value
        QLabel* valueLabel = new QLabel(QString::number(m_threshold), widget);
        if (!valueLabel) {
            delete widget;
            throw std::runtime_error("Failed to create value label");
        }

        // Test signal connections
        bool connected1 = connect(thresholdSlider,
                                  &QSlider::valueChanged,
                                  this,
                                  &EdgeDetectionParameters::setThreshold);

        bool connected2 =
          connect(thresholdSlider,
                  &QSlider::valueChanged,
                  valueLabel,
                  [valueLabel](int value) {
                      valueLabel->setText(QString::number(value));
                  });

        if (!connected1 || !connected2) {
            delete widget;
            throw std::runtime_error("Failed to connect threshold signals");
        }

        QHBoxLayout* sliderLayout = new QHBoxLayout();
        sliderLayout->addWidget(thresholdSlider);
        sliderLayout->addWidget(valueLabel);

        QWidget* sliderContainer = new QWidget(widget);
        sliderContainer->setLayout(sliderLayout);

        layout->addRow("阈值:", sliderContainer);

        return widget;

    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("EdgeDetectionParameters widget creation failed: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "EdgeDetectionParameters widget creation failed with unknown error");
    }
}

void EdgeDetectionParameters::resetToDefaults()
{
    try {
        setThreshold(100);
    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("Failed to reset EdgeDetectionParameters: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to reset EdgeDetectionParameters with unknown error");
    }
}

EdgeDetectionParameters* EdgeDetectionParameters::clone() const
{
    try {
        EdgeDetectionParameters* clone = new EdgeDetectionParameters();
        if (!clone) {
            throw std::runtime_error(
              "Failed to allocate EdgeDetectionParameters clone");
        }
        clone->setThreshold(m_threshold);
        return clone;
    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("Failed to clone EdgeDetectionParameters: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to clone EdgeDetectionParameters with unknown error");
    }
}

AlgorithmParams::Algorithm EdgeDetectionParameters::algorithmType() const
{
    return AlgorithmParams::EdgeDetection;
}

QVariantMap EdgeDetectionParameters::toVariantMap() const
{
    QVariantMap map;
    map["threshold"] = m_threshold;
    return map;
}

void EdgeDetectionParameters::fromVariantMap(const QVariantMap& map)
{
    if (map.contains("threshold")) {
        setThreshold(map["threshold"].toInt());
    }
}

// BrightnessContrastParameters implementation
BrightnessContrastParameters::BrightnessContrastParameters(QObject* parent)
  : AlgorithmParameters(parent)
  , m_brightness(0)
  , m_contrast(1.0)
{}

int BrightnessContrastParameters::brightness() const
{
    return m_brightness;
}

void BrightnessContrastParameters::setBrightness(int brightness)
{
    if (m_brightness != brightness) {
        m_brightness = brightness;
        emit parametersChanged();
    }
}

double BrightnessContrastParameters::contrast() const
{
    return m_contrast;
}

void BrightnessContrastParameters::setContrast(double contrast)
{
    if (m_contrast != contrast) {
        m_contrast = contrast;
        emit parametersChanged();
    }
}

QWidget* BrightnessContrastParameters::createParameterWidget(QWidget* parent)
{
    try {
        QWidget* widget = new QWidget(parent);
        if (!widget) {
            throw std::runtime_error("Failed to create widget container");
        }

        QFormLayout* layout = new QFormLayout(widget);
        if (!layout) {
            delete widget;
            throw std::runtime_error("Failed to create form layout");
        }

        QSpinBox* brightnessSpin = new QSpinBox(widget);
        if (!brightnessSpin) {
            delete widget;
            throw std::runtime_error("Failed to create brightness spinbox");
        }

        QDoubleSpinBox* contrastSpin = new QDoubleSpinBox(widget);
        if (!contrastSpin) {
            delete widget;
            throw std::runtime_error("Failed to create contrast spinbox");
        }

        // Validate current values before setting
        int safeBrightness = qBound(-100, m_brightness, 100);
        if (safeBrightness != m_brightness) {
            m_brightness = safeBrightness;
        }

        double safeContrast = qBound(0.0, m_contrast, 3.0);
        if (safeContrast != m_contrast) {
            m_contrast = safeContrast;
        }

        brightnessSpin->setRange(-100, 100);
        brightnessSpin->setValue(m_brightness);
        brightnessSpin->setToolTip("调整图像亮度 (-100 到 100)");

        contrastSpin->setRange(0.0, 3.0);
        contrastSpin->setSingleStep(0.1);
        contrastSpin->setDecimals(1);
        contrastSpin->setValue(m_contrast);
        contrastSpin->setToolTip("调整图像对比度 (0.0 到 3.0)");

        // Test signal connections
        bool connected1 = connect(brightnessSpin,
                                  QOverload<int>::of(&QSpinBox::valueChanged),
                                  this,
                                  &BrightnessContrastParameters::setBrightness);

        bool connected2 =
          connect(contrastSpin,
                  QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                  this,
                  &BrightnessContrastParameters::setContrast);

        if (!connected1 || !connected2) {
            delete widget;
            throw std::runtime_error(
              "Failed to connect brightness/contrast signals");
        }

        layout->addRow("亮度:", brightnessSpin);
        layout->addRow("对比度:", contrastSpin);

        return widget;

    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("BrightnessContrastParameters widget creation failed: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error("BrightnessContrastParameters widget creation "
                                 "failed with unknown error");
    }
}

void BrightnessContrastParameters::resetToDefaults()
{
    try {
        setBrightness(0);
        setContrast(1.0);
    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("Failed to reset BrightnessContrastParameters: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to reset BrightnessContrastParameters with unknown error");
    }
}

BrightnessContrastParameters* BrightnessContrastParameters::clone() const
{
    try {
        BrightnessContrastParameters* clone =
          new BrightnessContrastParameters();
        if (!clone) {
            throw std::runtime_error(
              "Failed to allocate BrightnessContrastParameters clone");
        }
        clone->setBrightness(m_brightness);
        clone->setContrast(m_contrast);
        return clone;
    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("Failed to clone BrightnessContrastParameters: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to clone BrightnessContrastParameters with unknown error");
    }
}

AlgorithmParams::Algorithm BrightnessContrastParameters::algorithmType() const
{
    return AlgorithmParams::BrightnessContrast;
}

QVariantMap BrightnessContrastParameters::toVariantMap() const
{
    QVariantMap map;
    map["brightness"] = m_brightness;
    map["contrast"] = m_contrast;
    return map;
}

void BrightnessContrastParameters::fromVariantMap(const QVariantMap& map)
{
    if (map.contains("brightness")) {
        setBrightness(map["brightness"].toInt());
    }
    if (map.contains("contrast")) {
        setContrast(map["contrast"].toDouble());
    }
}

// GrayscaleParameters implementation
GrayscaleParameters::GrayscaleParameters(QObject* parent)
  : AlgorithmParameters(parent)
{}

QWidget* GrayscaleParameters::createParameterWidget(QWidget* parent)
{
    try {
        QWidget* widget = new QWidget(parent);
        if (!widget) {
            throw std::runtime_error("Failed to create widget container");
        }

        QFormLayout* layout = new QFormLayout(widget);
        if (!layout) {
            delete widget;
            throw std::runtime_error("Failed to create form layout");
        }

        QLabel* infoLabel = new QLabel("无参数", widget);
        if (!infoLabel) {
            delete widget;
            throw std::runtime_error("Failed to create info label");
        }

        infoLabel->setToolTip("灰度化算法不需要额外参数");

        layout->addRow("灰度化算法:", infoLabel);

        return widget;

    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("GrayscaleParameters widget creation failed: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "GrayscaleParameters widget creation failed with unknown error");
    }
}

void GrayscaleParameters::resetToDefaults()
{
    // 灰度化无参数，但仍然发出信号以保持一致性
    try {
        emit parametersChanged();
    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("Failed to reset GrayscaleParameters: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to reset GrayscaleParameters with unknown error");
    }
}

GrayscaleParameters* GrayscaleParameters::clone() const
{
    try {
        GrayscaleParameters* clone = new GrayscaleParameters();
        if (!clone) {
            throw std::runtime_error(
              "Failed to allocate GrayscaleParameters clone");
        }
        return clone;
    } catch (const std::exception& e) {
        throw std::runtime_error(
          QString("Failed to clone GrayscaleParameters: %1")
            .arg(e.what())
            .toStdString());
    } catch (...) {
        throw std::runtime_error(
          "Failed to clone GrayscaleParameters with unknown error");
    }
}

AlgorithmParams::Algorithm GrayscaleParameters::algorithmType() const
{
    return AlgorithmParams::Grayscale;
}

QVariantMap GrayscaleParameters::toVariantMap() const
{
    // Grayscale has no parameters, return empty map
    return QVariantMap();
}

void GrayscaleParameters::fromVariantMap(const QVariantMap& map)
{
    // Grayscale has no parameters, nothing to restore
    Q_UNUSED(map);
}
