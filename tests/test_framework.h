#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class TestFailure : public std::runtime_error {
public:
    explicit TestFailure(const std::string& message)
        : std::runtime_error(message) {}
};

struct TestCaseInfo {
    std::string name;
    std::function<void()> body;
};

inline std::vector<TestCaseInfo>& testRegistry() {
    static std::vector<TestCaseInfo> tests;
    return tests;
}

class TestRegistrar {
public:
    TestRegistrar(const std::string& name, std::function<void()> body) {
        testRegistry().push_back({name, body});
    }
};

inline void failCheck(const char* expression,
                      const char* file,
                      int line,
                      const std::string& details = "") {
    std::ostringstream message;
    message << file << ":" << line << " CHECK failed: " << expression;
    if (!details.empty()) {
        message << " (" << details << ")";
    }
    throw TestFailure(message.str());
}

template <typename Actual, typename Expected>
inline void checkEqual(const Actual& actual,
                       const Expected& expected,
                       const char* actualExpression,
                       const char* expectedExpression,
                       const char* file,
                       int line) {
    if (!(actual == expected)) {
        std::ostringstream details;
        details << actualExpression << " != " << expectedExpression
                << ", actual=" << actual << ", expected=" << expected;
        failCheck("equality", file, line, details.str());
    }
}

inline void checkContains(const std::string& text,
                          const std::string& expectedSubstring,
                          const char* file,
                          int line) {
    if (text.find(expectedSubstring) == std::string::npos) {
        std::ostringstream details;
        details << "text=\"" << text << "\", missing=\""
                << expectedSubstring << "\"";
        failCheck("contains", file, line, details.str());
    }
}

inline void checkFileContains(const std::string& path,
                              const std::string& expectedSubstring,
                              const char* file,
                              int line) {
    std::ifstream input(path);
    if (!input.is_open()) {
        failCheck("file open", file, line, "path=\"" + path + "\"");
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    checkContains(buffer.str(), expectedSubstring, file, line);
}

inline int runAllTests() {
    int failed = 0;
    const std::vector<TestCaseInfo>& tests = testRegistry();

    std::cout << "Running " << tests.size() << " test(s)\n";

    for (const TestCaseInfo& test : tests) {
        try {
            test.body();
            std::cout << "[PASS] " << test.name << "\n";
        } catch (const std::exception& ex) {
            failed++;
            std::cout << "[FAIL] " << test.name << "\n"
                      << "       " << ex.what() << "\n";
        } catch (...) {
            failed++;
            std::cout << "[FAIL] " << test.name << "\n"
                      << "       Unknown exception\n";
        }
    }

    std::cout << "\nResult: " << (tests.size() - failed)
              << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}

#define TEST_CONCAT_IMPL(a, b) a##b
#define TEST_CONCAT(a, b) TEST_CONCAT_IMPL(a, b)

#define TEST_CASE(name) \
    static void TEST_CONCAT(testBody_, __LINE__)(); \
    static TestRegistrar TEST_CONCAT(testRegistrar_, __LINE__)(name, TEST_CONCAT(testBody_, __LINE__)); \
    static void TEST_CONCAT(testBody_, __LINE__)()

#define CHECK(expression) \
    do { \
        if (!(expression)) { \
            failCheck(#expression, __FILE__, __LINE__); \
        } \
    } while (false)

#define REQUIRE(expression) CHECK(expression)

#define CHECK_EQ(actual, expected) \
    do { \
        checkEqual((actual), (expected), #actual, #expected, __FILE__, __LINE__); \
    } while (false)

#define CHECK_CONTAINS(text, expectedSubstring) \
    do { \
        checkContains((text), (expectedSubstring), __FILE__, __LINE__); \
    } while (false)

#define CHECK_FILE_CONTAINS(path, expectedSubstring) \
    do { \
        checkFileContains((path), (expectedSubstring), __FILE__, __LINE__); \
    } while (false)

#endif
