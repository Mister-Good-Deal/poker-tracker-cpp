#include "Round.hpp"

namespace GameHandler {
    using std::chrono::duration_cast;
    using std::chrono::seconds;

    using Action = RoundAction::ActionType;

    auto Round::operator=(const Round& other) -> Round& {
        if (this != &other) {
            actions        = other.actions;
            pot            = other.pot;
            currentStreet  = other.currentStreet;
            lastActionTime = other.lastActionTime;
        }

        return *this;
    }

    auto Round::operator=(Round&& other) noexcept -> Round& {
        if (this != &other) {
            actions        = std::move(other.actions);
            pot            = other.pot;
            currentStreet  = other.currentStreet;
            lastActionTime = other.lastActionTime;
        }

        return *this;
    }

    auto Round::endStreet() -> void {
        switch (currentStreet) {
            case Street::PREFLOP: currentStreet = Street::FLOP; break;
            case Street::FLOP: currentStreet = Street::TURN; break;
            case Street::TURN: currentStreet = Street::RIVER; break;
            case Street::RIVER: currentStreet = Street::SHOWDOWN; break;
            case Street::SHOWDOWN: break;
        }
    }

    auto Round::start() -> void { lastActionTime = system_clock::now(); }

    void Round::call(const Player& player, int32_t amount) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(Action::CALL, player, duration_cast<seconds>(now - lastActionTime), amount);

        lastActionTime = now;
    }

    void Round::bet(const Player& player, int32_t amount) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(Action::BET, player, duration_cast<seconds>(now - lastActionTime), amount);

        lastActionTime = now;
    }

    void Round::check(const Player& player) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(Action::CHECK, player, duration_cast<seconds>(now - lastActionTime));

        lastActionTime = now;
    }

    void Round::fold(const Player& player) {
        auto now   = system_clock::now();
        auto index = static_cast<int>(currentStreet);

        actions.at(index).emplace_back(Action::FOLD, player, duration_cast<seconds>(now - lastActionTime));

        lastActionTime = now;
    }
}  // namespace GameHandler