/****************************************************************************
 * apps/se/armino/bk_security_api_wrapper_tz.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/security_hal.h>
#include <tinyara/seclink_drv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TFM NS Interface */
#include "tfm_ns_interface.h"
#include "tfm_dh_nsc.h"
#include "components/log.h"
#include <debug.h>

// /* Board-specific headers */
#include <driver/flash_types.h>
#include <driver/flash.h>

/* Flash Status Bits for protection */
#define FLASH_STATUS_BITS 0x3C

/* Add your implementation here */

/**
  * @brief  Erase the Flash Sector.
  * @param  address: specifies the address to be erased.
  * @retval None
  */
void ns_flash_erase(uint32_t address)
{
	bk_flash_erase_sector(address);
}

/**
  * @brief  Read the Flash Data.
  * @param  address: specifies the address to be read.
  * @param  len: specifies the length of data to be read.
  * @param  data: pointer to data buffer.
  * @retval Result of the operation.
  */
 static int ns_flash_read(uint32_t address, uint32_t len, uint8_t *data)
{
	int ret;

	ret = bk_flash_read_bytes(address, data, len);

	return ret;
}

/**
  * @brief  Write the Flash Data.
  * @param  address: specifies the address to be written.
  * @param  len: specifies the length of data to be written.
  * @param  data: pointer to data buffer.
  * @retval Result of the operation.
  */
static int ns_flash_write(uint32_t address, uint32_t len, uint8_t *data)
{
	int ret;

	ret = bk_flash_write_bytes(address, data, len);

	return ret;
}

/**
  * @brief  Verify the Flash Data.
  * @param  address: specifies the address to be read.
  * @param  len: specifies the length of data to be read.
  * @param  data: pointer to data buffer.
  * @param  en_display: enable display flag.
  * @retval Result of the operation.
  */
int bk_ss_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display)
{
	int ret;

	ret = bk_flash_read_bytes(address, data, len);
	if (ret == 0) {
		if (en_display) {
			printf("\n\r---- [Address = %02x] Start [len = %d]-----\n", address, len);
			for(int i = 0; i < len; i++){
				printf("%02x ", data[i]);
				if (!((i + 1) % 16)) {
					printf("\n");
				}
			}
			printf("\n\r---- [%s] End -----\n", __FUNCTION__);
		}
	}

	return ret;
}

/**
  * @brief  Verify the Flash Protection Bit is enabled.
  * @param  none
  * @return Result.
  * @retval 1: Flash Protection Bit is enabled.
  * @retval 0: Flash Protection Bit is disabled.
  */
int bk_verify_flash_protect(void)
{
	flash_protect_type_t protect_type;

	protect_type = bk_flash_get_protect_type();
	printf("Flash Protect Type: %d\n", protect_type);

	if (protect_type != FLASH_PROTECT_NONE) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  Set the Flash Status Bits.
  * @param  NewState: new state of the flash status bits.
  * @retval None
  */
static void ns_setstatusbits(unsigned int NewState)
{
	bk_flash_write_enable();
	bk_flash_write_status_reg(FLASH_STATUS_BITS);
	bk_flash_write_disable();
}

/**
  * @brief  Print the input string.
  * @param  input: pointer to input string.
  * @retval None
  */
static void ns_printf(const char *input)
{
	printf("%s\n", input);
}
