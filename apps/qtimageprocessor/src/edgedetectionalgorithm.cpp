#include "edgedetectionalgorithm.h"
#include <opencv2/imgproc.hpp>

EdgeDetectionAlgorithm::EdgeDetectionAlgorithm()
    : m_threshold(100)
{
}

cv::Mat EdgeDetectionAlgorithm::process(const cv::Mat& image, AlgorithmParams* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat gray, edges;
    int threshold = params ? params->edgeThreshold() : m_threshold;

    // 转换为灰度图
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    // 边缘检测
    cv::Canny(gray, edges, threshold, threshold * 2);

    // 转回BGR格式
    cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);

    return edges;
}

QString EdgeDetectionAlgorithm::name() const
{
    return "边缘检测";
}

QWidget* EdgeDetectionAlgorithm::createParameterWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QFormLayout* layout = new QFormLayout(widget);

    QSpinBox* thresholdSpin = new QSpinBox(widget);
    thresholdSpin->setRange(0, 255);
    thresholdSpin->setValue(m_threshold);

    layout->addRow("阈值:", thresholdSpin);

    return widget;
}

void EdgeDetectionAlgorithm::setThreshold(int threshold)
{
    m_threshold = qBound(0, threshold, 255);
}
