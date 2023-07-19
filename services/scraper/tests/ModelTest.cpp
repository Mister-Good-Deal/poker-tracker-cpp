#include <gtest/gtest.h>

#include <Model.hpp>

using nlohmann::json;
using Scraper::Model;

class ScraperTest : public ::testing::Test {};

TEST(ModelTest, displayScreenshot) {
    Model scraper("Winamax", {3840, 1080});

    for (const auto& [id, window] : scraper.getActiveWindows())
    {
        fmt::print("({}) - {}\n", id, window.title);
        //        cv::imshow(window.title, scraper.getScreenshot(id));
    }

    //    cv::waitKey(-1);
}

TEST(ModelTest, displayAllElements) {
    Model scraper("Winamax", {3840, 1080});

    std::ifstream fileReader(std::string(WINAMAX_DIR) + "/3840x1080x8_coordinates.json");
    scraper.loadFromJson(json::parse(fileReader));

    cv::Mat screenshot_1 = cv::imread(std::string(WINAMAX_DIR) + "/screen_1.png");
    cv::Mat screenshot_2 = cv::imread(std::string(WINAMAX_DIR) + "/screen_2.png");
    cv::Mat screenshot_3 = cv::imread(std::string(WINAMAX_DIR) + "/screen_3.png");
    cv::Mat screenshot_4 = cv::imread(std::string(WINAMAX_DIR) + "/screen_4.png");
    cv::Mat screenshot_5 = cv::imread(std::string(WINAMAX_DIR) + "/screen_5.png");
    cv::Mat screenshot_6 = cv::imread(std::string(WINAMAX_DIR) + "/screen_6.png");

    cv::imshow("screen_1", scraper.getWindowElementsView(screenshot_1));
    cv::imshow("screen_2", scraper.getWindowElementsView(screenshot_2));
    cv::imshow("screen_3", scraper.getWindowElementsView(screenshot_3));
    cv::imshow("screen_4", scraper.getWindowElementsView(screenshot_4));
    cv::imshow("screen_5", scraper.getWindowElementsView(screenshot_5));
    cv::imshow("screen_6", scraper.getWindowElementsView(screenshot_6));
    //    cv::imshow("screen_6_player_1", scraper.getPlayer1NameImg(screenshot_6));
    //    cv::imshow("screen_6_player_2", scraper.getPlayer2NameImg(screenshot_6));

    cv::waitKey(-1);
}

TEST(ModelTest, checkPlayerButtonScapping) {
    Model scraper("Winamax", {3840, 1080});

    cv::Mat screenshot = cv::imread(std::string(WINAMAX_DIR) + "/screen_2.png");
    cv::Mat img_1      = scraper.getGameTimeImg(screenshot);
    //    cv::Mat img_2      = scraper.getPlayer2BetImg(screenshot);
    //    cv::Mat img_3 = scraper.getPlayer3ButtonImg(screenshot);

    cv::imshow("img_1", img_1);
    //    cv::imshow("img_2", img_2);
    //    cv::imshow("img_3", img_3);
    cv::waitKey(1);

    cv::imwrite(std::string(WINAMAX_DIR) + "/58s.png", img_1);
    //    cv::imwrite(std::string(WINAMAX_DIR) + "/betAmount_2.png", img_2);
}

TEST(ModelTest, jsonRepresentationShouldBeCorrect) {
    Model scraper("Winamax", {3840, 1080});

    scraper.setFirstCardCoord(cv::Rect(1, 2, 8, 12));
    scraper.setSecondCardCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPotCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPrizePoolCoord(cv::Rect(1, 2, 8, 12));
    scraper.setBoardCard1Coord(cv::Rect(1, 2, 8, 12));
    scraper.setBoardCard2Coord(cv::Rect(1, 2, 8, 12));
    scraper.setBoardCard3Coord(cv::Rect(1, 2, 8, 12));
    scraper.setBoardCard4Coord(cv::Rect(1, 2, 8, 12));
    scraper.setBoardCard5Coord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer1NameCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer2NameCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer3NameCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer1ButtonCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer2ButtonCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer3ButtonCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer1StackCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer2StackCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer3StackCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer1BetCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer2BetCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer3BetCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer2HandCoord(cv::Rect(1, 2, 8, 12));
    scraper.setPlayer3HandCoord(cv::Rect(1, 2, 8, 12));
    scraper.setBlindLevelCoord(cv::Rect(1, 2, 8, 12));
    scraper.setBlindAmountCoord(cv::Rect(1, 2, 8, 12));
    scraper.setGameTimeCoord(cv::Rect(1, 2, 8, 12));

    // language=json
    auto jsonExpected = R"(
        {
            "firstCardCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "secondCardCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "potCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "prizePoolCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "boardCard1Coord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "boardCard2Coord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "boardCard3Coord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "boardCard4Coord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "boardCard5Coord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player1NameCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player2NameCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player3NameCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player1ButtonCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player2ButtonCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player3ButtonCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player1StackCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player2StackCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player3StackCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player1BetCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player2BetCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player3BetCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player2HandCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "player3HandCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "blindLevelCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "blindAmountCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 },
            "gameTimeCoord": { "topLeft": { "x": 1, "y": 2 }, "width": 8, "height": 12 }
        }
    )"_json;

    ASSERT_EQ(scraper.toJson(), jsonExpected);
}
