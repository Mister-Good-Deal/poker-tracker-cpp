#include "Logger.hpp"

namespace Logger {
    std::shared_ptr<quill::Logger> Quill::_logger = nullptr;

    auto Quill::getLogger() -> std::shared_ptr<quill::Logger> const {
        if (_logger == nullptr) { _logger = std::shared_ptr<quill::Logger>(_initLogger()); }

        return _logger;
    }

    auto Quill::_initLogger() -> quill::Logger* {
        quill::Config cfg;
        cfg.enable_console_colours = true;
        quill::configure(cfg);
        quill::start();

        auto* logger = quill::get_root_logger();

        // enable a backtrace that will get flushed when we log CRITICAL
        logger->init_backtrace(2, quill::LogLevel::Critical);

        return logger;
    }

}  // namespace Logger
