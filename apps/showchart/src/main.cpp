#include <QtWidgets/QApplication>
#include "losschart.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    LossChart chart;
    chart.setData(
      QList<QPointF>{ QPointF(1, 1), QPointF(2, 2), QPointF(3, 3) });
    chart.show();
    return a.exec();
}
