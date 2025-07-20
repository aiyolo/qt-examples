#pragma once

#include "../interfaces/IImageProcessor.h"
#include "../interfaces/IParams.h"

// 亮度对比度处理器 - 遵循单一职责原则
class BrightnessContrastProcessor : public IImageProcessor
{
public:
    explicit BrightnessContrastProcessor(int brightness = 0, double contrast = 1.0);

    cv::Mat process(const cv::Mat& input) const override;
    QString getName() const override;
    void setParams(const IParams* params) override;

    void setBrightness(int brightness);
    int getBrightness() const;

    void setContrast(double contrast);
    double getContrast() const;

private:
    int m_brightness;
    double m_contrast;
};
