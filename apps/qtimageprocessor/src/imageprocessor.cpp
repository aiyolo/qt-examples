#include "imageprocessor.h"
#include "algorithmparams.h"
#include "algorithmfactory.h"
#include <opencv2/imgproc.hpp>

ImageProcessor::ImageProcessor(AlgorithmParams *params)
    : m_params(params ? params : new AlgorithmParams())
{
}

QImage ImageProcessor::processImage(const QImage& image)
{
    if (image.isNull()) {
        return QImage();
    }

    cv::Mat src = qImageToCvMat(image);
    if (src.empty()) {
        return QImage();
    }

    cv::Mat dst;

    // 使用算法工厂创建对应的算法
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    ImageAlgorithm* algorithm = factory.createAlgorithm(m_params->algorithm());

    if (algorithm) {
        dst = algorithm->process(src, m_params);
        delete algorithm;
    }

    return cvMatToQImage(dst);
}

cv::Mat ImageProcessor::qImageToCvMat(const QImage& image)
{
    QImage converted = image.convertToFormat(QImage::Format_RGB888);
    return cv::Mat(converted.height(), converted.width(), CV_8UC3,
                   const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
}

QImage ImageProcessor::cvMatToQImage(const cv::Mat& mat)
{
    if (mat.empty()) {
        return QImage();
    }

    cv::Mat rgb;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    } else if (mat.channels() == 1) {
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    } else {
        rgb = mat.clone();
    }

    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step,
                  QImage::Format_RGB888).copy();
}
