/*
 * VIPTraf Rate Class
 */

#include <stdio.h>

#include "options.h"
#include "log.h"

#include "traf_rate.h"

Rate::Rate()
{
    size = 0;
    index = 0;
    rates = NULL;
    sma = 0l;
}

Rate::~Rate()
{
    size = 0;
    if(rates)
        free(rates);
    rates = NULL;
}

Rate::Rate(unsigned int new_size)
{
    size = 0;
    index = 0;
    rates = NULL;
    sma = 0l;

    Alloc(new_size);
}

void Rate::Init(void)
{
    index = 0;
    sma = 0UL;
    if(rates)
        memset(rates, 0, size * sizeof(rates[0]));
}

void Rate::Alloc(unsigned int new_size)
{
    if(rates)
    {
        free(rates);
        rates = NULL;
        size = 0;
    }

    rates = (unsigned long long *) xmalloc(new_size * sizeof(rates[0]));
    if(!rates)
        die("%s Alloc error", __FUNCTION__);
    size = new_size;
    Init();
}

void Rate::Add(unsigned long bytes, unsigned long ms)
{
    if(!rates)
        return;

    rates[index] = bytes * 1000ULL / ms;

    index++;
    if(index >= size)
        index = 0;

    /* compute the moving average */
    unsigned long long sum = 0ll;
    for(unsigned int i=0; i < size; i++)
        sum += rates[i];
    sma = sum / size;
    debug_log("%s: Add %li Average %li", __FUNCTION__, bytes, sma);
}

int Rate::Print(char *buf, unsigned bufsize)
{
    const char *suffix[] = { "k", "M", "G", "T", "P", "E", "Z", "Y" };
    unsigned n_suffix = ARRAY_SIZE(suffix);
    int chars;
    unsigned long rate = sma;

    snprintf(buf, bufsize, "%li", sma);
    return 5;

    if (options.actmode == KBITS)
    {
        unsigned long tmp = rate;
        unsigned int i = 0;
        unsigned long divider = 1000;

        rate *= 8;
        while(tmp >= 100000000)
        {
            tmp /= 1000;
            i++;
            divider *= 1000;
        }
        if (i >= n_suffix)
            chars = snprintf(buf, bufsize, "error");
        else
            chars = snprintf(buf, bufsize, "%9.2f %sbps", (double)rate / divider, suffix[i]);
    }
    else
    {
        unsigned int i = 0;

        while(rate > 99 * (1UL << 20))
        {
            rate >>= 10;
            i++;
        }
        if (i >= n_suffix)
            chars = snprintf(buf, bufsize, "error");
        else
            chars = snprintf(buf, bufsize, "%9.2f %sBps", (double)rate / 1024, suffix[i]);
    }
    buf[bufsize-1] = '\0';

    return chars;
}

int Rate::PrintNoUnits(char *buf, unsigned bufsize)
{
    unsigned long rate = sma;
    int chars;

    if (options.actmode == KBITS)
    {
        chars = snprintf(buf, bufsize, "%8.1f", (double)rate * 8 / 1000);
    }
    else
    {
        chars = snprintf(buf, bufsize, "%8.1f", (double)rate / 1024);
    }
    buf[bufsize-1] = '\0';

    return chars;
}

int Rate::PrintPps(char *buf, unsigned bufsize)
{
    int chars = snprintf(buf, bufsize, "%9lu pps", sma);

    buf[bufsize-1] = '\0';

    return chars;
}

//TO REDESIGN
int rate_print(unsigned long rate, char *buf, unsigned n)
{
    const char *suffix[] = { "k", "M", "G", "T", "P", "E", "Z", "Y" };
    unsigned n_suffix = ARRAY_SIZE(suffix);

    int chars;

    if (options.actmode == KBITS) {
        unsigned long tmp = rate;
        unsigned int i = 0;
        unsigned long divider = 1000;

        rate *= 8;
        while(tmp >= 100000000) {
            tmp /= 1000;
            i++;
            divider *= 1000;
        }
        if (i >= n_suffix)
            chars = snprintf(buf, n, "error");
        else
            chars = snprintf(buf, n, "%9.2f %sbps", (double)rate / divider, suffix[i]);
    } else {
        unsigned int i = 0;

        while(rate > 99 * (1UL << 20)) {
            rate >>= 10;
            i++;
        }
        if (i >= n_suffix)
            chars = snprintf(buf, n, "error");
        else
            chars = snprintf(buf, n, "%9.2f %sBps", (double)rate / 1024, suffix[i]);
    }
    buf[n - 1] = '\0';

    return chars;
}

int rate_print_pps(unsigned long rate, char *buf, unsigned n)
{
    int chars;

    chars = snprintf(buf, n, "%9lu pps", rate);
    buf[n - 1] = '\0';

    return chars;
}
