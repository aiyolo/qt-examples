#include "GrayscaleParams.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

GrayscaleParams::GrayscaleParams(QObject* parent)
    : IParams(parent)
{
}

QWidget* GrayscaleParams::createWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* label = new QLabel("灰度处理不需要参数", widget);
    layout->addWidget(label);

    return widget;
}

IParams* GrayscaleParams::clone() const
{
    return new GrayscaleParams();
}
