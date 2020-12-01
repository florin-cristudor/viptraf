#ifndef IPTRAF_NG_FLTMGR_H
#define IPTRAF_NG_FLTMGR_H

/***

fltmgr.h - filter list management routine prototypes

***/

struct filterfileent {
	char desc[35];
	char filename[40];
};

struct ffnode {
	struct filterfileent ffe;
	struct ffnode *next_entry;
	struct ffnode *prev_entry;
};

void makestdfiltermenu(struct MENU *menu);
void makemainfiltermenu(struct MENU *menu);
int loadfilterlist(struct ffnode **fltfile);
void save_filterlist(struct ffnode *fltlist);
void pickafilter(struct ffnode *files, struct ffnode **fltfile, int *aborted);
char *pickfilterbyname(struct ffnode *fltlist, char *filename);
void selectfilter(struct filterfileent *ffe, int *aborted);
void destroyfilterlist(struct ffnode *fltlist);
void get_filter_description(char *description, int *aborted, const char *pre_edit);
void genname(unsigned long n, char *m);
int nametoaddr(char *ascname, struct filter_addr *fa);
void listfileerr(int code);

#endif	/* IPTRAF_NG_FLTMGR_H */
