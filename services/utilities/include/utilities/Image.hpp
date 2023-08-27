#pragma once

#include <opencv4/opencv2/highgui.hpp>

#include <fmt/core.h>
#include <logger/Logger.hpp>
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

    using Logger = Logger::Quill;

    static constexpr double SIMILARITY_THRESHOLD = 0.15;

    static inline constexpr auto cvColorToString(const cv::Vec3b& color, std::string_view colorSpace = "BGR") -> std::string {
        return fmt::format("{}({}, {}, {})", colorSpace, color[0], color[1], color[2]);
    }

    static inline auto writeLogPlayerImage(const cv::Mat&   image,
                                           std::string_view logPath,
                                           std::string_view category,
                                           int32_t          playerNum) -> void {
        std::filesystem::directory_entry directory(fmt::format("{}/images/{}", logPath, category));

        if (!directory.exists()) { std::filesystem::create_directory(directory); }

        cv::imwrite(fmt::format("{}/images/{}/player_{}_{}.png", logPath, category, playerNum, getMsTimestamp()), image);
    }

    static inline auto writeLogGameImage(const cv::Mat& image, std::string_view logPath, std::string_view category) -> void {
        std::filesystem::directory_entry directory(fmt::format("{}/images/{}", logPath, category));

        if (!directory.exists()) { std::filesystem::create_directory(directory); }

        cv::imwrite(fmt::format("{}/images/{}/_{}.png", logPath, category, getMsTimestamp()), image);
    }

    static inline auto writeLearningImage(const cv::Mat&   image,
                                          std::string_view learningPath,
                                          std::string_view category,
                                          std::string_view name) -> void {
        std::filesystem::path            file(fmt::format("{}/images/{}/{}.png", learningPath, category, name));
        std::filesystem::directory_entry directory(file.parent_path());

        if (!std::filesystem::exists(file)) {
            if (!directory.exists()) { std::filesystem::create_directory(directory); }

            cv::imwrite(file, image);
        }
    }

    static inline auto writeImage(const cv::Mat& image, const std::filesystem::path& file) -> void {
        std::filesystem::directory_entry directory(file.root_directory());

        if (!directory.exists()) { std::filesystem::create_directory(directory); }

        cv::imwrite(file, image);
    }

    static inline auto cropCentered(cv::Mat& firstImage, cv::Mat& secondImage) -> void {
        // Determine smaller image, then crop the bigger one to the size of the smaller one. First image is the bigger one.
        if (secondImage.cols > firstImage.cols || secondImage.rows > firstImage.rows) { std::swap(firstImage, secondImage); }

        if (firstImage.cols < secondImage.cols || firstImage.rows < secondImage.rows) {
            throw std::runtime_error("The first image must be bigger than the second one.");
        }

        auto colsBorder = (firstImage.cols - secondImage.cols) / 2;
        auto rowsBorder = (firstImage.rows - secondImage.rows) / 2;

        firstImage(cv::Rect(colsBorder, rowsBorder, secondImage.cols, secondImage.rows)).copyTo(firstImage);
    }

    /**
     * The lower the score, the more similar the images are.
     *
     * @todo Use a better crop method (shape detection matching to center the crop).
     *
     * @param firstImage The first image to compare.
     * @param secondImage The second image to compare.
     * @param mask The mask to apply to the images.
     *
     * @return The similarity score.
     */
    static inline auto similarityScore(const cv::Mat& firstImage, const cv::Mat& secondImage, cv::InputArray& mask) -> double {
        cv::Mat firstImageCopy  = firstImage;
        cv::Mat secondImageCopy = secondImage;

        if (firstImage.rows != secondImage.rows || firstImage.cols != secondImage.cols) {
            LOG_DEBUG(Logger::getLogger(),
                      "The images size are not equals in similarity images computation ({}x{} != {}x{}), cropping the bigger one.",
                      firstImage.rows,
                      firstImage.cols,
                      secondImage.rows,
                      secondImage.cols);

            firstImageCopy  = firstImage.clone();
            secondImageCopy = secondImage.clone();
            cropCentered(firstImageCopy, secondImageCopy);
        }

        double similarity = cv::norm(firstImageCopy, secondImageCopy, cv::NORM_RELATIVE | cv::NORM_L2SQR, mask);

        return similarity;
    }

    static inline auto isSimilar(const cv::Mat&  firstImage,
                                 const cv::Mat&  secondImage,
                                 double          threshold = SIMILARITY_THRESHOLD,
                                 cv::InputArray& mask      = cv::noArray()) -> bool {
        return similarityScore(firstImage, secondImage, mask) <= threshold;
    }
}  // namespace Utilities::Image
