/*
 *  VIPTraf - TCPConnection
*/

#include "tcp_con.h"

/*

    int s_fstat;
    int d_fstat;

    int interfaces[20];

*/

TCPConnection::TCPConnection(int type, struct sockaddr_storage *s_addr, struct sockaddr_storage *d_addr)
{
    kind = type;
    if(s_addr)
        sa = *s_addr;
    else
        memset(&sa, 0, sizeof(sa));

    if(d_addr)
        da = *d_addr;
    else
        memset(&da, 0, sizeof(da));

    s_fqdn[0] = 0x00;
    d_fqdn[0] = 0x00;

    packets = 0l;
    bytes = 0l;

    stat = 0x00;

    prev = NULL;
    next = NULL;
}

::TCPConnection::~TCPConnection(void)
{
}

