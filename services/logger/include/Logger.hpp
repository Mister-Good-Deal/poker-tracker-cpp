#pragma once

#include <quill/Quill.h>

namespace Logger {
    class Quill {
        public:
            Quill() = delete;

            static auto getLogger() -> quill::Logger*;

        private:
            // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) => https://github.com/llvm/llvm-project/issues/47384
            static quill::Logger* _instance;

            static auto _initLogger() -> void;
    };
}  // namespace Logger
