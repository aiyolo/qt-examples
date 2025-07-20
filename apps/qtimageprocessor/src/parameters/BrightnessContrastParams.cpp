#include "BrightnessContrastParams.h"
#include <QSlider>
#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QWidget>

BrightnessContrastParams::BrightnessContrastParams(QObject* parent)
    : IParams(parent)
    , m_brightness(0)
    , m_contrast(100)
{
}

QWidget* BrightnessContrastParams::createWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QFormLayout* layout = new QFormLayout(widget);

    // 亮度滑块
    QSlider* brightnessSlider = new QSlider(Qt::Horizontal, widget);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(m_brightness);

    QLabel* brightnessLabel = new QLabel(QString::number(m_brightness), widget);

    // 对比度滑块
    QSlider* contrastSlider = new QSlider(Qt::Horizontal, widget);
    contrastSlider->setRange(0, 200);
    contrastSlider->setValue(m_contrast);

    QLabel* contrastLabel = new QLabel(QString::number(m_contrast), widget);

connect(brightnessSlider, &QSlider::valueChanged,
            this, &BrightnessContrastParams::onBrightnessChanged);
    connect(brightnessSlider, &QSlider::valueChanged,
            brightnessLabel, [brightnessLabel](int value) {
                brightnessLabel->setText(QString::number(value));
            });

connect(contrastSlider, &QSlider::valueChanged,
            this, &BrightnessContrastParams::onContrastChanged);
    connect(contrastSlider, &QSlider::valueChanged,
            contrastLabel, [contrastLabel](int value) {
                contrastLabel->setText(QString::number(value));
            });

    // 亮度布局
    QHBoxLayout* brightnessLayout = new QHBoxLayout();
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessLabel);

    QWidget* brightnessContainer = new QWidget(widget);
    brightnessContainer->setLayout(brightnessLayout);

    // 对比度布局
    QHBoxLayout* contrastLayout = new QHBoxLayout();
    contrastLayout->addWidget(contrastSlider);
    contrastLayout->addWidget(contrastLabel);

    QWidget* contrastContainer = new QWidget(widget);
    contrastContainer->setLayout(contrastLayout);

    layout->addRow("亮度:", brightnessContainer);
    layout->addRow("对比度:", contrastContainer);

    return widget;
}

IParams* BrightnessContrastParams::clone() const
{
    return new BrightnessContrastParams();
}

int BrightnessContrastParams::getBrightness() const
{
    return m_brightness;
}

void BrightnessContrastParams::setBrightness(int brightness)
{
    if (m_brightness != brightness) {
        m_brightness = brightness;
        emit parametersChanged();
    }
}

int BrightnessContrastParams::getContrast() const
{
    return m_contrast;
}

void BrightnessContrastParams::setContrast(int contrast)
{
    if (m_contrast != contrast) {
        m_contrast = contrast;
        emit parametersChanged();
    }
}

void BrightnessContrastParams::onBrightnessChanged(int brightness)
{
    setBrightness(brightness);
}

void BrightnessContrastParams::onContrastChanged(int contrast)
{
    setContrast(contrast);
}
