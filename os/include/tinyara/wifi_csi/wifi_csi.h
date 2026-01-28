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
 * include/tinyara/wifi_csi/wifi_csi.h
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_TINYARA_WIFI_CSI_H
#define __INCLUDE_TINYARA_WIFI_CSI_H

/* For the purposes of this driver, an Wificsi device is any device that
 * sends  wificsi data.
 *
 * The Wificsi driver is split into two parts:
 *
 * 1) An "upper half", generic driver that provides the comman Wificsi interface
 *    to application level code, and
 * 2) A "lower half", platform-specific driver that implements the low-level
 *    controls to configure and communicate with the wificsi device(s).
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include "wifi_csi_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CSI_BUFF_LEN 536	/*assuming max subcarrier: 244 (1019 = 43 + 244*4), as per rtk pdf */
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* CONFIG_WIFI_CSI - Enables Wificsi driver support
 * CONFIG_DEBUG_WIFICSI - If enabled (with CONFIG_DEBUG_FEATURES and, optionally,
 *   CONFIG_DEBUG_VERBOSE), this will generate output that can be used to
 *   debug Wificsi drivers.
 */

/* IOCTL Commands ***********************************************************/
/* The Wificsi module uses a standard character driver framework.  However, a
 * lot of the Wificsi driver functionality is configured via a device control
 * interface, such as sampling rate, volume, data format, etc.
 * The Wificsi ioctl commands are listed below:
 *
 *
 * CSIIOC_SET_CONFIG - Set wificsi device config for the specified mode
 *
 *   ioctl argument:  Pointer to the csi_config_args_t structure which specifies
 *                    the config args.
 *
 * CSIIOC_REGISTER_CALLBACK - Register callback to receive wifi csi data ready event.
 *
 *   ioctl argument:  None
 *
 * CSIIOC_UNREGISTER_CALLBACK - Unregister callback to receive wifi csi data ready event
 *
 *   ioctl argument:  None
 *
 * CSIIOC_GET_MAC_ADDR - Get Wifi csi device MAC address
 *
 *   ioctl argument:  Pointer to csifw_mac_info structure.
 *
 * CSIIOC_REGISTERMQ - Register message queue to receive message from kernel.
 *
 *   ioctl argument:  Pointer to message queue descriptor structure.
 * 
 * CSIIOC_UNREGISTERMQ - Unregsiter message queue.
 *
 *   ioctl argument:  Pointer to message queue descriptor structure.
 * 
 * CSIIOC_GET_DATA - Get csi data.
 *
 *   ioctl argument:  Pointer to csi_driver_buffer_args_t structure.
 */

#define CSIIOC_PARAM_INIT            _CSIIOC(1)
#define CSIIOC_SET_CONFIG            _CSIIOC(2)
#define CSIIOC_START_CSI             _CSIIOC(3)
#define CSIIOC_STOP_CSI              _CSIIOC(4)
#define CSIIOC_GET_MAC_ADDR          _CSIIOC(5)
#define CSIIOC_GET_DATA              _CSIIOC(6)

/* CSI Operation Callback Reasons ***************************************************/

#define CSI_CALLBACK_ERROR           0x00
#define CSI_CALLBACK_DATA_READY      0x01

/* Wifi CSI Message Queue message name */

#define CSI_MQ_NAME                  "WIFI_CSI_DRIVER_Q"
#define CSI_MQ_MSG_COUNT 5

/* Standard Wifi CSI Message Queue message IDs */

#define CSI_MSG_ERROR                0
#define CSI_MSG_DATA_READY_CB        1
#define CSI_MSG_USER                 64

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef struct CSI_DRIVER_BUFFER_ARGS_T {
	size_t buflen;
	unsigned char *buffer;
}csi_driver_buffer_args_t;

/* Typedef for lower-level to upper-level callback for buffer dequeuing */

typedef CODE void (*wifi_csi_callback_t)(FAR void *priv, uint16_t reason, char *buff, uint32_t csi_data_len);


/* Structure defining the messages passed to a listening wificsi thread
 * for notifying errors and data ready event.
 */
struct wifi_csi_msg_s {
	uint16_t msgId;							/* Message ID */
	uint16_t data_len;						/* CSI Data Length (inclusive of header) */
};

struct wifi_csi_lowerhalf_s;

/* This structure is a set of callback functions used to call from the upper-
 * half, generic Wificsi driver into lower-half, platform-specific logic that
 * supports the low-level functionality.
 */
struct wifi_csi_ops_s {
	/* Lower-half logic may support platform-specific read commands */

	CODE int (*read)(FAR char *buffer, size_t buflen);

	/* Lower-half logic may support platform-specific ioctl commands */

	CODE int (*ioctl)(int cmd, unsigned long arg);

	/* Get mac address of the wificsi device */

	CODE int (*getmacaddr)(char *mac_addr);

	/* Get csi data */

	CODE int (*getcsidata)(unsigned char *buffer, size_t buflen);

};

/* This structure is the generic form of state structure used by lower half
 * Wificsi driver.  This state structure is passed to the Wificsi driver when the
 * driver is initialized.  Then, on subsequent callbacks into the lower half
 * Wificsi logic, this structure is provided so that the Wificsi logic can
 * maintain state information.
 *
 * Normally that Wificsi logic will have its own, custom state structure
 * that is simply cast to struct wifi_csi_lowerhalf_s.  In order to perform such
 * casts, the initial fields of the custom state structure match the initial
 * fields of the following generic Wificsi state structure.
 */

struct wifi_csi_lowerhalf_s {
	/* The first field of this state structure must be a pointer to the Wificsi
	 * callback structure:
	 */

	FAR const struct wifi_csi_ops_s *ops;

	/* The bind data to the upper-half driver used for callbacks of dequeuing
	 * buffer, reporting asynchronous event, reporting errors, etc.
	 */

	FAR wifi_csi_callback_t upper_cb;

	/* The private opaque pointer to be passed to upper-layer during callbacks */

	FAR void *priv;  /* contains upperhalf instance ptr: wifi_csi_upperhalf_s */

	/* The custom Wificsi device state structure may include additional fields
	 * after the pointer to the Wificsi callback structure.
	 */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * "Upper-Half" Wificsi Driver Interfaces
 ****************************************************************************/
/****************************************************************************
 * Name: wifi_csi_register
 *
 * Description:
 *   This function binds an instance of a "lower half" Wificsi driver with the
 *   "upper half" Wificsi device and registers that device so that can be used
 *   by application code.
 *
 *   When this function is called, the "lower half" driver should be in the
 *   reset state (as if the shutdown() method had already been called).
 *
 * Input parameters:
 *   name - The name of the wificsi device.  This name will be used to generate
 *     a full path to the driver in the format "/dev/wificsi/[name]" in the TizenRT
 *     filesystem (i.e. the path "/dev/wificsi" will be prepended to the supplied
 *     device name.  The recommended convention is to name Wificsi drivers
 *     based on the type of functionality they provide, such as "/dev/wificsi/pcm0",
 *     "/dev/wificsi/midi0", "/dev/wificsi/mp30, etc.
 *   dev - A pointer to an instance of lower half wificsi driver.  This instance
 *     is bound to the Wificsi driver and must persists as long as the driver
 *     persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int wifi_csi_register(FAR const char *name, FAR struct wifi_csi_lowerhalf_s *dev);

#ifdef __cplusplus
}
#endif

#endif  /* __INCLUDE_TINYARA_WIFI_CSI_H */

