#pragma once

#include "imagealgorithm.h"
#include "algorithmparams.h"
#include <QSpinBox>
#include <QFormLayout>

class BlurAlgorithm : public ImageAlgorithm
{
public:
    BlurAlgorithm();
    cv::Mat process(const cv::Mat& image, AlgorithmParams* params) override;
    QString name() const override;

    bool hasParameters() const override { return true; }
    QWidget* createParameterWidget(QWidget* parent = nullptr) override;

    int kernelSize() const { return m_kernelSize; }
    void setKernelSize(int size);

private:
    int m_kernelSize;
};
