#include "Logger.hpp"

auto main() -> int {
    LOG_TRACE_L3(Logger::Quill::getLogger(), "This is a log trace l3 example {}", 1);
    LOG_TRACE_L2(Logger::Quill::getLogger(), "This is a log trace l2 example {} {}", 2, 2.3);
    LOG_TRACE_L1(Logger::Quill::getLogger(), "This is a log trace l1 {} example", "string");
    LOG_DEBUG(Logger::Quill::getLogger(), "This is a log debug example {}", 4);
    LOG_INFO(Logger::Quill::getLogger(), "This is a log info example {}", 5);
    LOG_WARNING(Logger::Quill::getLogger(), "This is a log warning example {}", 6);
    LOG_ERROR(Logger::Quill::getLogger(), "This is a log error example {}", 7);
    LOG_CRITICAL(Logger::Quill::getLogger(), "This is a log critical example {}", 8);
}
