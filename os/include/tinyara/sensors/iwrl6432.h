/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/spi/spi.h>
#include <queue.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PAYLOAD_HEADER 0x32
#define PAYLOAD_TAIL 0x34
#define PAYLOAD_SOF 0x02
#define PARAMETER_TX_SIZE 1032
#define PARAMETER_PACKET_LENGTH 1025
#define ACK_TX_SIZE 16
#define CLOUD_TX_SIZE 1520
#define TRACK_DATA_SIZE 88
#define IWRL6432_DATA_HEADER_SIZE 24
#ifndef CONFIG_IWRL6432_SEGNUM
#define CONFIG_IWRL6432_SEGNUM 6
#endif
#define IWRL6432_BUF_NUM CONFIG_IWRL6432_SEGNUM + 2 // +2 for Cloud and Track data
#define IWRL6432_CUBE_DATA_SIZE (CONFIG_IWRL6432_SEGNUM == 6 ? (16384 + IWRL6432_DATA_HEADER_SIZE) : (12228 + IWRL6432_DATA_HEADER_SIZE))

/* iwrl6432 Message ID */
#define IWRL6432_MSG_DEQUEUE 0
#define IWRL6432_MSG_STOP_FORCELY 1
#define IWRL6432_MSG_UNDERRUN 2
#define IWRL6432_MSG_READY_TO_USE 3
#define IWRL6432_MSG_TIMEOUT 4
#define IWRL6432_ACK 0
#define IWRL6432_NACK 1
#define IWRL6432_MAX_INIT_RETRY_COUNT 3

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef void (*iwrl6432_handler_t)(void *arg, bool state);

struct iwrl6432_config_s {

	/* SPI related configs */
	struct spi_io_config spi_config;

	/* callback function: attach the iwrl6432 interrupt handler to the GPIO interrupt */
	CODE int (*attach)(iwrl6432_handler_t handler, FAR char *arg);

	/* callback function: enable or disable gpio pin interrupt */
	CODE int (*irq_enable)(bool enable);

	CODE int (*ready_pin_status)(void);

	/* callback function: reset iwrl6432 */
	CODE int (*reset)(void);
};

struct iwrl6432_buf_s {
	struct dq_entry_s entry;		  /* double linked queue entry */
	uint16_t maxbyte;				  /* Total byte of buffer */
	uint16_t curbyte;				  /* Currently used */
	uint8_t data[IWRL6432_CUBE_DATA_SIZE]; /* Actual Buffer include Header */
	uint16_t msgId;					  /* msgId to be shared */
};

struct iwrl6432_msg_s {
	uint16_t msgId;	 /* msgID, see above Message ID */
	FAR void *pData; /* Buffer data being dequeued */
};

/****************************************************************************
 * Public Function
 ****************************************************************************/

/****************************************************************************
 * Name: iwrl6432_register
 *
 * Description:
 *  This function will register iwrl6432 mmwave sensor driver as /dev/imgN where N
 *  is the minor device number
 *
 * Input Parameters:
 *   devname  - The full path to the driver to register. E.g., "/dev/img0"
 *   config   - configuration for the iwrl6432 driver.
 *   spi      - pointer of spi dev
 *
 * Returned Value:
 *   iwrl6432_dev_s is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int iwrl6432_register(FAR const char *devname, FAR struct iwrl6432_config_s *config, FAR struct spi_dev_s *spi);

/****************************************************************************
 * Name: iwrl6432_set_state
 *
 * Description:
 *  This function will enable/disable ISR of iwrl6432 by condition of device by outside(e.g endp120)
 *
 * Input Parameters:
 *   enable       - state of iwrl6432
 *
 * Returned Value:
 *
 ****************************************************************************/
void iwrl6432_set_state(bool enable);
