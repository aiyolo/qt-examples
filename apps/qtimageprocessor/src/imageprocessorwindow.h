#pragma once

#include <QMainWindow>
#include <QImage>
#include <memory>

class SimpleImageViewer;
class QComboBox;
class QPushButton;
class QWidget;
class QVBoxLayout;
class AlgorithmParameters;

class ImageProcessorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageProcessorWindow(QWidget* parent = nullptr);
    ~ImageProcessorWindow();

private slots:
    void openImage();
    void saveProcessedImage();
    void onAlgorithmChanged(int index);
    void onParametersChanged();

private:
    void setupUI();
    void setupParameterArea();
    void processAndDisplay();
    void clearParameterWidgets();

    SimpleImageViewer* m_originalViewer;
    SimpleImageViewer* m_processedViewer;
    std::unique_ptr<AlgorithmParameters> m_algorithmParams;
    std::unique_ptr<class ImageProcessor> m_imageProcessor;
    QImage m_originalImage;
    QComboBox* m_algorithmCombo;
    QWidget* m_parameterWidget;
    QVBoxLayout* m_parameterLayout;
    QPushButton* m_openButton;
    QPushButton* m_saveButton;
};
