#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Database Admin");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("QtExamples");

    MainWindow window;
    window.show();

    return app.exec();
}
