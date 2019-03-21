/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdint.h>
#include <fixedmath.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "up_arch.h"

#include "chip/imxrt_wdog.h"
#include "imxrt_config.h"
#include "imxrt_log.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LOG_BUF_LENGTH 1024
#define LAST_LOG_BUF_LENGTH 32
/* Configuration ************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
uint8_t log_buffer[LOG_BUF_LENGTH];
uint8_t last_log_buffer[LAST_LOG_BUF_LENGTH];

uint32_t current_log_length = 0;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_log_write_kernelbuffer
 *
 * Description:
 *   Save the log when booting
 *
 ****************************************************************************/

void imxrt_log_write_kernelbuffer(const uint8_t *log_string, const uint32_t log_length)
{
	memset(last_log_buffer, 0, LAST_LOG_BUF_LENGTH);

	if(log_length > LAST_LOG_BUF_LENGTH-1) {
		memcpy(last_log_buffer, log_string, LAST_LOG_BUF_LENGTH-1);
	} else {
		memcpy(last_log_buffer, log_string, log_length);
	}

	if(current_log_length + log_length > LOG_BUF_LENGTH -1) {
		current_log_length = 0;
		return;
	}

	memcpy(&log_buffer[current_log_length], log_string, log_length);
	current_log_length += log_length;
	log_buffer[current_log_length] = '\n';
	current_log_length += 1;
}

/****************************************************************************
 * Name: imxrt_log_print_kernelbuffer
 *
 * Description:
 *   Called when kernel log can be printed
 *
 ****************************************************************************/

void imxrt_log_print_kernelbuffer(void)
{
	printf("\n");
	printf("#### LAST LOG ########\n");
	printf("%s", last_log_buffer);
	printf("\n");
	printf("#### TOTAL LOG #######\n");
	printf("%s", log_buffer);
	printf("\n");
}
