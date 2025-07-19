#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>

class AlgorithmParams;

class ImageAlgorithm
{
public:
    virtual ~ImageAlgorithm() = default;

    virtual cv::Mat process(const cv::Mat& image, AlgorithmParams* params) = 0;
    virtual QString name() const = 0;

    virtual bool hasParameters() const { return false; }
    virtual QWidget* createParameterWidget(QWidget* parent = nullptr) { return nullptr; }

protected:
    cv::Mat qImageToCvMat(const QImage& image);
    QImage cvMatToQImage(const cv::Mat& mat);
};
