#pragma once

#include "../interfaces/IImageProcessor.h"
#include "../interfaces/IParams.h"

// 边缘检测处理器 - 遵循单一职责原则
class EdgeDetectionProcessor : public IImageProcessor
{
public:
    explicit EdgeDetectionProcessor(int threshold = 100);

    cv::Mat process(const cv::Mat& input) const override;
    QString getName() const override;
    void setParams(const IParams* params) override;

    void setThreshold(int threshold);
    int getThreshold() const;

private:
    int m_threshold;
};
