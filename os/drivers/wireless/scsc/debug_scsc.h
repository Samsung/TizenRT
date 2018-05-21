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

#ifndef __DEBUG_SCSC_H__
#define __DEBUG_SCSC_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/time.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/clock.h>
#include <tinyara/fs/fs.h>
#include <tinyara/spi/spi.h>
#include <arpa/inet.h>
#include "dev.h"

/* Logging modules
 * =======================
 */
#define SLSI_INIT_DEINIT     (0)
#define SLSI_NETDEV          (1)
#define SLSI_MLME            (2)
#define SLSI_TX              (3)
#define SLSI_RX              (4)
#define SLSI_UDI             (5)
#define SLSI_WIFI_FCQ        (6)
#define SLSI_HIP_INIT_DEINIT (7)
#define SLSI_FUNC_TRACE      (8)
#define SLSI_FW_TEST         (9)
#define SLSI_RX_BA           (10)
#define SLSI_T20_80211       (11)

/**
 * debug logging functions
 * =======================
 */
#define SLSI_EWI(output, sdev, label, fmt, arg ...)     output(label ": " fmt, ## arg)
#define SLSI_EWI_NET(output, ndev, label, fmt, arg ...) output(label ": " fmt, ## arg)
#define SLSI_EWI_NODEV(output, label, fmt, arg ...)     output(label ": " fmt, ## arg)

#ifdef CONFIG_DEBUG_WLAN_DRIVER_ERROR

#define SLSI_ERR(sdev, fmt, arg ...)                        SLSI_EWI(pr_err, NULL, "E", fmt, ## arg)
#define SLSI_NET_ERR(ndev, fmt, arg ...)                    SLSI_EWI_NET(pr_err, NULL, "E", fmt, ## arg)
#define SLSI_ERR_NODEV(fmt, arg ...)                        SLSI_EWI_NODEV(pr_err, "E", fmt, ## arg)

#define SLSI_WARN(sdev, fmt, arg ...)                       SLSI_EWI(pr_warn, NULL, "W", fmt, ## arg)
#define SLSI_NET_WARN(ndev, fmt, arg ...)                   SLSI_EWI_NET(pr_warn, NULL, "W", fmt, ## arg)
#define SLSI_WARN_NODEV(fmt, arg ...)                       SLSI_EWI_NODEV(pr_warn, "W", fmt, ## arg)

#else

#define SLSI_ERR(sdev, fmt, arg ...)                        do {} while (0)
#define SLSI_NET_ERR(ndev, fmt, arg ...)                    do {} while (0)
#define SLSI_ERR_NODEV(fmt, arg ...)                        do {} while (0)

#define SLSI_WARN(sdev, fmt, arg ...)                       do {} while (0)
#define SLSI_NET_WARN(ndev, fmt, arg ...)                   do {} while (0)
#define SLSI_WARN_NODEV(fmt, arg ...)                       do {} while (0)

#endif							/* CONFIG_DEBUG_WLAN_DRIVER_ERROR */

#ifdef CONFIG_DEBUG_WLAN_DRIVER_INFO

#define SLSI_INFO(sdev, fmt, arg ...)                   SLSI_EWI(pr_info, NULL, "I", fmt, ## arg)
#define SLSI_NET_INFO(ndev, fmt, arg ...)               SLSI_EWI_NET(pr_info, NULL, "I", fmt, ## arg)
#define SLSI_INFO_NODEV(fmt, arg ...)                   SLSI_EWI_NODEV(pr_info, "I", fmt, ## arg)

#define SLSI_DBG1(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_DBG2(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_DBG3(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_DBG4(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)

#define SLSI_NET_DBG1(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_NET_DBG2(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_NET_DBG3(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_NET_DBG4(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)

#define SLSI_DBG1_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_DBG2_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_DBG3_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_DBG4_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)

#elif defined(CONFIG_DEBUG_WLAN_DRIVER_MORE)

#define SLSI_INFO(sdev, fmt, arg ...)                   SLSI_EWI(pr_info, NULL, "I", fmt, ## arg)
#define SLSI_NET_INFO(ndev, fmt, arg ...)               SLSI_EWI_NET(pr_info, NULL, "I", fmt, ## arg)
#define SLSI_INFO_NODEV(fmt, arg ...)                   SLSI_EWI(pr_info, NULL, "I", fmt, ## arg)

#define SLSI_DBG1(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_DBG2(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_DBG3(sdev, filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG4(sdev, filter, fmt, arg ...)           do {} while (0)

#define SLSI_NET_DBG1(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_NET_DBG2(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_NET_DBG3(dev, filter, fmt, arg ...)        do {} while (0)
#define SLSI_NET_DBG4(dev, filter, fmt, arg ...)        do {} while (0)

#define SLSI_DBG1_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_DBG2_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "V", fmt, ## arg)
#define SLSI_DBG3_NODEV(filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG4_NODEV(filter, fmt, arg ...)           do {} while (0)

#elif defined(CONFIG_DEBUG_WLAN_DRIVER_DEBUG)

#define SLSI_INFO(sdev, fmt, arg ...)                   SLSI_EWI(pr_info, NULL, "I", fmt, ## arg)
#define SLSI_NET_INFO(ndev, fmt, arg ...)               SLSI_EWI_NET(pr_info, NULL, "I", fmt, ## arg)
#define SLSI_INFO_NODEV(fmt, arg ...)                   SLSI_EWI(pr_info, NULL, "I", fmt, ## arg)

#define SLSI_DBG1(sdev, filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_DBG2(sdev, filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG3(sdev, filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG4(sdev, filter, fmt, arg ...)           do {} while (0)

#define SLSI_NET_DBG1(dev, filter, fmt, arg ...)        SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_NET_DBG2(dev, filter, fmt, arg ...)        do {} while (0)
#define SLSI_NET_DBG3(dev, filter, fmt, arg ...)        do {} while (0)
#define SLSI_NET_DBG4(dev, filter, fmt, arg ...)        do {} while (0)

#define SLSI_DBG1_NODEV(filter, fmt, arg ...)           SLSI_EWI(pr_info, NULL, "D", fmt, ## arg)
#define SLSI_DBG2_NODEV(filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG3_NODEV(filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG4_NODEV(filter, fmt, arg ...)           do {} while (0)

#else

#define SLSI_INFO(sdev, fmt, arg ...)                   do {} while (0)
#define SLSI_NET_INFO(ndev, fmt, arg ...)               do {} while (0)
#define SLSI_INFO_NODEV(fmt, arg ...)                   do {} while (0)

#define SLSI_DBG1(sdev, filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG2(sdev, filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG3(sdev, filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG4(sdev, filter, fmt, arg ...)           do {} while (0)

#define SLSI_NET_DBG1(dev, filter, fmt, arg ...)        do {} while (0)
#define SLSI_NET_DBG2(dev, filter, fmt, arg ...)        do {} while (0)
#define SLSI_NET_DBG3(dev, filter, fmt, arg ...)        do {} while (0)
#define SLSI_NET_DBG4(dev, filter, fmt, arg ...)        do {} while (0)

#define SLSI_DBG1_NODEV(filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG2_NODEV(filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG3_NODEV(filter, fmt, arg ...)           do {} while (0)
#define SLSI_DBG4_NODEV(filter, fmt, arg ...)           do {} while (0)
#endif							/* CONFIG_SCSC_DEFAULT_T20_WLAN_DEBUG */

#endif
