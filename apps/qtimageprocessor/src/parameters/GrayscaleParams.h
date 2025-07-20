#pragma once

#include "../interfaces/IParams.h"

// 灰度参数提供者 - 灰度处理不需要参数
class GrayscaleParams : public IParams
{
    Q_OBJECT

public:
explicit GrayscaleParams(QObject* parent = nullptr);

    QWidget* createWidget(QWidget* parent = nullptr) override;
IParams* clone() const override;
};
