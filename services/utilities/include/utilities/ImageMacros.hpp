#pragma once

#include <opencv4/opencv2/highgui.hpp>

#ifdef DISPLAY_OPENCV_IMG
    #define DISPLAY_IMAGE(windowTitle, src)                             \
        do {                                                            \
            cv::imshow(windowTitle + std::to_string(std::rand()), src); \
            cv::waitKey(0);                                             \
        } while (0)

    #define DISPLAY_VIDEO(windowTitle, src) \
        do {                                \
            cv::imshow(windowTitle, src);   \
            cv::waitKey(100);               \
        } while (0)
#else
    #define DISPLAY_IMAGE(windowTitle, src)  // Empty macro if DISPLAY_OPENCV_IMG is not defined
    #define DISPLAY_VIDEO(windowTitle, src)  // Empty macro if DISPLAY_OPENCV_IMG is not defined
#endif