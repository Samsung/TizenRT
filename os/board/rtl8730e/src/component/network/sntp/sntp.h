#ifndef __SNTP_H__
#define __SNTP_H__

#if (defined(CONFIG_SYSTEM_TIME64) && CONFIG_SYSTEM_TIME64)
#include "time64.h"
#else
#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void sntp_init(void);
void sntp_stop(void);

/* Realtek added */
#if (defined(CONFIG_SYSTEM_TIME64) && CONFIG_SYSTEM_TIME64)
void sntp_get_lasttime(long long *sec, long long *usec, unsigned int *tick);
#else
void sntp_get_lasttime(time_t *sec, time_t *usec, unsigned int *tick);
#endif

struct tm sntp_gen_system_time(int timezone);
struct tm sntp_gen_system_time_s(int timezone_s);

#ifdef __cplusplus
}
#endif

#endif /* __SNTP_H__ */
