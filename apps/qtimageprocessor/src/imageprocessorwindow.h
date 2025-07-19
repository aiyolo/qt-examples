#pragma once

#include <QMainWindow>
#include <QImage>
#include <QWidget>
#include <QVBoxLayout>

class SimpleImageViewer;
class ImageProcessor;
class AlgorithmParams;
class QComboBox;
class QPushButton;
class QScrollArea;

class ImageProcessorWindow : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessorWindow(QWidget* parent = nullptr);
    ~ImageProcessorWindow();

private slots:
    void openImage();
    void saveProcessedImage();
    void resetToOriginal();
    void updateProcessedImage();
    void onAlgorithmChanged(int index);

private:
    void setupConnections();
    void updateParameterControls();
    void processAndDisplay();

    SimpleImageViewer* m_originalViewer;
    SimpleImageViewer* m_processedViewer;
    ImageProcessor* m_imageProcessor;
    AlgorithmParams* m_algorithmParams;
    QImage m_originalImage;

    // 控件指针
    QComboBox* m_algorithmCombo;
    QWidget* m_parameterWidget;
    QVBoxLayout* m_parameterLayout;
    QScrollArea* m_parameterScrollArea;
    QPushButton* m_openButton;
    QPushButton* m_saveButton;
    QPushButton* m_resetButton;
};
