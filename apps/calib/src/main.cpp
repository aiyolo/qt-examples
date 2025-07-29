#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <opencv2/calib3d.hpp>
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
#include <opencv2/core/utils/logger.hpp>
#include <memory>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <climits>
#include <streambuf>
#include <fcntl.h>



void test_checkboard()
{
    auto mat = cv::imread("/Users/zhu/Pictures/chessboard.png", cv::IMREAD_UNCHANGED);
    cv::Mat gray;
    cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
    bool ok  = cv::checkChessboard(gray, cv::Size(6,4));
    std::cout << "result: " << std::boolalpha << ok << std::endl;

}

int main()
{
    test_checkboard();
}
