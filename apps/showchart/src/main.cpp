#include <QtWidgets/QApplication>
#include "losschart.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    LossChart chart;
    chart.setData(QList<QPointF>{
      QPointF(1.123, 1.7), QPointF(2.178, 2.3), QPointF(3.12, 3.4) });
    chart.resize(800, 600);
    chart.show();
    return a.exec();
}
