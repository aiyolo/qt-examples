#include "brightnesscontrastalgorithm.h"
#include <opencv2/imgproc.hpp>

BrightnessContrastAlgorithm::BrightnessContrastAlgorithm()
    : m_brightness(0)
    , m_contrast(1.0)
{
}

cv::Mat BrightnessContrastAlgorithm::process(const cv::Mat& image, AlgorithmParams* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat dst;
    int brightness = params ? params->brightness() : m_brightness;
    double contrast = params ? params->contrast() : m_contrast;

    image.convertTo(dst, -1, contrast, brightness);
    return dst;
}

QString BrightnessContrastAlgorithm::name() const
{
    return "亮度对比度";
}

QWidget* BrightnessContrastAlgorithm::createParameterWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QFormLayout* layout = new QFormLayout(widget);

    QSpinBox* brightnessSpin = new QSpinBox(widget);
    brightnessSpin->setRange(-100, 100);
    brightnessSpin->setValue(m_brightness);

    QDoubleSpinBox* contrastSpin = new QDoubleSpinBox(widget);
    contrastSpin->setRange(0.1, 3.0);
    contrastSpin->setSingleStep(0.1);
    contrastSpin->setValue(m_contrast);

    layout->addRow("亮度:", brightnessSpin);
    layout->addRow("对比度:", contrastSpin);

    return widget;
}

void BrightnessContrastAlgorithm::setBrightness(int brightness)
{
    m_brightness = brightness;
}

void BrightnessContrastAlgorithm::setContrast(double contrast)
{
    m_contrast = qMax(0.1, contrast);
}
