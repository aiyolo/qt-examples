#pragma once

#include "imagealgorithm.h"
#include "algorithmparams.h"

class GrayscaleAlgorithm : public ImageAlgorithm
{
public:
    GrayscaleAlgorithm();
    cv::Mat process(const cv::Mat& image, AlgorithmParams* params) override;
    QString name() const override;

    bool hasParameters() const override { return false; }
    QWidget* createParameterWidget(QWidget* parent = nullptr) override { return nullptr; }
};
