#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>

class AlgorithmParams;

class ImageProcessor
{
public:
    explicit ImageProcessor(AlgorithmParams* params = nullptr);

    QImage processImage(const QImage& image);

private:
    cv::Mat qImageToCvMat(const QImage& image);
    QImage cvMatToQImage(const cv::Mat& mat);

    AlgorithmParams* m_params;
};
