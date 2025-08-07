#include "losschart.h"

LossChart::LossChart(QWidget* parent)
    : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    customPlot->setBackground(QBrush(QColor(0, 0, 0)));
    customPlot->addGraph();

    // give the axes some labels:
    customPlot->xAxis->setLabel("Step");
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setLabel("Loss");
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    // customPlot->setWindowTitle("训练损失");

    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(
        0, 0, new QCPTextElement(customPlot, "训练损失", QFont("sans", 12, QFont::Bold)));

    // configure right and top axis to show ticks and labels:
    customPlot->xAxis2->setVisible(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(false);
    customPlot->yAxis2->setTickLabels(false);

    customPlot->graph(0)->rescaleAxes();

    // make left and bottom axes always transfer their ranges to right and top axes:
    // connect(customPlot->xAxis,
    //         SIGNAL(rangeChanged(QCPRange)),
    //         customPlot->xAxis2,
    //         SLOT(setRange(QCPRange)));
    // connect(customPlot->yAxis,
    //         SIGNAL(rangeChanged(QCPRange)),
    //         customPlot->yAxis2,
    //         SLOT(setRange(QCPRange)));

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
}

LossChart::~LossChart() {}
void LossChart::setData(const QList<QPointF>& data)
{

    QVector<double> x;
    QVector<double> y;
    for (const auto& p : data) {
        x.append(p.x());
        y.append(p.y());
    }
    customPlot->graph(0)->setData(x, y);
    customPlot->graph(0)->rescaleAxes();
    customPlot->replot();
}
void LossChart::onReceivedTrainLoss(const QVariant& qvariant)
{
    auto data = qvariant.value<QList<QPointF>>();
    setData(data);
}
