#pragma once

#include <QMainWindow>
#include <QImage>

class SimpleImageViewer;
class ImageProcessor;
class QComboBox;
class QSlider;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;

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
    void onBlurKernelChanged(int value);
    void onEdgeThresholdChanged(int value);
    void onBrightnessChanged(int value);
    void onContrastChanged(double value);

private:
    void setupConnections();
    void updateParameterControls();
    void processAndDisplay();

    SimpleImageViewer* m_originalViewer;
    SimpleImageViewer* m_processedViewer;
    ImageProcessor* m_imageProcessor;
    QImage m_originalImage;

    // 控件指针
    QComboBox* m_algorithmCombo;
    QSlider* m_blurSlider;
    QSpinBox* m_blurSpin;
    QSlider* m_edgeSlider;
    QSpinBox* m_edgeSpin;
    QSlider* m_brightnessSlider;
    QSpinBox* m_brightnessSpin;
    QSlider* m_contrastSlider;
    QDoubleSpinBox* m_contrastSpin;
    QPushButton* m_openButton;
    QPushButton* m_saveButton;
    QPushButton* m_resetButton;
};
