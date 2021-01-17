/*
 * VIPTraf Closing functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "viptraf.h"
#include "interface/video.h"

__attribute__((noreturn)) int shut_down(const char *file, int line, const char *function, int return_code, const char *format, ...)
{
    if(pVideo)
    {
        delete pVideo;
        pVideo = NULL;
    }

    if(return_code)
    {
        char message[4096] = "";
        int space_left = sizeof(message) - 1;

        if(file != NULL && function != NULL)
        {
            snprintf(message, space_left, "%s(%i):%s ", file, line, function);
            space_left = sizeof(message) - strlen(message) - 1;
        }

        const char  *pErrMsg = "";
        switch(return_code)
        {
            case ERROR_PID_FILE:
                pErrMsg = "PID file creation error";
                break;
            case ERROR_FORK:
                pErrMsg = "Fork error";
                break;
            case ERROR_INIT_VIDEO:
                pErrMsg = "The video cannot be init";
                break;
            case ERROR_MEMORY:
                pErrMsg = "Memory allocation error";
                break;
            case ERROR_SCREEN_SIZE:
                pErrMsg = "Screen must have minimum 24 lines and 80 columns";
                break;
            case ERROR_ONLY_ADMIN:
                pErrMsg = "This program can be run only by the system administrator";
                break;
            case ERROR_OPTIONS:
                pErrMsg = "Options error";
                break;
            case ERROR_DEMONIZE:
                pErrMsg = "Demonize error";
                break;
        }
        if(*pErrMsg != 0x00 && space_left > 1)
        {
            snprintf(message + strlen(message), space_left, "| %s. ", pErrMsg);
            space_left = sizeof(message) - strlen(message) - 1;
        }

        if(return_code == ERROR_ERRNO)
        {
            snprintf(message + strlen(message), space_left, "| Errno %i: %s. ", errno, strerror(errno));
            space_left = sizeof(message) - strlen(message) - 1;
        }

        if(*format != 0x00 && space_left > 1)
        {
            va_list args;
            va_start(args, format);
            vsnprintf(message + strlen(message), space_left, format, args);
            space_left = sizeof(message) - strlen(message) - 1;
            va_end(args);
        }

        fprintf(stderr, "\n%s\n", message);
    }

    exit(return_code);
}
