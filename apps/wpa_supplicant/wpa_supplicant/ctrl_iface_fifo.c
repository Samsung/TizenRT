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

#define IFNAME_MAX_LEN 8
#define COOKIE_LEN 8

/* Per-interface ctrl_iface */

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
 * struct wpa_ctrl_dst - Internal data structure of control interface monitors
 *
 * This structure is used to store information about registered control
 * interface monitors into struct wpa_supplicant. This data is private to
 * ctrl_iface_udp.c and should not be touched directly from other files.
 */
struct wpa_ctrl_dst {
	struct wpa_ctrl_dst *next;
	sem_t sem_write;
	char *ifname;
	int monitor_fd;
	int debug_level;
	int errors;
};

struct wpa_ifname {
	struct wpa_supplicant *wpa_s;
	char ifname[IFNAME_MAX_LEN];
	struct wpa_ifname *next;
};

struct ctrl_iface_priv {
	int ctrl_fd_req;
	int ctrl_fd_cfm;
	struct wpa_ctrl_dst ctrl_dst;
	int count;
	struct wpa_ifname *if_list;
};

static struct ctrl_iface_priv *l_file_desc = NULL;

static void wpa_supplicant_monitor_iface_send(struct ctrl_iface_priv *priv, int level, const char *buf, size_t len);

static int wpa_supplicant_ctrl_iface_attach(struct ctrl_iface_priv *priv, char *ifname)
{
	if (priv->ctrl_dst.monitor_fd >= 0) {
		return -1;
	}
	priv->ctrl_dst.monitor_fd = open(WPA_MONITOR_FIFO_DEV, O_WRONLY);
	if (priv->ctrl_dst.monitor_fd < 0) {
		wpa_printf(MSG_DEBUG, "fifo open error for %s: %s", WPA_MONITOR_FIFO_DEV, strerror(errno));
		return -1;
	}

	priv->ctrl_dst.ifname = ifname;
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

void wpa_supplicant_ctrl_iface_receive(int fd, void *eloop_ctx, void *sock_ctx)
{
	struct wpa_supplicant *wpa_s = NULL;	// = eloop_ctx;
	struct ctrl_iface_priv *priv = sock_ctx;
	char buf[256], *pos;
	int res;
	char *reply = NULL;
	size_t reply_len = 0;
	int new_attached = 0;
	//u8 cookie[COOKIE_LEN];
	char ifname[IFNAME_MAX_LEN];
	os_memset(ifname, 0, IFNAME_MAX_LEN);
	int ifname_len = 0;

	res = wpa_ctrl_recvfrom(fd, buf, sizeof(buf) - 1);
	if (res < 0) {
		wpa_printf(MSG_ERROR, "wpa_ctrl_recvfrom(ctrl_iface): %s", strerror(errno));
		return;
	}

	buf[res] = '\0';

	/* Find interface name */
	pos = strchr(buf, ' ');
	ifname_len = pos - buf;
	if (ifname_len <= 0 || ifname_len > IFNAME_MAX_LEN) {
		reply_len = 1;
		wpa_printf(MSG_ERROR, "Interface name not found\n");
		goto done;
	}
	strncpy(&ifname[0], buf, ifname_len);
	ifname[ifname_len] = '\0';

	/* Find supplicant interface data */
	struct wpa_ifname *tmp = priv->if_list;
	while (tmp) {
		if (os_strcmp(tmp->ifname, ifname) == 0) {
			wpa_s = tmp->wpa_s;
			break;
		}
		tmp = tmp->next;
	}

	if (wpa_s == NULL) {
		reply_len = 1;
		wpa_printf(MSG_ERROR, "Supplicant interface data not found\n");
		goto done;
	}

	pos = buf + ifname_len;
	while (*pos == ' ') {
		pos++;
	}

	if (os_strcmp(pos, "ATTACH") == 0) {
		if (wpa_supplicant_ctrl_iface_attach(priv, wpa_s->ifname)) {
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

	//FIXME: Might be needed if call back messages need to be dispatched
//    if (wpa_s->ifname) {
//        const char *_cmd;
//        char *cmd_buf = NULL;
//        size_t _cmd_len;
//
//        char *pos;
//        _cmd_len = os_strlen(wpa_s->ifname) + 1 + len;
//        cmd_buf = os_zalloc(_cmd_len);
//        if (cmd_buf == NULL)
//            return;
//
//        _cmd = cmd_buf;
//        pos = cmd_buf;
//        os_strlcpy(pos, wpa_s->ifname, _cmd_len);
//        pos += os_strlen(wpa_s->ifname);
//        *pos++ = ' ';
//        os_memcpy(pos, txt, len);
//
//        //os_free((char*)txt);
//        txt = _cmd;
//        len = _cmd_len;
//    }

	wpa_supplicant_monitor_iface_send(wpa_s->ctrl_priv, level, txt, len);
}

struct wpa_ifname *wpa_supplicant_ctrl_iface_init(struct wpa_supplicant *wpa_s)
{
	struct wpa_ifname *wpa_if;

	wpa_if = os_zalloc(sizeof(*wpa_if));
	if (wpa_if == NULL) {
		return NULL;
	}

	if (l_file_desc == NULL) {
		l_file_desc = os_zalloc(sizeof(*l_file_desc));
		if (l_file_desc == NULL) {
			return NULL;
		}
		l_file_desc->if_list = wpa_if;
	}

	wpa_if->wpa_s = wpa_s;

	os_strlcpy(wpa_if->ifname, wpa_s->ifname, sizeof(wpa_s->ifname));

	// TODO: What does this prevent????
	if (wpa_s->conf->ctrl_interface == NULL) {
		return wpa_if;
	}


	if (l_file_desc->count == 0) {

		l_file_desc->ctrl_fd_cfm = open(WPA_CTRL_FIFO_DEV_CFM, O_WRONLY);
		if (l_file_desc->ctrl_fd_cfm < 0) {
			wpa_printf(MSG_ERROR, "fifo open error for %s: %s", WPA_CTRL_FIFO_DEV_CFM, strerror(errno));
			os_free(l_file_desc);
			return NULL;
		}
		l_file_desc->ctrl_fd_req = open(WPA_CTRL_FIFO_DEV_REQ, O_RDONLY);
		if (l_file_desc->ctrl_fd_req < 0) {
			wpa_printf(MSG_ERROR, "fifo open error for %s: %s", WPA_CTRL_FIFO_DEV_REQ, strerror(errno));
			os_free(l_file_desc);
			return NULL;
		}
		l_file_desc->ctrl_dst.monitor_fd = -1;

		//FIXME: KSc if using a static to store fd's then check these instead.
		eloop_register_read_sock(l_file_desc->ctrl_fd_req, wpa_supplicant_ctrl_iface_receive, wpa_s, l_file_desc);
		sem_init(&l_file_desc->ctrl_dst.sem_write, 1, 0);	// Shared between processes (tasks in case of tinyara)
		wpa_msg_register_cb(wpa_supplicant_monitor_iface_msg_cb);

	} else {
		struct wpa_ifname *tmp_if_list = l_file_desc->if_list;

		while (tmp_if_list && tmp_if_list->next) {
			tmp_if_list = tmp_if_list->next;
		}
		tmp_if_list->next = wpa_if;
	}

	l_file_desc->count++;
	wpa_s->ctrl_priv = l_file_desc;
	return wpa_if;
}

void wpa_supplicant_ctrl_iface_deinit(struct wpa_ifname *wpaifname)
{
	//FIXME: Ksc only remove fifo if all interfaces are gone from the list
	struct ctrl_iface_priv *priv = l_file_desc;
	if (priv->count > 0) {
		struct wpa_ifname *prev = NULL;
		struct wpa_ifname *next = NULL;
		struct wpa_ifname *tmp = l_file_desc->if_list;
		wpa_printf(MSG_DEBUG, "_ctrl_iface_deinit priv->count: %d", priv->count);
		while (tmp) {
			if (!os_strcmp(tmp->ifname, wpaifname->ifname)) {
				next = tmp->next;
				os_free(tmp);
				priv->count--;
				if (priv->count == 0) {
					priv->if_list = NULL;
				} else if (tmp == priv->if_list) {
					priv->if_list = next;
				}
				if (prev != NULL) {
					prev->next = next;
				}
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}
	}
	if (priv->count == 0) {
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
				priv->ctrl_dst.monitor_fd = -1;
			}
			close(priv->ctrl_fd_req);
			close(priv->ctrl_fd_cfm);
			priv->ctrl_fd_req = -1;
			priv->ctrl_fd_cfm = -1;
		}
		sem_destroy(&priv->ctrl_dst.sem_write);
		if (l_file_desc) {
			os_free(l_file_desc);
			l_file_desc = NULL;
		}
	}
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
	//FIXME: Might be needed
//    int ifname_len[IFNAME_MAX_LEN];
//    os_memset(ifname_len,0,IFNAME_MAX_LEN);
//
//    os_snprintf(levelstr, sizeof(levelstr), "<%d>", level);
//    llen = os_strlen(levelstr);
//    ifname_len = os_strlen(priv->wpa_s->ifname) + 1/*whitespace*/;
//    sbuf = os_malloc(llen + len+ifname_len);
//    if (sbuf == NULL)
//        return;
//
//    // Format is "INTERFACE <level>MESSAGE"
//    os_memcpy(sbuf, &priv->wpa_s->ifname[0], ifname_len);
//    sbuf[ifname_len] = ' '; // insert space after ifname
//    os_memcpy(sbuf + ifname_len, levelstr, llen);
//    os_memcpy(sbuf + ifname_len + llen, buf, len);
//
//    idx = 0;
//    if (level >= priv->ctrl_dst.debug_level) {
//        wpa_printf(MSG_DEBUG, "CTRL_IFACE monitor send");
//        if (wpa_ctrl_sendto(priv->ctrl_dst.monitor_fd, sbuf, llen + len + ifname_len) < 0) {

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
 * Wrapper function to provide eloop offload of function call with synchronization.
 * To be called from elooper, post on semaphore when done.
 */
static void wpa_supplicant_ctrl_iface_write_offload(void *arg1, void *arg2)
{
	iface_write_params_t *params = (iface_write_params_t *) arg1;
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
	wpa_printf(MSG_DEBUG, "CTRL_IFACE - wait for monitor");
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
