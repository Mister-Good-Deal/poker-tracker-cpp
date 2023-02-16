#pragma once

#include <quill/Quill.h>

namespace Logger {
    class Quill {
        public:
            Quill() = delete;

            static auto getLogger() -> quill::Logger*;

        private:
            static quill::Logger* _logger;

            static auto _initLogger() -> quill::Logger*;
    };
}  // namespace Logger
