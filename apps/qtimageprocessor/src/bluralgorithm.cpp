#include "bluralgorithm.h"
#include "algorithmparameters.h"
#include <opencv2/imgproc.hpp>

BlurAlgorithm::BlurAlgorithm()
{
}

cv::Mat BlurAlgorithm::process(const cv::Mat& image, AlgorithmParameters* params)
{
    if (image.empty()) {
        return cv::Mat();
    }

    cv::Mat dst;
    int kernelSize = 5;

    if (params) {
        BlurParameters* blurParams = dynamic_cast<BlurParameters*>(params);
        if (blurParams) {
            kernelSize = blurParams->kernelSize();
        }
    }

    // 确保核大小为奇数
    if (kernelSize % 2 == 0) {
        kernelSize++;
    }

    cv::GaussianBlur(image, dst, cv::Size(kernelSize, kernelSize), 0);
    return dst;
}

QString BlurAlgorithm::name() const
{
    return "模糊";
}

QWidget* BlurAlgorithm::createParameterWidget(QWidget* parent)
{
    BlurParameters* params = new BlurParameters();
    return params->createParameterWidget(parent);
}

ImageAlgorithm* BlurAlgorithm::clone() const
{
    return new BlurAlgorithm();
}

AlgorithmParameters* BlurAlgorithm::createParameters() const
{
    return new BlurParameters();
}
