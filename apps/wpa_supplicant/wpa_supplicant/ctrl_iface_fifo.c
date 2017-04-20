/*
 * WPA Supplicant / FIFO Socket - based control and monitor interface
 * Copyright (c) Samsung SLSI 2016, Casper Bonde <c.bonde@samsung.com>
 *
 * Based on the UPD implementation by Jouni Malinen <j@w1.fi> see notice
 * in the _upd version of this file.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "includes.h"

#include "common.h"
#include "eloop.h"
#include "config.h"
#include "eapol_supp/eapol_supp_sm.h"
#include "wpa_supplicant_i.h"
#include "ctrl_iface.h"
#include "common/wpa_ctrl.h"

#ifdef CONFIG_CTRL_IFACE_FIFO

#define COOKIE_LEN 8

/* Per-interface ctrl_iface */

/**
 * struct wpa_ctrl_dst - Internal data structure of control interface monitors
 *
 * This structure is used to store information about registered control
 * interface monitors into struct wpa_supplicant. This data is private to
 * ctrl_iface_udp.c and should not be touched directly from other files.
 */
struct wpa_ctrl_dst {
	struct wpa_ctrl_dst *next;
	sem_t sem_write;
	int monitor_fd;
	int debug_level;
	int errors;
};

struct ctrl_iface_priv {
	struct wpa_supplicant *wpa_s;
	int ctrl_fd_req;
	int ctrl_fd_cfm;
	struct wpa_ctrl_dst ctrl_dst;
	u8 cookie[COOKIE_LEN];
};

static void wpa_supplicant_monitor_iface_send(struct ctrl_iface_priv *priv, int level, const char *buf, size_t len);

static int wpa_supplicant_ctrl_iface_attach(struct ctrl_iface_priv *priv)
{
	if (priv->ctrl_dst.monitor_fd >= 0) {
		return -1;
	}
	priv->ctrl_dst.monitor_fd = open(WPA_MONITOR_FIFO_DEV, O_WRONLY);
	if (priv->ctrl_dst.monitor_fd < 0) {
		wpa_printf(MSG_DEBUG, "fifo open error for %s: %s", WPA_MONITOR_FIFO_DEV, strerror(errno));
		return -1;
	}

	priv->ctrl_dst.debug_level = MSG_INFO;

	wpa_printf(MSG_DEBUG, "CTRL_IFACE monitor attached to %s", WPA_MONITOR_FIFO_DEV);
	return 0;
}

static int wpa_supplicant_ctrl_iface_detach(struct ctrl_iface_priv *priv)
{
	if ((priv->ctrl_dst.monitor_fd >= 0) && (close(priv->ctrl_dst.monitor_fd) < 0)) {
		wpa_printf(MSG_ERROR, "close(monitor_fd): %s", strerror(errno));
	}
	priv->ctrl_dst.monitor_fd = -1;
	return 0;
}

static int wpa_supplicant_ctrl_iface_level(struct ctrl_iface_priv *priv, char *level)
{
	if (priv->ctrl_dst.monitor_fd < 0) {
		wpa_printf(MSG_ERROR, "CTRL_IFACE LEVEL No monitor attached...");
		return -1;
	}
	wpa_printf(MSG_DEBUG, "CTRL_IFACE LEVEL %s", level);
	priv->ctrl_dst.debug_level = atoi(level);
	return 0;
}

static char *wpa_supplicant_ctrl_iface_get_cookie(struct ctrl_iface_priv *priv, size_t *reply_len)
{
	char *reply;
	reply = os_malloc(7 + 2 * COOKIE_LEN + 1);
	if (reply == NULL) {
		*reply_len = 1;
		return NULL;
	}

	os_memcpy(reply, "COOKIE=", 7);
	wpa_snprintf_hex(reply + 7, 2 * COOKIE_LEN + 1, priv->cookie, COOKIE_LEN);

	*reply_len = 7 + 2 * COOKIE_LEN;
	return reply;
}

void wpa_supplicant_ctrl_iface_receive(int fd, void *eloop_ctx, void *sock_ctx)
{
	struct wpa_supplicant *wpa_s = eloop_ctx;
	struct ctrl_iface_priv *priv = sock_ctx;
	char buf[256], *pos;
	int res;
	char *reply = NULL;
	size_t reply_len = 0;
	int new_attached = 0;
	u8 cookie[COOKIE_LEN];

	res = wpa_ctrl_recvfrom(fd, buf, sizeof(buf) - 1);
	if (res < 0) {
		wpa_printf(MSG_ERROR, "wpa_ctrl_recvfrom(ctrl_iface): %s", strerror(errno));
		return;
	}

	buf[res] = '\0';

	if (os_strcmp(buf, "GET_COOKIE") == 0) {
		reply = wpa_supplicant_ctrl_iface_get_cookie(priv, &reply_len);
		goto done;
	}

	/* TODO: Remove COOKIE
	 * Require that the client includes a prefix with the 'cookie' value
	 * fetched with GET_COOKIE command. This is used to verify that the
	 * client has access to a bidirectional link over UDP in order to
	 * avoid attacks using forged localhost IP address even if the OS does
	 * not block such frames from remote destinations.
	 */
	if (os_strncmp(buf, "COOKIE=", 7) != 0) {
		wpa_printf(MSG_DEBUG, "CTLR: No cookie in the request - " "drop request");
		return;
	}

	if (hexstr2bin(buf + 7, cookie, COOKIE_LEN) < 0) {
		wpa_printf(MSG_DEBUG, "CTLR: Invalid cookie format in the " "request - drop request");
		return;
	}

	if (os_memcmp(cookie, priv->cookie, COOKIE_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "CTLR: Invalid cookie in the request - " "drop request");
		return;
	}

	pos = buf + 7 + 2 * COOKIE_LEN;
	while (*pos == ' ') {
		pos++;
	}

	if (os_strcmp(pos, "ATTACH") == 0) {
		if (wpa_supplicant_ctrl_iface_attach(priv)) {
			reply_len = 1;
		} else {
			new_attached = 1;
			reply_len = 2;
		}
	} else if (os_strcmp(pos, "DETACH") == 0) {
		if (wpa_supplicant_ctrl_iface_detach(priv)) {
			reply_len = 1;
		} else {
			reply_len = 2;
		}
	} else if (os_strncmp(pos, "LEVEL ", 6) == 0) {
		if (wpa_supplicant_ctrl_iface_level(priv, pos + 6)) {
			reply_len = 1;
		} else {
			reply_len = 2;
		}
	} else {
		reply = wpa_supplicant_ctrl_iface_process(wpa_s, pos, &reply_len);
	}

done:
	if (reply) {
		wpa_ctrl_sendto(priv->ctrl_fd_cfm, reply, reply_len);
		os_free(reply);
	} else if (reply_len == 1) {
		wpa_ctrl_sendto(priv->ctrl_fd_cfm, "FAIL\n", 5);
	} else if (reply_len == 2) {
		wpa_ctrl_sendto(priv->ctrl_fd_cfm, "OK\n", 3);
	}

	if (new_attached) {
		eapol_sm_notify_ctrl_attached(wpa_s->eapol);
	}
}

static void wpa_supplicant_monitor_iface_msg_cb(void *ctx, int level, enum wpa_msg_type type, const char *txt, size_t len)
{
	struct wpa_supplicant *wpa_s = ctx;
	if (wpa_s == NULL || wpa_s->ctrl_iface == NULL) {
		return;
	}
	wpa_supplicant_monitor_iface_send(wpa_s->ctrl_iface, level, txt, len);
}

struct ctrl_iface_priv *wpa_supplicant_ctrl_iface_init(struct wpa_supplicant *wpa_s)
{
	struct ctrl_iface_priv *priv;

	priv = os_zalloc(sizeof(*priv));
	if (priv == NULL) {
		return NULL;
	}
	priv->wpa_s = wpa_s;

	os_get_random(priv->cookie, COOKIE_LEN);

	if (wpa_s->conf->ctrl_interface == NULL) {
		return priv;
	}

	priv->ctrl_fd_cfm = open(WPA_CTRL_FIFO_DEV_CFM, O_WRONLY);
	if (priv->ctrl_fd_cfm < 0) {
		wpa_printf(MSG_DEBUG, "fifo open error for %s: %s", WPA_CTRL_FIFO_DEV_CFM, strerror(errno));
		os_free(priv);
		return NULL;
	}

	priv->ctrl_fd_req = open(WPA_CTRL_FIFO_DEV_REQ, O_RDONLY);
	if (priv->ctrl_fd_req < 0) {
		wpa_printf(MSG_DEBUG, "fifo open error for %s: %s", WPA_CTRL_FIFO_DEV_REQ, strerror(errno));
		os_free(priv);
		return NULL;
	}

	priv->ctrl_dst.monitor_fd = -1;

	eloop_register_read_sock(priv->ctrl_fd_req, wpa_supplicant_ctrl_iface_receive, wpa_s, priv);
	sem_init(&priv->ctrl_dst.sem_write, 1, 0);	// Shared between processes (tasks in case of tinyara)
	wpa_msg_register_cb(wpa_supplicant_monitor_iface_msg_cb);

	return priv;
}

void wpa_supplicant_ctrl_iface_deinit(struct ctrl_iface_priv *priv)
{
	if (priv->ctrl_fd_req > -1) {
		eloop_unregister_read_sock(priv->ctrl_fd_req);
		if (priv->ctrl_dst.monitor_fd >= 0) {
			/*
			 * Wait before closing the control socket if
			 * there are any attached monitors in order to allow
			 * them to receive any pending messages.
			 */
			wpa_printf(MSG_DEBUG, "CTRL_IFACE wait for attached " "monitors to receive messages");
			os_sleep(0, 100000);
			close(priv->ctrl_dst.monitor_fd);
		}
		close(priv->ctrl_fd_req);
		close(priv->ctrl_fd_cfm);
	}
	sem_destroy(&priv->ctrl_dst.sem_write);
	os_free(priv);
}

/**
 * Write a message to the monitor interface from any context, will do context switch if needed.
 */
static void wpa_supplicant_monitor_iface_write(struct ctrl_iface_priv *priv, int level, const char *buf, size_t len)
{
	char levelstr[10];
	int idx;
	char *sbuf;
	int llen;

	os_snprintf(levelstr, sizeof(levelstr), "<%d>", level);

	llen = os_strlen(levelstr);
	sbuf = os_malloc(llen + len);
	if (sbuf == NULL) {
		return;
	}

	os_memcpy(sbuf, levelstr, llen);
	os_memcpy(sbuf + llen, buf, len);

	idx = 0;
	if (level >= priv->ctrl_dst.debug_level) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE monitor send");
		if (wpa_ctrl_sendto(priv->ctrl_dst.monitor_fd, sbuf, llen + len) < 0) {
			wpa_printf(MSG_ERROR, "sendto(CTRL_IFACE monitor): %s", strerror(errno));
			priv->ctrl_dst.errors++;
			if (priv->ctrl_dst.errors > 10) {
				wpa_supplicant_ctrl_iface_detach(priv);
			}
		} else {
			priv->ctrl_dst.errors = 0;
		}
	}
	os_free(sbuf);
}

/**
 * The elooper interface only supports two parameters, use this struct to parse more
 * parameters to the offload function.
 */
typedef struct iface_write_params_t {
	struct ctrl_iface_priv *priv;
	int level;
	const char *buf;
	size_t len;
} iface_write_params_t;

/**
 * Wrapper function to provide eloop offload of function call with synchronization.
 * To be called from elooper, post on semaphore when done.
 */
static void wpa_supplicant_ctrl_iface_write_offload(void *arg1, void *arg2)
{
	iface_write_params_t *params = (iface_write_params_t *)arg1;
	wpa_supplicant_monitor_iface_write(params->priv, params->level, params->buf, params->len);
	sem_post(&params->priv->ctrl_dst.sem_write);
}

/**
 * Send a message to the monitor iface. This shall be called only from the
 * context that created the interface.
 */
static void wpa_supplicant_monitor_iface_send(struct ctrl_iface_priv *priv, int level, const char *buf, size_t len)
{
	int current_pid = getpid();

	if (priv->ctrl_dst.monitor_fd < 0) {
		return;
	}

	wpa_printf(MSG_EXCESSIVE, "<ctrl_iface_fifo> Current pid %d other pid %d", current_pid, eloop_get_pid());

	if (eloop_get_pid() == current_pid) {
		// Safe to call iface_write directly
		wpa_supplicant_monitor_iface_write(priv, level, buf, len);
	} else {					// We need to offload the write to the e-looper to ensure synchronization
		bool keep_going = true;
		iface_write_params_t params;
		params.priv = priv;
		params.level = level;
		params.buf = buf;
		params.len = len;

		eloop_register_timeout(0, 0, (eloop_timeout_handler) wpa_supplicant_ctrl_iface_write_offload, &params, NULL);
		// We need a loop here to handle signal interrupts
		while (keep_going) {
			int res = sem_wait(&priv->ctrl_dst.sem_write);
			if (res) {
				int err = errno;
				wpa_printf(MSG_ERROR, "CTRL_IFACE - sem_wait error %d - %s", err, strerror(err));
				if (err != EINTR) {
					keep_going = false;    // Keep going only if we get interrupted by a signal
				} else {
					if (eloop_terminated()) {
						keep_going = false;    // Unless we are actually terminated
					}
				}
			} else {			// Succesfull wait, continue
				keep_going = false;
			}
		}
	}
}

/* TODO: I'm not sure the implemented functionality here does what it is supposed
 *       to. This implementation does the same as the UDP implementation, but it
 *       does not wait for a monitor to connect - it only waits for the first req. */
void wpa_supplicant_ctrl_iface_wait(struct ctrl_iface_priv *priv)
{
	wpa_printf(MSG_DEBUG, "CTRL_IFACE - %s - wait for monitor", priv->wpa_s->ifname);
	eloop_wait_for_read_sock(priv->ctrl_fd_req);
}

/* Global ctrl_iface */

struct ctrl_iface_global_priv {
	int ctrl_fd_req;
	int ctrl_fd_cfm;
	u8 cookie[COOKIE_LEN];
};

static char *wpa_supplicant_global_get_cookie(struct ctrl_iface_global_priv *priv, size_t *reply_len)
{
	char *reply;
	reply = os_malloc(7 + 2 * COOKIE_LEN + 1);
	if (reply == NULL) {
		*reply_len = 1;
		return NULL;
	}

	os_memcpy(reply, "COOKIE=", 7);
	wpa_snprintf_hex(reply + 7, 2 * COOKIE_LEN + 1, priv->cookie, COOKIE_LEN);

	*reply_len = 7 + 2 * COOKIE_LEN;
	return reply;
}

static void wpa_supplicant_global_ctrl_iface_receive(int fd, void *eloop_ctx, void *sock_ctx)
{
	struct wpa_global *global = eloop_ctx;
	struct ctrl_iface_global_priv *priv = sock_ctx;
	char buf[256], *pos;
	int res;
	char *reply;
	size_t reply_len;
	u8 cookie[COOKIE_LEN];

	res = wpa_ctrl_recvfrom(fd, buf, sizeof(buf) - 1);
	if (res < 0) {
		wpa_printf(MSG_ERROR, "recvfrom(ctrl_iface): %s", strerror(errno));
		return;
	}

	buf[res] = '\0';

	if (os_strcmp(buf, "GET_COOKIE") == 0) {
		reply = wpa_supplicant_global_get_cookie(priv, &reply_len);
		goto done;
	}

	if (os_strncmp(buf, "COOKIE=", 7) != 0) {
		wpa_printf(MSG_DEBUG, "CTLR: No cookie in the request - " "drop request");
		return;
	}

	if (hexstr2bin(buf + 7, cookie, COOKIE_LEN) < 0) {
		wpa_printf(MSG_DEBUG, "CTLR: Invalid cookie format in the " "request - drop request");
		return;
	}

	if (os_memcmp(cookie, priv->cookie, COOKIE_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "CTLR: Invalid cookie in the request - " "drop request");
		return;
	}

	pos = buf + 7 + 2 * COOKIE_LEN;
	while (*pos == ' ') {
		pos++;
	}

	reply = wpa_supplicant_global_ctrl_iface_process(global, pos, &reply_len);

done:
	if (reply) {
		wpa_ctrl_sendto(fd, reply, reply_len);
		os_free(reply);
	} else if (reply_len) {
		wpa_ctrl_sendto(fd, "FAIL\n", 5);
	}
}

struct ctrl_iface_global_priv *wpa_supplicant_global_ctrl_iface_init(struct wpa_global *global)
{
	struct ctrl_iface_global_priv *priv;

	priv = os_zalloc(sizeof(*priv));
	if (priv == NULL) {
		return NULL;
	}
	priv->ctrl_fd_req = -1;
	priv->ctrl_fd_cfm = -1;
	os_get_random(priv->cookie, COOKIE_LEN);

	if (global->params.ctrl_interface == NULL) {
		return priv;
	}

	wpa_printf(MSG_DEBUG, "Global control interface '%s'", global->params.ctrl_interface);

	if (mkfifo(WPA_CTRL_FIFO_DEV_GLOBAL_REQ, WPA_CTRL_FIFO_MK_MODE) != 0) {
		os_free(priv);
		return NULL;
	}

	if (mkfifo(WPA_CTRL_FIFO_DEV_GLOBAL_CFM, WPA_CTRL_FIFO_MK_MODE) != 0) {
		os_free(priv);
		return NULL;
	}

	priv->ctrl_fd_req = open(WPA_CTRL_FIFO_DEV_GLOBAL_REQ, O_RDONLY);
	if (priv->ctrl_fd_req < 0) {
		os_free(priv);
		return NULL;
	}

	priv->ctrl_fd_cfm = open(WPA_CTRL_FIFO_DEV_GLOBAL_CFM, O_WRONLY);
	if (priv->ctrl_fd_cfm < 0) {
		os_free(priv);
		return NULL;
	}

	eloop_register_read_sock(priv->ctrl_fd_req, wpa_supplicant_global_ctrl_iface_receive, global, priv);

	return priv;
}

void wpa_supplicant_global_ctrl_iface_deinit(struct ctrl_iface_global_priv *priv)
{
	if (priv->ctrl_fd_req >= 0) {
		eloop_unregister_read_sock(priv->ctrl_fd_req);
		close(priv->ctrl_fd_req);
		close(priv->ctrl_fd_cfm);
	}
	os_free(priv);
}
#endif
