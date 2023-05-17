#pragma once

namespace GameSession {
    class Session {
        public:
            Session()                     = default;
            Session(const Session& other) = default;
            Session(Session&& other) noexcept { *this = std::move(other); };

            virtual ~Session() = default;

            auto operator=(const Session& other) -> Session& = default;
            auto operator=(Session&& other) noexcept -> Session&;
    };
}  // namespace GameSession