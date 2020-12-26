/* For terms of usage/redistribution/modification see the LICENSE file */
/* For authors and contributors see the AUTHORS file */

/***

log.c - the iptraf logging facility

***/

#include "iptraf-ng-compat.h"

#include "attrs.h"
#include "deskman.h"
#include "dirs.h"
#include "log.h"

#include "interface/video.h"

#include "tui/input.h"
#include "tui/msgboxes.h"
#include "tui/winops.h"

#define TARGET_LOGNAME_MAX	160

int rotate_flag;
char target_logname[TARGET_LOGNAME_MAX];
char current_logfile[TARGET_LOGNAME_MAX];
char debug_logfile[TARGET_LOGNAME_MAX];


/*
 * Generates a log file based on a template for a particular instance of
 * a facility.   Used by the IP Traffic Monitor and LAN Station Monitor.
 */

char *gen_instance_logname(const char *stemplate, int instance_num)
{
	static char filename[80];

	snprintf(filename, 80, "%s-%d.log", stemplate, instance_num);
	return filename;
}

void input_logfile(char *target, int *logging)
{
	WINDOW *dlgwin;
	PANEL *dlgpanel;
	struct FIELDLIST fieldlist;
	int aborted;

    dlgwin = newwin(11, 60, (VideoMaxLines - 11) / 2, (VideoMaxCols - 60) / 2);
	dlgpanel = new_panel(dlgwin);

	wattrset(dlgwin, DLGBOXATTR);
	tx_colorwin(dlgwin);
	tx_box(dlgwin, ACS_VLINE, ACS_HLINE);
	mvwprintw(dlgwin, 0, 1, " Logging Enabled ");
	wattrset(dlgwin, DLGTEXTATTR);
	mvwprintw(dlgwin, 2, 2,
		  "Enter the name of the file to which to write the log.");
	mvwprintw(dlgwin, 4, 2,
		  "If you don't specify a path, the log file will");
	mvwprintw(dlgwin, 5, 2, "be placed in %s.", LOGDIR);
	wmove(dlgwin, 9, 2);
	stdkeyhelp(dlgwin);
	wprintw(dlgwin, " (turns logging off)");

    tx_initfields(&fieldlist, 1, 50, (VideoMaxLines - 1) / 2 + 2,
              (VideoMaxCols - 50) / 2 - 3, DLGTEXTATTR, FIELDATTR);
	tx_addfield(&fieldlist, 48, 0, 0, target);
	tx_fillfields(&fieldlist, &aborted);

	if (!aborted) {
		if (strchr(fieldlist.list->buf, '/') == NULL)
			snprintf(target, 48, "%s/%s", LOGDIR,
				 fieldlist.list->buf);
		else
			strncpy(target, fieldlist.list->buf, 48);
	}

	*logging = !aborted;

	tx_destroyfields(&fieldlist);
	del_panel(dlgpanel);
	delwin(dlgwin);
	update_panels();
	doupdate();
}

void opentlog(FILE ** fd, const char *logfilename)
{
	*fd = fopen(logfilename, "a");

	if (*fd == NULL)
		tui_error(ANYKEY_MSG, "Unable to open log file");

	rotate_flag = 0;
	strcpy(target_logname, "");
}

void genatime(time_t now, char *atime)
{
	memset(atime, 0, TIME_TARGET_MAX);
	strncpy(atime, ctime(&now), 26);
	atime[strlen(atime) - 1] = '\0';
}

void writelog(int logging, FILE * fd, const char *msg)
{
	char atime[TIME_TARGET_MAX];

	if (logging) {
		genatime(time(NULL), atime);
		fprintf(fd, "%s; %s\n", atime, msg);
	}

	fflush(fd);
}

void write_daemon_err(const char *msg, va_list vararg)
{
	char atime[TIME_TARGET_MAX];
	FILE *fd;

	genatime(time(NULL), atime);
	fd = fopen(DAEMONLOG, "a");
	fprintf(fd, "%s iptraf[%u]: ", atime, getpid());
	vfprintf(fd, msg, vararg);
	fprintf(fd, "\n");
	fclose(fd);
}

void write_daemon_log(const char *format, ...)
{
	char atime[TIME_TARGET_MAX];
	FILE *fd;
    va_list args;

    va_start(args, format);
	genatime(time(NULL), atime);
    fd = fopen(DAEMONLOG, "a");
    if (fd== NULL)
        return;
    fprintf(fd, "%s viptraf[%u]: ", atime, getpid());
	vfprintf(fd, format, args);
    va_end(args);
	fprintf(fd, "\n");
	fclose(fd);
}



void rotate_logfile(FILE ** fd, const char *name)
{
	fclose(*fd);
	*fd = fopen(name, "a");
	rotate_flag = 0;
}


void announce_rotate_prepare(FILE * fd)
{
	writelog(1, fd,
		 "***** USR1 signal received, preparing to reopen log file *****");
}

void announce_rotate_complete(FILE * fd)
{
	writelog(1, fd, "***** Logfile reopened *****");
}

void check_rotate_flag(FILE ** logfile)
{
	if (rotate_flag == 1) {
		announce_rotate_prepare(*logfile);
		rotate_logfile(logfile, target_logname);
		announce_rotate_complete(*logfile);
		rotate_flag = 0;
	}
}
