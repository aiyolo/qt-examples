#include "EdgeDetectionParams.h"
#include <QSlider>
#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QWidget>

EdgeDetectionParams::EdgeDetectionParams(QObject* parent)
    : IParams(parent)
    , m_threshold(100)
{
}

QWidget* EdgeDetectionParams::createWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QFormLayout* layout = new QFormLayout(widget);

    QSlider* thresholdSlider = new QSlider(Qt::Horizontal, widget);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(m_threshold);

    QLabel* valueLabel = new QLabel(QString::number(m_threshold), widget);

connect(thresholdSlider, &QSlider::valueChanged,
            this, &EdgeDetectionParams::onThresholdChanged);
    connect(thresholdSlider, &QSlider::valueChanged,
            valueLabel, [valueLabel](int value) {
                valueLabel->setText(QString::number(value));
            });

    QHBoxLayout* sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(thresholdSlider);
    sliderLayout->addWidget(valueLabel);

    QWidget* sliderContainer = new QWidget(widget);
    sliderContainer->setLayout(sliderLayout);

    layout->addRow("阈值:", sliderContainer);
    return widget;
}

IParams* EdgeDetectionParams::clone() const
{
    return new EdgeDetectionParams();
}

int EdgeDetectionParams::getThreshold() const
{
    return m_threshold;
}

void EdgeDetectionParams::setThreshold(int threshold)
{
    if (m_threshold != threshold) {
        m_threshold = threshold;
        emit parametersChanged();
    }
}

void EdgeDetectionParams::onThresholdChanged(int threshold)
{
    setThreshold(threshold);
}
