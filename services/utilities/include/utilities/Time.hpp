#pragma once

#include <cstdint>

#include <chrono>

namespace Utilities::Time {
    static inline auto getMsTimestamp() -> int64_t {
        using std::chrono::milliseconds;
        using std::chrono::system_clock;

        milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

        return static_cast<int64_t>(ms.count());
    }
}  // namespace Utilities::Time
