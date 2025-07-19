#pragma once

#include <QObject>

class AlgorithmParams : public QObject
{
    Q_OBJECT

public:
    enum Algorithm {
        Blur,
        EdgeDetection,
        BrightnessContrast,
        Grayscale
    };
    Q_ENUM(Algorithm)

    explicit AlgorithmParams(QObject *parent = nullptr);

    // Getter和Setter方法
    Algorithm algorithm() const;
    void setAlgorithm(Algorithm algorithm);

    int blurKernelSize() const;
    void setBlurKernelSize(int size);

    int edgeThreshold() const;
    void setEdgeThreshold(int threshold);

    int brightness() const;
    void setBrightness(int brightness);

    double contrast() const;
    void setContrast(double contrast);

signals:
    void parametersChanged();

private:
    Algorithm m_algorithm;
    int m_blurKernelSize;
    int m_edgeThreshold;
    int m_brightness;
    double m_contrast;
};
