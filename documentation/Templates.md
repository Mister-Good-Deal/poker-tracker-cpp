# Templates

Several template for classes, tests, etc. for CLion

## Class template

Implements [rule of five](https://en.cppreference.com/w/cpp/language/rule_of_three) C++ coding best practise.

**Header**

```c++
#parse("C File Header.h")
#pragma once

${NAMESPACES_OPEN}
    class ${NAME} {
        public:
            ${NAME}() = default;
            ${NAME}(const ${NAME}& other) = default;
            ${NAME}(${NAME}&& other) noexcept { *this = std::move(other); };
            
            virtual ~${NAME}() = default;
            
            auto operator=(const ${NAME}& other) -> ${NAME}& = default;
            auto operator=(${NAME}&& other) noexcept -> ${NAME}&;
    };
${NAMESPACES_CLOSE}
```

**Implementation**

```c++
#parse("C File Header.h")
#[[#include]]# "${HEADER_FILENAME}"

${NAMESPACES_OPEN_CPP}
    auto ${NAME}::operator=(${NAME}&& other) noexcept -> ${NAME}& {
        if (this != &other) {
            // @todo, prefer using var = std::move(...);
        }
        
        return *this;
    }
${NAMESPACES_CLOSE_CPP}
```

## Test template

GoogleTest template for testing a class.

**Implementation**

```c++
#[[#include]]# <gtest/gtest.h>

class ${FILE_NAME} : public ::testing::Test {};

TEST(${FILE_NAME}, todo) {
    // todo
}
```