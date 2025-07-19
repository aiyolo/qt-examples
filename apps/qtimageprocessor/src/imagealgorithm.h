#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>
#include "algorithmparameters.h"

class ImageAlgorithm
{
public:
    virtual ~ImageAlgorithm() = default;

    virtual cv::Mat process(const cv::Mat& image, AlgorithmParameters* params) = 0;
    virtual QString name() const = 0;

    virtual bool hasParameters() const { return false; }
    virtual QWidget* createParameterWidget(QWidget* parent = nullptr) { return nullptr; }

    virtual ImageAlgorithm* clone() const = 0;
    virtual AlgorithmParameters* createParameters() const = 0;

protected:
    cv::Mat qImageToCvMat(const QImage& image);
    QImage cvMatToQImage(const cv::Mat& mat);
};
