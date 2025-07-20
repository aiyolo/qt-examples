#pragma once

#include "IImageProcessor.h"
#include "IParams.h"
#include <QString>
#include <QStringList>
#include <memory>

enum class ProcessorType {
    Blur,
    EdgeDetection,
    BrightnessContrast,
    Grayscale
};

// 处理器工厂接口 - 遵循依赖倒置原则
class IProcessorFactory
{
public:
    virtual ~IProcessorFactory() = default;
    virtual std::unique_ptr<IImageProcessor> createProcessor(ProcessorType type) const = 0;
    virtual std::unique_ptr<IParams> createParameterProvider(ProcessorType type) const = 0;
    virtual QStringList getAvailableProcessors() const = 0;
    virtual ProcessorType getProcessorType(const QString& name) const = 0;
};
