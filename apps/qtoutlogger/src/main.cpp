#include "mainwindow.h"
#include "qdir.h"
#include "qfile.h"
#include "qmutex.h"
#include <QDateTime>
#include <QFileInfo>
#include <QSettings>
#include <QTextStream>
#include <QtWidgets/QApplication>
#include <qDebug>
#include <qthread.h>

#include "qylogger.h"
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
    // Set up logger with log file path, max file size (10MB), and max log files (3)
    setup_logger("./log", 10 * 1024 * 1024, 3);

    // std::thread t(
    //     []()
    //     {
    //         for (int i = 0; i < 1e5; i++)
    //         {
    //             qDebug() << "sub thread:" << i;
    //         }
    //     });
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1e5; i++)
    {
        qDebug() << "main thread: " << i;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms"
              << std::endl;
    // t.join();
}
