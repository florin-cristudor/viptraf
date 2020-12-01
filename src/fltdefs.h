#ifndef IPTRAF_NG_FLTDEFS_H
#define IPTRAF_NG_FLTDEFS_H

/***

fltdefs.h - declarations for the TCP, UDP, and misc IP filters

***/

#define FLT_FILENAME_MAX 	40

#define FLT_RESOLVE		1
#define FLT_DONTRESOLVE		0

#define F_ALL_IP    0
#define F_TCP       6
#define F_UDP       17
#define F_OTHERIP   59
#define F_ICMP      1
#define F_IGMP      2
#define F_OSPF      89
#define F_IGP       9
#define F_IGRP      88
#define F_GRE       47
#define F_L2TP      115
#define F_IPSEC_AH  51
#define F_IPSEC_ESP 50
#define F_IPV6          41  //IPv6 Encapsulation
#define F_IPV6_ROUTE    43  //IPv6 Routing Header
#define F_IPv6_FRAG     44  //IPv6 Fragment Header
#define F_IPV6_ICMP     58  //ICMP for IPv6



#define MATCH_OPPOSITE_ALWAYS       1
#define MATCH_OPPOSITE_USECONFIG    2

/*
 * IP filter parameter entry
 */
struct hostparams {
	char s_fqdn[FQDN_SIZE];
	char d_fqdn[FQDN_SIZE];
	char s_mask[20];
	char d_mask[20];
	in_port_t sport1;
	in_port_t sport2;
	in_port_t dport1;
	in_port_t dport2;
	int filters[256];
	char protolist[70];
	char reverse;
	char match_opposite;
};


struct ipv4_address
{
    struct in_addr  addr;
    unsigned long   mask;
};

struct ipv6_address
{
    struct in6_addr addr;
    unsigned long   prefix;
};

struct filter_addr {
    __u8            type;
    __u8            is_null;
    union
    {
        struct ipv4_address ipv4;
        struct ipv6_address ipv6;
    } un;
};

struct filterent {
    struct hostparams hp;
    struct filter_addr src;
    struct filter_addr dst;

//	unsigned long saddr;
//	unsigned long daddr;
//	unsigned long smask;
//	unsigned long dmask;
	unsigned int index;
	struct filterent *next_entry;
	struct filterent *prev_entry;
};

struct filterlist {
	struct filterent *head;
	struct filterent *tail;
	unsigned int lastpos;
};

#endif	/* IPTRAF_NG_FLTDEFS_H */
