#include "Round.hpp"

namespace GameHandler {
    using std::chrono::duration_cast;
    using std::chrono::seconds;

    using enum RoundAction::ActionType;
    using enum Round::Street;

    auto Round::operator=(const Round& other) -> Round& {
        if (this != &other)
        {
            actions        = other.actions;
            pot            = other.pot;
            currentStreet  = other.currentStreet;
            lastActionTime = other.lastActionTime;
        }

        return *this;
    }

    auto Round::operator=(Round&& other) noexcept -> Round& {
        if (this != &other)
        {
            actions        = std::move(other.actions);
            pot            = other.pot;
            currentStreet  = other.currentStreet;
            lastActionTime = other.lastActionTime;
        }

        return *this;
    }

    auto Round::endStreet() -> void {
        switch (currentStreet)
        {
            case PREFLOP: currentStreet = FLOP; break;
            case FLOP: currentStreet = TURN; break;
            case TURN: currentStreet = RIVER; break;
            case RIVER: currentStreet = SHOWDOWN; break;
            case SHOWDOWN: break;
        }
    }

    auto Round::start() -> void { lastActionTime = system_clock::now(); }

    void Round::call(const Player& player, int32_t amount) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(CALL, player, duration_cast<seconds>(now - lastActionTime), amount);

        lastActionTime = now;
    }

    void Round::bet(const Player& player, int32_t amount) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(BET, player, duration_cast<seconds>(now - lastActionTime), amount);

        lastActionTime = now;
    }

    void Round::check(const Player& player) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(CHECK, player, duration_cast<seconds>(now - lastActionTime));

        lastActionTime = now;
    }

    void Round::fold(const Player& player) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(FOLD, player, duration_cast<seconds>(now - lastActionTime));

        lastActionTime = now;
    }
}  // namespace GameHandler