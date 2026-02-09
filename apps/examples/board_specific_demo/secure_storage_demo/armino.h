/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#ifndef __ARMINO_H
#define __ARMINO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <tinyara/config.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

extern int bk_ss_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display);
extern void ns_flash_erase(uint32_t address);
extern int bk_verify_flash_protect(void);

/****************************************************************************
 * Public Definitions
 ****************************************************************************/

#define BOARD_SS_FLASH_READ bk_ss_flash_read
#define BOARD_FLASH_ERASE ns_flash_erase
#define BOARD_VERIFY_FLASH_PROTECT bk_verify_flash_protect
#define BOARD_NAME "Armino"

static uint32_t SLOT0_START_ADDR = 0x5B000;
static uint32_t SLOT1_START_ADDR = 0x5E000;
static uint32_t SLOT_AREA_OFFSET = 0x10000;

#endif /* __ARMINO_H */
