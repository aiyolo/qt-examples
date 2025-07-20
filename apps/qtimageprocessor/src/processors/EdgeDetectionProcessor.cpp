#include "EdgeDetectionProcessor.h"
#include "../parameters/EdgeDetectionParams.h"
#include <opencv2/imgproc.hpp>

EdgeDetectionProcessor::EdgeDetectionProcessor(int threshold)
    : m_threshold(threshold)
{
}

cv::Mat EdgeDetectionProcessor::process(const cv::Mat& input) const
{
    if (input.empty()) {
        return cv::Mat();
    }

    cv::Mat gray, result;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }

    cv::Canny(gray, result, m_threshold, m_threshold * 2);
    return result;
}

QString EdgeDetectionProcessor::getName() const
{
    return "边缘检测";
}

void EdgeDetectionProcessor::setParams(const IParams* params)
{
    if (const auto* edgeParams = dynamic_cast<const EdgeDetectionParams*>(params)) {
        m_threshold = edgeParams->getThreshold();
    }
}

void EdgeDetectionProcessor::setThreshold(int threshold)
{
    m_threshold = threshold;
}

int EdgeDetectionProcessor::getThreshold() const
{
    return m_threshold;
}
