#include "grayscalealgorithm.h"
#include <opencv2/imgproc.hpp>

GrayscaleAlgorithm::GrayscaleAlgorithm()
{
}

cv::Mat GrayscaleAlgorithm::process(const cv::Mat& image, AlgorithmParams* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat gray, dst;

    // 转换为灰度图
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    // 转回BGR格式以便显示
    cv::cvtColor(gray, dst, cv::COLOR_GRAY2BGR);

    return dst;
}

QString GrayscaleAlgorithm::name() const
{
    return "灰度化";
}
