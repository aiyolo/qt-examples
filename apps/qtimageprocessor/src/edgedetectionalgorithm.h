#pragma once

#include "imagealgorithm.h"
#include "algorithmparams.h"
#include <QSpinBox>
#include <QFormLayout>

class EdgeDetectionAlgorithm : public ImageAlgorithm
{
public:
    EdgeDetectionAlgorithm();
    cv::Mat process(const cv::Mat& image, AlgorithmParams* params) override;
    QString name() const override;

    bool hasParameters() const override { return true; }
    QWidget* createParameterWidget(QWidget* parent = nullptr) override;

    int threshold() const { return m_threshold; }
    void setThreshold(int threshold);

private:
    int m_threshold;
};
