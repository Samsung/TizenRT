/*
 * wpa_supplicant/hostapd / Empty OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */

#include <common/bk_include.h>
#include "bk_fake_clock.h"

#include "includes.h"
#include "os.h"
#include <components/system.h>
#include <driver/trng.h>

void os_sleep(os_time_t sec, os_time_t usec)
{
	rtos_delay_milliseconds(sec * 1000 + usec/1000);
}

time_t os_time(time_t *tm)
{
	uint64_t ticks = bk_get_tick();
	time_t t = ticks / bk_get_ticks_per_second();
	if (tm)
		*tm = t;

	return t;
}

int os_get_time(struct os_time *t)
{
	uint64_t ticks = bk_get_tick();
	if (t) {
		t->sec = ticks / bk_get_ticks_per_second();
		t->usec = ((ticks % bk_get_ticks_per_second()) * 1000000) / bk_get_ticks_per_second();
	}

	return 0;
}


int os_get_reltime(struct os_reltime *t)
{
	uint64_t ticks = bk_get_tick();
	if (t) {
		t->sec = ticks / bk_get_ticks_per_second();
		t->usec = ((ticks % bk_get_ticks_per_second()) * 1000000) / bk_get_ticks_per_second();
	}

	return 0;
}


int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t)
{
	return -1;
}

int os_gmtime(os_time_t t, struct os_tm *tm)
{
	return -1;
}


int os_daemonize(const char *pid_file)
{
	return -1;
}


void os_daemonize_terminate(const char *pid_file)
{
}

int os_get_random(unsigned char *buf, size_t sz)
{
	int r;
	int len;

	while (sz > 0) {
		len = sizeof(r) > sz ? sz : sizeof(r);
		//modified by beken
		r = bk_rand();
		os_memcpy(buf, &r, len);
		buf += len;
		sz -= len;
	}

	return 0;
}

unsigned long os_random(void)
{
	//modified by beken
	return bk_rand();
}


char * os_rel2abs_path(const char *rel_path)
{
	return NULL; /* strdup(rel_path) can be used here */
}


int os_program_init(void)
{
	return 0;
}


void os_program_deinit(void)
{
}


int os_setenv(const char *name, const char *value, int overwrite)
{
	return -1;
}


int os_unsetenv(const char *name)
{
	return -1;
}


extern void wpa_dbg(void *ctx, int level, const char *fmt, ...);

char *os_readfile(const char *name, size_t *len)
{
	return NULL;
}


int os_fdatasync(FILE *stream)
{
	return 0;
}


int os_exec(const char *program, const char *arg, int wait_completion)
{
	return -1;
}

#if 0
int os_memcmp_const(const void *a, const void *b, size_t len)
{
	const u8 *aa = a;
	const u8 *bb = b;
	size_t i;
	u8 res;

	for (res = 0, i = 0; i < len; i++)
		res |= aa[i] ^ bb[i];

	return res;
}
#endif

void * os_memdup(const void *src, size_t len)
{
	void *r = os_malloc(len == 0 ? 1 : len);

	if (r && src)
		os_memcpy(r, src, len);
	return r;
}

// eof

