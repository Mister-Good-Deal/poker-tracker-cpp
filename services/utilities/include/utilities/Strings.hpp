#pragma once

#include <cstdint>

#include <algorithm>
#include <charconv>
#include <stdexcept>
#include <string>

#include <string_view>

namespace Utilities::Strings {
    class InvalidNumberException : public std::runtime_error {
        public:
            explicit InvalidNumberException(const std::string& arg)
              : runtime_error(arg) {};
    };

    static inline auto constexpr ltrim(std::string& s) -> void {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    }

    static inline auto constexpr rtrim(std::string& s) -> void {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    }

    static inline auto constexpr fullTrim(std::string& s) -> void {
        s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char ch) { return std::isspace(ch); }), s.end());
    }

    static inline auto constexpr removeChar(std::string& s, char toRemove) -> void {
        s.erase(std::remove(s.begin(), s.end(), toRemove), s.end());
    }

    static inline auto constexpr replaceChar(std::string& s, char find, char replace) -> void {
        std::replace(s.begin(), s.end(), find, replace);
    }

    static inline auto constexpr trim(std::string& s) -> void {
        rtrim(s);
        ltrim(s);
    }

    static inline auto constexpr containsInCharacters(const std::string& str, const std::string& chars) -> bool {
        for (const char& c : chars) {
            if (str.find(c) != std::string::npos) { return true; }
        }

        return false;
    }

    static inline auto constexpr toFloat(std::string_view s) -> double {
        double value = 0.0;

        auto error = std::from_chars(s.begin(), s.end(), value);

        if (error.ec != std::errc()) { throw InvalidNumberException("Could not convert string to float"); }

        return value;
    }

    static inline auto constexpr toInt(std::string_view s) -> int32_t {
        int32_t value = 0;

        auto error = std::from_chars(s.begin(), s.end(), value);

        if (error.ec != std::errc()) { throw InvalidNumberException("Could not convert string to int"); }

        return value;
    }
}  // namespace Utilities::Strings