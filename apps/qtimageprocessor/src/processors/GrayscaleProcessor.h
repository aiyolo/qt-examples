#pragma once

#include "../interfaces/IImageProcessor.h"
#include "../interfaces/IParams.h"

// 灰度化处理器 - 遵循单一职责原则
class GrayscaleProcessor : public IImageProcessor
{
public:
    GrayscaleProcessor() = default;

    cv::Mat process(const cv::Mat& input) const override;
    QString getName() const override;
    void setParams(const IParams* params) override;
};
