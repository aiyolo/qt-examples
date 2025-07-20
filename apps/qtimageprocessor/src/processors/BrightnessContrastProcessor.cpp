#include "BrightnessContrastProcessor.h"
#include "../parameters/BrightnessContrastParams.h"

BrightnessContrastProcessor::BrightnessContrastProcessor(int brightness, double contrast)
    : m_brightness(brightness)
    , m_contrast(contrast)
{
}

cv::Mat BrightnessContrastProcessor::process(const cv::Mat& input) const
{
    if (input.empty()) {
        return cv::Mat();
    }

    cv::Mat result;
    input.convertTo(result, -1, m_contrast, m_brightness);
    return result;
}

QString BrightnessContrastProcessor::getName() const
{
    return "亮度对比度";
}

void BrightnessContrastProcessor::setParams(const IParams* params)
{
    if (const auto* bcParams = dynamic_cast<const BrightnessContrastParams*>(params)) {
        m_brightness = bcParams->getBrightness();
        m_contrast = bcParams->getContrast() / 100.0; // 转换为0-2范围
    }
}

void BrightnessContrastProcessor::setBrightness(int brightness)
{
    m_brightness = brightness;
}

int BrightnessContrastProcessor::getBrightness() const
{
    return m_brightness;
}

void BrightnessContrastProcessor::setContrast(double contrast)
{
    m_contrast = contrast;
}

double BrightnessContrastProcessor::getContrast() const
{
    return m_contrast;
}
