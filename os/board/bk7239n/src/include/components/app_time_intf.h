#ifndef __APP_TIME_INTF_H__
#define __APP_TIME_INTF_H__

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t mday;
    uint8_t month;
    uint16_t year;
    uint8_t wday;
} user_datetime_t;

int app_time_rtc_ntp_sync_init(void);
int app_time_timestr_get(char *buf,unsigned char len);
int app_time_datetime_get(user_datetime_t *pdt);

#endif
