#pragma once

#include <QImage>
#include <memory>
#include <QString>
#include <QObject>
#include "../interfaces/IProcessorFactory.h"
#include "../interfaces/IParams.h"

// 图像处理服务 - 遵循单一职责原则
class ImageProcessingService : public QObject
{
    Q_OBJECT

public:
    explicit ImageProcessingService(std::unique_ptr<IProcessorFactory> factory, QObject* parent = nullptr);
    ~ImageProcessingService();

    QImage processImage(const QImage& image);
    void setProcessorType(ProcessorType type);
    QStringList getProcessorNames() const;
    ProcessorType getProcessorType(const QString& name) const;

    // 参数访问方法
    QVariant getParameter(const QString& name) const;
    void setParameter(const QString& name, const QVariant& value);
    IParams* getParameterProvider() const;
    QStringList getAvailableProcessors() const;

private:
    void updateProcessor();
    void updateParameterProvider();

    std::unique_ptr<IProcessorFactory> m_factory;
    std::unique_ptr<IImageProcessor> m_currentProcessor;
    std::unique_ptr<IParams> m_parameterProvider;
    ProcessorType m_currentType;
};
