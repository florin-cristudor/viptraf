#ifndef TRAF_LOG_H
#define TRAF_LOG_H

#include <stdio.h>

#include "iptraf-ng-compat.h"

#define LOG_FILENAME_MAX	160
#define LOG_TIME_MAX        30

#define LOG_ENABLE  true
#define LOG_DISABLE false

class Log
{
public:
    Log(bool enable);
    ~Log();

    bool Open(const char *file_name);
    bool Open(void);
    bool IsOpened(void) { return fd_log != NULL; }
    int  Write(const char *format, ...);
    bool Close(void);
    void Set(bool enable);
    bool Rotate(void);

private:
    FILE *fd_log;
    char name[LOG_FILENAME_MAX];

    int enabled:1;
};

#endif // TRAF_LOG_H
