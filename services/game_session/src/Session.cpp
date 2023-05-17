#include "Session.hpp"

namespace GameSession {
    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other)
        {
            // @todo, prefer using var = std::move(...);
        }

        return *this;
    }
}  // namespace GameSession