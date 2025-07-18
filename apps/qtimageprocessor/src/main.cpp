#include <QApplication>
#include "imageprocessorwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ImageProcessorWindow w;
    w.show();

    return a.exec();
}
