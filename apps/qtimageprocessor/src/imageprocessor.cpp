#include "imageprocessor.h"
#include "algorithmfactory.h"
#include "algorithmparameters.h"
#include <opencv2/imgproc.hpp>
#include <memory>

ImageProcessor::ImageProcessor(std::unique_ptr<AlgorithmParameters> params)
    : m_parameters(std::move(params)) {}

ImageProcessor::~ImageProcessor() = default;

QImage ImageProcessor::processImage(const QImage& image)
{
    m_lastError.clear();

    if (image.isNull() || !m_parameters) {
        return QImage();
    }

    cv::Mat src = qImageToCvMat(image);
    if (src.empty()) {
        return QImage();
    }

    try {
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        AlgorithmParams::Algorithm algorithmType = m_parameters->algorithmType();

        std::unique_ptr<ImageAlgorithm> algorithm(factory.createAlgorithm(algorithmType));
        if (!algorithm) {
            return QImage();
        }

        cv::Mat dst = algorithm->process(src, m_parameters.get());
        if (dst.empty()) {
            return QImage();
        }

        return cvMatToQImage(dst);

    } catch (const std::exception& e) {
        m_lastError = e.what();
        return QImage();
    }
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

void ImageProcessor::setParameters(std::unique_ptr<AlgorithmParameters> params)
{
    m_parameters = std::move(params);
    m_lastError.clear();
}

QString ImageProcessor::lastError() const
{
    return m_lastError;
}
