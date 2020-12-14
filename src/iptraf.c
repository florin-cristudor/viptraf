/* For terms of usage/redistribution/modification see the LICENSE file */
/* For authors and contributors see the AUTHORS file */

/*
IPTraf
An IP Network Statistics Utility
*/

#include "iptraf-ng-compat.h"
#include "built-in.h"

#include "tui/menurt.h"
#include "tui/winops.h"

#include "dirs.h"
#include "deskman.h"
#include "fltdefs.h"
#include "fltselect.h"
#include "fltmgr.h"
#include "fltedit.h"
#include "serv.h"
#include "options_old.h"
#include "attrs.h"
#include "rvnamed.h"
#include "logvars.h"
#include "detstats.h"
#include "ifstats.h"
#include "itrafmon.h"
#include "pktsize.h"
#include "hostmon.h"
#include "log.h"

#include "parse-options.h"

#include "video.h"
#include "video_ncurses.h"

#include "options.h"

#include "desktop.h"

Options opts;

#define WITHALL 1
#define WITHOUTALL 0

const char *ALLSPEC = "all";

#define CMD(name, h) { .cmd = #name, .fn = cmd_##name, .help = h }
#define CMD_END() { NULL, NULL, NULL }

struct cmd_struct {
    const char *cmd;
    int (*fn)(int, char **);
    const char *help;
};



/*
 * Important globals used throughout the
 * program.
 */
int exitloop = 0;
int daemonized = 0;
int facility_running = 0;




static void press_enter_to_continue(void)
{
	fprintf(stderr, "Press Enter to continue.\n");
	getchar();
}

static void clearfiles(const char *prefix, const char *directory)
{
	DIR *dir;
	struct dirent *dir_entry;
	char target_name[PATH_MAX];

	dir = opendir(directory);

	if (dir == NULL) {
		fprintf(stderr, "\nUnable to read directory %s\n%s\n",
			directory, strerror(errno));
		press_enter_to_continue();
		return;
	}

	do {
		dir_entry = readdir(dir);
		if (dir_entry != NULL) {
			if (strncmp(dir_entry->d_name, prefix, strlen(prefix))
			    == 0) {
				snprintf(target_name, sizeof(target_name) - 1,
					 "%s/%s", directory, dir_entry->d_name);
				target_name[sizeof(target_name) - 1] = '\0';
				unlink(target_name);
			}
		}
	} while (dir_entry != NULL);

	closedir(dir);
}

static void removetags(void)
{
	clearfiles("iptraf", LOCKDIR);
}

static void remove_sockets(void)
{
	clearfiles(SOCKET_PREFIX, WORKDIR);
}

/*
 * USR2 handler.  Used to normally exit a daemonized facility.
 */

static void term_usr2_handler(int s __unused)
{
	exitloop = 1;
}

static void init_break_menu(struct MENU *break_menu)
{
    tx_initmenu(break_menu, 6, 20, (VideoMaxLines - 6) / 2, VideoMaxCols / 2, BOXATTR,
		    STDATTR, HIGHATTR, BARSTDATTR, BARHIGHATTR, DESCATTR);
	tx_additem(break_menu, " By packet ^s^ize",
		   "Displays packet counts by packet size range");
	tx_additem(break_menu, " By TCP/UDP ^p^ort",
		   "Displays packet and byte counts by service port");
	tx_additem(break_menu, NULL, NULL);
	tx_additem(break_menu, " E^x^it menu", "Return to main menu");
}

/*
 * Get the ball rolling: The program interface routine.
 */

static void program_interface(void)
{
	struct MENU menu;
	struct MENU break_menu;

	int endloop = 0;
	int row = 1;
	int break_row = 1;
	int aborted;
	int break_aborted;

	char ifname[IFNAMSIZ];
	char *ifptr = NULL;

	/*
	 * Load saved filter
	 */
	loadfilters();
	indicate("");

    tx_initmenu(&menu, 15, 35, (VideoMaxLines - 16) / 2, (VideoMaxCols - 35) / 2, BOXATTR,
		    STDATTR, HIGHATTR, BARSTDATTR, BARHIGHATTR, DESCATTR);

	tx_additem(&menu, " IP traffic ^m^onitor",
		   "Displays current IP traffic information");
	tx_additem(&menu, " General interface ^s^tatistics",
		   "Displays some statistics for attached interfaces");
	tx_additem(&menu, " ^D^etailed interface statistics",
		   "Displays more statistics for a selected interface");
	tx_additem(&menu, " Statistical ^b^reakdowns...",
		   "Facilities for traffic counts by packet size or TCP/UDP port");
	tx_additem(&menu, " ^L^AN station monitor",
		   "Displays statistics on detected LAN stations");
	tx_additem(&menu, NULL, NULL);
	tx_additem(&menu, " ^F^ilters...",
		   "Allows you to select traffic display and logging criteria");
	tx_additem(&menu, NULL, NULL);
	tx_additem(&menu, " C^o^nfigure...", "Set various program options");
	tx_additem(&menu, NULL, NULL);
	tx_additem(&menu, " ^A^bout...", "Displays program info");
	tx_additem(&menu, NULL, NULL);
	tx_additem(&menu, " E^x^it", "Exits program");

	endloop = 0;

	do {
		tx_showmenu(&menu);
		tx_operatemenu(&menu, &row, &aborted);

		switch (row) {
		case 1:
			selectiface(ifname, WITHALL, &aborted);
			if (!aborted) {
				if (strcmp(ifname, "") != 0)
					ifptr = ifname;
				else
					ifptr = NULL;
                debug_log("Start IP Monitor");
				ipmon(0, ifptr);
			}
			break;
		case 2:
			ifstats(0);
			break;
		case 3:
			selectiface(ifname, WITHOUTALL, &aborted);
			if (!aborted)
				detstats(ifname, 0);
			break;
		case 4:
			break_row = 1;
			init_break_menu(&break_menu);
			tx_showmenu(&break_menu);
			tx_operatemenu(&break_menu, &break_row, &break_aborted);

			switch (break_row) {
			case 1:
				selectiface(ifname, WITHOUTALL, &aborted);
				if (!aborted)
					packet_size_breakdown(ifname, 0);
				break;
			case 2:
				selectiface(ifname, WITHOUTALL, &aborted);
				if (!aborted)
					servmon(ifname, 0);
				break;
			case 4:
				break;
			}
			tx_destroymenu(&break_menu);
			break;
		case 5:
			selectiface(ifname, WITHALL, &aborted);
			if (!aborted) {
				if (strcmp(ifname, "") != 0)
					ifptr = ifname;
				else
					ifptr = NULL;
				hostmon(0, ifptr);
			}
			break;
		case 7:
			config_filters();
			savefilters();
			break;
		case 9:
			setoptions();
			saveoptions();
			break;
		case 11:
			about();
			break;
		case 13:
			endloop = 1;
			break;
		}
	} while (!endloop);

	tx_destroymenu(&menu);
}

static const char *const iptraf_ng_usage[] = {
	VIPTRAF_NAME " [options]",
	VIPTRAF_NAME " [options] -B [-i <iface> | -d <iface> | -s <iface> | -z <iface> | -l <iface> | -g]",
	NULL
};

static int help_opt, f_opt, g_opt, facilitytime, B_opt;
static char *i_opt, *d_opt, *s_opt, *z_opt, *l_opt, *L_opt;

static struct options iptraf_ng_options[] = {
	OPT__HELP(&help_opt),
	OPT_GROUP(""),
	OPT_STRING('i', NULL, &i_opt, "iface",
		   "start the IP traffic monitor (use '-i all' for all interfaces)"),
	OPT_STRING('d', NULL, &d_opt, "iface",
		   "start the detailed statistics facility on an interface"),
	OPT_STRING('s', NULL, &s_opt, "iface",
		   "start the TCP and UDP monitor on an interface"),
	OPT_STRING('z', NULL, &z_opt, "iface",
		   "shows the packet size counts on an interface"),
	OPT_STRING('l', NULL, &l_opt, "iface",
		   "start the LAN station monitor (use '-l all' for all LAN interfaces)"),
	OPT_BOOL('g', NULL, &g_opt, "start the general interface statistics"),
	OPT_GROUP(""),
	OPT_BOOL('B', NULL, &B_opt,
		 "run in background (use only with one of the above parameters"),
	OPT_BOOL('f', NULL, &f_opt,
		 "clear all locks and counters"
		 /*. Use with great caution. Normally used to recover from an abnormal termination */
	    ),
	OPT_INTEGER('t', NULL, &facilitytime,
		    "run only for the specified <n> number of minutes"),
	OPT_STRING('L', NULL, &L_opt, "logfile",
		   "specifies an alternate log file"),
	//    OPT_INTEGER('I', NULL, &I_opt, "the log interval for all facilities except the IP traffic monitor. Value is in minutes"),
// PHIL  From manual:
// Sets the logging interval (in minutes) when the -L parameter is used. This over-
// rides the Log interval... setting in the Configure... menu. If omitted, the configured
// value is used. This parameter is ignored when the -L parameter is omitted and
// logging is disabled.
// The value specified here will affect all facilities except for the IP traffic monitor.
	OPT_END()
};

static int create_pidfile(void)
{
	int fd = open(VIPTRAF_PIDFILE, O_WRONLY|O_CREAT, 0644);
	if (fd < 0) {
		perror("can not open " VIPTRAF_PIDFILE);
		return -1;
	}

	if (lockf(fd, F_TLOCK, 0) < 0) {
		error("The PID file is locked " VIPTRAF_PIDFILE ". "
		      "Maybe other viptraf instance is running?can not acquire ");
		return -1;
	}

	fcntl(fd, F_SETFD, FD_CLOEXEC);

	char buf[sizeof(long) * 3 + 2];
	int len = sprintf(buf, "%lu\n", (long) getpid());
    if (write(fd, buf, len)) {}
    if (ftruncate(fd, len)) {}
	/* we leak opened+locked fd intentionally */
	return 0;
}

static void sanitize_dir(const char *dir)
{
	/* Check whether LOCKDIR exists (/var/run is on a tmpfs in Ubuntu) */
	if (access(dir, F_OK) != 0) {
		if (mkdir(dir, 0700) == -1)
			die("Cannot create %s: %s", dir, strerror(errno));

		if (chown(dir, 0, 0) == -1)
			die("Cannot change owner of %s: %s", dir,
			    strerror(errno));
	}
}

static void handle_internal_command(int argc, char **argv,
                                    const struct cmd_struct *commands)
{
	const char *cmd = argv[0];

	for (const struct cmd_struct *p = commands; p->cmd; ++p)
	{
		if (!strcmp(p->cmd, cmd))
			exit(p->fn(argc, argv));
	}
}

int shutdown(int return_code, const char *format, ...)
{
    if(pVideo)
    {
        delete pVideo;
        pVideo = NULL;
    }

    if(return_code)
    {
        va_list args;
        va_start(args, format);
        die(format, args);
        va_end(args);
    }

    exit(return_code);
}

int main(int argc, char **argv)
{
	int current_log_interval = 0;

    debug_log("Start program");

	if (geteuid() != 0)
		die("This program can be run only by the system administrator");

	const struct cmd_struct commands[] = {
		CMD(capture, "capture packet"),
		CMD_END(),
	};

	/* stupid, but for now needed machinery with argc, args
	 *
	 */
	char **internal_argv = argv;
	argc--;
	internal_argv++;

	if (argc > 0)
		handle_internal_command(argc, internal_argv, commands);

	argc++;

	/*
	 * Parse command line
	 */

	parse_opts(argc, argv, iptraf_ng_options, iptraf_ng_usage);

	if (help_opt)
		parse_usage_and_die(iptraf_ng_usage, iptraf_ng_options);

	int command = 0;

	command |= (i_opt) ? (1 << 0) : 0;
	command |= (d_opt) ? (1 << 1) : 0;
	command |= (s_opt) ? (1 << 2) : 0;
	command |= (z_opt) ? (1 << 3) : 0;
	command |= (l_opt) ? (1 << 4) : 0;
	command |= (g_opt) ? (1 << 5) : 0;

	if (__builtin_popcount(command) > 1)
		die("only one of -i|-d|-s|-z|-l|-g options must be used");

	strcpy(current_logfile, "");

	if (f_opt) {
		removetags();
		remove_sockets();
	}

	if (B_opt) {
		if (!command)
			die("one of -i|-d|-s|-z|-l|-g option is missing\n");
		daemonized = 1;
		setenv("TERM", "linux", 1);
	}

	if (L_opt) {
		if (strchr(L_opt, '/') != NULL)
			strncpy(current_logfile, L_opt, 80);
		else
			strncpy(current_logfile, get_path(T_LOGDIR, L_opt), 80);
	}
#if 0				/* this could never work */
	/* origin
	   } else if (opt == 'I') {
	   //this could never work
	   current_log_interval = atoi(optarg);
	   if (current_log_interval == 0)
	   fprintf(stderr, "Invalid log interval value\n");

	   exit(1);
	   } else if (opt == 'G') {
	 */
	if (I_opt == 0) {
		fprintf(stderr, "fatal: Invalid log interval value\n");
		exit(1);
	} else
		current_log_interval = I_opt;
#endif

	if ((getenv("TERM") == NULL) && (!daemonized))
		die("Your TERM variable is not set.\n"
		    "Please set it to an appropriate value");

	loadoptions();


	if (create_pidfile() < 0)
        return shutdown(-1, "");

	/*
	 * If a facility is directly invoked from the command line, check for
	 * a daemonization request
	 */

	if (daemonized && command) {
		switch (fork()) {
		case 0:	/* child */
			setsid();
            FILE *f;
            f = freopen("/dev/null", "w", stdout);  /* redirect std output */
            if (NULL == f)
                die("%s(): f == NULL", __FUNCTION__);
            f = freopen("/dev/null", "r", stdin);   /* redirect std input */
            if (NULL == f)
                die("%s(): f == NULL", __FUNCTION__);
            f = freopen("/dev/null", "w", stderr);  /* redirect std error */
            if (NULL == f)
                die("%s(): f == NULL", __FUNCTION__);
			signal(SIGUSR2, term_usr2_handler);

			options.logging = 1;
			break;
		case -1:	/* error */
		default:	/* parent */
            return shutdown(-2, "Fork error, %s cannot run in background", VIPTRAF_NAME);
		}
	}

	sanitize_dir(LOCKDIR);
	sanitize_dir(WORKDIR);

    pVideo = new VideoNcurses;
    if(!pVideo)
        return shutdown(-3, "Unable to create video object");
    if(!daemonized)
    {
        pVideo->Init();
        if(!pVideo->IsEnabled())
            return shutdown(-4, "The video cannot be init");
        pVideo->InitColors(options.color);
    }

	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);

	/*
	 * Set logfilename variable to NULL if -L was specified without an
	 * appropriate facility on the command line.
	 */

	if (command == 0)
		strcpy(current_logfile, "");

	/*
	 * If by this time the logfile is still acceptable, obtain the
	 * logspan from the command line if so specified.
	 */

	if (current_logfile[0] != '\0') {
		options.logging = 1;
		if (current_log_interval != 0) {
			options.logspan = current_log_interval;
		}
	}

	/*
	 * Load saved filter
	 */
	loadfilters();
//    indicate("");

	/* bad, bad, bad name draw_desktop()
	 * hide all into tui_top_panel(char *msg)
	 * */

    Desktop *pdesk = new Desktop();
    if(!pdesk)
        shutdown(-6, "Unable to init the desktop");
    pdesk->Run();

    /* simplify
	if (g_opt)
		ifstats(facilitytime);
	else if (i_opt)
		if (strcmp(i_opt, "all") == 0)
			ipmon(facilitytime, NULL);
		else
			ipmon(facilitytime, i_opt);
	else if (l_opt)
		if (strcmp(l_opt, "all") == 0)
			hostmon(facilitytime, NULL);
		else
			hostmon(facilitytime, l_opt);
	else if (d_opt)
		detstats(d_opt, facilitytime);
	else if (s_opt)
		servmon(s_opt, facilitytime);
	else if (z_opt)
		packet_size_breakdown(z_opt, facilitytime);
    else
        program_interface();
*/
//cleanup:
    shutdown(0, "");
	unlink(VIPTRAF_PIDFILE);
//bailout:
	return 0;
}
