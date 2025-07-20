#include "GrayscaleProcessor.h"
#include "../parameters/GrayscaleParams.h"
#include <opencv2/imgproc.hpp>

cv::Mat GrayscaleProcessor::process(const cv::Mat& input) const
{
    if (input.empty()) {
        return cv::Mat();
    }

    cv::Mat result;
    if (input.channels() == 3) {
        cv::cvtColor(input, result, cv::COLOR_BGR2GRAY);
    } else {
        result = input.clone();
    }

    return result;
}

QString GrayscaleProcessor::getName() const
{
    return "灰度化";
}

void GrayscaleProcessor::setParams(const IParams* params)
{
    // 灰度化处理器不需要参数，但实现接口
    Q_UNUSED(params);
}
