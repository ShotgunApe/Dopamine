#include "doctest/doctest.h"
#include "debugScreen.h"

#include <mutex>

#define printf psvDebugScreenPrintf

using namespace doctest;

struct VitaListener : public IReporter
{
    // caching pointers/references to objects of these types - safe to do
    std::ostream&         stdout_stream;
    const ContextOptions& opt;
    const TestCaseData*   tc;
    std::mutex            mutex;

    // constructor has to accept the ContextOptions by ref as a single argument
    VitaListener(const ContextOptions& in)
            : stdout_stream(*in.cout)
            , opt(in) {}

    void report_query(const QueryData& /*in*/) override {}

    void test_run_start() override {
        std::lock_guard<std::mutex> lock(mutex);
        printf('[doctest] doctest version is "%d.%d.%d"\n', DOCTEST_VERSION_MAJOR, DOCTEST_VERSION_MINOR, DOCTEST_VERSION_PATCH);
        printf("===============================================================================\n");
    }

    void test_run_end(const TestRunStats& stats) override {
        std::lock_guard<std::mutex> lock(mutex);

        printf("===============================================================================\n\n");
        printf("[doctest] test cases: %6d | %6d passed | %6d failed\n", stats.numTestCases, (stats.numTestCases - stats.numTestCasesFailed), stats.numTestCasesFailed);
        printf("[doctest] assertions: %6d | %6d passed | %6d failed\n", stats.numAsserts, (stats.numAsserts - stats.numAssertsFailed), stats.numAssertsFailed);

        if (stats.numTestCasesFailed > 0 || stats.numAssertsFailed > 0)
            printf("Status: FAILURE!\n");
        else
            printf("Status: PASSED!\n");
    }

    void test_case_start(const TestCaseData& in) override { tc = &in; }

    // called when a test case is reentered because of unfinished subcases
    void test_case_reenter(const TestCaseData& /*in*/) override {}

    void test_case_end(const CurrentTestCaseStats& in) override {
        if (in.failure_flags) {
            printf("test case:  %s\n", tc->m_name);
            printf("status:     FAILED\n");
        }
    }

    void test_case_exception(const TestCaseException& in) override {
        std::lock_guard<std::mutex> lock(mutex);
        printf("test case:  %s\n", tc ? tc->m_name : "Unknown");
        printf("exception:  %s\n", in.error_string);
        printf("status:     FAILED due to exception\n");
    }

    void subcase_start(const SubcaseSignature& in) override {
        std::lock_guard<std::mutex> lock(mutex);
        printf("  subcase:  %s\n", in.m_name);
    }

    void subcase_end() override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void log_assert(const AssertData& in) override {

        if(!in.m_failed && !opt.success)
            return;

        std::lock_guard<std::mutex> lock(mutex);

        printf("line:       %d\n", in.m_line);
        if (tc)
            printf("test case:  %s\n", tc->m_name);

        if (in.m_expr)
            printf("expression: %s\n", in.m_expr);

        printf("result:     %s\n", in.m_failed ? "FAILED" : "PASSED");
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