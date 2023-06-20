#pragma once

#include <gtest/gtest.h>

#include <opencv2/highgui.hpp>

#ifdef DISPLAY_OPENCV_IMG
    #define DISPLAY_IMAGE(windowTitle, src)                             \
        do                                                              \
        {                                                               \
            cv::imshow(windowTitle + std::to_string(std::rand()), src); \
            cv::waitKey(0);                                             \
        } while (0)
#else
    #define DISPLAY_IMAGE(windowTitle, src)  // Empty macro if DISPLAY_OPENCV_IMG is not defined
#endif

#define EXPECT_THROW_WITH_MESSAGE(code, exception_type, expected_message) \
    do                                                                    \
    {                                                                     \
        EXPECT_THROW(                                                     \
            try { code; } catch (const exception_type& e) {               \
                EXPECT_STREQ(e.what(), expected_message);                 \
                throw;                                                    \
            },                                                            \
            exception_type);                                              \
    } while (0)
