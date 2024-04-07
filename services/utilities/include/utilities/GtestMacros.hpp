#pragma once

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <fmt/format.h>

namespace Utilities::Gtest {
    using nlohmann::json;

    void compareJson(const json& actualJson, const json& expectedJson, const std::string& path, std::string& differences) {
        if (actualJson.is_object() && expectedJson.is_object()) {
            for (const auto& [key, value] : actualJson.items()) {
                auto currentPath = fmt::format("{}.{}", path, key);

                if (expectedJson.contains(key)) {
                    compareJson(value, expectedJson.at(key), currentPath, differences);
                } else {
                    differences += fmt::format("Missing key in actual json: {}\n", currentPath);
                }
            }

            for (const auto& [key, value] : expectedJson.items()) {
                auto currentPath = fmt::format("{}.{}", path, key);

                if (!actualJson.contains(key)) {
                    differences += fmt::format("Extra key in expected json: {} = {}\n", currentPath, value);
                }
            }
        } else if (actualJson.is_array() && expectedJson.is_array()) {
            for (size_t i = 0; i < std::max(actualJson.size(), expectedJson.size()); ++i) {
                auto currentPath = fmt::format("{}[{}]", path, i);

                if (i >= actualJson.size()) {
                    differences += fmt::format("Extra element in expected json at: {} => {}\n", currentPath, expectedJson[i].dump());
                } else if (i >= expectedJson.size()) {
                    differences += fmt::format("Missing element in actual json at: {}\n", currentPath);
                } else if (actualJson[i] != expectedJson[i]) {
                    compareJson(actualJson[i], expectedJson[i], currentPath, differences);
                }
            }
        } else if (actualJson != expectedJson) {
            differences += fmt::format("Difference at {}: Expected: {}, Actual: {}\n", path, expectedJson.dump(), actualJson.dump());
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

#define EXPECT_JSON_EQ(actualJson, expectedJson)                                                   \
    do {                                                                                           \
        std::string differences;                                                                   \
                                                                                                   \
        Utilities::Gtest::compareJson(actualJson, expectedJson, "Object", differences);            \
                                                                                                   \
        if (!differences.empty()) { ADD_FAILURE() << "JSON differences found:\n" << differences; } \
    } while (0)
