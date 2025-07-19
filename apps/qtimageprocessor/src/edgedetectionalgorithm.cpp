#include "edgedetectionalgorithm.h"
#include "algorithmparameters.h"
#include <opencv2/imgproc.hpp>

EdgeDetectionAlgorithm::EdgeDetectionAlgorithm()
{
}

cv::Mat EdgeDetectionAlgorithm::process(const cv::Mat& image, AlgorithmParameters* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat gray, edges;

    // 转换为灰度图
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    int threshold = 100;
    if (params) {
        EdgeDetectionParameters* edgeParams = dynamic_cast<EdgeDetectionParameters*>(params);
        if (edgeParams) {
            threshold = edgeParams->threshold();
        }
    }

    // 使用Canny边缘检测
    cv::Canny(gray, edges, threshold, threshold * 2);

    // 转换回3通道以便显示
    cv::Mat result;
    cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);

    return result;
}

QString EdgeDetectionAlgorithm::name() const
{
    return "边缘检测";
}

QWidget* EdgeDetectionAlgorithm::createParameterWidget(QWidget* parent)
{
    EdgeDetectionParameters* params = new EdgeDetectionParameters();
    return params->createParameterWidget(parent);
}

ImageAlgorithm* EdgeDetectionAlgorithm::clone() const
{
    return new EdgeDetectionAlgorithm();
}

AlgorithmParameters* EdgeDetectionAlgorithm::createParameters() const
{
    return new EdgeDetectionParameters();
}
