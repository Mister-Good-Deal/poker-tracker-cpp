#include "Logger.hpp"

namespace Logger {
    quill::Logger* Quill::_instance = nullptr;

    auto Quill::getLogger() -> quill::Logger* {
        if (_instance == nullptr)
        {
            _initLogger();
            _instance = quill::get_root_logger();
        }

        return _instance;
    }

    auto Quill::_initLogger() -> void {
        quill::Config cfg;
        cfg.enable_console_colours = true;
        quill::configure(cfg);
        quill::start();

        // enable a backtrace that will get flushed when we log CRITICAL
        quill::get_root_logger()->init_backtrace(2, quill::LogLevel::Critical);
    }

}  // namespace Logger
