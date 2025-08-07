#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

using namespace QtCharts;

class CustomChartView : public QChartView {
public:
    explicit CustomChartView(QWidget *parent = nullptr);

    void setData(const QList<QPointF> &points);
    void setLineStyle(const QPen &pen);
    void setBackground(const QBrush &brush);
    void setTitle(const QString &title);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsLineItem *crosshairV, *crosshairH;
    QGraphicsEllipseItem *intersectionPoint;
    QGraphicsTextItem *xAxisLabel;
    QGraphicsTextItem *yAxisLabel;
    QGraphicsTextItem *intersectionLabel;
    QPoint dragStartPos_;  // 用于记录拖动起始位置
};

#endif // CUSTOMCHARTVIEW_H
