#pragma once

#include "../interfaces/IImageProcessor.h"
#include "../interfaces/IParams.h"

// 模糊处理器 - 遵循单一职责原则
class BlurProcessor : public IImageProcessor
{
public:
    explicit BlurProcessor(int kernelSize = 5);

    cv::Mat process(const cv::Mat& input) const override;
    QString getName() const override;
    void setParams(const IParams* params) override;

    void setKernelSize(int size);
    int getKernelSize() const;

private:
    int m_kernelSize;
};
