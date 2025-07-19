#include "imagealgorithm.h"
#include <opencv2/imgproc.hpp>

cv::Mat ImageAlgorithm::qImageToCvMat(const QImage& image)
{
    QImage converted = image.convertToFormat(QImage::Format_RGB888);
    return cv::Mat(converted.height(), converted.width(), CV_8UC3,
                   const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
}

QImage ImageAlgorithm::cvMatToQImage(const cv::Mat& mat)
{
    if (mat.empty()) {
        return QImage();
    }

    cv::Mat rgb;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    } else if (mat.channels() == 1) {
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    } else {
        rgb = mat.clone();
    }

    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step,
                  QImage::Format_RGB888).copy();
}
