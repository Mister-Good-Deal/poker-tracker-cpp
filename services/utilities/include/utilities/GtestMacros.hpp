#pragma once

#include <gtest/gtest.h>

#define EXPECT_THROW_WITH_MESSAGE(code, exception_type, expected_message) \
    do {                                                                  \
        EXPECT_THROW(                                                     \
            try { code; } catch (const exception_type& e) {               \
                EXPECT_STREQ(e.what(), expected_message);                 \
                throw;                                                    \
            },                                                            \
            exception_type);                                              \
    } while (0)
