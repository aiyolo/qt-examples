#pragma once

#include "imagealgorithm.h"
#include "algorithmparams.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>

class BrightnessContrastAlgorithm : public ImageAlgorithm
{
public:
    BrightnessContrastAlgorithm();
    cv::Mat process(const cv::Mat& image, AlgorithmParams* params) override;
    QString name() const override;

    bool hasParameters() const override { return true; }
    QWidget* createParameterWidget(QWidget* parent = nullptr) override;

    int brightness() const { return m_brightness; }
    void setBrightness(int brightness);

    double contrast() const { return m_contrast; }
    void setContrast(double contrast);

private:
    int m_brightness;
    double m_contrast;
};
