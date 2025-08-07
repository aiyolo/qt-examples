#include "customchartview.h"

CustomChartView::CustomChartView(QWidget *parent)
    : QChartView(new QChart(), parent)
{
    crosshairV = new QGraphicsLineItem(chart());
    crosshairH = new QGraphicsLineItem(chart());
    QPen pen(Qt::red, 1, Qt::DashLine);
    crosshairV->setPen(pen);
    crosshairH->setPen(pen);
    crosshairV->hide();
    crosshairH->hide();

    intersectionPoint = new QGraphicsEllipseItem(chart());
    intersectionPoint->setRect(-5, -5, 10, 10);
    intersectionPoint->setBrush(QBrush(Qt::blue));
    intersectionPoint->hide();

    setMouseTracking(true);
}

void CustomChartView::setData(const QList<QPointF> &points)
{
    chart()->removeAllSeries();

    QLineSeries *series = new QLineSeries();
    for (const QPointF &point : points) {
        series->append(point);
    }

    chart()->addSeries(series);
    chart()->createDefaultAxes();
}

void CustomChartView::setLineStyle(const QPen &pen)
{
    if (!chart()->series().isEmpty()) {
        QLineSeries *series = qobject_cast<QLineSeries*>(chart()->series().first());
        if (series) {
            series->setPen(pen);
        }
    }
}

void CustomChartView::setBackground(const QBrush &brush)
{
    chart()->setBackgroundBrush(brush);
}

void CustomChartView::setTitle(const QString &title)
{
    chart()->setTitle(title);
}

void CustomChartView::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event);

    // 获取鼠标在图表中的位置
    QPointF mousePos = event->pos();
    QRectF plotArea = chart()->plotArea();

    // 确保鼠标在绘图区域内
    if (!plotArea.contains(mousePos)) {
        crosshairV->hide();
        crosshairH->hide();
        intersectionPoint->hide();
        return;
    }

    // 将鼠标位置转换为图表坐标系的值
    QPointF mouseValue = chart()->mapToValue(mousePos);

    // 获取坐标轴范围
    auto *xAxis = qobject_cast<QValueAxis*>(chart()->axisX());
    auto *yAxis = qobject_cast<QValueAxis*>(chart()->axisY());
    if (!xAxis || !yAxis) {
        return;
    }

    qreal xMin = xAxis->min();
    qreal xMax = xAxis->max();
    qreal yMin = yAxis->min();
    qreal yMax = yAxis->max();

    // 更新垂直线 - 使用正确的坐标转换
    QPointF topPoint = chart()->mapToPosition(QPointF(mouseValue.x(), yMax));
    QPointF bottomPoint = chart()->mapToPosition(QPointF(mouseValue.x(), yMin));
    crosshairV->setLine(topPoint.x(), topPoint.y(), bottomPoint.x(), bottomPoint.y());
    crosshairV->show();

    // 更新水平线 - 使用正确的坐标转换
    QPointF leftPoint = chart()->mapToPosition(QPointF(xMin, mouseValue.y()));
    QPointF rightPoint = chart()->mapToPosition(QPointF(xMax, mouseValue.y()));
    crosshairH->setLine(leftPoint.x(), leftPoint.y(), rightPoint.x(), rightPoint.y());
    crosshairH->show();

    // 查找与曲线的交点
    QLineSeries *series = qobject_cast<QLineSeries*>(chart()->series().first());
    if (series && series->count() > 1) {
        QPointF intersection;
        bool found = false;

        // 遍历线段查找交点
        for (int i = 0; i < series->count() - 1; ++i) {
            QPointF p1 = series->at(i);
            QPointF p2 = series->at(i + 1);

            // 检查垂直线是否与线段相交
            if ((mouseValue.x() >= p1.x() && mouseValue.x() <= p2.x()) ||
                (mouseValue.x() <= p1.x() && mouseValue.x() >= p2.x())) {

                // 计算交点的y值
                qreal t = (mouseValue.x() - p1.x()) / (p2.x() - p1.x());
                qreal y = p1.y() + t * (p2.y() - p1.y());

                // 确保交点在y轴范围内
                if (y >= yMin && y <= yMax) {
                    intersection = QPointF(mouseValue.x(), y);
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            QPointF intersectionPos = chart()->mapToPosition(intersection);
            intersectionPoint->setPos(intersectionPos.x(), intersectionPos.y());
            intersectionPoint->show();
            qDebug() << "Intersection point:" << intersection;
        } else {
            intersectionPoint->hide();
        }
    }
}

void CustomChartView::mouseReleaseEvent(QMouseEvent *event)
{
    crosshairV->hide();
    crosshairH->hide();
    intersectionPoint->hide();
    QChartView::mouseReleaseEvent(event);
}
