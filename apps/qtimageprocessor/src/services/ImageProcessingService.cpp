#include "ImageProcessingService.h"
#include "../interfaces/IImageProcessor.h"
#include "../interfaces/IParams.h"
#include "../parameters/BlurParams.h"
#include "../parameters/EdgeDetectionParams.h"
#include "../parameters/BrightnessContrastParams.h"
#include "../parameters/GrayscaleParams.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

// QImage 和 cv::Mat 之间的转换函数
static cv::Mat QImageToCvMat(const QImage& image)
{
    QImage converted = image.convertToFormat(QImage::Format_RGBA8888);
    return cv::Mat(converted.height(), converted.width(), CV_8UC4,
                   const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
}

static QImage CvMatToQImage(const cv::Mat& mat)
{
    if (mat.empty()) {
        return QImage();
    }

    QImage::Format format;
    if (mat.channels() == 1) {
        format = QImage::Format_Grayscale8;
    } else if (mat.channels() == 3) {
        format = QImage::Format_RGB888;
    } else if (mat.channels() == 4) {
        format = QImage::Format_RGBA8888;
    } else {
        return QImage();
    }

    QImage image(mat.data, mat.cols, mat.rows, mat.step, format);
    return image.copy();
}

ImageProcessingService::ImageProcessingService(std::unique_ptr<IProcessorFactory> factory, QObject* parent)
    : QObject(parent)
    , m_factory(std::move(factory))
    , m_currentType(ProcessorType::Grayscale)
{
    updateProcessor();
    updateParameterProvider();
}

ImageProcessingService::~ImageProcessingService() = default;

QImage ImageProcessingService::processImage(const QImage& image)
{
    if (!m_currentProcessor || image.isNull()) {
        return image;
    }

    // 转换 QImage 到 cv::Mat
    cv::Mat inputMat = QImageToCvMat(image);
    if (inputMat.empty()) {
        return image;
    }

    // 处理图像
    cv::Mat outputMat = m_currentProcessor->process(inputMat);
    if (outputMat.empty()) {
        return image;
    }

    // 转换回 QImage
    return CvMatToQImage(outputMat);
}

void ImageProcessingService::setProcessorType(ProcessorType type)
{
    if (m_currentType != type) {
        m_currentType = type;
        updateProcessor();
        updateParameterProvider();
    }
}

QStringList ImageProcessingService::getProcessorNames() const
{
    return {"模糊", "边缘检测", "亮度对比度", "灰度化"};
}

QStringList ImageProcessingService::getAvailableProcessors() const
{
    return getProcessorNames();
}

ProcessorType ImageProcessingService::getProcessorType(const QString& name) const
{
    if (name == "模糊") return ProcessorType::Blur;
    if (name == "边缘检测") return ProcessorType::EdgeDetection;
    if (name == "亮度对比度") return ProcessorType::BrightnessContrast;
    if (name == "灰度化") return ProcessorType::Grayscale;
    return ProcessorType::Grayscale;
}

QVariant ImageProcessingService::getParameter(const QString& name) const
{
    if (!m_parameterProvider) {
        return QVariant();
    }

    // 根据参数名称返回对应的参数值
    switch (m_currentType) {
    case ProcessorType::Blur: {
        auto* provider = dynamic_cast<BlurParams*>(m_parameterProvider.get());
        if (provider && name == "kernelSize") {
            return provider->getKernelSize();
        }
        break;
    }
    case ProcessorType::EdgeDetection: {
        auto* provider = dynamic_cast<EdgeDetectionParams*>(m_parameterProvider.get());
        if (provider && name == "threshold") {
            return provider->getThreshold();
        }
        break;
    }
    case ProcessorType::BrightnessContrast: {
        auto* provider = dynamic_cast<BrightnessContrastParams*>(m_parameterProvider.get());
        if (provider) {
            if (name == "brightness") return provider->getBrightness();
            if (name == "contrast") return provider->getContrast();
        }
        break;
    }
    case ProcessorType::Grayscale:
        break;
    }

    return QVariant();
}

void ImageProcessingService::setParameter(const QString& name, const QVariant& value)
{
    if (!m_parameterProvider) {
        return;
    }

    // 根据参数名称设置对应的参数值
    switch (m_currentType) {
    case ProcessorType::Blur: {
        auto* provider = dynamic_cast<BlurParams*>(m_parameterProvider.get());
        if (provider && name == "kernelSize") {
            provider->setKernelSize(value.toInt());
        }
        break;
    }
    case ProcessorType::EdgeDetection: {
        auto* provider = dynamic_cast<EdgeDetectionParams*>(m_parameterProvider.get());
        if (provider && name == "threshold") {
            provider->setThreshold(value.toInt());
        }
        break;
    }
    case ProcessorType::BrightnessContrast: {
        auto* provider = dynamic_cast<BrightnessContrastParams*>(m_parameterProvider.get());
        if (provider) {
            if (name == "brightness") provider->setBrightness(value.toInt());
            if (name == "contrast") provider->setContrast(value.toInt());
        }
        break;
    }
    case ProcessorType::Grayscale:
        break;
    }
}

void ImageProcessingService::updateProcessor()
{
    m_currentProcessor = m_factory->createProcessor(m_currentType);
    if (m_currentProcessor && m_parameterProvider) {
        m_currentProcessor->setParams(m_parameterProvider.get());
    }
}

void ImageProcessingService::updateParameterProvider()
{
    m_parameterProvider = m_factory->createParameterProvider(m_currentType);
    if (m_parameterProvider) {
        connect(m_parameterProvider.get(), &IParams::parametersChanged,
                this, [this]() {
                    if (m_currentProcessor && m_parameterProvider) {
                        m_currentProcessor->setParams(m_parameterProvider.get());
                    }
                });

        if (m_currentProcessor) {
            m_currentProcessor->setParams(m_parameterProvider.get());
        }
    }
}

IParams* ImageProcessingService::getParameterProvider() const
{
    return m_parameterProvider.get();
}
