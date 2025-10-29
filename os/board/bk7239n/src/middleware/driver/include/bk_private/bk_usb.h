// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_include.h>
#include <components/usb_types.h>
#include <components/usb.h>

#define UVC_DEMO_SUPPORT102
#define USB_FAILURE                (1)
#define USB_SUCCESS                (0)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
extern void usb_init(void);
extern void usb_exit(void);
extern uint32_t MUSB_HfiRead( uint32_t first_block, uint32_t block_num, uint8_t
                              *dest);
extern uint32_t MUSB_HfiWrite( uint32_t first_block, uint32_t block_num, uint8_t
                               *dest);
extern void MGC_RegisterCBTransferComplete(FUNCPTR func);

#if defined(CONFIG_USB_MSD)
UINT32 usb_read (UINT32 pos, const void *buffer, UINT32 size);
UINT32 usb_write (UINT32 pos, const void *buffer, UINT32 size);
#else
UINT32 usb_read (char *user_buf, UINT32 count, UINT32 op_flag);
UINT32 usb_write (char *user_buf, UINT32 count, UINT32 op_flag);
#endif

#if (defined(CONFIG_SOC_BK7251)) || (defined(CONFIG_SOC_BK7271))
#define USB_PLUG_FAILURE                (1)
#define USB_PLUG_SUCCESS                (0)

#define USB_PLUG_DEV_NAME               "usb_plug"
#include "bk_gpio.h"

void usb_plug_inout_init(void);
void usb_plug_inout_exit(void);
#endif // (CONFIG_SOC_BK7251)

#ifdef __cplusplus
}
#endif
