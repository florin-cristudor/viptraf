#ifndef IPTRAF_NG_IPFILTER_H
#define IPTRAF_NG_IPFILTER_H

void gethostparams(struct hostparams *data, const char *init_saddr, const char *init_smask,
		   const char *init_sport1, const char *init_sport2, const char *init_daddr,
		   const char *init_dmask, const char *init_dport1, const char *init_dport2,
		   const char *initinex, const char *initmatchop, int *aborted);
void ipfilterselect(int *faborted);
int ipfilter(unsigned long saddr, unsigned long daddr, in_port_t sport,
         in_port_t dport, unsigned int protocol);
int ip6filter(struct in6_addr *saddr, struct in6_addr *daddr, in_port_t sport,
         in_port_t dport, unsigned int protocol);

#endif	/* IPTRAF_NG_IPFILTER_H */
