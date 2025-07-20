#include "ProcessorFactory.h"
#include "../processors/BlurProcessor.h"
#include "../processors/EdgeDetectionProcessor.h"
#include "../processors/BrightnessContrastProcessor.h"
#include "../processors/GrayscaleProcessor.h"
#include "../parameters/BlurParams.h"
#include "../parameters/EdgeDetectionParams.h"
#include "../parameters/BrightnessContrastParams.h"
#include "../parameters/GrayscaleParams.h"
#include <memory>

ProcessorFactory& ProcessorFactory::getInstance()
{
    static ProcessorFactory instance;
    return instance;
}

ProcessorFactory::ProcessorFactory()
{
    initializeProcessorMap();
}

void ProcessorFactory::initializeProcessorMap()
{
    m_processorNames[ProcessorType::Blur] = "模糊";
    m_processorNames[ProcessorType::EdgeDetection] = "边缘检测";
    m_processorNames[ProcessorType::BrightnessContrast] = "亮度对比度";
    m_processorNames[ProcessorType::Grayscale] = "灰度化";

    // 反向映射
    for (auto it = m_processorNames.begin(); it != m_processorNames.end(); ++it) {
        m_nameToType[it.value()] = it.key();
    }
}

std::unique_ptr<IImageProcessor> ProcessorFactory::createProcessor(ProcessorType type) const
{
    switch (type) {
        case ProcessorType::Blur:
            return std::make_unique<BlurProcessor>();
        case ProcessorType::EdgeDetection:
            return std::make_unique<EdgeDetectionProcessor>();
        case ProcessorType::BrightnessContrast:
            return std::make_unique<BrightnessContrastProcessor>();
        case ProcessorType::Grayscale:
            return std::make_unique<GrayscaleProcessor>();
        default:
            return nullptr;
    }
}

std::unique_ptr<IParams> ProcessorFactory::createParameterProvider(ProcessorType type) const
{
    switch (type) {
        case ProcessorType::Blur:
return std::make_unique<BlurParams>();
        case ProcessorType::EdgeDetection:
return std::make_unique<EdgeDetectionParams>();
        case ProcessorType::BrightnessContrast:
return std::make_unique<BrightnessContrastParams>();
        case ProcessorType::Grayscale:
return std::make_unique<GrayscaleParams>();
        default:
            return nullptr;
    }
}

QStringList ProcessorFactory::getAvailableProcessors() const
{
    QStringList names;
    for (auto it = m_processorNames.begin(); it != m_processorNames.end(); ++it) {
        names << it.value();
    }
    return names;
}

ProcessorType ProcessorFactory::getProcessorType(const QString& name) const
{
    return m_nameToType.value(name, ProcessorType::Blur);
}
