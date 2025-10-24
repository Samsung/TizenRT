// Copyright 2020-2021 Beken
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

#include "usb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief     uvc ota demo init
 *
 * This API initializes UVC_OTA_DEMO to apply for the required resources
 *
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_demo_init(void);

/**
 * @brief     uvc ota demo deinit
 *
 * This API release uninstalling UVC_OTA_DEMO to apply for the required resources
 *
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_demo_deinit(void);

/**
 * @brief     Check whether the upgrade is required
 *
 * The passed parameters are compared with the original firmware, and the returned values
 * determine whether the firmware needs to be upgraded.
 *
 * @param:
 *        szFirmwareVersion:Enter the version number of the new firmware
 *        aucFirmwareData:Keep parameters for subsequent development
 *        iFirmwareLen:Length of firmware to be upgraded. The minimum unit is bytes
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_demo_upgrade_check(char *szFirmwareVersion, char *aucFirmwareData, int iFirmwareLen);

/**
 * @brief     Download firmware from the network
 *
 * Fill in the web address for network download, you can control the download speed and 
 * transfer to the usb device speed.
 *
 * @param:
 *        uri:Uniform Resource Idntifier
 *        len:The string length of the URI
 *        transfer_wait_ms:The transmission interval is used to control the transmission speed
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_http_download(const char *uri, uint32_t len, uint8_t transfer_wait_ms);

/**
 * @brief     Acquired transmission state
 *
 * Get the percentage of current downloads and whether to transfer 
 *
 * @param:
 *        http_percentage:Percentage of Internet downloads
 *        usb_transfer_sta:Whether the transmission to the device is coplete
 *
 * @PS:Check whether the device is upgraded only when both parts of the transmission are complete
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_http_to_device_status(float *http_percentage, bool *usb_transfer_sta);

/**
 * @brief     Check whether the device has received the packets successfully
 *
 * After the transmission is complete, send a message to the device to check whether the transmission is
 * complete. Otherwise it may cause
 *
 * @param:
 *        result: 1:ok  0:fail
 *
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_demo_transmit_result(uint32_t *result);

/**
 * @brief     Get the status of the upgrade
 *
 * The upgrade status is obtained after the transmission is complete
 *
 * @param:
 *        piData: Percentage of the upgrade process after receiving data
 *
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_ota_demo_get_upgrade_persent(int *piData);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

