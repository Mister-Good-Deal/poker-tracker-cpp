#include "Logger.hpp"

namespace Logger {
    using quill::FilenameAppend::Date;

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) => https://github.com/llvm/llvm-project/issues/47384
    quill::Logger* Quill::_instance = nullptr;

    auto Quill::getLogger() -> quill::Logger* {
        if (_instance == nullptr)
        {
            _initLogger();
            _instance = quill::get_logger("custom");
        }

        return _instance;
    }

    auto Quill::_initLogger() -> void {
        quill::Config config;

        config.enable_console_colours = true;
        config.backend_thread_yield   = true;

        quill::configure(config);
        quill::start(true);

        auto fileHandler    = quill::file_handler(LOGGER_FILE_NAME, "a", Date);
        auto consoleHandler = quill::stdout_handler();

        fileHandler->set_pattern(LOG_PATTERN.data(), "%D %H:%M:%S");
        consoleHandler->set_pattern(LOG_PATTERN.data(), "%D %H:%M:%S");

        quill::Logger* logger = quill::create_logger("custom", {fileHandler, consoleHandler});

        // enable a backtrace that will get flushed when we log CRITICAL
        logger->init_backtrace(2, quill::LogLevel::Critical);
        logger->set_log_level(quill::LogLevel::TraceL3);
    }

}  // namespace Logger
