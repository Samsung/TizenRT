/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_TEST_DRV_H
#define __INCLUDE_TINYARA_TEST_DRV_H

/* This file will be used to provide definitions to support
 * kernel test case framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>

#include <iotbus/iotbus_common.h>

#ifdef CONFIG_IOTDEV

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define IOTDEV_DRVPATH                       "/dev/iotdev"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

typedef iotbus_int_type_e id_evt_type;

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif


/****************************************************************************
 * Name: id_drv_register
 *
 * Description:
 *   This function creates a device node like "/dev/iotdev" which will be used
 *   by the iotbus to get events from peripheral
 *
 *
 ****************************************************************************/

void id_drv_register(void);

/****************************************************************************
 * Name: id_post_evt
 *
 * Description:
 *   This function creates a device node like "/dev/iotdev" which will be used
 *   by the iotbus to get events from peripheral
 *
 *
 ****************************************************************************/
int id_post_evt(id_evt_type evt);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_KERNEL_TEST_DRV */
#endif							/* __INCLUDE_TINYARA_TEST_FW_H */
