#include "bluralgorithm.h"
#include <opencv2/imgproc.hpp>

BlurAlgorithm::BlurAlgorithm()
    : m_kernelSize(5)
{
}

cv::Mat BlurAlgorithm::process(const cv::Mat& image, AlgorithmParams* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat dst;
    int kernelSize = params ? params->blurKernelSize() : m_kernelSize;

    // 确保核大小为奇数
    if (kernelSize % 2 == 0) {
        kernelSize++;
    }

    cv::GaussianBlur(image, dst, cv::Size(kernelSize, kernelSize), 0);
    return dst;
}

QString BlurAlgorithm::name() const
{
    return "模糊";
}

QWidget* BlurAlgorithm::createParameterWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QFormLayout* layout = new QFormLayout(widget);

    QSpinBox* kernelSpin = new QSpinBox(widget);
    kernelSpin->setRange(1, 21);
    kernelSpin->setSingleStep(2);
    kernelSpin->setValue(m_kernelSize);

    layout->addRow("核大小:", kernelSpin);

    return widget;
}

void BlurAlgorithm::setKernelSize(int size)
{
    m_kernelSize = qMax(1, size);
}
