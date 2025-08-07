#include <QtWidgets/QApplication>
#include <QPainter>
#include "customchartview.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CustomChartView *chartView = new CustomChartView();

    // 设置数据点和样式
    chartView->setData({{0, 6}, {2, 4}, {3, 8}, {7, 4}, {10, 5}});
    chartView->setLineStyle(QPen(Qt::blue, 2, Qt::SolidLine));
    chartView->setBackground(QBrush(QColor(200, 200, 255)));
    chartView->setTitle("调试十字线和交点");

    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();

    return app.exec();
}
