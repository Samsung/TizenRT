#ifndef _CSIFW_LOGGER_H_
#define _CSIFW_LOGGER_H_

#include <system/system_log.h>

#define LOG_TAG "CSIFW"
extern int SYSTEM_TAG_CSIFW;

#define CSIFW_LOGD(fmt, ...)       SYSTEM_LOG_DEBUG(SYSTEM_TAG_CSIFW, fmt, ##__VA_ARGS__);
#define CSIFW_LOGI(fmt, ...)       SYSTEM_LOG_INFO(SYSTEM_TAG_CSIFW, fmt, ##__VA_ARGS__);
#define CSIFW_LOGE(fmt, ...)       SYSTEM_LOG_ERROR(SYSTEM_TAG_CSIFW, fmt, ##__VA_ARGS__);

#endif /* _CSIFW_LOGGER_H_ */
