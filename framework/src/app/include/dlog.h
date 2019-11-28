#ifndef __DLOG_H__
#define __DLOG_H__

#include <stdio.h>

#define LOGE(fmt, args...) printf(fmt, ##args)
#define LOGW(fmt, args...) printf(fmt, ##args)
#define LOGD(fmt, args...) printf(fmt, ##args)
#define LOGI(fmt, args...) printf(fmt, ##args)

#endif
