/*
 * Event loop based on select() loop
 * Copyright (c) 2002-2005, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is os_free software; you can redistribute it and/or modify
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
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <os/mem.h>
#include <utils/os.h>

#ifdef CONFIG_NATIVE_WINDOWS
#include "common.h"
#endif /* CONFIG_NATIVE_WINDOWS */

#include "controlapp_eloop.h"
#include "vendor_specific.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#ifndef __maybe_unused
#define __maybe_unused __attribute__((unused))
#endif

void vendor_deinit();
extern int controlappc_service_socket_init(void);

struct controlapp_eloop_sock {
	int sock;
	void *eloop_data;
	void *user_data;
	void (*handler)(int sock, void *eloop_ctx, void *sock_ctx);
};

struct controlapp_eloop_timeout {
	struct os_reltime time;
	void *eloop_data;
	void *user_data;
	void (*handler)(void *eloop_ctx, void *sock_ctx);
	struct controlapp_eloop_timeout *next;
};

struct controlapp_eloop_signal {
	int sig;
	void *user_data;
	void (*handler)(int sig, void *eloop_ctx, void *signal_ctx);
	int signaled;
};

struct controlapp_eloop_data {
	void *user_data;

	int max_sock, reader_count;
	struct controlapp_eloop_sock *readers;

	struct controlapp_eloop_timeout *timeout;

	int signal_count;
	struct controlapp_eloop_signal *signals;
	int signaled;
	int pending_terminate;

	int terminate;
};

static struct controlapp_eloop_data controlapp_eloop;


void controlapp_eloop_init(void *user_data)
{
	memset(&controlapp_eloop, 0, sizeof(controlapp_eloop));
	controlapp_eloop.user_data = user_data;
}


int controlapp_eloop_register_read_sock(int sock,
			     void (*handler)(int sock, void *eloop_ctx,
					     void *sock_ctx),
			     void *eloop_data, void *user_data)
{
	struct controlapp_eloop_sock *tmp;

	tmp = (struct controlapp_eloop_sock *)
		os_realloc(controlapp_eloop.readers,
			(controlapp_eloop.reader_count + 1) * sizeof(struct controlapp_eloop_sock));
	if (tmp == NULL) {
		os_printf("XXX: %s %d failed\n", __func__, __LINE__);
		return -1;
	}

	os_printf("XXX: %s %d: sock 0x%x\n", __func__, __LINE__, sock);

	tmp[controlapp_eloop.reader_count].sock = sock;
	tmp[controlapp_eloop.reader_count].eloop_data = eloop_data;
	tmp[controlapp_eloop.reader_count].user_data = user_data;
	tmp[controlapp_eloop.reader_count].handler = handler;
	controlapp_eloop.reader_count++;
	controlapp_eloop.readers = tmp;
	if (sock > controlapp_eloop.max_sock)
		controlapp_eloop.max_sock = sock;

	return 0;
}


void controlapp_eloop_unregister_read_sock(int sock)
{
	int i;

	if (controlapp_eloop.readers == NULL || controlapp_eloop.reader_count == 0)
		return;

	for (i = 0; i < controlapp_eloop.reader_count; i++) {
		if (controlapp_eloop.readers[i].sock == sock)
			break;
	}
	if (i == controlapp_eloop.reader_count)
		return;
	if (i != controlapp_eloop.reader_count - 1) {
		memmove(&controlapp_eloop.readers[i], &controlapp_eloop.readers[i + 1],
			(controlapp_eloop.reader_count - i - 1) *
			sizeof(struct controlapp_eloop_sock));
	}
	controlapp_eloop.reader_count--;
}


int controlapp_eloop_register_timeout(unsigned int secs, unsigned int usecs,
			   void (*handler)(void *eloop_ctx, void *timeout_ctx),
			   void *eloop_data, void *user_data)
{
	struct controlapp_eloop_timeout *timeout, *tmp, *prev;

	timeout = (struct controlapp_eloop_timeout *) os_malloc(sizeof(*timeout));
	if (timeout == NULL)
		return -1;
	os_get_reltime(&timeout->time);
	timeout->time.sec += secs;
	timeout->time.usec += usecs;
	while (timeout->time.usec >= 1000000) {
		timeout->time.sec++;
		timeout->time.usec -= 1000000;
	}
	timeout->eloop_data = eloop_data;
	timeout->user_data = user_data;
	timeout->handler = handler;
	timeout->next = NULL;

	os_printf("XXX: %s handler %p, eloop_data %p\n", __func__, handler, eloop_data);

	if (controlapp_eloop.timeout == NULL) {
		controlapp_eloop.timeout = timeout;
		return 0;
	}

	prev = NULL;
	tmp = controlapp_eloop.timeout;
	while (tmp != NULL) {
		//if (timercmp(&timeout->time, &tmp->time, <))
		if (os_reltime_before(&timeout->time, &tmp->time))
			break;
		prev = tmp;
		tmp = tmp->next;
	}

	if (prev == NULL) {
		timeout->next = controlapp_eloop.timeout;
		controlapp_eloop.timeout = timeout;
	} else {
		timeout->next = prev->next;
		prev->next = timeout;
	}

	return 0;
}


int controlapp_eloop_cancel_timeout(void (*handler)(void *eloop_ctx, void *sock_ctx),
			 void *eloop_data, void *user_data)
{
	struct controlapp_eloop_timeout *timeout, *prev, *next;
	int removed = 0;

	prev = NULL;
	timeout = controlapp_eloop.timeout;
	while (timeout != NULL) {
		next = timeout->next;

		if (timeout->handler == handler &&
		    (timeout->eloop_data == eloop_data ||
		     eloop_data == ELOOP_ALL_CTX) &&
		    (timeout->user_data == user_data ||
		     user_data == ELOOP_ALL_CTX)) {
			if (prev == NULL)
				controlapp_eloop.timeout = next;
			else
				prev->next = next;
			os_free(timeout);
			removed++;
		} else
			prev = timeout;

		timeout = next;
	}

	os_printf("XXX: %s handler %p, eloop_data %p, removed %d\n", __func__, handler, eloop_data, removed);

	return removed;
}


#if !defined(CONFIG_NATIVE_WINDOWS) && !defined(BEKEN_API)
static void controlapp_eloop_handle_alarm(int sig)
{
	fprintf(stderr, "controlapp_eloop: could not process SIGINT or SIGTERM in two "
		"seconds. Looks like there\n"
		"is a bug that ends up in a busy loop that "
		"prevents clean shutdown.\n"
		"Killing program forcefully.\n");
	vendor_deinit();
	exit(1);
}


static void controlapp_eloop_handle_signal(int sig)
{
	int i;

#if !defined(CONFIG_NATIVE_WINDOWS) && !defined(BEKEN_API)
	if ((sig == SIGINT || sig == SIGTERM) && !controlapp_eloop.pending_terminate) {
		/* Use SIGALRM to break out from potential busy loops that
		 * would not allow the program to be killed. */
		controlapp_eloop.pending_terminate = 1;
		signal(SIGALRM, eloop_handle_alarm);
		alarm(2);
	}
#endif /* CONFIG_NATIVE_WINDOWS */

	controlapp_eloop.signaled++;
	for (i = 0; i < controlapp_eloop.signal_count; i++) {
		if (controlapp_eloop.signals[i].sig == sig) {
			controlapp_eloop.signals[i].signaled++;
			break;
		}
	}
}
#endif /* CONFIG_NATIVE_WINDOWS */


static void controlapp_eloop_process_pending_signals(void)
{
	int i;

	if (controlapp_eloop.signaled == 0)
		return;
	controlapp_eloop.signaled = 0;

	if (controlapp_eloop.pending_terminate) {
#if !defined(CONFIG_NATIVE_WINDOWS) && !defined(BEKEN_API)
		alarm(0);
#endif /* CONFIG_NATIVE_WINDOWS */
		controlapp_eloop.pending_terminate = 0;
	}

	for (i = 0; i < controlapp_eloop.signal_count; i++) {
		if (controlapp_eloop.signals[i].signaled) {
			controlapp_eloop.signals[i].signaled = 0;
			controlapp_eloop.signals[i].handler(controlapp_eloop.signals[i].sig,
						 controlapp_eloop.user_data,
						 controlapp_eloop.signals[i].user_data);
		}
	}
}


#if !defined(CONFIG_NATIVE_WINDOWS) && !defined(BEKEN_API)
int controlapp_eloop_register_signal(int sig,
			  void (*handler)(int sig, void *eloop_ctx,
					  void *signal_ctx),
			  void *user_data)
{
	struct controlapp_eloop_signal *tmp;

	tmp = (struct controlapp_eloop_signal *)
		os_realloc(controlapp_eloop.signals,
			(controlapp_eloop.signal_count + 1) *
			sizeof(struct controlapp_eloop_signal));
	if (tmp == NULL)
		return -1;

	tmp[controlapp_eloop.signal_count].sig = sig;
	tmp[controlapp_eloop.signal_count].user_data = user_data;
	tmp[controlapp_eloop.signal_count].handler = handler;
	tmp[controlapp_eloop.signal_count].signaled = 0;
	controlapp_eloop.signal_count++;
	controlapp_eloop.signals = tmp;
	signal(sig, eloop_handle_signal);

	return 0;
}
#endif


void controlapp_eloop_run(void)
{
	fd_set *rfds;
	int i, res;
	struct os_reltime now;
	struct os_reltime tv __maybe_unused;
	struct timeval _tv;
	int noprint = 1;

	//bk_printf("%s %d\n", __func__, __LINE__);

	rfds = os_malloc(sizeof(*rfds));
	if (rfds == NULL) {
		os_printf("eloop_run - os_malloc failed\n");
		return;
	}

	//bk_printf("%s %d\n", __func__, __LINE__);
	while (!controlapp_eloop.terminate /*&&
		(controlapp_eloop.timeout || controlapp_eloop.reader_count > 0)*/) {
		//bk_printf("%s %d\n", __func__, __LINE__);
		noprint = 1;

#if 0
		if (controlapp_eloop.timeout) {
			os_get_reltime(&now);
			//if (timercmp(&now, &controlapp_eloop.timeout->time, <))
			//	timersub(&controlapp_eloop.timeout->time, &now, &tv);
			if (os_reltime_before(&now, &controlapp_eloop.timeout->time))
				os_reltime_sub(&controlapp_eloop.timeout->time, &now, &tv);
			else
				tv.sec = tv.usec = 0;
#if 0
			os_printf("next timeout in %lu.%06lu sec\n",
			       tv.sec, tv.usec);
#endif
			_tv.tv_sec = tv.sec;
			_tv.tv_usec = tv.usec;
		} else {
			_tv.tv_sec = 0;
			_tv.tv_usec = 100000;  /* 100ms */
			noprint = 0;
		}
		if (_tv.tv_sec == 0 && _tv.tv_usec == 0)
			_tv.tv_usec = 100000;  /* 100ms */
#else
		_tv.tv_sec = 0;
		_tv.tv_usec = 100000;  /* 100ms */
#endif

		if (controlapp_eloop.reader_count == 0 && controlapp_eloop.timeout == NULL) {
			os_sleep(0, 100000);
			continue;
		}

		FD_ZERO(rfds);
		for (i = 0; i < controlapp_eloop.reader_count; i++)
			FD_SET(controlapp_eloop.readers[i].sock, rfds);

		if (!noprint)
		os_printf("XXX: %s %d, max_sock %d, sec %d, usec %d\n", __func__, __LINE__, controlapp_eloop.max_sock, _tv.tv_sec, _tv.tv_usec);

		res = select(controlapp_eloop.max_sock + 1, rfds, NULL, NULL, &_tv);
		if (res < 0 /*&& errno != EINTR*/) {
			os_printf("XXX: select failed errno %d\n", errno);
			os_free(rfds);
			return;
		}
		controlapp_eloop_process_pending_signals();

		/* check if some registered timeouts have occurred */
		if (controlapp_eloop.timeout) {
			struct controlapp_eloop_timeout *tmp;

			os_get_reltime(&now);
			//if (!timercmp(&now, &controlapp_eloop.timeout->time, <))
			if (!os_reltime_before(&now, &controlapp_eloop.timeout->time))
			{
				tmp = controlapp_eloop.timeout;
				controlapp_eloop.timeout = controlapp_eloop.timeout->next;
				tmp->handler(tmp->eloop_data,
					     tmp->user_data);
				os_free(tmp);
			}
		}

		if (res <= 0) {
			if (!noprint)
			os_printf("XXX: %s %d, res %d\n", __func__, __LINE__, res);
			continue;
		}

		for (i = 0; i < controlapp_eloop.reader_count; i++) {
			if (!noprint)
			os_printf("XXX: %s %d\n", __func__, __LINE__);
			if (FD_ISSET(controlapp_eloop.readers[i].sock, rfds)) {
				if (!noprint)
				os_printf("XXX: %s %d\n", __func__, __LINE__);
				controlapp_eloop.readers[i].handler(
					controlapp_eloop.readers[i].sock,
					controlapp_eloop.readers[i].eloop_data,
					controlapp_eloop.readers[i].user_data);
			}
		}
	}
	//bk_printf("%s %d\n", __func__, __LINE__);

	os_free(rfds);
}


void controlapp_eloop_terminate(void)
{
	controlapp_eloop.terminate = 1;
}


void controlapp_eloop_destroy(void)
{
	struct controlapp_eloop_timeout *timeout, *prev;

	timeout = controlapp_eloop.timeout;
	while (timeout != NULL) {
		prev = timeout;
		timeout = timeout->next;
		os_free(prev);
	}
	os_free(controlapp_eloop.readers);
	os_free(controlapp_eloop.signals);
}


int controlapp_eloop_terminated(void)
{
	return controlapp_eloop.terminate;
}

void controlapp_eloop_thread(void *arg)
{
	int service_socket = -1;

    controlapp_eloop_init(NULL);

	service_socket = controlappc_service_socket_init();
	os_printf("eloop service_socket %d\n", service_socket);
    controlapp_eloop_run();

	os_printf("%s: eloop stopped\n", __func__);
	if (service_socket >= 0) {
		close(service_socket);
	}
	controlapp_eloop_destroy();

	rtos_delete_thread(NULL);
}

