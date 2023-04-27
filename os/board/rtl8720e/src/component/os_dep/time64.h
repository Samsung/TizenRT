#ifndef	_TIME64_DEF_H
#define _TIME64_DEF_H

/*
 The following macros __GNUC__ and __ICCARM__ are set depending on if GCC or IAR is used.
*/
#if defined(__GNUC__)

#define __time_t_defined   //__time_t is defined here because it is used by sys/_timespec.h as a guard in defining time_t.
#define time_t long long   //defined time_t to be long long before it is defined in sys/_timespec.h
/*
 time.h has to be included because it contains some structures and function
 that will be used by gmtime_r(), __wrap_localtime), __wrap_mktime() in libc_wrap.c.
 It is placed below the above definitions for __time_time_defined and time_t because in header files included
 in time.h, time_t will be defined again to long. Therefore time_t has to be defined to long long first on top.
*/
#include <time.h>

#elif defined(__ICCARM__)
#include "c/time64.h"

#endif
#endif