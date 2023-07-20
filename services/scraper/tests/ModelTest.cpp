#include <gtest/gtest.h>

#include <scraper/Model.hpp>

using nlohmann::json;
using Scraper::Model;
using Scraper::windowSize_t;

class ScraperTest : public ::testing::Test {};

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
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

    scraper.setFirstCardCoord(cv::Rect(2971, 4982, 89, 4023));
    scraper.setSecondCardCoord(cv::Rect(1125, 982, 2951, 3567));
    scraper.setPotCoord(cv::Rect(1234, 1467, 4261, 957));
    scraper.setPrizePoolCoord(cv::Rect(1073, 3568, 3274, 2348));
    scraper.setBoardCard1Coord(cv::Rect(4567, 4976, 1268, 4023));
    scraper.setBoardCard2Coord(cv::Rect(2093, 3785, 1067, 1235));
    scraper.setBoardCard3Coord(cv::Rect(3790, 2978, 3021, 2210));
    scraper.setBoardCard4Coord(cv::Rect(4567, 567, 1002, 4578));
    scraper.setBoardCard5Coord(cv::Rect(965, 4876, 1572, 3940));
    scraper.setPlayer1NameCoord(cv::Rect(3780, 4780, 2001, 3010));
    scraper.setPlayer2NameCoord(cv::Rect(2014, 1900, 3976, 3500));
    scraper.setPlayer3NameCoord(cv::Rect(157, 3470, 2450, 880));
    scraper.setPlayer1ButtonCoord(cv::Rect(3487, 1679, 4258, 4800));
    scraper.setPlayer2ButtonCoord(cv::Rect(101, 2909, 969, 2530));
    scraper.setPlayer3ButtonCoord(cv::Rect(1487, 3679, 3031, 201));
    scraper.setPlayer1StackCoord(cv::Rect(1028, 2190, 345, 489));
    scraper.setPlayer2StackCoord(cv::Rect(3245, 4580, 4056, 1890));
    scraper.setPlayer3StackCoord(cv::Rect(2301, 4809, 4670, 2450));
    scraper.setPlayer1BetCoord(cv::Rect(3097, 3678, 4679, 3510));
    scraper.setPlayer2BetCoord(cv::Rect(890, 2800, 4789, 1325));
    scraper.setPlayer3BetCoord(cv::Rect(4320, 4057, 1290, 3495));
    scraper.setPlayer2HandCoord(cv::Rect(1019, 3809, 3050, 120));
    scraper.setPlayer3HandCoord(cv::Rect(1235, 3909, 4503, 2364));
    scraper.setBlindLevelCoord(cv::Rect(1859, 4706, 1248, 4687));
    scraper.setBlindAmountCoord(cv::Rect(4307, 1090, 3150, 4300));
    scraper.setGameTimeCoord(cv::Rect(3265, 3897, 498, 2313));

    // language=json
    auto jsonExpected = R"(
        {
            "roomName": "Winamax",
            "windowSize": { "width": 3840, "height": 1080 },
            "elementsBoxes": {
                "firstCard": { "topLeft": { "x": 2971, "y": 4982 }, "width": 89, "height": 4023 },
                "secondCard": { "topLeft": { "x": 1125, "y": 982 }, "width": 2951, "height": 3567 },
                "pot": { "topLeft": { "x": 1234, "y": 1467 }, "width": 4261, "height": 957 },
                "prizePool": { "topLeft": { "x": 1073, "y": 3568 }, "width": 3274, "height": 2348 },
                "boardCard1": { "topLeft": { "x": 4567, "y": 4976 }, "width": 1268, "height": 4023 },
                "boardCard2": { "topLeft": { "x": 2093, "y": 3785 }, "width": 1067, "height": 1235 },
                "boardCard3": { "topLeft": { "x": 3790, "y": 2978 }, "width": 3021, "height": 2210 },
                "boardCard4": { "topLeft": { "x": 4567, "y": 567 }, "width": 1002, "height": 4578 },
                "boardCard5": { "topLeft": { "x": 965, "y": 4876 }, "width": 1572, "height": 3940 },
                "player1Name": { "topLeft": { "x": 3780, "y": 4780 }, "width": 2001, "height": 3010 },
                "player2Name": { "topLeft": { "x": 2014, "y": 1900 }, "width": 3976, "height": 3500 },
                "player3Name": { "topLeft": { "x": 157, "y": 3470 }, "width": 2450, "height": 880 },
                "player1Button": { "topLeft": { "x": 3487, "y": 1679 }, "width": 4258, "height": 4800 },
                "player2Button": { "topLeft": { "x": 101, "y": 2909 }, "width": 969, "height": 2530 },
                "player3Button": { "topLeft": { "x": 1487, "y": 3679 }, "width": 3031, "height": 201 },
                "player1Stack": { "topLeft": { "x": 1028, "y": 2190 }, "width": 345, "height": 489 },
                "player2Stack": { "topLeft": { "x": 3245, "y": 4580 }, "width": 4056, "height": 1890 },
                "player3Stack": { "topLeft": { "x": 2301, "y": 4809 }, "width": 4670, "height": 2450 },
                "player1Bet": { "topLeft": { "x": 3097, "y": 3678 }, "width": 4679, "height": 3510 },
                "player2Bet": { "topLeft": { "x": 890, "y": 2800 }, "width": 4789, "height": 1325 },
                "player3Bet": { "topLeft": { "x": 4320, "y": 4057 }, "width": 1290, "height": 3495 },
                "player2Hand": { "topLeft": { "x": 1019, "y": 3809 }, "width": 3050, "height": 120 },
                "player3Hand": { "topLeft": { "x": 1235, "y": 3909 }, "width": 4503, "height": 2364 },
                "blindLevel": { "topLeft": { "x": 1859, "y": 4706 }, "width": 1248, "height": 4687 },
                "blindAmount": { "topLeft": { "x": 4307, "y": 1090 }, "width": 3150, "height": 4300 },
                "gameTime": { "topLeft": { "x": 3265, "y": 3897 }, "width": 498, "height": 2313 }
             }
         }
    )"_json;

    EXPECT_EQ(scraper.toJson(), jsonExpected);
}

TEST(ModelTest, loadFromJsonShouldSetCorrectAttributes) {
    Model scraper("Winamax", {3840, 1080});

    // language=json
    auto jsonInput = R"(
        {
            "roomName": "Unibet",
            "windowSize": { "width": 1280, "height": 720 },
            "elementsBoxes": {
                "firstCard": { "topLeft": { "x": 1234, "y": 2345 }, "width": 3456, "height": 4567 },
                "secondCard": { "topLeft": { "x": 2345, "y": 3465 }, "width": 432, "height": 546 },
                "pot": { "topLeft": { "x": 3456, "y": 4567 }, "width": 5467, "height": 1234 },
                "prizePool": { "topLeft": { "x": 4567, "y": 1234 }, "width": 1234, "height": 2345 },
                "boardCard1": { "topLeft": { "x": 123, "y": 456 }, "width": 789, "height": 1011 },
                "boardCard2": { "topLeft": { "x": 456, "y": 789 }, "width": 1011, "height": 1213 },
                "boardCard3": { "topLeft": { "x": 789, "y": 1011 }, "width": 1213, "height": 1415 },
                "boardCard4": { "topLeft": { "x": 1011, "y": 1213 }, "width": 1415, "height": 1617 },
                "boardCard5": { "topLeft": { "x": 1213, "y": 1415 }, "width": 1617, "height": 1819 },
                "player1Name": { "topLeft": { "x": 1415, "y": 1617 }, "width": 1819, "height": 2011 },
                "player2Name": { "topLeft": { "x": 1617, "y": 1819 }, "width": 2011, "height": 2113 },
                "player3Name": { "topLeft": { "x": 1819, "y": 2011 }, "width": 2113, "height": 2215 },
                "player1Button": { "topLeft": { "x": 2011, "y": 2215 }, "width": 2317, "height": 2419 },
                "player2Button": { "topLeft": { "x": 2215, "y": 2317 }, "width": 2419, "height": 2521 },
                "player3Button": { "topLeft": { "x": 2317, "y": 2521 }, "width": 2623, "height": 2725 },
                "player1Stack": { "topLeft": { "x": 2419, "y": 2623 }, "width": 2725, "height": 2827 },
                "player2Stack": { "topLeft": { "x": 2521, "y": 2725 }, "width": 2827, "height": 2929 },
                "player3Stack": { "topLeft": { "x": 2623, "y": 2827 }, "width": 2929, "height": 3131 },
                "player1Bet": { "topLeft": { "x": 2725, "y": 2929 }, "width": 3131, "height": 3233 },
                "player2Bet": { "topLeft": { "x": 2827, "y": 3131 }, "width": 3233, "height": 3335 },
                "player3Bet": { "topLeft": { "x": 2929, "y": 3233 }, "width": 3335, "height": 3437 },
                "player2Hand": { "topLeft": { "x": 3131, "y": 3335 }, "width": 3437, "height": 3539 },
                "player3Hand": { "topLeft": { "x": 3233, "y": 3437 }, "width": 3621, "height": 3623 },
                "blindLevel": { "topLeft": { "x": 3335, "y": 3539 }, "width": 3635, "height": 3637 },
                "blindAmount": { "topLeft": { "x": 3437, "y": 3635 }, "width": 3637, "height": 3739 },
                "gameTime": { "topLeft": { "x": 3539, "y": 3839 }, "width": 3941, "height": 4043 }
            }
        }
    )"_json;

    scraper.loadFromJson(jsonInput);

    EXPECT_EQ(scraper.getRoomName(), "Unibet");
    EXPECT_EQ(scraper.getWindowSize(), windowSize_t(1280, 720));
    EXPECT_EQ(scraper.getFirstCardCoord(), cv::Rect(1234, 2345, 3456, 4567));
    EXPECT_EQ(scraper.getSecondCardCoord(), cv::Rect(2345, 3465, 432, 546));
    EXPECT_EQ(scraper.getPotCoord(), cv::Rect(3456, 4567, 5467, 1234));
    EXPECT_EQ(scraper.getPrizePoolCoord(), cv::Rect(4567, 1234, 1234, 2345));
    EXPECT_EQ(scraper.getBoardCard1Coord(), cv::Rect(123, 456, 789, 1011));
    EXPECT_EQ(scraper.getBoardCard2Coord(), cv::Rect(456, 789, 1011, 1213));
    EXPECT_EQ(scraper.getBoardCard3Coord(), cv::Rect(789, 1011, 1213, 1415));
    EXPECT_EQ(scraper.getBoardCard4Coord(), cv::Rect(1011, 1213, 1415, 1617));
    EXPECT_EQ(scraper.getBoardCard5Coord(), cv::Rect(1213, 1415, 1617, 1819));
    EXPECT_EQ(scraper.getPlayer1NameCoord(), cv::Rect(1415, 1617, 1819, 2011));
    EXPECT_EQ(scraper.getPlayer2NameCoord(), cv::Rect(1617, 1819, 2011, 2113));
    EXPECT_EQ(scraper.getPlayer3NameCoord(), cv::Rect(1819, 2011, 2113, 2215));
    EXPECT_EQ(scraper.getPlayer1ButtonCoord(), cv::Rect(2011, 2215, 2317, 2419));
    EXPECT_EQ(scraper.getPlayer2ButtonCoord(), cv::Rect(2215, 2317, 2419, 2521));
    EXPECT_EQ(scraper.getPlayer3ButtonCoord(), cv::Rect(2317, 2521, 2623, 2725));
    EXPECT_EQ(scraper.getPlayer1StackCoord(), cv::Rect(2419, 2623, 2725, 2827));
    EXPECT_EQ(scraper.getPlayer2StackCoord(), cv::Rect(2521, 2725, 2827, 2929));
    EXPECT_EQ(scraper.getPlayer3StackCoord(), cv::Rect(2623, 2827, 2929, 3131));
    EXPECT_EQ(scraper.getPlayer1BetCoord(), cv::Rect(2725, 2929, 3131, 3233));
    EXPECT_EQ(scraper.getPlayer2BetCoord(), cv::Rect(2827, 3131, 3233, 3335));
    EXPECT_EQ(scraper.getPlayer3BetCoord(), cv::Rect(2929, 3233, 3335, 3437));
    EXPECT_EQ(scraper.getPlayer2HandCoord(), cv::Rect(3131, 3335, 3437, 3539));
    EXPECT_EQ(scraper.getPlayer3HandCoord(), cv::Rect(3233, 3437, 3621, 3623));
    EXPECT_EQ(scraper.getBlindLevelCoord(), cv::Rect(3335, 3539, 3635, 3637));
    EXPECT_EQ(scraper.getBlindAmountCoord(), cv::Rect(3437, 3635, 3637, 3739));
    EXPECT_EQ(scraper.getGameTimeCoord(), cv::Rect(3539, 3839, 3941, 4043));
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)