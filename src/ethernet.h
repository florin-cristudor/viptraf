#ifndef ETHERNET_H
#define ETHERNET_H

#include "traf_rate.h"

class Ethernet
{
public:
    Ethernet(uint8_t *my_mac);

    bool IsMe(uint8_t *test_mac);

    Ethernet *ListFindMac(uint8_t *test_mac);
    void UpdateRates(unsigned long ms);
    void ListUpdateRates(unsigned long ms);

    Ethernet *prev;
    Ethernet *next;

private:
    void Init();

    unsigned long long  packets_in;
    unsigned long long  bytes_in;
    unsigned long long  ip_packets_in;
    unsigned long       bytes_new_in;
    Rate rate_in;
    unsigned long long  packets_out;
    unsigned long long  bytes_out;
    unsigned long long  ip_packets_out;
    unsigned long       bytes_new_out;
    Rate rate_out;

    uint8_t             mac[ETH_ALEN];
    char                mac_ascii[18];
    char                description[65];
    char                itf_name[IFNAMSIZ];



    //??
    int withdesc;
    int printed;
    unsigned int        linktype;
};

/*
struct ethtabent {
    int type;
    Rate traf_rate_in;
    Rate traf_rate_out;
    union {
        struct {
            unsigned long long inpcount;        packets_in
            unsigned long long inbcount;
            unsigned long long inippcount;
            unsigned long inspanbr;             bytes_new_in
            unsigned long long outpcount;
            unsigned long long outbcount;
            unsigned long long outippcount;
            unsigned long outspanbr;            bytes_new_out
//TODEL			struct rate inrate;
//TODEL			struct rate outrate;

        } figs;

        struct {
            char eth_addr[ETH_ALEN];    //address
            char ascaddr[18];
            char desc[65];
            char ifname[IFNAMSIZ];
            int withdesc;
            int printed;
            unsigned int linktype;
        } desc;
    } un;

    unsigned int index;
    struct ethtabent *prev_entry;
    struct ethtabent *next_entry;
};
*/

#endif // ETHERNET_H
