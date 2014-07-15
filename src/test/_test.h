#include "base.h"

typedef struct Result Result;

struct Result
{
    int pass;
    int fail;
};

# define START Result test_result = {0, 0};
# define PASS test_result.pass++
# define FAIL test_result.fail++
# define END  result(test_result); return (test_result.fail == 0);
# define ABORT test_result.fail = -1; END

# define TEST(x) \
    if (!bse_quiet_exceptions) { print_test("TEST", __FILE__, __LINE__, __STRING(x)); } \
    if (x) { PASS; } else { report(__FILE__, __LINE__, __STRING(x)); FAIL; }
# define TEST_FATAL(x) \
    if (!bse_quiet_exceptions) { print_test("TEST_FATAL", __FILE__, __LINE__, __STRING(x)); } \
    if (x) { PASS; } else { report(__FILE__, __LINE__, __STRING(x)); ABORT; }
# define TEST_FAIL(s) \
    report(__FILE__, __LINE__, "test requirement not met: " s); \
    FAIL;
# define TEST_FAIL_FATAL(s) \
    TEST_FAIL(s) \
    ABORT)

void report(const char *file, int line, const char *expr);
void print_test(const char *type, const char *file, int line, const char *expr);
void result(Result r);
