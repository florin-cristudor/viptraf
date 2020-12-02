/*
 *  VIPTraf - TCPConnection Class
*/

#include "addr.h"

#include "tcp_con.h"

/*

    int s_fstat;
    int d_fstat;

    int interfaces[20];

*/

TCPConnection::TCPConnection():rate(5)
{
    Init();
}

TCPConnection::TCPConnection(struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr):rate(5)
{
    Init();

    if(s_addr)
    {
        if(s_addr->ss_family == AF_INET)
        {
            kind = TCP_CON_IPV4;
            sa4 = *((struct sockaddr_in *)s_addr);
        }
        else if(s_addr->ss_family == AF_INET6)
        {
            kind = TCP_CON_IPV6;
            sa6 = *((struct sockaddr_in6 *)s_addr);
        }
    }

    if(d_addr)
    {
        if(d_addr->ss_family == AF_INET)
        {
            da4 = *((struct sockaddr_in *)d_addr);
        }
        else if(d_addr->ss_family == AF_INET6)
        {
            da6 = *((struct sockaddr_in6 *)d_addr);
        }
    }
}

void TCPConnection::Init()
{
    kind = 0;
    spanbr = 0l;

    memset(&sa4, 0, sizeof(sa4));
    memset(&da4, 0, sizeof(da4));
    memset(&sa6, 0, sizeof(sa6));
    memset(&da6, 0, sizeof(da6));

    s_fqdn[0] = 0x00;
    d_fqdn[0] = 0x00;

    packets = 0l;
    bytes = 0l;

    stat = 0x00;

    prev = NULL;
    next = NULL;

    //to be checked
    spanbr = 0l;
}

TCPConnection::~TCPConnection(void)
{
}

bool TCPConnection::IsMe(struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr)
{
    if(!s_addr || !d_addr)
        die("%s Error in call", __FUNCTION__);
    if(s_addr->ss_family != d_addr->ss_family)
        die("%s Error in parameters", __FUNCTION__);
    if(s_addr->ss_family == AF_INET6 && kind == TCP_CON_IPV4) return false;
    if(s_addr->ss_family == AF_INET && kind == TCP_CON_IPV6) return false;

    switch(kind)
    {
        case TCP_CON_IPV4:
            if(sa4.sin_addr.s_addr == (*((struct sockaddr_in*)s_addr)).sin_addr.s_addr)
            {
                if(da4.sin_addr.s_addr == (*((struct sockaddr_in*)d_addr)).sin_addr.s_addr)
                    return true;
            }
            else if(sa4.sin_addr.s_addr == (*((struct sockaddr_in*)d_addr)).sin_addr.s_addr)
            {
                if(da4.sin_addr.s_addr == (*((struct sockaddr_in*)s_addr)).sin_addr.s_addr)
                    return true;
            }
            return false;
        case TCP_CON_IPV6:
            if(in6_compare(&sa6, (struct sockaddr_in6*)s_addr))
            {
                if(in6_compare(&da6, (struct sockaddr_in6*)d_addr))
                    return true;
            }
            else if(in6_compare(&sa6, (struct sockaddr_in6*)d_addr))
            {
                if(in6_compare(&da6, (struct sockaddr_in6*)s_addr))
                    return true;
            }
            return false;
         default:
            return false;
    }
}


TCPConnection * TCPConnection::ListGetConnection(sockaddr_storage *s_addr, sockaddr_storage *d_addr)
{
    if(!s_addr || !d_addr)
        die("%s Bad parameters", __FUNCTION__);

    TCPConnection *crs = this;
    while(crs)
    {
        if(crs->IsMe(s_addr, d_addr)) return crs;
        crs = crs->next;
    }
    return NULL;
}

void TCPConnection::ListUpdateRates(unsigned long ms)
{
    TCPConnection *crs = this;
    while(crs)
    {
        crs->AddRates(ms);
        crs = crs->next;
    }
}

//static void update_flowrates(struct tcptable *table, unsigned long msecs)
//{
//	struct tcptableent *entry;
//	for (entry = table->head; entry != NULL; entry = entry->next_entry) {
//		rate_add_rate(&entry->rate, entry->spanbr, msecs);
//		entry->spanbr = 0;
//	}
//}
