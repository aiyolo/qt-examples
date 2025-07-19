#include "algorithmparams.h"

AlgorithmParams::AlgorithmParams(QObject *parent)
    : QObject(parent)
    , m_algorithm(Blur)
    , m_blurKernelSize(5)
    , m_edgeThreshold(100)
    , m_brightness(0)
    , m_contrast(1.0)
{
}

AlgorithmParams::Algorithm AlgorithmParams::algorithm() const
{
    return m_algorithm;
}

void AlgorithmParams::setAlgorithm(Algorithm algorithm)
{
    if (m_algorithm != algorithm) {
        m_algorithm = algorithm;
        emit parametersChanged();
    }
}

int AlgorithmParams::blurKernelSize() const
{
    return m_blurKernelSize;
}

void AlgorithmParams::setBlurKernelSize(int size)
{
    if (m_blurKernelSize != size) {
        m_blurKernelSize = size;
        emit parametersChanged();
    }
}

int AlgorithmParams::edgeThreshold() const
{
    return m_edgeThreshold;
}

void AlgorithmParams::setEdgeThreshold(int threshold)
{
    if (m_edgeThreshold != threshold) {
        m_edgeThreshold = threshold;
        emit parametersChanged();
    }
}

int AlgorithmParams::brightness() const
{
    return m_brightness;
}

void AlgorithmParams::setBrightness(int brightness)
{
    if (m_brightness != brightness) {
        m_brightness = brightness;
        emit parametersChanged();
    }
}

double AlgorithmParams::contrast() const
{
    return m_contrast;
}

void AlgorithmParams::setContrast(double contrast)
{
    if (m_contrast != contrast) {
        m_contrast = contrast;
        emit parametersChanged();
    }
}
