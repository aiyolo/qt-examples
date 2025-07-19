#include "brightnesscontrastalgorithm.h"
#include "algorithmparameters.h"
#include <opencv2/imgproc.hpp>

BrightnessContrastAlgorithm::BrightnessContrastAlgorithm()
{
}

cv::Mat BrightnessContrastAlgorithm::process(const cv::Mat& image, AlgorithmParameters* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    int brightness = 0;
    double contrast = 1.0;

    if (params) {
        BrightnessContrastParameters* bcParams = dynamic_cast<BrightnessContrastParameters*>(params);
        if (bcParams) {
            brightness = bcParams->brightness();
            contrast = bcParams->contrast();
        }
    }

    cv::Mat result = image.clone();

    // 调整亮度和对比度
    result.convertTo(result, -1, contrast, brightness);

    return result;
}

QString BrightnessContrastAlgorithm::name() const
{
    return "亮度对比度";
}

QWidget* BrightnessContrastAlgorithm::createParameterWidget(QWidget* parent)
{
    BrightnessContrastParameters* params = new BrightnessContrastParameters();
    return params->createParameterWidget(parent);
}

ImageAlgorithm* BrightnessContrastAlgorithm::clone() const
{
    return new BrightnessContrastAlgorithm();
}

AlgorithmParameters* BrightnessContrastAlgorithm::createParameters() const
{
    return new BrightnessContrastParameters();
}
