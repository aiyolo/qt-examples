#include "BlurProcessor.h"
#include "../parameters/BlurParams.h"
#include <opencv2/imgproc.hpp>

BlurProcessor::BlurProcessor(int kernelSize)
    : m_kernelSize(kernelSize)
{
}

cv::Mat BlurProcessor::process(const cv::Mat& input) const
{
    if (input.empty()) {
        return cv::Mat();
    }

    cv::Mat result;
    cv::GaussianBlur(input, result, cv::Size(m_kernelSize, m_kernelSize), 0);
    return result;
}

QString BlurProcessor::getName() const
{
    return "模糊";
}

void BlurProcessor::setParams(const IParams* params)
{
    if (const auto* blurParams = dynamic_cast<const BlurParams*>(params)) {
        m_kernelSize = blurParams->getKernelSize();
    }
}

void BlurProcessor::setKernelSize(int size)
{
    m_kernelSize = size;
}

int BlurProcessor::getKernelSize() const
{
    return m_kernelSize;
}
