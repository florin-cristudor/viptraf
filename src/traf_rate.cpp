/*
 * VIPTraf Rate Class
 */

#include "options.h"

#include <stdio.h>
#include "traf_rate.h"

Rate::Rate()
{
    n = 0;
    index = 0;
    rates = NULL;
    sma = 0l;
}

Rate::~Rate()
{
    n = 0;
    if(rates)
        free(rates);
    rates = NULL;
}

void Rate::Init(void)
{
    index = 0;
    sma = 0UL;
    if(rates)
        memset(rates, 0, n * sizeof(rates[0]));
}

void Rate::Alloc(unsigned int new_n)
{
    rates = (unsigned long long *) xmalloc(n * sizeof(rates[0]));
    n = new_n;
    Init();
}

void Rate::Add(unsigned long bytes, unsigned long ms)
{
    if(!rates)
        return;

    rates[index] = bytes * 1000ULL / ms;

    if((index + 1) >= n)
        index = 0;
    else
        index++;

    /* compute the moving average */
    unsigned long long sum = 0ll;
    for(unsigned int i=0; i < n; i++)
        sum += rates[i];
    sma = sum / n;
}

int Rate::Print(char *buf, unsigned bufsize)
{
    const char *suffix[] = { "k", "M", "G", "T", "P", "E", "Z", "Y" };
    unsigned n_suffix = ARRAY_SIZE(suffix);
    int chars;
    unsigned long rate = sma;

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
    buf[n-1] = '\0';

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
    buf[n-1] = '\0';

    return chars;
}

int Rate::PrintPps(char *buf, unsigned bufsize)
{
    int chars = snprintf(buf, bufsize, "%9lu pps", sma);

    buf[n-1] = '\0';

    return chars;
}
