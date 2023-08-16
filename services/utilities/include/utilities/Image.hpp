#pragma once

#include <opencv4/opencv2/highgui.hpp>

#include <utilities/Time.hpp>

#ifdef DISPLAY_OPENCV_IMG
    #define DISPLAY_IMAGE(windowTitle, src)                             \
        do {                                                            \
            cv::imshow(windowTitle + std::to_string(std::rand()), src); \
            cv::waitKey(0);                                             \
        } while (0)

    #define DISPLAY_VIDEO(windowTitle, src) \
        do {                                \
            cv::imshow(windowTitle, src);   \
            cv::waitKey(10);                \
        } while (0)
#else
    #define DISPLAY_IMAGE(windowTitle, src)  // Empty macro if DISPLAY_OPENCV_IMG is not defined
    #define DISPLAY_VIDEO(windowTitle, src)  // Empty macro if DISPLAY_OPENCV_IMG is not defined
#endif

namespace Utilities::Image {
    using Time::getMsTimestamp;

    static inline constexpr auto cvColorToString(const cv::Vec3b& color) -> std::string {
        return "(" + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " + std::to_string(color[2]) + ")";
    }

    static inline auto writeImage(const cv::Mat& image, std::string_view logPath, std::string_view category) -> void {
        cv::imwrite(fmt::format("{}/images/{}/{}.png", logPath, category, getMsTimestamp()), image);
    }
}  // namespace Utilities::Image
