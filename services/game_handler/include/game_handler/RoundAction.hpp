#pragma once

#include <chrono>

#include <game_handler/Player.hpp>

namespace GameHandler {
    using std::chrono::seconds;

    class RoundAction {
        public:
            enum class ActionType : int32_t { CHECK = 0, CALL, BET, FOLD, NONE };

            static constexpr auto actionToString(ActionType action) -> std::string {
                switch (action) {
                    case ActionType::CHECK: return "Check";
                    case ActionType::CALL: return "Call";
                    case ActionType::BET: return "Bet";
                    case ActionType::FOLD: return "Fold";
                    case ActionType::NONE: return "None";
                    default: throw std::invalid_argument("The given action is invalid");
                }
            }

            RoundAction(const RoundAction& other)     = default;
            RoundAction(RoundAction&& other) noexcept = default;
            RoundAction(ActionType action, Player player, seconds time, int32_t amount = 0) :
                _action(action), _player(std::move(player)), _time(time), _amount(amount){};

            virtual ~RoundAction() = default;

            auto operator=(const RoundAction& other) -> RoundAction& = default;
            auto operator=(RoundAction&& other) noexcept -> RoundAction&;

            [[nodiscard]] auto toJson() const -> json;

        private:
            ActionType _action;
            Player     _player;
            seconds    _time;
            int32_t    _amount = 0;
    };
}  // namespace GameHandler
