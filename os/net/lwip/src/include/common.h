/*
 * common.h
 */

#ifndef _DNRD_COMMON_H_
#define _DNRD_COMMON_H_
#include <tinyara/config.h>
//#include "osdep_service.h"

#define MAX_SERV           3          /* maximum number of DNS servers */
//#define ENABLE_TCP
struct dnssrv_t {
	int                    sock;      /* for communication with server */
	struct sockaddr_in     addr;      /* IP address of server */
	char                  *domain;    /* optional domain to match.  Set to
					 zero for a default server */
};
//extern rtos_mutex_t dns_server_entry_lock; //abhishek TODO Change
typedef void	            *_mutex;

extern _mutex dns_server_entry_lock;

extern int     isDNRDRunning;
//extern const char*         version;   /* the version number for this program */
//extern const char*         progname;  /* the name of this program */
extern int                 opt_debug; /* debugging option */
//extern const char*         pid_file; /* File containing current daemon's PID */
extern int                 isock;     /* for communication with clients */
//extern int                 tcpsock;   /* same as isock, but for tcp requests */
extern struct dnssrv_t     dns_srv[]; /* DNS server information struct */
extern int                 serv_act; /* index into dns_srv for active server */
extern unsigned int        serv_cnt;  /* number of DNS servers */
extern struct sockaddr_in  recv_addr; /* address on which we receive queries */
//extern uid_t               daemonuid; /* to switch to once daemonised */
//extern gid_t               daemongid; /* to switch to once daemonised */
//extern int                 gotterminal;
//extern char		   master_param[200];
//extern sem_t               dnrd_sem;  /* Used for all thread synchronization */

/* kill any currently running copies of dnrd */
int kill_current(void);

/* print messages to stderr or syslog */
void log_msg(const char *fmt, ...);

/* same, but only if debugging is turned on */
void log_debug(const char *fmt, ...);

/* cleanup everything and exit */
void cleanexit(int status);

/* Reads in the domain name as a string, allocates space for the CNAME
   version of it */
char *make_cname(const char *text);
void sprintf_cname(const char *cname, int namesize, char *buf, int bufsize);

/* Dumping DNS packets */
int dump_dnspacket(char *type, char *packet, int len);


#endif  /* _DNRD_COMMON_H_ */
