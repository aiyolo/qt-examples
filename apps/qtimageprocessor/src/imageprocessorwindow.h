#pragma once

#include <QMainWindow>
#include <QImage>
#include <memory>

class SimpleImageViewer;
class QComboBox;
class QPushButton;
class QWidget;
class QVBoxLayout;
class ImageProcessingService;
class IParameterProvider;

// 主窗口 - 遵循单一职责原则，只负责UI交互
class ImageProcessorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageProcessorWindow(QWidget* parent = nullptr);
    ~ImageProcessorWindow();

private slots:
    void openImage();
    void saveProcessedImage();
    void onProcessorChanged(int index);
    void onParametersChanged();

private:
    void setupUI();
    void setupParameterArea();
    void processAndDisplay();
    void clearParameterWidgets();

    // UI 组件
    SimpleImageViewer* m_originalViewer;
    SimpleImageViewer* m_processedViewer;
    QComboBox* m_processorCombo;
    QWidget* m_parameterWidget;
    QVBoxLayout* m_parameterLayout;
    QPushButton* m_openButton;
    QPushButton* m_saveButton;

    // 业务逻辑 - 使用组合而不是继承
    std::unique_ptr<ImageProcessingService> m_processingService;
    QImage m_originalImage;
};
