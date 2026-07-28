#ifndef __AFT_MAIN_H
#define __AFT_MAIN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


int send_stream_start(void);
int send_stream_buf(const uint8_t *buffer, uint32_t length);
int send_stream_end(void);

#ifdef __cplusplus
}
#endif

#endif /* __AFT_MAIN_H */
