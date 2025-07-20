#include "BlurParams.h"
#include <QSpinBox>
#include <QFormLayout>
#include <QWidget>

BlurParams::BlurParams(QObject* parent)
    : IParams(parent)
    , m_kernelSize(5)
{
}
QWidget* BlurParams::createWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QFormLayout* layout = new QFormLayout(widget);

    QSpinBox* kernelSpin = new QSpinBox(widget);
    kernelSpin->setRange(1, 21);
    kernelSpin->setSingleStep(2);
    kernelSpin->setValue(m_kernelSize);

    connect(kernelSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BlurParams::onKernelSizeChanged);

    layout->addRow("核大小:", kernelSpin);
    return widget;
}

IParams* BlurParams::clone() const
{
    return new BlurParams();
}

int BlurParams::getKernelSize() const
{
    return m_kernelSize;
}

void BlurParams::setKernelSize(int size)
{
    if (m_kernelSize != size) {
        m_kernelSize = size;
        emit parametersChanged();
    }
}

void BlurParams::onKernelSizeChanged(int size)
{
    setKernelSize(size);
}
