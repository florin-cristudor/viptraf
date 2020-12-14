/*
 * VIPTraf Options Class
 */
#include <stdio.h>
#include <string.h>

#include "dirs.h"
#include "options.h"

Options opts;

Options::Options()
{
    InitDefaultOptions();
    if(ReadOptionsFromConfigFile())
        SaveOptionsToConfigFile();
}

const char *Options::GetValueString(int opt_idx)
{
    switch(opt_idx)
    {
    case OPTION_COLOR:
    case OPTION_LOGGING:
    case OPTION_REVLOOK:
    case OPTION_SERVNAMES:
    case OPTION_PROMISC:
    case OPTION_MAC:
    case OPTION_V6INV4ASV6:
        if(options.un.integer & (1 << opt_idx))
            return "On";
        else
            return "Off";
    case OPTION_ACTMODE:
        if(options.un.bits.actmode)
            return "kbytes/s";
        else
            return "kbits/s";
    case OPTION_TIMEOUT:
        snprintf(buffer, sizeof(buffer), "%i mins", options.timeout);
        return buffer;
    case OPTION_LOGSPAN:
        snprintf(buffer, sizeof(buffer), "%i mins", options.logspan);
        return buffer;
    case OPTION_UPDRATE:
        snprintf(buffer, sizeof(buffer), "%i mins", options.updrate);
        return buffer;
    case OPTION_CLOSEDINT:
        snprintf(buffer, sizeof(buffer), "%i mins", options.closedint);
        return buffer;
    default:
        return "unknown";
    }
}

void Options::ToggleBit(int bit_idx)
{
    if(bit_idx < OPTION_TIMEOUT)
    {
        if(options.un.integer & (1 << bit_idx))
            options.un.integer &= ~(1 << bit_idx);
        else
            options.un.integer |= 1 << bit_idx;
    }
    SaveOptionsToConfigFile();
}

void Options::SetValue(int opt_idx, int value)
{
    if(opt_idx < OPTION_TIMEOUT)
    {
        if(value)
            options.un.integer |= 1 << opt_idx;
        else
            options.un.integer &= ~(1 << opt_idx);
    }
    if(opt_idx >= OPTION_TIMEOUT && opt_idx <= OPTION_CLOSEDINT)
    {
        unsigned int *pvalue = &options.timeout;
        pvalue[opt_idx - OPTION_TIMEOUT] = (unsigned int)value;
    }
    SaveOptionsToConfigFile();
}

int Options::GetValue(int opt_idx)
{
    if(opt_idx < OPTION_TIMEOUT)
        return (options.un.integer & opt_idx) != 0;
    if(opt_idx >= OPTION_TIMEOUT && opt_idx <= OPTION_CLOSEDINT)
    {
        unsigned int *pvalue = &options.timeout;
        return (int)pvalue[opt_idx - OPTION_TIMEOUT];
    }

    return 0;
}

int Options::ReadOptionsFromConfigFile()
{
    FILE *fd;

    fd = fopen(OPTSFILE, "rb");

    if(!fd)
        return -1;

    if (fread(&options, sizeof(options), 1, fd) != 1)
    {
        fclose(fd);
        return -2;
    }

    fclose(fd);
    return 0;
}

int Options::SaveOptionsToConfigFile()
{
    FILE *fd;

    fd = fopen(OPTSFILE, "wb");

    if(!fd)
        return -1;

    if (fwrite(&options, sizeof(options), 1, fd) != 1)
    {
        fclose(fd);
        return -2;
    }

    fclose(fd);
    return 0;
}

void Options::InitDefaultOptions()
{
    memset(&options, 0x00, sizeof(options));
    options.un.bits.color = 1;
    options.un.bits.v6inv4asv6 = 1;
    options.un.bits.actmode = 1;
    options.timeout = 15;
    options.logspan = 3600;
}
