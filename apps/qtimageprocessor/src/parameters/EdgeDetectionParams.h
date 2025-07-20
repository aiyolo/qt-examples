#pragma once

#include "../interfaces/IParams.h"
#include <memory>

// 边缘检测参数提供者 - 使用组合而不是继承
class EdgeDetectionParams : public IParams
{
    Q_OBJECT

public:
explicit EdgeDetectionParams(QObject* parent = nullptr);

    QWidget* createWidget(QWidget* parent = nullptr) override;
IParams* clone() const override;

    int getThreshold() const;
    void setThreshold(int threshold);

private slots:
    void onThresholdChanged(int threshold);

private:
    int m_threshold;
};
