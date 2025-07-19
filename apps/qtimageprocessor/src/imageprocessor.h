#pragma once

#include <memory>
#include <QImage>
#include "algorithmparameters.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class ImageProcessor
{
public:
    explicit ImageProcessor(std::unique_ptr<AlgorithmParameters> params = nullptr);
    ~ImageProcessor();

    QImage processImage(const QImage& image);
    void setParameters(std::unique_ptr<AlgorithmParameters> params);
    QString lastError() const;

private:
    cv::Mat qImageToCvMat(const QImage& image);
    QImage cvMatToQImage(const cv::Mat& mat);

    std::unique_ptr<AlgorithmParameters> m_parameters;
    QString m_lastError;
};
