/*
 * iperf, Copyright (c) 2014, 2016, 2017, The Regents of the University of
 * California, through Lawrence Berkeley National Laboratory (subject
 * to receipt of any required approvals from the U.S. Dept. of
 * Energy).  All rights reserved.
 *
 * If you have questions about your rights to use or distribute this
 * software, please contact Berkeley Lab's Technology Transfer
 * Department at TTD@lbl.gov.
 *
 * NOTICE.  This software is owned by the U.S. Department of Energy.
 * As such, the U.S. Government has been granted for itself and others
 * acting on its behalf a paid-up, nonexclusive, irrevocable,
 * worldwide license in the Software to reproduce, prepare derivative
 * works, and perform publicly and display publicly.  Beginning five
 * (5) years after the date permission to assert copyright is obtained
 * from the U.S. Department of Energy, and subject to any subsequent
 * five (5) year renewals, the U.S. Government is granted for itself
 * and others acting on its behalf a paid-up, nonexclusive,
 * irrevocable, worldwide license in the Software to reproduce,
 * prepare derivative works, distribute copies to the public, perform
 * publicly and display publicly, and to permit others to do so.
 *
 * This code is distributed under a BSD style license, see the LICENSE
 * file for complete information.
 */
/* iperf_util.c
 *
 * Iperf utility functions
 *
 */
#include "iperf_config.h"

#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include "platform_stdlib.h"
#include "lwipconf.h" //realtek add

#include <cJSON.h>
#include "iperf.h"
#include "iperf_api.h"

/*
 * Read entropy from /dev/urandom
 * Errors are fatal.
 * Returns 0 on success.
 */
int readentropy(void *out, size_t outsize)
{
	/* To avoid gcc warnings */
	(void) out;
	(void) outsize;
#if 0
	static FILE *frandom;
	static const char rndfile[] = "/dev/urandom";

	if (!outsize) {
		return 0;
	}

	if (frandom == NULL) {
		frandom = fopen(rndfile, "rb");
		if (frandom == NULL) {
			iperf_errexit(NULL, "error - failed to open %s: %s\n",
						  rndfile, strerror(errno));
		}
		setbuf(frandom, NULL);
	}
	if (fread(out, 1, outsize, frandom) != outsize) {
		iperf_errexit(NULL, "error - failed to read %s: %s\n",
					  rndfile,
					  feof(frandom) ? "EOF" : strerror(errno));
	}
#endif
	return 0;
}


/* make_cookie
 *
 * Generate and return a cookie string
 *
 * Iperf uses this function to create test "cookies" which
 * server as unique test identifiers. These cookies are also
 * used for the authentication of stream connections.
 * Assumes cookie has size (COOKIE_SIZE + 1) char's.
 */

void
make_cookie(char *cookie)
{
	unsigned char *out = (unsigned char *)cookie;
	size_t pos;
	static const unsigned char rndchars[] = "abcdefghijklmnopqrstuvwxyz234567";

	readentropy(out, COOKIE_SIZE);
	for (pos = 0; pos < (COOKIE_SIZE - 1); pos++) {
		out[pos] = rndchars[out[pos] % (sizeof(rndchars) - 1)];
	}
	out[pos] = '\0';
}


/* is_closed
 *
 * Test if the file descriptor fd is closed.
 *
 * Iperf uses this function to test whether a TCP stream socket
 * is closed, because accepting and denying an invalid connection
 * in iperf_tcp_accept is not considered an error.
 */

int
is_closed(int fd)
{
	struct timeval tv;
	fd_set readset;

	FD_ZERO(&readset);
	FD_SET(fd, &readset);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select(fd + 1, &readset, NULL, NULL, &tv) < 0) {
		if (errno == EBADF) {
			return 1;
		}
	}
	return 0;
}


double
timeval_to_double(struct timeval *tv)
{
	double d;

	d = tv->tv_sec + tv->tv_usec / 1000000;

	return d;
}

int
timeval_equals(struct timeval *tv0, struct timeval *tv1)
{
	if (tv0->tv_sec == tv1->tv_sec && tv0->tv_usec == tv1->tv_usec) {
		return 1;
	} else {
		return 0;
	}
}

double
timeval_diff(struct timeval *tv0, struct timeval *tv1)
{
	double time1, time2;

	time1 = tv0->tv_sec + (tv0->tv_usec / 1000000.0);
	time2 = tv1->tv_sec + (tv1->tv_usec / 1000000.0);

	time1 = time1 - time2;
	if (time1 < 0) {
		time1 = -time1;
	}
	return time1;
}

#if 0
int
delay(int64_t ns)
{
	struct timespec req, rem;

	req.tv_sec = 0;

	while (ns >= 1000000000L) {
		ns -= 1000000000L;
		req.tv_sec += 1;
	}

	req.tv_nsec = ns;

	while (nanosleep(&req, &rem) == -1)
		if (EINTR == errno) {
			memcpy(&req, &rem, sizeof(rem));
		} else {
			return -1;
		}
	return 0;
}
#endif

# ifdef DELAY_SELECT_METHOD
int
delay(int us)
{
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = us;
	(void) select(1, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
	return 1;
}
#endif

char *RemoveCRLF(char *psz)
{
	char *s = psz;
	char *d = s;
	while (*d != '\0') {
		s++;
		if (*d == '\r' || *d == '\n' ||  *d == '\t' || *d == '<' || *d == '%') {
			*d = *s;
			continue;
		}
		d++;
		*d = *s;
	}
	*d = '\0';
	return psz;
}

void cpu_util(double pcpu[3])
{
	if (pcpu == NULL) {
		return;
	}
#if defined configGENERATE_RUN_TIME_STATS && (configGENERATE_RUN_TIME_STATS == 1)
	struct cpu_utils cpu_util;
	char cBuffer[512];
	char *ptr;
	u8 i = 0;
	u8 flag_first = 1;

	char *idle0_name = "IDLE0";
	char *idle1_name = "IDLE1";
	char *tcpip_name = "TCP_IP";
	u64 total_count = 0;
	u32 user_count = 0, idle_count = 0, sys_count = 0;
	double user_percent = 0, sys_percent = 0;

	vTaskGetRunTimeStats((char *)cBuffer);
	//printf("cBuffer:%s\n",cBuffer);
	ptr = cBuffer;
	while (*ptr != '\0') {

		if (i % 3 == 0) {
			if (flag_first) {
				ptr = strtok(cBuffer,  "\t");
				flag_first = 0;
			} else {
				ptr = strtok(NULL, "\t");
			}

			ptr = RemoveCRLF(ptr);
			if (ptr == NULL || strlen(ptr) == 0) {
				break;
			}
			cpu_util.task_name = (char *) malloc(strlen(ptr) + 1);
			memset(cpu_util.task_name, 0, strlen(ptr) + 1);
			memcpy(cpu_util.task_name, ptr, strlen(ptr));
		} else if (i % 3 == 1) {
			ptr = strtok(NULL, "\t\t");
			ptr = RemoveCRLF(ptr);
			cpu_util.count = (u32) atoi(ptr);
			total_count = total_count + cpu_util.count;
		} else {
			ptr = strtok(NULL,  "\r\n");
			ptr = RemoveCRLF(ptr);
			cpu_util.util = (u8) atoi(ptr);

			if (memcmp(cpu_util.task_name, g_server_task.task_name, 9) == 0  && g_server_task.task) {
				user_count = user_count + cpu_util.count;
			} else if (memcmp(cpu_util.task_name, g_client_task.task_name, 9) == 0 && g_client_task.task) {
				user_count = user_count + cpu_util.count;
			} else if (memcmp(cpu_util.task_name, tcpip_name, strlen(tcpip_name)) == 0) {
				user_count = user_count + cpu_util.count;
			} else if (memcmp(cpu_util.task_name, idle0_name, strlen(idle0_name)) == 0) {
				idle_count = idle_count + cpu_util.count;
			} else if (memcmp(cpu_util.task_name, idle1_name, strlen(idle1_name)) == 0) {
				idle_count = idle_count + cpu_util.count;
			}

			free(cpu_util.task_name);
		}
		i++;
	}

	user_percent = (user_count * 100.0) / total_count;
	sys_count = total_count - user_count - idle_count ;
	sys_percent = (sys_count * 100.0) / total_count;
	//printf("user_percent:%f,sys_percent:%f\n",user_percent,sys_percent);
	pcpu[0] = user_percent + sys_percent;
	pcpu[1] = user_percent;
	pcpu[2] = sys_percent;
#else
	pcpu[0] = 1;
	pcpu[1] = 0;
	pcpu[2] = 0;
#endif
}

const char *
get_system_info(void)
{
#if 0
	static char buf[1024];
	struct utsname  uts;

	memset(buf, 0, 1024);
	uname(&uts);

	snprintf(buf, sizeof(buf), "%s %s %s %s %s", uts.sysname, uts.nodename,
			 uts.release, uts.version, uts.machine);

	return buf;
#else
	return "NULL";

#endif
}


const char *
get_optional_features(void)
{
	static char features[1024];
	unsigned int numfeatures = 0;

	snprintf(features, sizeof(features), "Optional features available: ");

#if defined(HAVE_CPU_AFFINITY)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "CPU affinity setting",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_CPU_AFFINITY */

#if defined(HAVE_FLOWLABEL)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "IPv6 flow label",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_FLOWLABEL */

#if defined(HAVE_SCTP)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "SCTP",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_SCTP */

#if defined(HAVE_TCP_CONGESTION)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "TCP congestion algorithm setting",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_TCP_CONGESTION */

#if defined(HAVE_SENDFILE)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "sendfile / zerocopy",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_SENDFILE */

#if defined(HAVE_SO_MAX_PACING_RATE)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "socket pacing",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_SO_MAX_PACING_RATE */

#if defined(HAVE_SSL)
	if (numfeatures > 0) {
		strncat(features, ", ",
				sizeof(features) - strlen(features) - 1);
	}
	strncat(features, "authentication",
			sizeof(features) - strlen(features) - 1);
	numfeatures++;
#endif /* HAVE_SSL */

	if (numfeatures == 0) {
		strncat(features, "None",
				sizeof(features) - strlen(features) - 1);
	}

	return features;
}

/* Helper routine for building cJSON objects in a printf-like manner.
**
** Sample call:
**   j = iperf_json_printf("foo: %b  bar: %d  bletch: %f  eep: %s", b, i, f, s);
**
** The four formatting characters and the types they expect are:
**   %b  boolean           int
**   %d  integer           int64_t
**   %f  floating point    double
**   %s  string            char *
** If the values you're passing in are not these exact types, you must
** cast them, there is no automatic type coercion/widening here.
**
** The colons mark the end of field names, and blanks are ignored.
**
** This routine is not particularly robust, but it's not part of the API,
** it's just for internal iperf3 use.
*/
cJSON *
iperf_json_printf(const char *format, ...)
{
	cJSON *o;
	va_list argp;
	const char *cp;
	char name[100];
	char *np;
	cJSON *j;

	o = cJSON_CreateObject();
	if (o == NULL) {
		printf("iperf_json_printf, i malloc fail\n");
		return NULL;
	}
	va_start(argp, format);
	np = name;
	for (cp = format; *cp != '\0'; ++cp) {
		switch (*cp) {
		case ' ':
			break;
		case ':':
			*np = '\0';
			break;
		case '%':
			++cp;
			switch (*cp) {
			case 'b':
				j = cJSON_CreateBool(va_arg(argp, int));
				break;
			case 'd':
				j = cJSON_CreateNumber(va_arg(argp, int64_t));
				break;
			case 'f':
				j = cJSON_CreateNumber(va_arg(argp, double));
				break;
			case 's':
				j = cJSON_CreateString(va_arg(argp, char *));
				break;
			default:
				va_end(argp);
				return NULL;
			}
			if (j == NULL) {
				printf("iperf_json_printf, j malloc fail\n");
				va_end(argp);
				return NULL;
			}
			cJSON_AddItemToObject(o, name, j);
			np = name;
			break;
		default:
			*np++ = *cp;
			break;
		}
	}
	va_end(argp);
	return o;
}

/* Debugging routine to dump out an fd_set. */
void
iperf_dump_fdset(FILE *fp, char *str, int nfds, fd_set *fds)
{
	int fd;
	int comma;

	fprintf(fp, "%s: [", str);
	comma = 0;
	for (fd = 0; fd < nfds; ++fd) {
		if (FD_ISSET(fd, fds)) {
			if (comma) {
				fprintf(fp, ", ");
			}
			fprintf(fp, "%d", fd);
			comma = 1;
		}
	}
	fprintf(fp, "]\n");
}
