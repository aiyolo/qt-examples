#pragma once

#include "../interfaces/IProcessorFactory.h"
#include "../interfaces/IParams.h"
#include <QMap>

// 处理器工厂实现 - 遵循开闭原则和依赖倒置原则
class ProcessorFactory : public IProcessorFactory
{
public:
    static ProcessorFactory& getInstance();

    std::unique_ptr<IImageProcessor> createProcessor(ProcessorType type) const override;
    std::unique_ptr<IParams> createParameterProvider(ProcessorType type) const override;
    QStringList getAvailableProcessors() const override;
    ProcessorType getProcessorType(const QString& name) const override;

public:
    ProcessorFactory();
    void initializeProcessorMap();

    QMap<ProcessorType, QString> m_processorNames;
    QMap<QString, ProcessorType> m_nameToType;
};
