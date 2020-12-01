#ifndef VIPTRAF_LOG_H
#define VIPTRAF_LOG_H

/***

log.h - the iptraf logging facility header file

***/

#define TIME_TARGET_MAX     30

char *gen_instance_logname(const char *stemplate, int instance_id);
void input_logfile(char *target, int *aborted);
void opentlog(FILE ** fd, const char *logfilename);
void writelog(int logging, FILE * fd, const char *msg);
void genatime(time_t now, char *atime);
void write_daemon_err(const char *msg, va_list vararg);

#define DEBUG 1
#ifdef DEBUG
#define debug_log(...) { write_daemon_log(__VA_ARGS__); }
#else
#define debug_log(x, ...) {}
#endif

void write_daemon_log(const char *format, ...);
void rotate_logfile(FILE ** fd, const char *name);
void check_rotate_flag(FILE ** fd);
void announce_rotate_prepare(FILE * fd);
void announce_rotate_complete(FILE * fd);

#endif	/* VIPTRAF_LOG_H */
