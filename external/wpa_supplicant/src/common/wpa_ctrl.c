/*
 * wpa_supplicant/hostapd control interface library
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "includes.h"

#ifdef CONFIG_CTRL_IFACE

#ifdef CONFIG_CTRL_IFACE_UNIX
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#endif							/* CONFIG_CTRL_IFACE_UNIX */
#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
#include <netdb.h>
#endif							/* CONFIG_CTRL_IFACE_UDP_REMOTE */

#ifdef ANDROID
#include <dirent.h>
#include <sys/stat.h>
#include <cutils/sockets.h>
#include "private/android_filesystem_config.h"
#endif							/* ANDROID */

#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
#include <net/if.h>
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */

#ifdef CONFIG_CTRL_IFACE_FIFO
#include "pthread.h"
#endif

#include "wpa_ctrl.h"
#include "common.h"

#if defined(CONFIG_CTRL_IFACE_UNIX) || defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_FIFO)
#define CTRL_IFACE_SOCKET
#endif

static int wpa_ctrl_request_ext(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len, char *reply, size_t *reply_len, void (*msg_cb)(char *msg, size_t len));

/**
 * struct wpa_ctrl - Internal structure for control interface library
 *
 * This structure is used by the wpa_supplicant/hostapd control interface
 * library to store internal data. Programs using the library should not touch
 * this data directly. They can only use the pointer to the data structure as
 * an identifier for the control interface connection and use this as one of
 * the arguments for most of the control interface library functions.
 */
struct wpa_ctrl {
#ifdef CONFIG_CTRL_IFACE_UDP
	int s;
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
	struct sockaddr_in6 local;
	struct sockaddr_in6 dest;
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	struct sockaddr_in local;
	struct sockaddr_in dest;
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	char *cookie;
	char *remote_ifname;
	char *remote_ip;
#endif							/* CONFIG_CTRL_IFACE_UDP */
#ifdef CONFIG_CTRL_IFACE_UNIX
	int s;
	struct sockaddr_un local;
	struct sockaddr_un dest;
#endif							/* CONFIG_CTRL_IFACE_UNIX */
#ifdef CONFIG_CTRL_IFACE_NAMED_PIPE
	HANDLE pipe;
#endif							/* CONFIG_CTRL_IFACE_NAMED_PIPE */
#ifdef CONFIG_CTRL_IFACE_FIFO
	int s;						// For sending requests
	int s_cfm;					// For receiving confirms
	int monitor;				// The monitor file handle - read only
	char *cookie;
	pthread_mutex_t busy;		// Ensure only a single thread uses the if at any given time
	/*
	 * the following variables are used for the context switch during command write
	 * They are protected by the busy mutex.
	 * */
	int write_ret;
	char *write_cmd;
	size_t write_cmdlen;
	char *write_reply;
	size_t *write_replylen;
	void *write_msgcb;
	sem_t write_start;
	sem_t write_complete;
	bool write_stop;
	pthread_t write_thread;
	int pid;
#endif
};

#ifdef CONFIG_CTRL_IFACE_UNIX

#ifndef CONFIG_CTRL_IFACE_CLIENT_DIR
#define CONFIG_CTRL_IFACE_CLIENT_DIR "/tmp"
#endif							/* CONFIG_CTRL_IFACE_CLIENT_DIR */
#ifndef CONFIG_CTRL_IFACE_CLIENT_PREFIX
#define CONFIG_CTRL_IFACE_CLIENT_PREFIX "wpa_ctrl_"
#endif							/* CONFIG_CTRL_IFACE_CLIENT_PREFIX */

struct wpa_ctrl *wpa_ctrl_open(const char *ctrl_path)
{
	struct wpa_ctrl *ctrl;
	static int counter = 0;
	int ret;
	size_t res;
	int tries = 0;
	int flags;

	if (ctrl_path == NULL) {
		return NULL;
	}

	ctrl = os_zalloc(sizeof(*ctrl));
	if (ctrl == NULL) {
		return NULL;
	}

	ctrl->s = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (ctrl->s < 0) {
		os_free(ctrl);
		return NULL;
	}

	ctrl->local.sun_family = AF_UNIX;
	counter++;
try_again:
	ret = os_snprintf(ctrl->local.sun_path, sizeof(ctrl->local.sun_path), CONFIG_CTRL_IFACE_CLIENT_DIR "/" CONFIG_CTRL_IFACE_CLIENT_PREFIX "%d-%d", (int)getpid(), counter);
	if (os_snprintf_error(sizeof(ctrl->local.sun_path), ret)) {
		close(ctrl->s);
		os_free(ctrl);
		return NULL;
	}
	tries++;
	if (bind(ctrl->s, (struct sockaddr *)&ctrl->local, sizeof(ctrl->local)) < 0) {
		if (errno == EADDRINUSE && tries < 2) {
			/*
			 * getpid() returns unique identifier for this instance
			 * of wpa_ctrl, so the existing socket file must have
			 * been left by unclean termination of an earlier run.
			 * Remove the file and try again.
			 */
			unlink(ctrl->local.sun_path);
			goto try_again;
		}
		close(ctrl->s);
		os_free(ctrl);
		return NULL;
	}
#ifdef ANDROID
	chmod(ctrl->local.sun_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	chown(ctrl->local.sun_path, AID_SYSTEM, AID_WIFI);

	if (os_strncmp(ctrl_path, "@android:", 9) == 0) {
		if (socket_local_client_connect(ctrl->s, ctrl_path + 9, ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_DGRAM) < 0) {
			close(ctrl->s);
			unlink(ctrl->local.sun_path);
			os_free(ctrl);
			return NULL;
		}
		return ctrl;
	}

	/*
	 * If the ctrl_path isn't an absolute pathname, assume that
	 * it's the name of a socket in the Android reserved namespace.
	 * Otherwise, it's a normal UNIX domain socket appearing in the
	 * filesystem.
	 */
	if (*ctrl_path != '/') {
		char buf[21];
		os_snprintf(buf, sizeof(buf), "wpa_%s", ctrl_path);
		if (socket_local_client_connect(ctrl->s, buf, ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_DGRAM) < 0) {
			close(ctrl->s);
			unlink(ctrl->local.sun_path);
			os_free(ctrl);
			return NULL;
		}
		return ctrl;
	}
#endif							/* ANDROID */

	ctrl->dest.sun_family = AF_UNIX;
	if (os_strncmp(ctrl_path, "@abstract:", 10) == 0) {
		ctrl->dest.sun_path[0] = '\0';
		os_strlcpy(ctrl->dest.sun_path + 1, ctrl_path + 10, sizeof(ctrl->dest.sun_path) - 1);
	} else {
		res = os_strlcpy(ctrl->dest.sun_path, ctrl_path, sizeof(ctrl->dest.sun_path));
		if (res >= sizeof(ctrl->dest.sun_path)) {
			close(ctrl->s);
			os_free(ctrl);
			return NULL;
		}
	}
	if (connect(ctrl->s, (struct sockaddr *)&ctrl->dest, sizeof(ctrl->dest)) < 0) {
		close(ctrl->s);
		unlink(ctrl->local.sun_path);
		os_free(ctrl);
		return NULL;
	}

	/*
	 * Make socket non-blocking so that we don't hang forever if
	 * target dies unexpectedly.
	 */
	flags = fcntl(ctrl->s, F_GETFL);
	if (flags >= 0) {
		flags |= O_NONBLOCK;
		if (fcntl(ctrl->s, F_SETFL, flags) < 0) {
			perror("fcntl(ctrl->s, O_NONBLOCK)");
			/* Not fatal, continue on. */
		}
	}

	return ctrl;
}

void wpa_ctrl_close(struct wpa_ctrl *ctrl)
{
	if (ctrl == NULL) {
		return;
	}
	unlink(ctrl->local.sun_path);
	if (ctrl->s >= 0) {
		close(ctrl->s);
	}
	os_free(ctrl);
}

#ifdef ANDROID
/**
 * wpa_ctrl_cleanup() - Delete any local UNIX domain socket files that
 * may be left over from clients that were previously connected to
 * wpa_supplicant. This keeps these files from being orphaned in the
 * event of crashes that prevented them from being removed as part
 * of the normal orderly shutdown.
 */
void wpa_ctrl_cleanup(void)
{
	DIR *dir;
	struct dirent entry;
	struct dirent *result;
	size_t dirnamelen;
	size_t maxcopy;
	char pathname[PATH_MAX];
	char *namep;

	if ((dir = opendir(CONFIG_CTRL_IFACE_CLIENT_DIR)) == NULL) {
		return;
	}

	dirnamelen = (size_t)os_snprintf(pathname, sizeof(pathname), "%s/", CONFIG_CTRL_IFACE_CLIENT_DIR);
	if (dirnamelen >= sizeof(pathname)) {
		closedir(dir);
		return;
	}
	namep = pathname + dirnamelen;
	maxcopy = PATH_MAX - dirnamelen;
	while (readdir_r(dir, &entry, &result) == 0 && result != NULL) {
		if (os_strlcpy(namep, entry.d_name, maxcopy) < maxcopy) {
			unlink(pathname);
		}
	}
	closedir(dir);
}
#endif							/* ANDROID */

#else							/* CONFIG_CTRL_IFACE_UNIX */

#ifdef ANDROID
void wpa_ctrl_cleanup(void)
{
}
#endif							/* ANDROID */

#endif							/* CONFIG_CTRL_IFACE_UNIX */

#ifdef CONFIG_CTRL_IFACE_FIFO

void wpa_ctrl_write_handler(void *param)
{
	struct wpa_ctrl *ctrl = (struct wpa_ctrl *)param;
	ctrl->write_stop = false;
	/* wait for someone that wants to write from other context */
	while (!ctrl->write_stop) {
		sem_wait(&ctrl->write_start);
		if (ctrl->write_stop) {
			sem_post(&ctrl->write_complete);
			pthread_exit(NULL);
		}
		ctrl->write_ret = wpa_ctrl_request_ext(ctrl, ctrl->write_cmd, ctrl->write_cmdlen, ctrl->write_reply, ctrl->write_replylen, ctrl->write_msgcb);
		sem_post(&ctrl->write_complete);
	}
	pthread_exit(NULL);
}

#endif

#if defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_FIFO)
struct wpa_ctrl *wpa_ctrl_open(const char *ctrl_path)
{
	struct wpa_ctrl *ctrl;
	char buf[128];
	size_t len;
#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	struct hostent *h;
#endif							/* CONFIG_CTRL_IFACE_UDP_REMOTE */

	ctrl = os_zalloc(sizeof(*ctrl));
	if (ctrl == NULL) {
		return NULL;
	}
	/* WARNING: UDP is unreliable (indirectly selected by use of SOCK_DRAM), hence not suitable
	 * for API interfaces.
	 *  - change to use TCP (SOCK_STREAM), as this is a reliable transport. */
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
	ctrl->s = socket(PF_INET6, SOCK_STREAM, 0);
#elif defined(CONFIG_CTRL_IFACE_FIFO)

	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&(ctrl->busy), &mutexAttr);

	ctrl->monitor = -1;
	ctrl->s_cfm = -1;
	ctrl->s = open(WPA_CTRL_FIFO_DEV_REQ, O_WRONLY);
	if (ctrl->s >= 0) {
		ctrl->s_cfm = open(WPA_CTRL_FIFO_DEV_CFM, O_RDONLY);
		if (ctrl->s_cfm < 0) {
			close(ctrl->s);
			ctrl->s = -1;
		}
	}
	sem_init(&ctrl->write_start, 0, 0);
	sem_init(&ctrl->write_complete, 0, 0);
	if (pthread_create(&ctrl->write_thread, NULL, (void *)wpa_ctrl_write_handler, ctrl)) {
		wpa_printf(MSG_ERROR, "Failed to create thread for writer\n");
	} else {
		pthread_setname_np(ctrl->write_thread, "WPA Ctrl Iface FIFO");
	}
	ctrl->pid = getpid();		// store the initial pid
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	ctrl->s = socket(PF_INET, SOCK_STREAM, 0);
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	if (ctrl->s < 0) {
		perror("unable to open ctrl interface");
		os_free(ctrl);
		return NULL;
	}
#ifdef CONFIG_CTRL_IFACE_UDP
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
	ctrl->local.sin6_family = AF_INET6;
#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	ctrl->local.sin6_addr = in6addr_any;
#else							/* CONFIG_CTRL_IFACE_UDP_REMOTE */
	inet_pton(AF_INET6, "::1", &ctrl->local.sin6_addr);
#endif							/* CONFIG_CTRL_IFACE_UDP_REMOTE */
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	ctrl->local.sin_family = AF_INET;
#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	ctrl->local.sin_addr.s_addr = INADDR_ANY;
#else							/* CONFIG_CTRL_IFACE_UDP_REMOTE */
	ctrl->local.sin_addr.s_addr = htonl((127 << 24) | 1);
#endif							/* CONFIG_CTRL_IFACE_UDP_REMOTE */
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */

	if (bind(ctrl->s, (struct sockaddr *)&ctrl->local, sizeof(ctrl->local)) < 0) {
		close(ctrl->s);
		os_free(ctrl);
		return NULL;
	}
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
	ctrl->dest.sin6_family = AF_INET6;
	inet_pton(AF_INET6, "::1", &ctrl->dest.sin6_addr);
	ctrl->dest.sin6_port = htons(WPA_CTRL_IFACE_PORT);
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	ctrl->dest.sin_family = AF_INET;
	ctrl->dest.sin_addr.s_addr = htonl((127 << 24) | 1);
	ctrl->dest.sin_port = htons(WPA_CTRL_IFACE_PORT);
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */

#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	if (ctrl_path) {
		char *port, *name;
		int port_id;
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
		char *scope;
		int scope_id = 0;
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */

		name = os_strdup(ctrl_path);
		if (name == NULL) {
			close(ctrl->s);
			os_free(ctrl);
			return NULL;
		}
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
		port = os_strchr(name, ',');
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
		port = os_strchr(name, ':');
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */

		if (port) {
			port_id = atoi(&port[1]);
			port[0] = '\0';
		} else {
			port_id = WPA_CTRL_IFACE_PORT;
		}

#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
		scope = os_strchr(name, '%');
		if (scope) {
			scope_id = if_nametoindex(&scope[1]);
			scope[0] = '\0';
		}
		h = gethostbyname2(name, AF_INET6);
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
		h = gethostbyname(name);
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
		ctrl->remote_ip = os_strdup(name);
		os_free(name);
		if (h == NULL) {
			perror("gethostbyname");
			close(ctrl->s);
			os_free(ctrl->remote_ip);
			os_free(ctrl);
			return NULL;
		}
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
		ctrl->dest.sin6_scope_id = scope_id;
		ctrl->dest.sin6_port = htons(port_id);
		os_memcpy(&ctrl->dest.sin6_addr, h->h_addr, h->h_length);
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
		ctrl->dest.sin_port = htons(port_id);
		os_memcpy(&ctrl->dest.sin_addr.s_addr, h->h_addr, h->h_length);
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
	} else {
		ctrl->remote_ip = os_strdup("localhost");
	}
#endif							/* CONFIG_CTRL_IFACE_UDP_REMOTE */

	if (connect(ctrl->s, (struct sockaddr *)&ctrl->dest, sizeof(ctrl->dest)) < 0) {
#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
		char addr[INET6_ADDRSTRLEN];
		wpa_printf(MSG_ERROR, "connect(%s:%d) failed: %s", inet_ntop(AF_INET6, &ctrl->dest.sin6_addr, addr, sizeof(ctrl->dest)), ntohs(ctrl->dest.sin6_port), strerror(errno));
#else							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
		wpa_printf(MSG_ERROR, "connect(%s:%d) failed: %s", inet_ntoa(ctrl->dest.sin_addr), ntohs(ctrl->dest.sin_port), strerror(errno));
#endif							/* CONFIG_CTRL_IFACE_UDP_IPV6 */
		close(ctrl->s);
		os_free(ctrl->remote_ip);
		os_free(ctrl);
		return NULL;
	}
#endif

	len = sizeof(buf) - 1;
	if (wpa_ctrl_request(ctrl, "GET_COOKIE", 10, buf, &len, NULL) == 0) {
		buf[len] = '\0';
		ctrl->cookie = os_strdup(buf);
	}
#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	if (wpa_ctrl_request(ctrl, "IFNAME", 6, buf, &len, NULL) == 0) {
		buf[len] = '\0';
		ctrl->remote_ifname = os_strdup(buf);
	}
#endif
	return ctrl;
}

#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE

char *wpa_ctrl_get_remote_ifname(struct wpa_ctrl *ctrl)
{
#define WPA_CTRL_MAX_PS_NAME 100
	static char ps[WPA_CTRL_MAX_PS_NAME] = { };
	os_snprintf(ps, WPA_CTRL_MAX_PS_NAME, "%s/%s", ctrl->remote_ip, ctrl->remote_ifname);
	return ps;
}
#endif

void wpa_ctrl_close(struct wpa_ctrl *ctrl)
{
#ifdef CONFIG_CTRL_IFACE_FIFO
	if (ctrl->monitor >= 0) {
		close(ctrl->monitor);
	}
	if (ctrl->s_cfm >= 0) {
		close(ctrl->s_cfm);
	}

	ctrl->write_stop = true;
	sem_post(&ctrl->write_start);
	pthread_join(ctrl->write_thread, 0);
	sem_destroy(&ctrl->write_start);
	sem_destroy(&ctrl->write_complete);

#endif
	if (ctrl->s >= 0) {
		close(ctrl->s);
	}
	os_free(ctrl->cookie);
#ifdef CONFIG_CTRL_IFACE_UDP
	os_free(ctrl->remote_ifname);
	os_free(ctrl->remote_ip);
#endif
	pthread_mutex_destroy(&(ctrl->busy));
	os_free(ctrl);
}

#endif

#if defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_FIFO)

#define CTRL_TO_HEX(val) (((val) <= 9) ? (val) + '0' : (val) - 10 + 'a')
#define CTRL_FROM_HEX(val) (((val) <= '9') ? (val) - '0' : (val) - 'a' + 10)

/***
 * Writes the header to buf and returns the pointer to the next free entry.
 * Ensure the buffer provided it at least CTRL_HEADER_SIZE + msg_len in size
 */
char *wpa_ctrl_set_header(char *buf, unsigned short msg_len)
{
	buf[3] = CTRL_TO_HEX(msg_len & 0x0f);
	buf[2] = CTRL_TO_HEX(msg_len >> 4 & 0x0f);
	buf[1] = CTRL_TO_HEX(msg_len >> 8 & 0x0f);
	buf[0] = CTRL_TO_HEX(msg_len >> 12 & 0x0f);
	return &buf[4];
}

/***
 * Extracts a header set by wpa_ctrl_set_header() from a buffer
 */
int wpa_ctrl_get_header(char *buf)
{
	unsigned short len;
	len = CTRL_FROM_HEX(buf[3]);
	len |= CTRL_FROM_HEX(buf[2]) << 4;
	len |= CTRL_FROM_HEX(buf[1]) << 8;
	len |= CTRL_FROM_HEX(buf[0]) << 12;
	return len;
}

/***
 * Receive a message from a STREAM based socket - e.g. a reliable TCP socket
 */
int wpa_ctrl_recvfrom(int sock, char *buf, size_t len
#ifdef CONFIG_CTRL_IFACE_UDP
					  , int flags, struct sockaddr *from, size_t *fromlen
#endif
					 )
{
	size_t msg_size;
	size_t offset = 0;
	ssize_t amount = 0;
	size_t recv_len;
	char *pos;
	int res;
#ifdef CONFIG_CTRL_IFACE_UDP
	UNUSED(flags);
#endif
	while ((amount >= 0) && ((amount + offset) < CTRL_HEADER_SIZE)) {
#ifdef CONFIG_CTRL_IFACE_UDP
		amount = recvfrom(sock, buf + offset, CTRL_HEADER_SIZE - offset, 0, from, fromlen);
#else
		amount = read(sock, buf + offset, CTRL_HEADER_SIZE - offset);
#endif
	}
	if (amount < 0) {
		return amount;
	} else {
		msg_size = wpa_ctrl_get_header(buf);
		offset = 0;
		amount = 0;
		buf[CTRL_HEADER_SIZE] = '\0';
		wpa_printf(MSG_EXCESSIVE, "received message size: %d ('%s')", msg_size, buf);
	}
	while ((amount >= 0) && ((amount + offset) < msg_size)) {
		if (msg_size > len) {	// This is BAD!!! read out the message to keep alignment.
			pos = buf;
		} else {
			pos = buf + offset;
		}
		recv_len = msg_size - offset;
#ifdef CONFIG_CTRL_IFACE_UDP
		amount = recvfrom(sock, pos, recv_len, 0, from, fromlen);
#else
		amount += read(sock, pos, recv_len);
#endif
	}
	if (msg_size > len) {
		return -1;				// Buffer too small for the message, return error
	}
	if (amount >= 0) {
		res = amount + offset;
	} else {
		res = amount;
	}
	return res;
}

static int _wpa_ctrl_sendto(int sock, const char *buf, size_t len
#ifdef CONFIG_CTRL_IFACE_UDP
							, struct sockaddr *to, size_t tolen
#endif
						   )
{
	size_t offset = 0;
	ssize_t amount = 0;
	struct os_reltime started_at;
	errno = 0;
	started_at.sec = 0;
	started_at.usec = 0;
	while ((offset += amount) < len) {
#ifdef CONFIG_CTRL_IFACE_UDP
		if ((amount = sendto(sock, buf + offset, len - offset, 0, to, tolen)) < 0) {
#else
		if ((amount = write(sock, buf + offset, len - offset)) < 0) {
#endif
			int err = errno;
			wpa_printf(MSG_ERROR, "_wpa_ctrl_sendto() error: %s", strerror(err));

			if (err == EAGAIN || err == EBUSY || err == EWOULDBLOCK) {
				amount = 0;		// Ensure we do not rewind the data pointer
				/*
				 * Must be a non-blocking socket... Try for a bit
				 * longer before giving up.
				 * CB: This is a comment from the original code - I see no reason for it
				 *     to be non-blocking, and a comment on why it have to be non-blocking
				 *     would have been more useful.
				 */
				if (started_at.sec == 0) {
					os_get_reltime(&started_at);
				} else {
					struct os_reltime n;
					os_get_reltime(&n);
					/* Try for a few seconds. */
					if (os_reltime_expired(&n, &started_at, 5)) {
						return -1;
					}
				}
			}
		}
	}
	return len;
}

/***
 * Transmit a length header followed by the data specified by buf.
 * WARNING: The function have a timeout of 5 seconds, hence if
 * if is not possible to deliver data within this time, the API
 * interface is broken, as sync is lost.
 * This timeout is inherited from the original UDP implementation,
 * but I have no idea why this timeout is needed, as it makes no sense
 * for an API to simply discard data.
 ***/
int wpa_ctrl_sendto(int sock, const char *buf, size_t len
#ifdef CONFIG_CTRL_IFACE_UDP
					, int flags, struct sockaddr *to, size_t tolen
#endif
				   )
{
	char header[CTRL_HEADER_SIZE];
	wpa_ctrl_set_header(header, len);
	_wpa_ctrl_sendto(sock, header, CTRL_HEADER_SIZE
#ifdef CONFIG_CTRL_IFACE_UDP
					 , to, tolen
#endif
					);
	_wpa_ctrl_sendto(sock, buf, len
#ifdef CONFIG_CTRL_IFACE_UDP
					 , to, tolen
#endif
					);

#ifdef CONFIG_CTRL_IFACE_UDP
	UNUSED(flags);
#endif
	return len + CTRL_HEADER_SIZE;
}

#endif							/* CONFIG_CTRL_IFACE_UDP || CONFIG_CTRL_IFACE_FIFO */

#ifdef CTRL_IFACE_SOCKET

static int wpa_ctrl_request_ext(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len, char *reply, size_t *reply_len, void (*msg_cb)(char *msg, size_t len))
{
	struct timeval tv;
	struct os_reltime started_at;
	int res;
	fd_set rfds;
	const char *_cmd;
	char *cmd_buf = NULL;
	size_t _cmd_len;
	int read_fd = ctrl->s;
	int write_fd = ctrl->s;
#ifdef CONFIG_CTRL_IFACE_FIFO
	read_fd = ctrl->s_cfm;
#endif

	// TODO: Remove the overhead of cookie
#if defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_FIFO)
	if (ctrl->cookie) {
		char *pos;
		_cmd_len = os_strlen(ctrl->cookie) + 1 + cmd_len;
		cmd_buf = os_malloc(_cmd_len);
		if (cmd_buf == NULL) {
			return -1;
		}
		_cmd = cmd_buf;
		pos = cmd_buf;
		os_strlcpy(pos, ctrl->cookie, _cmd_len);
		pos += os_strlen(ctrl->cookie);
		*pos++ = ' ';
		os_memcpy(pos, cmd, cmd_len);
	} else
#endif							/* CONFIG_CTRL_IFACE_UDP */
	{
		_cmd = cmd;
		_cmd_len = cmd_len;
	}
#if defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_FIFO)
#ifdef CONFIG_CTRL_IFACE_UDP
	res = wpa_ctrl_sendto(write_fd, _cmd, _cmd_len, 0, (struct sockaddr *)&ctrl->dest, sizeof(ctrl->dest));
#else
	res = wpa_ctrl_sendto(write_fd, _cmd, _cmd_len);
#endif
	if (res < 0) {
		os_free(cmd_buf);
		return -1;
	}
	UNUSED(started_at);
#else
	errno = 0;
	started_at.sec = 0;
	started_at.usec = 0;
retry_send:
	if (sendto(write_fd, _cmd, _cmd_len, 0, (struct sockaddr *)&ctrl->dest, sizeof(ctrl->dest)) < 0) {
		if (errno == EAGAIN || errno == EBUSY || errno == EWOULDBLOCK) {
			/*
			 * Must be a non-blocking socket... Try for a bit
			 * longer before giving up.
			 */
			if (started_at.sec == 0) {
				os_get_reltime(&started_at);
			} else {
				struct os_reltime n;
				os_get_reltime(&n);
				/* Try for a few seconds. */
				if (os_reltime_expired(&n, &started_at, 5)) {
					goto send_err;
				}
			}
			os_sleep(1, 0);
			goto retry_send;
		}
send_err:
		os_free(cmd_buf);
		return -1;
	}
#endif
	os_free(cmd_buf);

	for (;;) {
		struct sockaddr_in from;
		socklen_t fromlen = sizeof(from);

		tv.tv_sec = 10;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(read_fd, &rfds);
		res = select(read_fd + 1, &rfds, NULL, NULL, &tv);
		if (res < 0) {
			return res;
		}
		if (FD_ISSET(read_fd, &rfds)) {
#ifdef CONFIG_CTRL_IFACE_UDP
			res = wpa_ctrl_recvfrom(read_fd, reply, *reply_len, 0, (struct sockaddr *)&from, &fromlen);
#elif defined(CONFIG_CTRL_IFACE_FIFO)
			res = wpa_ctrl_recvfrom(read_fd, reply, *reply_len);
			UNUSED(from);
			UNUSED(fromlen);
#else
			res = recvfrom(read_fd, reply, *reply_len, 0, (struct sockaddr *)&from, &fromlen);
#endif
			if (res < 0) {
				return res;
			}
			if (res > 0 && reply[0] == '<') {
				/* This is an unsolicited message from
				 * wpa_supplicant, not the reply to the
				 * request. Use msg_cb to report this to the
				 * caller. */
				if (msg_cb) {
					/* Make sure the message is nul
					 * terminated. */
					if ((size_t)res == *reply_len) {
						res = (*reply_len) - 1;
					}
					reply[res] = '\0';
					msg_cb(reply, res);
				}
				continue;
			}
			*reply_len = res;
			break;
		} else {
			return -2;
		}
	}
	return 0;
}
#endif							/* CTRL_IFACE_SOCKET */

int wpa_ctrl_request(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len, char *reply, size_t *reply_len, void (*msg_cb)(char *msg, size_t len))
{
#ifdef CONFIG_CTRL_IFACE_FIFO
	/* We use a recursive lock, to ensure only a single thread access the ctrl interface
	 * at any given time. It needs to be recursive to allow the call-back to issue a new
	 * command. */
	int ret;
	pthread_mutex_lock(&(ctrl->busy));
	int current_pid = getpid();
	wpa_printf(MSG_EXCESSIVE, "Current pid %d other pid %d\n", current_pid, ctrl->pid);

	if (current_pid == ctrl->pid) {	// we are in the right context
		ret = wpa_ctrl_request_ext(ctrl, cmd, cmd_len, reply, reply_len, msg_cb);
	} else {
		ctrl->write_cmd = (char *)cmd;
		ctrl->write_cmdlen = (size_t)cmd_len;
		ctrl->write_reply = (char *)reply;
		ctrl->write_replylen = reply_len;
		ctrl->write_msgcb = (void *)msg_cb;
		sem_post(&ctrl->write_start);	//let the right context know that we have data ready for it
		sem_wait(&ctrl->write_complete);	// wait for the writer thread to finish
		reply = ctrl->write_reply;
		ret = ctrl->write_ret;
	}
	pthread_mutex_unlock(&(ctrl->busy));
	return ret;

#else
	return wpa_ctrl_request_ext(ctrl, cmd, cmd_len, reply, reply_len, msg_cb);
#endif
}

static int wpa_ctrl_attach_helper(struct wpa_ctrl *ctrl, int attach)
{
	char buf[10];
	int ret;
	size_t len = 10;

	ret = wpa_ctrl_request(ctrl, attach ? "ATTACH" : "DETACH", 6, buf, &len, NULL);
	if (ret < 0) {
		return ret;
	}
	if (len == 3 && os_memcmp(buf, "OK\n", 3) == 0) {
		if (attach) {
			ctrl->monitor = open(WPA_MONITOR_FIFO_DEV, O_RDONLY);
			if (ctrl->monitor < 0) {
				return -1;    // TODO: Do we need to send detach?
			}
		} else {
			close(ctrl->monitor);
			ctrl->monitor = -1;
		}

		return 0;
	}
	return -1;
}

int wpa_ctrl_attach(struct wpa_ctrl *ctrl)
{
	return wpa_ctrl_attach_helper(ctrl, 1);
}

int wpa_ctrl_detach(struct wpa_ctrl *ctrl)
{
	return wpa_ctrl_attach_helper(ctrl, 0);
}

#ifdef CTRL_IFACE_SOCKET

int wpa_ctrl_recv(struct wpa_ctrl *ctrl, char *reply, size_t *reply_len)
{
	int res;
	int read_fd = ctrl->s;
#ifdef CONFIG_CTRL_IFACE_FIFO
	read_fd = ctrl->s_cfm;
#endif
#ifdef CONFIG_CTRL_IFACE_UDP
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);
	res = wpa_ctrl_recvfrom(read_fd, reply, *reply_len, 0, (struct sockaddr *)&from, &fromlen);
#else
	res = wpa_ctrl_recvfrom(read_fd, reply, *reply_len);
#endif
	if (res < 0) {
		return res;
	}
	if (*reply_len > res) {
		reply[res] = '\0';    // Ensure the string is zero terminated - if there is room
	}
	*reply_len = res;
	return 0;
}

int wpa_ctrl_recv_monitor(struct wpa_ctrl *ctrl, char *reply, size_t *reply_len)
{
	int res;
#ifdef CONFIG_CTRL_IFACE_UDP
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);
	res = wpa_ctrl_recvfrom(ctrl->monitor, reply, *reply_len, 0, (struct sockaddr *)&from, &fromlen);
#else
	res = wpa_ctrl_recvfrom(ctrl->monitor, reply, *reply_len);
#endif
	if (res < 0) {
		return res;
	}
	*reply_len = res;
	return 0;
}

/* This is used only for the monitor channel */
int wpa_ctrl_pending(struct wpa_ctrl *ctrl)
{
	int read_fd = ctrl->s;
#ifdef CONFIG_CTRL_IFACE_FIFO
	read_fd = ctrl->monitor;
#endif
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(read_fd, &rfds);
	select(read_fd + 1, &rfds, NULL, NULL, NULL);
	return FD_ISSET(read_fd, &rfds);
}

/* This is used both for control an monitor, hence we store a copy of the monitor FD
 * in ctrl->s, as you will need a separate connection for CTRL and monitor. */
int wpa_ctrl_get_fd(struct wpa_ctrl *ctrl)
{
#ifdef CONFIG_CTRL_IFACE_FIFO
	return ctrl->s_cfm;
#else
	return ctrl->s;
#endif
}

#endif							/* CTRL_IFACE_SOCKET */

#ifdef CONFIG_CTRL_IFACE_NAMED_PIPE

#ifndef WPA_SUPPLICANT_NAMED_PIPE
#define WPA_SUPPLICANT_NAMED_PIPE "WpaSupplicant"
#endif
#define NAMED_PIPE_PREFIX TEXT("\\\\.\\pipe\\") TEXT(WPA_SUPPLICANT_NAMED_PIPE)

struct wpa_ctrl *wpa_ctrl_open(const char *ctrl_path)
{
	struct wpa_ctrl *ctrl;
	DWORD mode;
	TCHAR name[256];
	int i, ret;

	ctrl = os_malloc(sizeof(*ctrl));
	if (ctrl == NULL) {
		return NULL;
	}
	os_memset(ctrl, 0, sizeof(*ctrl));

#ifdef UNICODE
	if (ctrl_path == NULL) {
		ret = _snwprintf(name, 256, NAMED_PIPE_PREFIX);
	} else {
		ret = _snwprintf(name, 256, NAMED_PIPE_PREFIX TEXT("-%S"), ctrl_path);
	}
#else							/* UNICODE */
	if (ctrl_path == NULL) {
		ret = os_snprintf(name, 256, NAMED_PIPE_PREFIX);
	} else {
		ret = os_snprintf(name, 256, NAMED_PIPE_PREFIX "-%s", ctrl_path);
	}
#endif							/* UNICODE */
	if (os_snprintf_error(256, ret)) {
		os_free(ctrl);
		return NULL;
	}

	for (i = 0; i < 10; i++) {
		ctrl->pipe = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		/*
		 * Current named pipe server side in wpa_supplicant is
		 * re-opening the pipe for new clients only after the previous
		 * one is taken into use. This leaves a small window for race
		 * conditions when two connections are being opened at almost
		 * the same time. Retry if that was the case.
		 */
		if (ctrl->pipe != INVALID_HANDLE_VALUE || GetLastError() != ERROR_PIPE_BUSY) {
			break;
		}
		WaitNamedPipe(name, 1000);
	}
	if (ctrl->pipe == INVALID_HANDLE_VALUE) {
		os_free(ctrl);
		return NULL;
	}

	mode = PIPE_READMODE_MESSAGE;
	if (!SetNamedPipeHandleState(ctrl->pipe, &mode, NULL, NULL)) {
		CloseHandle(ctrl->pipe);
		os_free(ctrl);
		return NULL;
	}

	return ctrl;
}

void wpa_ctrl_close(struct wpa_ctrl *ctrl)
{
	CloseHandle(ctrl->pipe);
	os_free(ctrl);
}

int wpa_ctrl_request(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len, char *reply, size_t *reply_len, void (*msg_cb)(char *msg, size_t len))
{
	DWORD written;
	DWORD readlen = *reply_len;

	if (!WriteFile(ctrl->pipe, cmd, cmd_len, &written, NULL)) {
		return -1;
	}

	if (!ReadFile(ctrl->pipe, reply, *reply_len, &readlen, NULL)) {
		return -1;
	}
	*reply_len = readlen;

	return 0;
}

int wpa_ctrl_recv(struct wpa_ctrl *ctrl, char *reply, size_t *reply_len)
{
	DWORD len = *reply_len;
	if (!ReadFile(ctrl->pipe, reply, *reply_len, &len, NULL)) {
		return -1;
	}
	*reply_len = len;
	return 0;
}

int wpa_ctrl_pending(struct wpa_ctrl *ctrl)
{
	DWORD left;

	if (!PeekNamedPipe(ctrl->pipe, NULL, 0, NULL, &left, NULL)) {
		return -1;
	}
	return left ? 1 : 0;
}

int wpa_ctrl_get_fd(struct wpa_ctrl *ctrl)
{
	return -1;
}

#endif							/* CONFIG_CTRL_IFACE_NAMED_PIPE */

#endif							/* CONFIG_CTRL_IFACE */
