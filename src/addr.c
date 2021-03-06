/*
 * VIPTraf addr.c
 */
#include "iptraf-ng-compat.h"
//end old inc
#include "close.h"
#include "viptraf.h"

bool in6_compare(struct sockaddr_in6 *a1, struct sockaddr_in6 *a2)
{
    if(!a1 || !a2)
        exit_program(ERROR_GENERAL, "Bad call");
    for(int i=0; i<16; i++)
        if(a1->sin6_addr.s6_addr[i] != a2->sin6_addr.s6_addr[i])
            return false;
    return true;
}
