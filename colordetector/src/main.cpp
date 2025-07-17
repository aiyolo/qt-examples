#include <cstdio>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <sstream>

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

int main()
{
    fmt::print("Hello World!\n");
    cv::Vec3i v1(1, 2, 3);
    cv::Vec3i v2(4, 5, 6);

    auto diff = v1 - v2;

    cv::Vec3b b1(1, 2, 3);
    cv::Vec3b b2(4, 5, 6);
    // sub will cause saturate cast
    auto diff0 = b2 - b1;
    auto diff2 = cv::norm(b1 - b2);
    auto diff3 = cv::normL2Sqr<uchar, uchar>(diff0.val, 3);
    fmt::print("diff: {}\n", diff3);
}
