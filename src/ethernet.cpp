#include "ethernet.h"

Ethernet::Ethernet(uint8_t *my_mac): rate_in(5), rate_out(5)
{
    Init();
    if(!my_mac)
        die("%s Bad call", __FUNCTION__);

    memcpy(mac, my_mac, sizeof(mac));
    sprintf(mac_ascii, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void Ethernet::Init()
{
    packets_in = 0ULL;
    bytes_in = 0ULL;
    ip_packets_in = 0ULL;
    bytes_new_in = 0l;

    packets_out = 0ULL;
    bytes_out = 0ULL;
    ip_packets_out = 0ULL;
    bytes_new_out = 0l;

    memset(mac, 0, sizeof(mac));
    memset(mac_ascii, 0, sizeof(mac_ascii));
    memset(description, 0, sizeof(description));
    memset(itf_name, 0, sizeof(itf_name));

    prev = NULL;
    next = NULL;

    //??
    withdesc = 0;
    printed = 0;
    linktype = 0;
}

bool Ethernet::IsMe(uint8_t *test_mac)
{
    if(!test_mac)
        die("%s Bad call", __FUNCTION__);

    if(memcmp(mac, test_mac, sizeof(mac)) == 0)
        return true;
    return false;
}

Ethernet *Ethernet::ListFindMac(uint8_t *test_mac)
{
    if(!test_mac)
        die("%s Bad call", __FUNCTION__);

    if(IsMe(test_mac))
        return this;
    Ethernet *crs = next;
    while(crs)
    {
        if(crs->IsMe(test_mac))
            return crs;
    }

    return NULL;
}

void Ethernet::UpdateRates(unsigned long ms)
{
    rate_in.Add(bytes_new_in, ms);
    bytes_new_in = 0l;
    rate_out.Add(bytes_new_out, ms);
    bytes_new_out = 0l;
}

void Ethernet::ListUpdateRates(unsigned long ms)
{
    UpdateRates(ms);
    Ethernet *crs = next;
    while(crs)
    {
        crs->UpdateRates(ms);
        crs=crs->next;
    }
}
