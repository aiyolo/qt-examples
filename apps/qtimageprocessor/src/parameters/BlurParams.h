#pragma once

#include "../interfaces/IParams.h"
#include <memory>

// 模糊参数提供者 - 使用组合而不是继承
class BlurParams : public IParams
{
    Q_OBJECT

public:
explicit BlurParams(QObject* parent = nullptr);

    QWidget* createWidget(QWidget* parent = nullptr) override;
IParams* clone() const override;

    int getKernelSize() const;
    void setKernelSize(int size);

private slots:
    void onKernelSizeChanged(int size);

private:
    int m_kernelSize;
};
