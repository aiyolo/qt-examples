#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>

class IParams;

// 图像处理接口 - 遵循接口隔离原则
class IImageProcessor
{
public:
    virtual ~IImageProcessor() = default;
    virtual cv::Mat process(const cv::Mat& input) const = 0;
    virtual QString getName() const = 0;
    virtual void setParams(const IParams* params) = 0;
};
