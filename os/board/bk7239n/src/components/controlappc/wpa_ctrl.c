/*
 * wpa_supplicant/hostapd control interface library
 * Copyright (c) 2004-2005, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#ifndef CONFIG_NATIVE_WINDOWS
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#endif /* CONFIG_NATIVE_WINDOWS */

#include "vendor_specific.h"
#include "wpa_ctrl.h"
#include "utils.h"
#ifdef CONFIG_NATIVE_WINDOWS
#include "common.h"
#endif /* CONFIG_NATIVE_WINDOWS */


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
	int s;
#ifdef CONFIG_CTRL_IFACE_UDP
	struct sockaddr_in local;
	struct sockaddr_in dest;
	char *cookie;
#else /* CONFIG_CTRL_IFACE_UDP */
	struct sockaddr_un local;
	struct sockaddr_un dest;
#endif /* CONFIG_CTRL_IFACE_UDP */
};


struct wpa_ctrl * wpa_ctrl_open(const char *ctrl_path)
{
	struct wpa_ctrl *ctrl;
#ifndef CONFIG_CTRL_IFACE_UDP
	static int counter = 0;
#else
    unsigned short port = 0;
    char buf[128];
	size_t len;
#endif /* CONFIG_CTRL_IFACE_UDP */

	ctrl = malloc(sizeof(*ctrl));
	if (ctrl == NULL)
		return NULL;
	memset(ctrl, 0, sizeof(*ctrl));

#ifdef CONFIG_CTRL_IFACE_UDP
	ctrl->s = socket(PF_INET, SOCK_DGRAM, 0);
	if (ctrl->s < 0) {
		perror("socket");
		free(ctrl);
		return NULL;
	}

	ctrl->local.sin_family = AF_INET;
#ifdef CONFIG_CTRL_IFACE_UDP
	ctrl->local.sin_addr.s_addr = INADDR_ANY;
#else /* CONFIG_CTRL_IFACE_UDP_REMOTE */
	ctrl->local.sin_addr.s_addr = htonl((127 << 24) | 1);
#endif /* CONFIG_CTRL_IFACE_UDP_REMOTE */
	if (bind(ctrl->s, (struct sockaddr *) &ctrl->local,
		 sizeof(ctrl->local)) < 0) {
		close(ctrl->s);
		free(ctrl);
		return NULL;
	}

    sscanf(ctrl_path, "udp:%hu", &port);

	ctrl->dest.sin_family = AF_INET;
	ctrl->dest.sin_addr.s_addr = htonl((127 << 24) | 1);
	ctrl->dest.sin_port = htons(port);
	if (connect(ctrl->s, (struct sockaddr *) &ctrl->dest,
		    sizeof(ctrl->dest)) < 0) {
		perror("connect");
		close(ctrl->s);
		free(ctrl);
		return NULL;
	}

	len = sizeof(buf) - 1;
	if (wpa_ctrl_request(ctrl, "GET_COOKIE", 10, buf, &len, NULL) == 0) {
		buf[len] = '\0';
		ctrl->cookie = strdup(buf);
	    printf("%s\n\n", ctrl->cookie);
	}
#else /* CONFIG_CTRL_IFACE_UDP */
	ctrl->s = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (ctrl->s < 0) {
		free(ctrl);
		return NULL;
	}

	ctrl->local.sun_family = AF_UNIX;
	snprintf(ctrl->local.sun_path, sizeof(ctrl->local.sun_path),
		 "/tmp/wpa_ctrl_%d-%d", getpid(), counter++);
	if (bind(ctrl->s, (struct sockaddr *) &ctrl->local,
		    sizeof(ctrl->local)) < 0) {
		close(ctrl->s);
		free(ctrl);
		return NULL;
	}

	ctrl->dest.sun_family = AF_UNIX;
	snprintf(ctrl->dest.sun_path, sizeof(ctrl->dest.sun_path), "%s",
		 ctrl_path);
	if (connect(ctrl->s, (struct sockaddr *) &ctrl->dest,
		    sizeof(ctrl->dest)) < 0) {
		close(ctrl->s);
		unlink(ctrl->local.sun_path);
		free(ctrl);
		return NULL;
	}
#endif /* CONFIG_CTRL_IFACE_UDP */

	return ctrl;
}


void wpa_ctrl_close(struct wpa_ctrl *ctrl)
{
#ifndef CONFIG_CTRL_IFACE_UDP
	unlink(ctrl->local.sun_path);
#else
    if (ctrl->cookie)
        free(ctrl->cookie);
#endif /* CONFIG_CTRL_IFACE_UDP */
	close(ctrl->s);
	free(ctrl);
}


int wpa_ctrl_request(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len,
		     char *reply, size_t *reply_len,
		     void (*msg_cb)(char *msg, size_t len))
{
	struct timeval tv;
	int res;
	fd_set rfds;
	const char *_cmd;
#ifdef CONFIG_CTRL_IFACE_UDP
	char *cmd_buf = NULL;
#endif
	size_t _cmd_len;

#ifdef CONFIG_CTRL_IFACE_UDP
	if (ctrl->cookie) {
		char *pos;
		_cmd_len = strlen(ctrl->cookie) + 1 + cmd_len;
		cmd_buf = malloc(_cmd_len);
		if (cmd_buf == NULL)
			return -1;
		_cmd = cmd_buf;
		pos = cmd_buf;
		strlcpy(pos, ctrl->cookie, _cmd_len);
		pos += strlen(ctrl->cookie);
		*pos++ = ' ';
		memcpy(pos, cmd, cmd_len);
	} else
#endif /* CONFIG_CTRL_IFACE_UDP */
	{
		_cmd = cmd;
		_cmd_len = cmd_len;
	}

	if (send(ctrl->s, _cmd, _cmd_len, 0) < 0) {
		return -1;
	}

	for (;;) {
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ctrl->s, &rfds);
		res = select(ctrl->s + 1, &rfds, NULL, NULL, &tv);
		if (FD_ISSET(ctrl->s, &rfds)) {
			res = recv(ctrl->s, reply, *reply_len, 0);
		    if (res < 0 && errno == EINTR) {
			    continue;
			}
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
					if ((size_t) res == *reply_len)
						res = (*reply_len) - 1;
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


static int wpa_ctrl_attach_helper(struct wpa_ctrl *ctrl, int attach)
{
	char buf[10];
	int ret;
	size_t len = 10;

	ret = wpa_ctrl_request(ctrl, attach ? "ATTACH" : "DETACH", 6,
			       buf, &len, NULL);
	if (ret < 0)
		return ret;
	if (len == 3 && memcmp(buf, "OK\n", 3) == 0)
		return 0;
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


int wpa_ctrl_recv(struct wpa_ctrl *ctrl, char *reply, size_t *reply_len)
{
	int res;

	res = recv(ctrl->s, reply, *reply_len, 0);
	if (res < 0)
		return res;
	*reply_len = res;
	return 0;
}


int wpa_ctrl_pending(struct wpa_ctrl *ctrl)
{
	struct timeval tv;
	int res;
	fd_set rfds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ctrl->s, &rfds);
	res = select(ctrl->s + 1, &rfds, NULL, NULL, &tv);
	if (res < 0) {
		printf("select error\n");
	}
	return FD_ISSET(ctrl->s, &rfds);
}


int wpa_ctrl_get_fd(struct wpa_ctrl *ctrl)
{
	return ctrl->s;
}
