/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __DRV_TYPES_SDIO_H__
#define __DRV_TYPES_SDIO_H__

/* SDIO Header Files */
#ifdef PLATFORM_LINUX
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>

#ifdef CONFIG_PLATFORM_SPRD
#include <linux/gpio.h>
#include <custom_gpio.h>
#endif /* CONFIG_PLATFORM_SPRD */
#endif

#ifdef PLATFORM_OS_XP
#include <wdm.h>
#include <ntddsd.h>
#endif

#ifdef PLATFORM_OS_CE
#include <sdcardddk.h>
#endif

#define RTW_SDIO_CLK_33M 33000000
#define RTW_SDIO_CLK_40M 40000000
#define RTW_SDIO_CLK_80M 80000000
#define RTW_SDIO_CLK_160M 160000000

typedef struct sdio_data {
	u8 func_number;

	u8 tx_block_mode;
	u8 rx_block_mode;
	u32 block_transfer_len;

#ifdef PLATFORM_LINUX
	struct sdio_func *func;
	_thread_hdl_ sys_sdio_irq_thd;
	unsigned int clock;
	unsigned int timing;
	u8 sd3_bus_mode;
#endif

#ifdef PLATFORM_OS_XP
	PDEVICE_OBJECT pphysdevobj;
	PDEVICE_OBJECT pfuncdevobj;
	PDEVICE_OBJECT pnextdevobj;
	SDBUS_INTERFACE_STANDARD sdbusinft;
	u8 nextdevstacksz;
#endif

#ifdef PLATFORM_OS_CE
	SD_DEVICE_HANDLE hDevice;
	SD_CARD_RCA sd_rca;
	SD_CARD_INTERFACE card_intf;
	BOOLEAN enableIsarWithStatus;
	WCHAR active_path[MAX_ACTIVE_REG_PATH];
	SD_HOST_BLOCK_CAPABILITY sd_host_blk_cap;
#endif
} SDIO_DATA, *PSDIO_DATA;

#define dvobj_to_sdio_func(d) ((d)->intf_data.func)

#define RTW_SDIO_ADDR_CMD52_BIT (1 << 17)
#define RTW_SDIO_ADDR_CMD52_GEN(a) (a | RTW_SDIO_ADDR_CMD52_BIT)
#define RTW_SDIO_ADDR_CMD52_CLR(a) (a & ~RTW_SDIO_ADDR_CMD52_BIT)
#define RTW_SDIO_ADDR_CMD52_CHK(a) (a & RTW_SDIO_ADDR_CMD52_BIT ? 1 : 0)

#define RTW_SDIO_ADDR_F0_BIT (1 << 18)
#define RTW_SDIO_ADDR_F0_GEN(a) (a | RTW_SDIO_ADDR_F0_BIT)
#define RTW_SDIO_ADDR_F0_CLR(a) (a & ~RTW_SDIO_ADDR_F0_BIT)
#define RTW_SDIO_ADDR_F0_CHK(a) (a & RTW_SDIO_ADDR_F0_BIT ? 1 : 0)

#endif
