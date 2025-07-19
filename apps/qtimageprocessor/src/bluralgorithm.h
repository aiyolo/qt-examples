#pragma once

#include "imagealgorithm.h"

class BlurAlgorithm : public ImageAlgorithm
{
public:
    BlurAlgorithm();
    ~BlurAlgorithm() override = default;

    cv::Mat process(const cv::Mat& image, AlgorithmParameters* params) override;
    QString name() const override;
    bool hasParameters() const override { return true; }
    QWidget* createParameterWidget(QWidget* parent = nullptr) override;
    ImageAlgorithm* clone() const override;
    AlgorithmParameters* createParameters() const override;
};
