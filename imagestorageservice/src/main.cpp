#include <qcoreapplication.h>
#include <fmt/format.h>
#include <opencv2/opencv.hpp>
#include "ImageStorageService.h"
#include <qtimer.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    fmt::println("Hello World!");

    for( int i=0; i<255; i++){
        std::string image_name = fmt::format("{:03d}.png", i);
        fmt::println("saving image {}", image_name);
        cv::Mat image = cv::Mat(10000, 10000, CV_8UC3, cv::Scalar(i, i, i));
        ImageStorageService::Ins()->saveImageAsync(QString::fromStdString(image_name), image);
    }
    QTimer::singleShot(10000, [&]() {
        QCoreApplication::quit();
    });
    a.exec();
}
