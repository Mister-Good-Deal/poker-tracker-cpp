#pragma once

#include <cstdint>

#include <chrono>

namespace Utilities::Time {
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;

    static inline auto getMsTimestamp() -> int64_t {
        using std::chrono::milliseconds;
        using std::chrono::system_clock;

        milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

        return static_cast<int64_t>(ms.count());
    }

    static inline auto now() -> time_point { return std::chrono::steady_clock::now(); }
}  // namespace Utilities::Time
