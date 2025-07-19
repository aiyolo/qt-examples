#pragma once

#include <QWidget>
#include <QObject>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSlider>
#include <QVariantMap>
#include "algorithmparams.h"

class AlgorithmParameters : public QObject
{
    Q_OBJECT

public:
    explicit AlgorithmParameters(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~AlgorithmParameters() = default;

    // Algorithm type identification
    virtual AlgorithmParams::Algorithm algorithmType() const = 0;

    // Widget creation
    virtual QWidget* createParameterWidget(QWidget* parent = nullptr) = 0;
    virtual void resetToDefaults() = 0;
    virtual AlgorithmParameters* clone() const = 0;

    // Serialization methods for parameter consistency
    virtual QVariantMap toVariantMap() const = 0;
    virtual void fromVariantMap(const QVariantMap& map) = 0;

signals:
    void parametersChanged();
};

// 模糊算法参数
class BlurParameters : public AlgorithmParameters
{
    Q_OBJECT

public:
    explicit BlurParameters(QObject* parent = nullptr);

    // Algorithm type identification
    AlgorithmParams::Algorithm algorithmType() const override;

    int kernelSize() const;
    void setKernelSize(int size);

    QWidget* createParameterWidget(QWidget* parent = nullptr) override;
    void resetToDefaults() override;
    BlurParameters* clone() const override;

    // Serialization methods
    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

private:
    int m_kernelSize;
};

// 边缘检测算法参数
class EdgeDetectionParameters : public AlgorithmParameters
{
    Q_OBJECT

public:
    explicit EdgeDetectionParameters(QObject* parent = nullptr);

    // Algorithm type identification
    AlgorithmParams::Algorithm algorithmType() const override;

    int threshold() const;
    void setThreshold(int threshold);

    QWidget* createParameterWidget(QWidget* parent = nullptr) override;
    void resetToDefaults() override;
    EdgeDetectionParameters* clone() const override;

    // Serialization methods
    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

private:
    int m_threshold;
};

// 亮度对比度算法参数
class BrightnessContrastParameters : public AlgorithmParameters
{
    Q_OBJECT

public:
    explicit BrightnessContrastParameters(QObject* parent = nullptr);

    // Algorithm type identification
    AlgorithmParams::Algorithm algorithmType() const override;

    int brightness() const;
    void setBrightness(int brightness);

    double contrast() const;
    void setContrast(double contrast);

    QWidget* createParameterWidget(QWidget* parent = nullptr) override;
    void resetToDefaults() override;
    BrightnessContrastParameters* clone() const override;

    // Serialization methods
    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

private:
    int m_brightness;
    double m_contrast;
};

// 灰度化算法参数
class GrayscaleParameters : public AlgorithmParameters
{
    Q_OBJECT

public:
    explicit GrayscaleParameters(QObject* parent = nullptr);

    // Algorithm type identification
    AlgorithmParams::Algorithm algorithmType() const override;

    QWidget* createParameterWidget(QWidget* parent = nullptr) override;
    void resetToDefaults() override;
    GrayscaleParameters* clone() const override;

    // Serialization methods
    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;
};
