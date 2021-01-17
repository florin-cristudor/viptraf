/*
 * VIPTraf Log Class
 */
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "close.h"
#include "viptraf.h"

#include "traf_log.h"

Log::Log(bool enable)
{
    fd_log = NULL;
    name[0] = 0x00;
    enabled = 0;

    Set(enable);
}

Log::~Log()
{
    if(enabled)
        Write("Log closed\n");

    Close();
}

bool Log::Open(const char *file_name)
{
    strncpy(name, file_name, sizeof(name));

    return Open();
}

bool Log::Open()
{
    if(!enabled)
        return false;

    fd_log = fopen(name, "a");

    return fd_log != NULL;
}

bool Log::Close()
{
    if(fd_log)
    {
        fclose(fd_log);
        fd_log = NULL;
    }

    return true;
}

void Log::Set(bool enable)
{
    Write(enable ? "Log enabled" : "Log disabled");
    enabled = enable;

    if(!enable)
        Close();
}

bool Log::Rotate()
{
    Close();
    return Open();
}

int Log::Write(const char *format, ...)
{
    if(!format)
        exit_program(ERROR_GENERAL, "Bad call");

    if(!enabled)
        return -1;

    if(!fd_log)
        return -2;

    char atime[LOG_TIME_MAX];

    atime[0] = 0x00;
    time_t now = time(NULL);

    strncpy(atime, ctime(&now), LOG_TIME_MAX-1);
    atime[LOG_TIME_MAX - 1] = 0x00;

    va_list args;
    va_start(args, format);

    fprintf(fd_log, "%s; ", atime);
    vfprintf(fd_log, format, args);
    va_end(args);
    fprintf(fd_log, "\n");

    fflush(fd_log);

    return 0;
}

