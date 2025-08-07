#pragma once

#include "qcustomplot.h"

class LossChart : public QWidget
{
    Q_OBJECT

public:
    LossChart(QWidget* parent = nullptr);
    ~LossChart();

    void setData(const QList<QPointF>& data);

public slots:
    void onReceivedTrainLoss(const QVariant& qvariant);

private:
    QCustomPlot* customPlot;
};
