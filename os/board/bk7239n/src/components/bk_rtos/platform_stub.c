/**
 ******************************************************************************
 * @file    platform_init.c
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide functions called by _BK_ to drive stm32f2xx
 *          platform: - e.g. power save, reboot, platform initialize
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 BEKEN Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of BEKEN Corporation.
 ******************************************************************************
 */
#include <common/bk_include.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <os/mem.h>

#include <os/os.h>
#include "common/bk_assert.h"
#include <components/system.h>
#include <sys/time.h>

/************** wrap C library functions **************/
#define SECSPERMIN  60L
#define MINSPERHOUR 60L
#define HOURSPERDAY 24L
#define SECSPERHOUR (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY  (SECSPERHOUR * HOURSPERDAY)
#define DAYSPERWEEK 7
#define MONSPERYEAR 12

#define YEAR_BASE   1900
#define EPOCH_YEAR      1970
#define EPOCH_WDAY      4
#define EPOCH_YEARS_SINCE_LEAP 2
#define EPOCH_YEARS_SINCE_CENTURY 70
#define EPOCH_YEARS_SINCE_LEAP_CENTURY 370

#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

/* Move epoch from 01.01.1970 to 01.03.0000 (yes, Year 0) - this is the first
 * day of a 400-year long "era", right after additional day of leap year.
 * This adjustment is required only for date calculation, so instead of
 * modifying time_t value (which would require 64-bit operations to work
 * correctly) it's enough to adjust the calculated number of days since epoch.
 */
#define EPOCH_ADJUSTMENT_DAYS   719468L
/* year to which the adjustment was made */
#define ADJUSTED_EPOCH_YEAR 0
/* 1st March of year 0 is Wednesday */
#define ADJUSTED_EPOCH_WDAY 3
/* there are 97 leap years in 400-year periods. ((400 - 97) * 365 + 97 * 366) */
#define DAYS_PER_ERA        146097L
/* there are 24 leap years in 100-year periods. ((100 - 24) * 365 + 24 * 366) */
#define DAYS_PER_CENTURY    36524L
/* there is one leap year every 4 years */
#define DAYS_PER_4_YEARS    (3 * 365 + 366)
/* number of days in a non-leap year */
#define DAYS_PER_YEAR       365
/* number of days in January */
#define DAYS_IN_JANUARY     31
/* number of days in non-leap February */
#define DAYS_IN_FEBRUARY    28
/* number of years per era */
#define YEARS_PER_ERA       400

struct tm* __wrap_gmtime_r(const time_t *__restrict tim_p, struct tm *__restrict res)
{
    long days, rem;
    const time_t lcltime = *tim_p;
    int era, weekday, year;
    unsigned erayear, yearday, month, day;
    unsigned long eraday;

    days = lcltime / SECSPERDAY + EPOCH_ADJUSTMENT_DAYS;
    rem = lcltime % SECSPERDAY;
    if (rem < 0)
    {
        rem += SECSPERDAY;
        --days;
    }

    /* compute hour, min, and sec */
    res->tm_hour = (int) (rem / SECSPERHOUR);
    rem %= SECSPERHOUR;
    res->tm_min = (int) (rem / SECSPERMIN);
    res->tm_sec = (int) (rem % SECSPERMIN);

    /* compute day of week */
    if ((weekday = ((ADJUSTED_EPOCH_WDAY + days) % DAYSPERWEEK)) < 0)
        weekday += DAYSPERWEEK;
    res->tm_wday = weekday;

    /* compute year, month, day & day of year */
    /* for description of this algorithm see
     * http://howardhinnant.github.io/date_algorithms.html#civil_from_days */
    era = (days >= 0 ? days : days - (DAYS_PER_ERA - 1)) / DAYS_PER_ERA;
    eraday = days - era * DAYS_PER_ERA; /* [0, 146096] */
    erayear = (eraday - eraday / (DAYS_PER_4_YEARS - 1) + eraday / DAYS_PER_CENTURY - eraday / (DAYS_PER_ERA - 1)) / 365; /* [0, 399] */
    yearday = eraday - (DAYS_PER_YEAR * erayear + erayear / 4 - erayear / 100); /* [0, 365] */
    month = (5 * yearday + 2) / 153; /* [0, 11] */
    day = yearday - (153 * month + 2) / 5 + 1; /* [1, 31] */
    month += month < 10 ? 2 : -10;
    year = ADJUSTED_EPOCH_YEAR + erayear + era * YEARS_PER_ERA + (month <= 1);

    res->tm_yday =
            yearday >= DAYS_PER_YEAR - DAYS_IN_JANUARY - DAYS_IN_FEBRUARY ?
                    yearday - (DAYS_PER_YEAR - DAYS_IN_JANUARY - DAYS_IN_FEBRUARY) : yearday + DAYS_IN_JANUARY + DAYS_IN_FEBRUARY + isleap(erayear);
    res->tm_year = year - YEAR_BASE;
    res->tm_mon = month;
    res->tm_mday = day;

    res->tm_isdst = 0;

    return (res);
}


__attribute__((weak)) void *__wrap_malloc(size_t size)
{
	return os_malloc(size);
}

__attribute__((weak)) void *__wrap__malloc_r(void *p, size_t size)
{
	return os_malloc(size);
}

__attribute__((weak)) void __wrap_free(void *pv)
{
	os_free(pv);
}

__attribute__((weak)) void *__wrap_calloc(size_t a, size_t b)
{
	void *pvReturn;

	pvReturn = os_malloc(a * b);
	if (pvReturn)
    {
        os_memset(pvReturn, 0, a*b);
    }

	return pvReturn;
}

__attribute__((weak)) void *__wrap_realloc(void *pv, size_t size)
{
	return os_realloc(pv, size);
}

__attribute__((weak)) void __wrap__free_r(void *p, void *x)
{
	__wrap_free(x);
}

__attribute__((weak)) void *__wrap__realloc_r(void *p, void *x, size_t sz)
{
	return __wrap_realloc(x, sz);
}

__attribute__((weak)) void *__wrap_zalloc(size_t size)
{
	return os_zalloc(size);
}

int __wrap_strlen (char *src)
{
    int ret = 0;

    for (ret = 0; *src; src++)
        ret++;

    return ret;
}

int __wrap_strncmp(const char *s1, const char *s2, size_t n)
{
    BK_ASSERT(s1 && s2); /* ASSERT VERIFIED */

    if(0 == n) return 0;

    while(--n && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

bool printf_is_init(void);
void shell_log_out_port(int level, char *tag, const char *fmt, va_list args);
int __wrap_printf(const char *fmt, ...)
{

    va_list args;
    if(!printf_is_init())
        return 0;

    if(!bk_get_printf_enable())
        return 0;

    va_start(args,fmt);
    shell_log_out_port(BK_LOG_WARN, NULL, fmt, args);
    va_end(args);

    return 0;
}


int __wrap_iprintf(const char *fmt, ...)
{
    va_list args;
    if(!printf_is_init())
        return 0;

    if(!bk_get_printf_enable())
        return 0;

    va_start(args,fmt);
    shell_log_out_port(BK_LOG_WARN,NULL,fmt,args);
    va_end(args);

    return 0;
}

int __wrap_vprintf(const char *format,va_list args)
{
    char string[128];
    int len;

    len=vsnprintf(string, sizeof(string)-1, format, args);
    string[sizeof(string)-1]=0;

    printf(string);
    return len;
}



int __wrap_viprintf(const char *format,va_list args)
{
    char string[128];
    int len;

    len=vsnprintf(string, sizeof(string)-1, format, args);
    string[sizeof(string)-1]=0;

    printf(string);
    return len;
}

void __wrap___assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
	os_printf("%s %d func %s expr %s\n", file, line, func, failedexpr);
	BK_ASSERT(0); /* ASSERT VERIFIED */
}

unsigned long __wrap_strtoul (const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    unsigned long result = 0;
    unsigned long cutoff;
    int cutlim;
    int overflow = 0;
    int any = 0;

    if (!nptr)
        return 0;
    // Skip leading whitespace
    while (isspace((unsigned char)*s)) {
        s++;
    }
    // Handle optional sign
    if (*s == '-') {
        s++;
    } else if (*s == '+') {
        s++;
    }
    // Determine the base if it's 0
    if (base == 0) {
        if (*s == '0') {
            if (tolower((unsigned char)s[1]) == 'x') {
                base = 16;
                s += 2;
            } else {
                base = 8;
                s++;
            }
        } else {
            base = 10;
        }
    } else if (base == 16 && *s == '0' && tolower((unsigned char)s[1]) == 'x') {
        s += 2;
    }
    // Set cutoff values to handle overflow
    cutoff = ULONG_MAX / (unsigned long)base;
    cutlim = ULONG_MAX % (unsigned long)base;
    // Convert the number
    for (; *s; s++) {
        int c = *s;
        if (isdigit((unsigned char)c)) {
            c -= '0';
        } else if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c) - 'a' + 10;
        } else {
            break;
        }
        if (c >= base) {
            break;
        }
        if (any < 0 || result > cutoff || (result == cutoff && c > cutlim)) {
            overflow = 1;
        } else {
            any = 1;
            result = result * base + c;
        }
    }
    if (any == 0) {
        s = nptr;
    }
    if (endptr != NULL) {
        *endptr = (char *)(any ? s : nptr);
    }
    if (overflow) {
        errno = ERANGE;
        result = ULONG_MAX;
    }

    return result;
}

long __wrap_strtol(const char *nptr, char **endptr, int base) {
    const char *s = nptr;
    long result = 0;
    long cutoff;
    long cutlim;
    int overflow = 0;
    int any = 0;
    int negative = 0;

    if (!nptr) {
        return 0;
    }
    // Skip leading whitespace
    while (isspace((unsigned char)*s)) {
        s++;
    }
    // Handle optional sign
    if (*s == '-') {
        negative = 1;
        s++;
    } else if (*s == '+') {
        s++;
    }
    // Determine the base if it's 0
    if (base == 0) {
        if (*s == '0') {
            if (tolower((unsigned char)s[1]) == 'x') {
                base = 16;
                s += 2;
            } else {
                base = 8;
                s++;
            }
        } else {
            base = 10;
        }
    } else if (base == 16 && *s == '0' && tolower((unsigned char)s[1]) == 'x') {
        s += 2;
    }
    // Set cutoff values to handle overflow
    cutoff = LONG_MAX / (long)base;
    cutlim = LONG_MAX % (long)base;

    // Convert the number
    for (; *s; s++) {
        int c = *s;
        if (isdigit((unsigned char)c)) {
            c -= '0';
        } else if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c) - 'a' + 10;
        } else {
            break;
        }
        if (c >= base) {
            break;
        }
        if (any < 0 || result > cutoff || (result == cutoff && c > cutlim)) {
            overflow = 1;
        } else {
            any = 1;
            result = result * base + c;
        }
    }
    if (any == 0) {
        s = nptr;
    }
    if (endptr != NULL) {
        *endptr = (char *)(any ? s : nptr);
    }
    if (overflow) {
        errno = ERANGE;
        if (negative) {
            result = -result;
        }
    }

    return negative ? -result : result;
}


int __wrap_sscanf(const char *str, const char *format, ...) {
    int rv = 0;
    va_list ap;

    va_start(ap, format);
    rv = vsscanf(str, format, ap);
    va_end(ap);

    return rv;
}

int __wrap_rand(void)
{
	return bk_rand();
}

// eof

