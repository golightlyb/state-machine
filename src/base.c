/*

 src/base.c - default callbacks and exceptions.
 
 ------------------------------------------------------------------------------
 
 This file is in the public domain; copyright has been disclaimed by the
 author.
 
 ------------------------------------------------------------------------------
 
 This is called by macros defined in base.h; you will not call these functions
 directly.
 
*/

#include "base.h"
#include <stdio.h> // fprintf
#include <string.h>
#include <errno.h>
#include <malloc.h>

#ifdef BSE_GRAPHICAL_EXCEPTIONS
#   ifndef BSE_WINDOWS
#       error BSE_GRAPHICAL_EXCEPTIONS requires you define BSE_WINDOWS
#   endif
#   define STRICT
#   define WIN32_LEAN_AND_MEAN // ew
#   include <windows.h>
#   include <stdlib.h> // exit
#endif

int bse_quiet_exceptions = 0; // the test harness can set this to 1 to suppress


void *bse_default_malloc(size_t size, void *arg)
{
    UNUSED(arg);
    return malloc(size);
}


void bse_default_free(void *ptr, size_t size, void *arg)
{
    UNUSED(size);
    UNUSED(arg);
    free(ptr);
}


void bse_print_exception
(
    const char *type,
    const char *file,
    unsigned int line,
    const char *func,
    const char *label,
    const char *msg,
    int errnum
)
{
    if (bse_quiet_exceptions) { return; }
    char errnobuf[256];
    
    if (!errnum)
    {
        sprintf(errnobuf, "(no errno)");
    }
    else
    {
        strncpy(errnobuf, strerror(errnum), 256);
        errnobuf[255] = '\0';
    }
    
    if (!msg)
    {
        fprintf
        (
            stderr,
            PROG_ID ": %s:%u %s: %s 'err_%s' at line %u\n"
            "    (errno: %d, '%s')\n",
              file, line, func, type, label, line,
              errnum, errnobuf
        );
    }
    else
    {
        fprintf
        (
            stderr,
            PROG_ID ": %s:%u %s: %s 'err_%s' at line %u\n"
            "    (errno: %d, '%s')\n"
            "    (detail: '%s')\n",
              file, line, func, type, label, line,
              errnum, errnobuf,
              msg
        );
    }
    
    fflush(stderr);
    
#   ifdef BSE_GRAPHICAL_EXCEPTIONS
        char buf[4096];
        
        snprintf
        (
            buf,
            4096,
            "An error has occurred in this application. "
            "The following information was provided about the error:\r\n\r\n"
            
            "%s ID: err_%s\r\n"
            "Location: %s:%u\r\n"
            "Within: %s\r\n"
            "Errno: %d, '%s'\r\n"
            "Detail: %s\r\n\r\n"
            
            "Command line:\r\n\"%s\"\r\n\r\n"
            
            "The application may be able to recover or exit gracefully. "
            "Press 'Okay' to continue, or 'Cancel' to terminate this "
            "application immediately.\r\n\r\n"
            ,
            type, label,
            file, line,
            func,
            errnum, errnobuf,
            msg,
            GetCommandLine()
        );

        if (IDCANCEL == MessageBox
        (
            NULL,
            buf,
            "Error ("PROG_ID")",
            MB_OKCANCEL | MB_ICONWARNING | MB_APPLMODAL | MB_DEFBUTTON1
        ))
        { exit(1); }
        
#   endif
}


void bse_print_warning
(
    const char *type,
    const char *file,
    unsigned int line,
    const char *func,
    const char *msg,
    int errnum
)
{
    if (bse_quiet_exceptions) { return; }
    char errnobuf[256];
    
    if (!errnum)
    {
        sprintf(errnobuf, "(no errno)");
    }
    else
    {
        strncpy(errnobuf, strerror(errnum), 256);
        errnobuf[255] = '\0';
    }
    
    if (!msg)
    {
        fprintf
        (
            stderr,
            PROG_ID ": %s:%u %s: %s at line %u\n"
            "    (errno: %d, '%s')\n",
              file, line, func, type, line,
              errnum, errnobuf
        );
    }
    else
    {
        fprintf
        (
            stderr,
            PROG_ID ": %s:%u %s: %s at line %u\n"
            "    (errno: %d, '%s')\n"
            "    (detail: '%s')\n",
              file, line, func, type, line,
              errnum, errnobuf,
              msg
        );
    }
    
    fflush(stderr);
    
#   ifdef BSE_GRAPHICAL_EXCEPTIONS
        char buf[4096];
        
        snprintf
        (
            buf,
            4096,
            "This application has encountered an unexpected problem that it "
            "believes you should know about. "
            "The following information was provided:\r\n\r\n"
            
            "Location: %s:%u\r\n"
            "Within: %s\r\n"
            "Errno: %d, '%s'\r\n"
            "Detail: %s\r\n\r\n"
            
            "Command line:\r\n\"%s\"\r\n\r\n"
            
            "The application should be able to recover. "
            "Press 'Okay' to continue.\r\n\r\n"
            ,
            file, line,
            func,
            errnum, errnobuf,
            msg,
            GetCommandLine()
        );

        if (IDCANCEL == MessageBox
        (
            NULL,
            buf,
            "Warning ("PROG_ID")",
            MB_OK | MB_ICONWARNING | MB_APPLMODAL | MB_DEFBUTTON1
        ))
        { exit(1); }
        
#   endif
}


void bse_print_info
(
    const char *file,
    unsigned int line,
    const char *func,
    const char *msg,
    const char *value_title,
    int value
)
{
    if (bse_quiet_exceptions) { return; }
    
    if (value_title)
    {
        fprintf
        (
            stdout,
            "INFO: " PROG_ID ":\n"
            "INFO:    function: %s\n"
            "INFO:    location: %s:%u\n"
            "INFO:      detail: %s\n"
            "INFO:       value: %s %d\n",
                func, file, line, msg, value_title, value
        );
    }
    else
    {
        fprintf
        (
            stdout,
            "INFO: " PROG_ID ":\n"
            "INFO:    function: %s\n"
            "INFO:    location: %s:%u\n"
            "INFO:      detail: %s\n",
                func, file, line, msg
        );
    }
    
    fflush(stdout);
}
