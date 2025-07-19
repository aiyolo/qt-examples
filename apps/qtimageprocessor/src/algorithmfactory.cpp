#include "algorithmfactory.h"
#include "bluralgorithm.h"
#include "edgedetectionalgorithm.h"
#include "brightnesscontrastalgorithm.h"
#include "grayscalealgorithm.h"

AlgorithmFactory& AlgorithmFactory::instance()
{
    static AlgorithmFactory instance;
    return instance;
}

AlgorithmFactory::AlgorithmFactory()
{
    // 注册所有算法
    registerAlgorithm(new BlurAlgorithm());
    registerAlgorithm(new EdgeDetectionAlgorithm());
    registerAlgorithm(new BrightnessContrastAlgorithm());
    registerAlgorithm(new GrayscaleAlgorithm());
}

AlgorithmFactory::~AlgorithmFactory()
{
    // 清理所有算法
    for (auto it = m_algorithms.begin(); it != m_algorithms.end(); ++it) {
        delete it.value();
    }
    m_algorithms.clear();
    m_nameToType.clear();
}

void AlgorithmFactory::registerAlgorithm(ImageAlgorithm* algorithm)
{
    if (!algorithm) return;

    // 根据算法名称确定类型
    QString name = algorithm->name();
    AlgorithmParams::Algorithm type;

    if (name == "模糊") {
        type = AlgorithmParams::Blur;
    } else if (name == "边缘检测") {
        type = AlgorithmParams::EdgeDetection;
    } else if (name == "亮度对比度") {
        type = AlgorithmParams::BrightnessContrast;
    } else if (name == "灰度化") {
        type = AlgorithmParams::Grayscale;
    } else {
        return;
    }

    m_algorithms[type] = algorithm;
    m_nameToType[name] = type;
}

ImageAlgorithm* AlgorithmFactory::createAlgorithm(AlgorithmParams::Algorithm type)
{
    if (m_algorithms.contains(type)) {
        // 返回算法的副本
        if (type == AlgorithmParams::Blur) {
            return new BlurAlgorithm();
        } else if (type == AlgorithmParams::EdgeDetection) {
            return new EdgeDetectionAlgorithm();
        } else if (type == AlgorithmParams::BrightnessContrast) {
            return new BrightnessContrastAlgorithm();
        } else if (type == AlgorithmParams::Grayscale) {
            return new GrayscaleAlgorithm();
        }
    }
    return nullptr;
}

QStringList AlgorithmFactory::algorithmNames() const
{
    return m_nameToType.keys();
}

AlgorithmParams::Algorithm AlgorithmFactory::algorithmType(const QString& name) const
{
    return m_nameToType.value(name, AlgorithmParams::Blur);
}
