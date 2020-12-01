#ifndef _VIPTRAF_TCP_CON_H_
#define _VIPTRAF_TCP_CON_H_

#define     TCP_CON_IPV4    4
#define     TCP_CON_IPV6    6

#include <stdint.h>
#include <sys/socket.h>

#include "iptraf-ng-compat.h"

class TCPConnection
{
    public:
        TCPConnection(int type, struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr);
        ~TCPConnection();

    private:

    uint8_t   kind;

    struct sockaddr_storage sa;  //source address
    struct sockaddr_storage da;  //destination address

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
    struct rate rate;
    time_t lastupdate;
    time_t conn_starttime;
    struct tcp_hashentry *hash_node;

*/

#endif //_VIPTRAF_TCP_CON_H_
