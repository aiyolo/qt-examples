#include "grayscalealgorithm.h"
#include "algorithmparameters.h"
#include <opencv2/imgproc.hpp>

GrayscaleAlgorithm::GrayscaleAlgorithm()
{
}

cv::Mat GrayscaleAlgorithm::process(const cv::Mat& image, AlgorithmParameters* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat gray;

    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);
    } else {
        gray = image.clone();
    }

    return gray;
}

QString GrayscaleAlgorithm::name() const
{
    return "灰度化";
}

QWidget* GrayscaleAlgorithm::createParameterWidget(QWidget* parent)
{
    GrayscaleParameters* params = new GrayscaleParameters();
    return params->createParameterWidget(parent);
}

ImageAlgorithm* GrayscaleAlgorithm::clone() const
{
    return new GrayscaleAlgorithm();
}

AlgorithmParameters* GrayscaleAlgorithm::createParameters() const
{
    return new GrayscaleParameters();
}
