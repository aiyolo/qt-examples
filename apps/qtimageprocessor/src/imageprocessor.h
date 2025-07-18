#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>

class ImageProcessor
{
public:
    enum Algorithm {
        Blur,
        EdgeDetection,
        BrightnessContrast,
        Grayscale
    };

    ImageProcessor();

    QImage processImage(const QImage& image);

    void setAlgorithm(Algorithm algorithm);
    Algorithm getAlgorithm() const;

    void setBlurKernelSize(int size);
    int getBlurKernelSize() const;

    void setEdgeThreshold(int threshold);
    int getEdgeThreshold() const;

    void setBrightness(int brightness);
    int getBrightness() const;

    void setContrast(double contrast);
    double getContrast() const;

private:
    cv::Mat qImageToCvMat(const QImage& image);
    QImage cvMatToQImage(const cv::Mat& mat);

    Algorithm m_algorithm;
    int m_blurKernelSize;
    int m_edgeThreshold;
    int m_brightness;
    double m_contrast;
};
