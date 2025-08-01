#include <algorithm>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <qelapsedtimer.h>
#include <sstream>
#include <opencv2/core/hal/hal.hpp>
#include <qtimer>
#include <QImage>

// fmt print cv::Vec3i

template<typename T>
struct fmt::formatter<cv::Vec<T, 3>> : fmt::formatter<std::string>
{
    template<typename FormatContext>
    auto format(const cv::Vec<T, 3>& v, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({}, {}, {})", v[0], v[1], v[2]);
    }
};

void write_array_to_file(const std::string& filename, const cv::Mat& array)
{
    FILE* file = fopen(filename.c_str(), "w");
    if (file != nullptr) {
        cv::print(array, file);
        return;
    }
}

cv::Mat computeAngle(const cv::Mat& image)
{
    CV_Assert(image.channels() == 2);
    cv::Mat angle(image.size(), CV_64F);

    for (int i = 0; i < image.rows; i++) {
        const auto xy_ptr = image.ptr<cv::Vec2d>(i);
        auto angle_ptr = angle.ptr<double>(i);
        for (int j = 0; j < image.cols; j++) {
            const auto xy = xy_ptr[j];
            auto ang = std::atan2(xy[0], xy[1]);
            if (ang < 0) {
                ang += CV_PI;
            }
            ang *= 180.0 / CV_PI;
            angle_ptr[j] = ang;
        }
    }
    return angle;
}

void test_phase()
{
    // create a two channel mat;
    cv::Mat image(5, 5, CV_64FC2);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            int val = i * image.cols + j;
            image.at<cv::Vec2d>(i, j) = cv::Vec2d(val, 2 * val);
        }
    }
    cv::print(image);
    std::cout << std::endl;
    auto angle = computeAngle(image);
    cv::print(angle);
    std::cout << std::endl;
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    cv::Mat angle1;
    cv::phase(channels[0], channels[1], angle1);
    // cv::multiply(angle1, 180. / CV_PI, angle1);
    cv::print(angle1);
    std::cout << std::endl;
    std::cout << image.dims << "," << image.size();

    cv::Mat mat1(cv::Size(4, 1), CV_32FC1), mat2(cv::Size(4, 1), CV_32FC1),
      mat3;
    mat1.at<float>(0, 0) = 1;
    mat2.at<float>(0, 0) = 1;
    mat1.at<float>(0, 1) = 1;
    mat2.at<float>(0, 1) = -1;
    mat1.at<float>(0, 2) = -1;
    mat2.at<float>(0, 2) = -1;
    mat1.at<float>(0, 3) = -1;
    mat2.at<float>(0, 3) = 1;
    cv::phase(mat2, mat1, mat3);
    std::cout << std::endl;
    cv::print(mat3);
}

void fastAtan()
{
    double y1 = -2.33, x1 = 1.1, r;
    cv::hal::fastAtan64f(&y1, &x1, &r, 1, false);
}

void normalAtan()
{
    double y1 = -2.33, x1 = 1.1, r;
    r = std::atan2(y1, x1);
}

void test_print()
{
    cv::Vec3b b1(1, 2, 3);
    cv::Vec3b b2(4, 5, 6);
    // sub will cause saturate cast
    auto diff0 = b2 - b1;
    auto diff2 = cv::norm(b1 - b2);
    auto diff3 = cv::normL2Sqr<uchar, uchar>(diff0.val, 3);
}

void compare_fastatan()
{
    std::cout << std::atan2(1, 1) << "," << std::atan2(1, -1) << ","
              << std::atan2(-1, -1) << "," << std::atan2(-1, 1);

    std::cout << std::endl;
    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 100000000; ++i) {
        fastAtan();
    }
    std::cout << "fastAtan: " << timer.elapsed() << std::endl;
    QElapsedTimer timer1;
    timer1.start();
    for (int i = 0; i < 100000000; ++i) {
        normalAtan();
    }
    std::cout << "normalAtan: " << timer1.elapsed() << std::endl;
}

void test_mask()
{
    cv::Mat mat1 = cv::Mat::eye(4, 4, CV_32FC1);
    mat1.at<float>(0, 0) = NAN;
    mat1.at<float>(0, 1) = NAN;
    mat1.at<float>(0, 2) = NAN;
    mat1.at<float>(0, 3) = NAN;

    // mat1.at<float>(1, 0) = NAN;
    // mat1.at<float>(2, 0) = NAN;
    // mat1.at<float>(3, 0) = NAN;

    cv::Mat mask = mat1 == mat1;
    cv::print(mask);
    cv::Rect rect = cv::boundingRect(mask);
    std::cout << rect.x << "," << rect.y << "," << rect.width << ","
              << rect.height << std::endl;
}

cv::Mat cast_to(cv::Mat input, double v_min, double v_max)
{
    auto alpha = (255 + 255) / (v_max - v_min);
    auto beta = -255 - alpha * v_min;
    cv::Mat result;
    input.convertTo(result, CV_8U, alpha, beta);
    return result;
}
void test_convert()
{
    cv::Mat a = cv::Mat::zeros(5, 5, CV_32F);
    cv::randn(a, 0, 0.1);
    cv::print(a);
    std::cout << std::endl;

    double v_min, v_max;
    cv::minMaxIdx(a, &v_min, &v_max);
    std::cout << "v_min: " << v_min << ",v_max:" << v_max << std::endl;

    auto result = cast_to(a, -0.1, 0.1);
    cv::print(result);
}
void test_qimage()
{
    auto processed_qimage =
      QImage("C:/Users/zhu/Downloads/waviness/waviness.png");
    qDebug() << processed_qimage.isNull();
    qDebug() << processed_qimage.format();
    qDebug() << processed_qimage.width() << processed_qimage.height();
    // if (processed_qimage.isNull()) {
    //     processed_qimage = QImage(processed_qimage.width(),
    //                               processed_qimage.height(),
    //                               QImage::Format_RGB888);
    //     processed_qimage.fill(Qt::black);
    // }
    processed_qimage.save("new.png");
}

void test_save_image()
{
    cv::Mat mat(200, 200, CV_8UC3, cv::Scalar(0, 0, 0));
    QString path = u8"中文路径/测试图片.png";
    QFileInfo info(path);
    auto dir = info.absolutePath();
    qDebug() << "dir:" << dir;
    if (!QDir(dir).exists()) {
        QDir().mkpath(dir);
    }
    cv::imwrite(path.toLocal8Bit().toStdString(), mat);
}

static cv::Mat cast_to_uint8(const cv::Mat& image, double v_min, double v_max)
{
    if (v_min == v_max) {
        cv::minMaxIdx(image, &v_min, &v_max);
        if (v_min == v_max) {
            v_max = v_max + 1;
            v_min = v_min - 1;
        }
    }
    cv::Mat ret;
    cv::multiply(image, 255. / (v_max - v_min), ret);
    cv::add(ret, -255. * v_min / (v_max - v_min), ret);
    ret.convertTo(ret, CV_8U);
    return ret;
}

static QImage qimage_add_levels(
  const cv::Mat& z,
  double v_min,
  double v_max,
  cv::ColormapTypes color_map = cv::COLORMAP_VIRIDIS)
{
    auto result = cast_to_uint8(z, v_min, v_max);
    cv::Mat dst;
    cv::applyColorMap(result, dst, color_map);
    cv::Mat alpha = z == z;
    if (cv::countNonZero(alpha) == 0) {
        QImage black_img = QImage(dst.cols, dst.rows, QImage::Format_RGB888);
        black_img.fill(Qt::black);
        return black_img;
    }
    QImage q_img = QImage(dst.cols, dst.rows, QImage::Format_RGBA8888);
    cv::Mat bgraImage(dst.size(), CV_8UC4, q_img.bits());
    cv::Mat channels[4];
    cv::split(dst, channels);
    std::swap(channels[0], channels[2]);
    channels[3] = alpha;
    cv::merge(channels, 4, bgraImage);
    return q_img;
}

double computeMedianDepth(const cv::Mat& depthMat)
{
    if (depthMat.channels() != 1 || depthMat.type() != CV_64F) {
        throw std::invalid_argument(
          "Input must be a single-channel CV_64F matrix");
    }

    std::vector<double> depths;
    depths.reserve(depthMat.total());
    for (int i = 0; i < depthMat.rows; ++i) {
        for (int j = 0; j < depthMat.cols; ++j) {
            double val = depthMat.at<double>(i, j);
            if (std::isfinite(val)) {
                depths.push_back(val);
            }
        }
    }

    if (depths.empty()) {
        throw std::runtime_error("No valid depth values found");
    }

    std::sort(depths.begin(), depths.end());

    size_t size = depths.size();
    if (size % 2 == 0) {
        return (depths[size / 2 - 1] + depths[size / 2]) / 2.0;
    } else {
        return depths[size / 2];
    }
}
void test_waviness()
{

    cv::Mat mat =
      cv::imread("C:/Users/zhu/Downloads/waviness.tiff", cv::IMREAD_UNCHANGED);
    double img_min = 0;
    double img_max = 100. / 1000 / 1000;
    double range = 10. / 1e6;
    cv::minMaxIdx(mat, &img_min, &img_max);
    // auto mean = (img_min + img_max) / 2;
    auto mean = computeMedianDepth(mat);
    qDebug() << "mean:" << mean;
    auto low = mean - range / 2;
    auto high = mean + range / 2;
    qDebug() << "img_min:" << img_min << ",img_max:" << img_max;
    auto q_img = qimage_add_levels(mat, low, high);
    q_img.save("waviness.png");
}

int main()
{
    // cv::Mat mat =
    //   cv::imread("C:/Users/zhu/Downloads/waviness.tiff",
    //   cv::IMREAD_UNCHANGED);
    // qDebug() << mat.cols << "," << mat.rows;
    // auto mask = mat == mat;
    // auto rect = cv::boundingRect(mask);
    // qDebug() << rect.x << ", " << rect.y << "," << rect.width << ","
    //          << rect.height;
    // test_save_image();
    test_waviness();
}
