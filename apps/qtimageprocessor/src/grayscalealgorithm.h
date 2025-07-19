#pragma once

#include "imagealgorithm.h"

class GrayscaleAlgorithm : public ImageAlgorithm
{
public:
    GrayscaleAlgorithm();
    ~GrayscaleAlgorithm() override = default;

    cv::Mat process(const cv::Mat& image, AlgorithmParameters* params) override;
    QString name() const override;
    bool hasParameters() const override { return false; }
    QWidget* createParameterWidget(QWidget* parent = nullptr) override;
    ImageAlgorithm* clone() const override;
    AlgorithmParameters* createParameters() const override;
};
