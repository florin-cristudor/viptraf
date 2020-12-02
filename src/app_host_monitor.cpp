/*
 * VIPtraf - App Host Monitor Class
 */
#include <signal.h>
#include <string.h>

#include "options.h"
#include "dirs.h"
#include "logvars.h"
#include "log.h"
#include "error.h"
#include "packet.h"
#include "ifaces.h"

#include "app_host_monitor.h"

HostMonitorApp::HostMonitorApp()
{
    capture_enabled = 0;
    packet_enabled = 0;
    memset(&capture, 0, sizeof(struct capt));
    list = NULL;
    fd_log = NULL;
}

HostMonitorApp::~HostMonitorApp()
{
    CleanUp(0);
}

int HostMonitorApp::CleanUp(int return_code)
{
    if(capture_enabled)
    {
        capt_destroy(&capture);
        capture_enabled = 0;
    }

    if(fd_log)
    {
        fclose(fd_log);
        fd_log = NULL;
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

    strcpy(current_logfile, ""); //???

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

/*
 * SIGUSR1 logfile rotation handler
 */

static void rotate_lanlog(int s __unused)
{
    rotate_flag = 1;
    strcpy(target_logname, current_logfile);
    signal(SIGUSR1, rotate_lanlog);
}

int HostMonitorApp::Run(time_t facilitytime, const char *itf_name)
{
    int logging = options.logging;
    int key;
    struct timespec now;
    struct timespec next_screen_update;
    time_t log_next = INT_MAX;
    time_t endtime = INT_MAX;

    clock_gettime(CLOCK_MONOTONIC, &now);
    memset(&next_screen_update, 0, sizeof(next_screen_update));

    struct timespec last_time = now;
    time_t starttime = now.tv_sec;

    if(logging)
    {
        if(current_logfile[0] == 0)
        {
            strncpy(current_logfile, gen_instance_logname(LANLOG, getpid()), 80);
            if (!daemonized)
                input_logfile(current_logfile, &logging);
        }
        if(logging)
        {
            opentlog(&fd_log, current_logfile);
            if(fd_log == NULL)
                logging = 0;
        }
        if(logging)
        {
            signal(SIGUSR1, rotate_lanlog);
            rotate_flag = 0;
            writelog(logging, fd_log, "******** LAN traffic monitor started ********");
            log_next = now.tv_sec + options.logspan;
        }
    }

    if(itf_name && !dev_up(itf_name))
    {
        err_iface_down();
        return CleanUp(-1);
    }

    if(capt_init(&capture, itf_name) == -1)
    {        write_error("Unable to initialize packet capture interface");
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
//            updateethrates(&table, msecs);

//            printelapsedtime(now.tv_sec - starttime, 15, table.borderwin);

//            print_packet_drops(capt_get_dropped(&capt), table.borderwin, 49);

            if(logging && (now.tv_sec > log_next))
            {
                check_rotate_flag(&fd_log);
//                writeethlog(table.head, now.tv_sec - starttime, fd_log);
                log_next = now.tv_sec + options.logspan;
            }

            if(now.tv_sec > endtime)
                break;

            last_time = now;
        }

        if(time_after(&now, &next_screen_update))
        {
            print_visible_entries(&table);
            update_panels();
            doupdate();

            set_next_screen_update(&next_screen_update, &now);
        }

        if(capt_get_packet(&capture, &pkt, &key, table.tabwin) == -1)
        {
            write_error("Packet receive failed");
            break;
        }

        if(TestForStop(key))
            break;

        if(key != ERR)
            hostmon_process_key(&table, key);

        if(pkt.pkt_len > 0)
        {
            hostmon_process_packet(&table, &pkt);
            capt_put_packet(&capt, &pkt);
        }
    }

    if(logging)
    {
        signal(SIGUSR1, SIG_DFL);
        writeethlog(table.head, time(NULL) - starttime, logfile);
        writelog(logging, logfile, "******** LAN traffic monitor stopped ********");
    }

/*
    struct ethtab table;
    initethtab(&table);
*/
    return CleanUp(0);
}


