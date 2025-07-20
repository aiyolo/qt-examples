#pragma once

#include "../interfaces/IParams.h"
#include <memory>

// 亮度对比度参数提供者 - 使用组合而不是继承
class BrightnessContrastParams : public IParams
{
    Q_OBJECT

public:
explicit BrightnessContrastParams(QObject* parent = nullptr);

    QWidget* createWidget(QWidget* parent = nullptr) override;
IParams* clone() const override;

    int getBrightness() const;
    void setBrightness(int brightness);
    int getContrast() const;
    void setContrast(int contrast);

private slots:
    void onBrightnessChanged(int brightness);
    void onContrastChanged(int contrast);

private:
    int m_brightness;
    int m_contrast;
};
