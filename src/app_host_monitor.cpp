/*
 * VIPtraf - App Host Monitor Class
 */
#include <signal.h>
#include <string.h>

#include "iptraf-ng-compat.h"
#include "options_old.h"
#include "dirs.h"
#include "logvars.h"
#include "log.h"
#include "error.h"
#include "packet.h"
#include "ifaces.h"
#include "deskman.h"
#include "timer.h"

#include <mutex>
#include "traf_log.h"
#include "app_host_monitor.h"

HostMonitorApp::HostMonitorApp()
{
    capture_enabled = 0;
    packet_enabled = 0;
    memset(&capture, 0, sizeof(struct capt));
    list = NULL;
}

HostMonitorApp::~HostMonitorApp()
{
    CleanUp(0);
}

int HostMonitorApp::CleanUp(int return_code)
{
    signal(SIGUSR1, SIG_DFL);

    if(capture_enabled)
    {
        capt_destroy(&capture);
        capture_enabled = 0;
    }

    Ethernet *crs = list;
    while(crs)
    {
        Ethernet *next = crs->next;
        delete crs;
        crs = next;
    }
    list = NULL;

    if(packet_enabled)
    {
        packet_destroy(&packet);
        packet_enabled = 0;
    }

    return return_code;
}

bool HostMonitorApp::TestForStop(int ch)
{
    switch(ch)
    {
        case 'q':
        case 'Q':
        case 'x':
        case 'X':
        case 27:
        case 24:
            return true;
        default:
            return false;
    }
}

int hma_log_rotate;
std::mutex hma_mutex_rotate;

static void hma_sighandler(int signum __unused)
{
    hma_mutex_rotate.lock();
     hma_log_rotate = 1;
    hma_mutex_rotate.unlock();

   return;
}


int HostMonitorApp::Run(time_t facilitytime, const char *itf_name)
{
    int key;
    struct timespec now;
    struct timespec next_screen_update;
    time_t log_next = INT_MAX;
    time_t endtime = INT_MAX;
    Log log(options.logging);

//    if (options.logging && !daemonized) //???
//        input_logfile(current_logfile, &logging);
    hma_log_rotate = 0;
    log.Open(gen_instance_logname(LANLOG, getpid()));
    signal(SIGUSR1, hma_sighandler);

    log.Write("******** LAN traffic monitor started ********");

    clock_gettime(CLOCK_MONOTONIC, &now);
    log_next = now.tv_sec + options.logspan;

    memset(&next_screen_update, 0, sizeof(next_screen_update));

    struct timespec last_time = now;
//    time_t starttime = now.tv_sec;

    if(itf_name && !dev_up(itf_name))
    {
        log.Write("Interface %s is down. Abort!", itf_name);
        err_iface_down();
        return CleanUp(-1);
    }

    if(capt_init(&capture, itf_name) == -1)
    {
        log.Write("Error initializing packet capture interface. Abort!");
        write_error("Unable to initialize packet capture interface");
        return CleanUp(-2);
    }
    capture_enabled = 1;

    packet_init(&packet);

    if(facilitytime != 0)
        endtime = now.tv_sec + facilitytime * 60;

    while(1)
    {
        clock_gettime(CLOCK_MONOTONIC, &now);
        if(now.tv_sec > last_time.tv_sec)
        {
            unsigned long msecs = timespec_diff_msec(&now, &last_time);
            if(list)
                list->ListUpdateRates(msecs);

//            printelapsedtime(now.tv_sec - starttime, 15, table.borderwin);

//            print_packet_drops(capt_get_dropped(&capt), table.borderwin, 49);

            if(log.IsOpened() && (now.tv_sec > log_next))
            {
                hma_mutex_rotate.lock();
                 if(hma_log_rotate)
                 {
                    log.Rotate();
                    hma_log_rotate = 0;
                 }
                hma_mutex_rotate.unlock();

//                writeethlog(table.head, now.tv_sec - starttime, fd_log);
                log_next = now.tv_sec + options.logspan;
            }

            if(now.tv_sec > endtime)
                break;

            last_time = now;
        }

        if(time_after(&now, &next_screen_update))
        {
//            print_visible_entries(&table);
            update_panels();
            doupdate();

            set_next_screen_update(&next_screen_update, &now);
        }

/*        if(capt_get_packet(&capture, &packet, &key, table.tabwin) == -1)
        {
            write_error("Packet receive failed");
            break;
        }
*/
        if(TestForStop(key))
            break;

//        if(key != ERR)
//            hostmon_process_key(&table, key);

        key = ERR;

        if(packet.pkt_len > 0)
        {
//            hostmon_process_packet(&table, &packet);
//            capt_put_packet(&capt, &packet);
        }
    }

//        writeethlog(table.head, time(NULL) - starttime, logfile);
    log.Write("******** LAN traffic monitor stopped ********");

    return CleanUp(0);
}
