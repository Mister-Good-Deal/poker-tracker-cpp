#pragma once

#include <quill/Quill.h>

namespace Logger {
    class Quill {
        public:
            static constexpr std::string_view LOGGER_FILE_NAME = "app.log";
            // Formatters names in quill/src/PatternFormatter.cpp
            static constexpr std::string_view LOG_PATTERN =
                "%(level_name:<9) %(ascii_time)\t[%(thread_name:<12)]\t%(fileline:<25) - %(message)";

            Quill() = delete;

            static auto getLogger() -> quill::Logger*;

        private:
            // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) => https://github.com/llvm/llvm-project/issues/47384
            static quill::Logger* _instance;

            static auto _initLogger() -> void;
    };
}  // namespace Logger
