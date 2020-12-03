#ifndef APP_HOST_MONITOR_H
#define APP_HOST_MONITOR_H

#include <stdio.h>
#include <time.h>

#include "iptraf-ng-compat.h"
#include "capt.h"

#include "ethernet.h"

class HostMonitorApp
{
public:
    HostMonitorApp();
    ~HostMonitorApp();

    int Run(time_t facilitytime, const char *itf_name);

private:
    int CleanUp(int return_code);
    bool TestForStop(int ch);

    struct capt     capture;
    struct pkt_hdr  packet;

    Ethernet *list;

    int capture_enabled:1;
    int packet_enabled:1;
};

#endif // APP_HOST_MONITOR_H
