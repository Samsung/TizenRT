#ifndef _TLS_LOG_H_
#define _TLS_LOG_H_

#include <components/log.h>

#define MBEDTLS_TAG "MBEDTLS"

#define MBEDTLS_LOGI(...) BK_LOGI(MBEDTLS_TAG, ##__VA_ARGS__)
#define MBEDTLS_LOGW(...) BK_LOGW(MBEDTLS_TAG, ##__VA_ARGS__)
#define MBEDTLS_LOGE(...) BK_LOGE(MBEDTLS_TAG, ##__VA_ARGS__)
#define MBEDTLS_LOGD(...) BK_LOGD(MBEDTLS_TAG, ##__VA_ARGS__)

#endif

