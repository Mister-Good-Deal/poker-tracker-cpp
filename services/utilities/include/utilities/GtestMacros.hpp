#pragma once

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <fmt/format.h>

namespace Utilities::Gtest {
    void compareJson(const nlohmann::json& json1, const nlohmann::json& json2, const std::string& path, std::string& differences) {
        if (json1.is_object() && json2.is_object()) {
            for (const auto& [key, value] : json1.items()) {
                auto currentPath = fmt::format("{}.{}", path, key);

                if (json2.contains(key)) {
                    compareJson(value, json2.at(key), currentPath, differences);
                } else {
                    differences += fmt::format("Missing key in json2: {}\n", currentPath);
                }
            }
            for (const auto& [key, value] : json2.items()) {
                auto currentPath = fmt::format("{}.{}", path, key);

                if (!json1.contains(key)) { differences += fmt::format("Extra key in json2: {} = {}\n", currentPath, value); }
            }
        } else if (json1.is_array() && json2.is_array()) {
            for (size_t i = 0; i < std::max(json1.size(), json2.size()); ++i) {
                auto currentPath = fmt::format("{}[{}]", path, i);

                if (i >= json1.size()) {
                    differences += fmt::format("Extra element in json2 at: {} => {}\n", currentPath, json2[i].dump());
                } else if (i >= json2.size()) {
                    differences += fmt::format("Missing element in json2 at: {}\n", currentPath);
                } else if (json1[i] != json2[i]) {
                    compareJson(json1[i], json2[i], currentPath, differences);
                }
            }
        } else if (json1 != json2) {
            differences += fmt::format("Difference at {}: Expected: {}, Actual: {}\n", path, json1.dump(), json2.dump());
        }
    }
}  // namespace Utilities::Gtest

#define EXPECT_THROW_WITH_MESSAGE(code, exception_type, expected_message) \
    do {                                                                  \
        EXPECT_THROW(                                                     \
            try { code; } catch (const exception_type& e) {               \
                EXPECT_STREQ(e.what(), expected_message);                 \
                throw;                                                    \
            },                                                            \
            exception_type);                                              \
    } while (0)

#define EXPECT_JSON_EQ(json1, json2)                                                               \
    do {                                                                                           \
        std::string differences;                                                                   \
                                                                                                   \
        Utilities::Gtest::compareJson(json1, json2, "Object", differences);                        \
                                                                                                   \
        if (!differences.empty()) { ADD_FAILURE() << "JSON differences found:\n" << differences; } \
    } while (0)