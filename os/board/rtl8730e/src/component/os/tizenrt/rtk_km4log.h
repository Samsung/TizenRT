#ifndef _RTK_KM4_LOG_H_
#define	_RTK_KM4_LOG_H_

#include <stdint.h>
#include <tinyara/config.h>
#include <osif.h>
#include "section_config.h"
#include "ameba_soc.h"
#include "inic_ipc_cfg.h"
#ifndef CONFIG_KM4_MAX_LOG_QUEUE_SIZE
#define CONFIG_KM4_MAX_LOG_QUEUE_SIZE 5
#endif

#ifndef CONFIG_KM4_MAX_LOG_BUFFER_SIZE
#define CONFIG_KM4_MAX_LOG_BUFFER_SIZE 256
#endif

/* node to store the message to the queue. */
typedef struct km4log_msg {
	void * buffer;			// pointer to contents. contents will be freed within this task after consumption
	uint16_t buffer_len;	// buffer len
} km4log_msg_t;

#endif

