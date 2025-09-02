#include "doctest/doctest.h"

#include <imgui_vita.h>

#include <mutex>
#include <cstring>
#include <sstream>
#include <testStream.h>

#define printf(...)                             \
do {                                            \
    char buf[1024];                             \
    snprintf(buf, sizeof(buf), __VA_ARGS__);    \
    outputBuffer << buf;                        \
} while(0)                                      \

using namespace doctest;

struct VitaListener : public IReporter
{
    // caching pointers/references to objects of these types - safe to do
    std::ostream&         stdout_stream;
    const ContextOptions& opt;
    const TestCaseData*   tc;
    std::mutex            mutex;

    // constructor has to accept the ContextOptions by ref as a single argument
    VitaListener(const ContextOptions& in): stdout_stream(*in.cout), opt(in) {}

    void report_query(const QueryData& /*in*/) override {}

    void test_run_start() override {
        std::lock_guard<std::mutex> lock(mutex);
        printf("[doctest] doctest version is \"%d.%d.%d\"\n\n", DOCTEST_VERSION_MAJOR, DOCTEST_VERSION_MINOR, DOCTEST_VERSION_PATCH);
        printf("===============================================================================\n");
    }

    void test_run_end(const TestRunStats& stats) override {
        std::lock_guard<std::mutex> lock(mutex);

        printf("===============================================================================\n");
        printf("[doctest] test cases: %4d | %4d passed | %4d failed\n", stats.numTestCases, (stats.numTestCases - stats.numTestCasesFailed), stats.numTestCasesFailed);
        printf("[doctest] assertions: %4d | %4d passed | %4d failed\n", stats.numAsserts, (stats.numAsserts - stats.numAssertsFailed), stats.numAssertsFailed);

        if (stats.numTestCasesFailed > 0 || stats.numAssertsFailed > 0)
            printf("[doctest] Status: FAILURE!\n\n");
        else
            printf("[doctest] Status: PASSED!\n\n");
    }

    void test_case_start(const TestCaseData& in) override {
        tc = &in;

        const char* filename = std::strrchr(in.m_file.c_str(), '/');
        filename = filename ? filename + 1 : in.m_file.c_str();

        printf("\n%s\n", filename);
        if (tc)
            printf("TEST CASE:  %s\n\n", tc->m_name);
    }

    // called when a test case is reentered because of unfinished subcases
    void test_case_reenter(const TestCaseData& /*in*/) override {}

    void test_case_end(const CurrentTestCaseStats& in) override {}

    void test_case_exception(const TestCaseException& in) override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void subcase_start(const SubcaseSignature& in) override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void subcase_end() override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void log_assert(const AssertData& in) override {

        if(!in.m_failed && !opt.success)
            return;

        std::lock_guard<std::mutex> lock(mutex);

        const char* filename = std::strrchr(in.m_file, '/');
        filename = filename ? filename + 1 : in.m_file;

        if (in.m_failed) {
            printf("\n%s(%d) ERROR!\n", filename, in.m_line);
            printf("    CHECK(%s)\n\n", in.m_expr);
        }
    }

    void log_message(const MessageData& in) override {
        std::lock_guard<std::mutex> lock(mutex);
        printf("message:    %s\n", in.m_string.c_str());
        printf("location:   %s(%d)\n", in.m_file, in.m_line);
    }

    void test_case_skipped(const TestCaseData& in) override {
        std::lock_guard<std::mutex> lock(mutex);
        printf("[doctest] test case skipped: %s\n", in.m_name);
    }
};

REGISTER_LISTENER("VitaListener", 1, VitaListener);