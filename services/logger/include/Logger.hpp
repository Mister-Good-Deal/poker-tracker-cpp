#pragma once

#include <quill/Quill.h>

namespace Logger {
    class Quill {
        public:
            Quill() = delete;

            static auto getLogger() -> std::shared_ptr<quill::Logger> const;

        private:
            static std::shared_ptr<quill::Logger> _logger;

            static auto _initLogger() -> quill::Logger*;
    };
}  // namespace Logger
