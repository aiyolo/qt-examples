#include "imageprocessor.h"
#include <opencv2/imgproc.hpp>

ImageProcessor::ImageProcessor()
    : m_algorithm(Blur)
    , m_blurKernelSize(5)
    , m_edgeThreshold(100)
    , m_brightness(0)
    , m_contrast(1.0)
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

    switch (m_algorithm) {
    case Blur:
        cv::GaussianBlur(src, dst, cv::Size(m_blurKernelSize, m_blurKernelSize), 0);
        break;

    case EdgeDetection:
        if (src.channels() == 3) {
            cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
        }
        cv::Canny(src, dst, m_edgeThreshold, m_edgeThreshold * 2);
        cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
        break;

    case BrightnessContrast:
        dst = src.clone();
        dst.convertTo(dst, -1, m_contrast, m_brightness);
        break;

    case Grayscale:
        if (src.channels() == 3) {
            cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
            cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
        } else {
            dst = src.clone();
        }
        break;
    }

    return cvMatToQImage(dst);
}

void ImageProcessor::setAlgorithm(Algorithm algorithm)
{
    m_algorithm = algorithm;
}

ImageProcessor::Algorithm ImageProcessor::getAlgorithm() const
{
    return m_algorithm;
}

void ImageProcessor::setBlurKernelSize(int size)
{
    m_blurKernelSize = size;
    if (m_blurKernelSize % 2 == 0) {
        m_blurKernelSize++;  // 确保是奇数
    }
}

int ImageProcessor::getBlurKernelSize() const
{
    return m_blurKernelSize;
}

void ImageProcessor::setEdgeThreshold(int threshold)
{
    m_edgeThreshold = threshold;
}

int ImageProcessor::getEdgeThreshold() const
{
    return m_edgeThreshold;
}

void ImageProcessor::setBrightness(int brightness)
{
    m_brightness = brightness;
}

int ImageProcessor::getBrightness() const
{
    return m_brightness;
}

void ImageProcessor::setContrast(double contrast)
{
    m_contrast = contrast;
}

double ImageProcessor::getContrast() const
{
    return m_contrast;
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
