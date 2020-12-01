#ifndef TCP_CON_H
#define TCP_CON_H

#define     TCP_CON_IPV4    4
#define     TCP_CON_IPV6    6

#include <stdint.h>
#include <sys/socket.h>

#include "iptraf-ng-compat.h"

#include "traf_rate.h"

class TCPConnection
{
    public:
        TCPConnection(struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr);
        ~TCPConnection();

        void SetStat(int new_stat) { stat = new_stat; }
        bool IsMe(struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr);

        void AddRates(unsigned long ms) { rate.Add(spanbr, ms); }

        TCPConnection *get_next(void) { return next; }
        TCPConnection *get_prev(void) { return prev; }


    private:

    uint8_t   kind;

    struct sockaddr_in sa4;  //source address IPv4
    struct sockaddr_in da4;  //destination address IPv4
    struct sockaddr_in6 sa6;  //source address IPv6
    struct sockaddr_in6 da6;  //destination address IPv6


    char s_fqdn[FQDN_SIZE];   // source fully-qualified domain names
    char d_fqdn[FQDN_SIZE];   // destination fully-qualified domain names

    int s_fstat;
    int d_fstat;

    unsigned long long packets;     // packet count
    unsigned long long bytes;       // byte count

    unsigned int stat;  // TCP flags

    int interfaces[20];

    TCPConnection *prev;
    TCPConnection *next;

    Rate rate;

    //to be checked
    unsigned long spanbr;

};

/*
    char smacaddr[18];
    char s_sname[11];   // Service names, maxlen=10
    char d_sname[11];
    unsigned int protocol;
    unsigned int win;
    unsigned int psize;
    unsigned long finack;
    int partial;
    int finsent;
    char ifname[IFNAMSIZ];
    unsigned int index;
    int timedout;
    int inclosed;
    int half_bracket;
    unsigned long spanbr;

    time_t lastupdate;
    time_t conn_starttime;
    struct tcp_hashentry *hash_node;

*/

TCPConnection * get_tcp_connection(struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr);
void tcpcon_list_update_flowrates(unsigned long ms);

#endif // TCP_CON_H
