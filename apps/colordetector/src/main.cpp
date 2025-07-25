#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <qelapsedtimer.h>
#include <sstream>
#include <opencv2/core/hal/hal.hpp>
#include <qtimer>

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

int main()
{
    test_mask();
}
