#pragma once

#include <QWidget>
#include <QObject>

// 参数接口 - 遵循接口隔离原则
class IParams : public QObject
{
    Q_OBJECT

public:
    explicit IParams(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IParams() = default;
    virtual QWidget* createWidget(QWidget* parent = nullptr) = 0;
    virtual IParams* clone() const = 0;

signals:
    void parametersChanged();
};
