#pragma once

#include <chrono>

#include <game_handler/Player.hpp>

namespace GameHandler {
    using std::chrono::seconds;

    class RoundAction {
        public:
            enum class ActionType : int32_t { CHECK = 0, CALL, BET, RAISE, FOLD, PAY_SMALL_BLIND, PAY_BIG_BLIND, NONE };

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

            [[nodiscard]] auto _requiresAmount(ActionType action) const -> bool;
    };
}  // namespace GameHandler

// Custom formatter for Position
namespace fmt {
    using GameHandler::RoundAction;

    template<>
    struct formatter<RoundAction::ActionType> : formatter<string_view> {
            // parse is inherited from formatter<string_view>.
            template<typename FormatContext>
            auto format(RoundAction::ActionType action, FormatContext& ctx) const {
                string_view name = "unknown";

                switch (action) {
                    case RoundAction::ActionType::CHECK: name = "Check"; break;
                    case RoundAction::ActionType::CALL: name = "Call"; break;
                    case RoundAction::ActionType::BET: name = "Bet"; break;
                    case RoundAction::ActionType::RAISE: name = "Raise"; break;
                    case RoundAction::ActionType::FOLD: name = "Fold"; break;
                    case RoundAction::ActionType::PAY_SMALL_BLIND: name = "Pay small blind"; break;
                    case RoundAction::ActionType::PAY_BIG_BLIND: name = "Pay big blind"; break;
                    case RoundAction::ActionType::NONE: name = "None"; break;
                }

                return formatter<string_view>::format(name, ctx);
            }
    };
}  // namespace fmt
