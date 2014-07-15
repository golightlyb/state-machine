/*

 src/test/_host.h - Simple sandboxing test harness.
 
 ------------------------------------------------------------------------------
 
 Copyright (c) 2014 Ben Golightly <golightly.ben@googlemail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ------------------------------------------------------------------------------
 
 This is a simple test framework that uses fork to execute tests in a child
 process so that abnormal termination (e.g. a segfault) can be caught and
 treated as a failure.
 
 See also: src/test/_plan.h which details the functions that are executed by
 separate forked processes, in order, and /src/test/ *.c which are the actual
 implementations of the tests.
 
 20140610:
 * _test.h TEST_FAIL changed to accept a string and TEST_FAIL_FATAL added
 * _host.c add usage and parse argc for --verbose
 
*/

/* Test cases may call a function with invalid input or state and verify that
 * the function returns a correct error code. However, the implementation may
 * print error or warning messages. This is permitted behaviour but is not
 * useful in automated testing. Therefore these messages will be suppressed by
 * default unless the --verbose command line argument is used for test
 * and the ID of the test is also included as a command line argument.
 * e.g. test-linux64 --verbose test_1 test_2 test_3 */


#ifdef BSE_WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>     // strlen
#include "base.h"       // X (exception)
#include "test/_test.h"

int run(int (*fp)(void), int verbose)
{
    bse_quiet_exceptions = (0 == verbose); // suppress all errors from src/base.c
    int retval = fp();
    bse_quiet_exceptions = 0; // turn error reporting back on
    return retval;
}

#endif


#ifdef BSE_LINUX

#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid
#include <stdio.h>      // printf
#include <stdlib.h>     // _Exit
#include <unistd.h>     // fork
#include <errno.h>
#include <string.h>     // strlen
#include "base.h"       // X (exception)
#include "test/_test.h"


int run(int (*fp)(void), int verbose)
{
    pid_t pid = fork();
    
    if (pid == -1)
    {
        switch (errno)
        {
            case EAGAIN: X2(fork, "resource limit encountered");
            case ENOMEM: X2(fork, "not enough memory to attempt");
            case ENOSYS: X2(fork, "unsupported on this platform");
            default:     X2(fork, "unknown error");
        }
    }
    else if (pid == 0) // in child
    {
        bse_quiet_exceptions = (0 == verbose); // suppress all errors from src/base.c
        int result = fp();
        bse_quiet_exceptions = 0; // turn error reporting back on
        
        _Exit(result);
    }
    else // in parent where pid == child pid
    {
        int status;
        
        if (-1 == waitpid(pid, &status, 0))
        {
            switch (errno)
            {
                case ECHILD: X2(wait, "no waitable child process");
                case EINTR:  X2(wait, "unblocked signal or SIGCHLD caught");
                case EINVAL: X2(wait, "invalid options argument to waitpid");
                default:     X2(wait, "unknown error");
            }
        }
        
        if (!WIFEXITED(status)) { X2(wait, "child process terminated abnormally"); }
        return (int) (WEXITSTATUS(status));
    }
    
    err_wait:
    err_fork:
        return 0;
}
#endif


int test(int (*fp)(void), const char *name, const char *desc, int verbose)
{
    char align[16];
    size_t i = strlen(name);
    
    if (i < 22) { i = 22 - i; } else { i = 0; }
    
    align[i] = '\0';
    while (i) { i--; align[i] = ' '; }
    
    fflush(stdout);
    printf("[%s]%s Testing %s ... ", name, align, desc);
    if (verbose) { printf("VERBOSE\n"); }
    fflush(stdout);
    
    return (run(fp, verbose));
}


void print_test(const char *type, const char *file, int line, const char *expr)
{
    // in verbose mode
    fflush(stdout);
    printf("test: %s:%d: %s(%s)\n", file, line, type, expr);
    fflush(stdout);
}


void report(const char *file, int line, const char *expr)
{
    fflush(stdout);
    printf("\nFAIL: %s:%d: (%s)", file, line, expr);
    fflush(stdout);
}


void result(Result r)
{
    fflush(stdout);
    
    if (r.fail == -1)
    {
        printf("\nABORT (%d/\?\?\? pass)\n", r.pass);
    }
    else if (r.fail > 0)
    {
        printf("\nFAIL: %d passes, %d fails.\n", r.pass, r.fail);
    }
    else
    {
        printf("PASS (%d)\n", r.pass);
    }
    
    fflush(stdout);
}


int contains(int argc, char *argv[], const char *key)
{
    for (int i = 1; i < argc; i++)
    {
        if (0 == strcmp(argv[i], key)) { printf("contains %s\n", key); return 1; }
    }
    
    return 0;
}


int main(int argc, char *argv[])
{
    int pass = 1;
    int mode_verbose = 0;
    
    if (argc >= 2)
    {
        if (0 == strcmp(argv[1], "--verbose"))
        {
            mode_verbose = 1;
        }
        else
        {
            printf("Usage: %s [--verbose [TESTS]]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    
#   define V(a) \
        
    
#   define T(a, b) \
        int a(void); \
        pass = test(a, __STRING(a), b, \
            mode_verbose && contains(argc, argv, __STRING(a))) && pass;
        
#   include "_plan.h"
    
    fflush(stdout);
    if (pass)
    {
        printf("\nOVERALL SUCCESS.\n");
        return EXIT_SUCCESS;
    }
    else
    {
        printf("\nOVERALL FAIL.\n");
        return EXIT_FAILURE;
    }
}

