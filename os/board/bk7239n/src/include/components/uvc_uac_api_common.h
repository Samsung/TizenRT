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
 * @brief     Registering the callback function for UVC and UAC fusion devices
 *
 * 
 * Register the corresponding callback function based on the structure, 
 * and be sure to fill in the specific type to facilitate the management 
 * of the callback function
 *  parameter: uvc_uac_all_cb 
 *              E_USB_DEVICE_T dev;
 *              FUNCPTR uvc_uac_connect_cb;
 *              FUNCPTR uvc_uac_disconnect_cb;
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_uac_register_callback(bk_uvc_uac_app_callback *uvc_uac_all_cb);

/**
 * @brief     UVC connect the callback function registration function registration
 *
 * This API connect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is connected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uvc_register_connect_callback(void *param);

/**
 * @brief     UVC disconnect the callback function registration function registration
 *
 * This API disconnect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is disconnected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uvc_register_disconnect_callback(void *param);

/**
 * @brief     Obtain UVC relate parameters
 *
 * 
 * Including basic parameters,VID PID FORMAT FRAME EndPoint
 *
 *  parameter: uvc_device_info 
 *               uint16_t vendor_id;
 *               uint16_t product_id;
 *               bk_uvc_format_index format_index;
 *               bk_uvc_all_frame all_frame;
 *               uint32_t endpoints_num;
 *               struct s_bk_usb_endpoint_descriptor *ep_desc;
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uvc_get_param(bk_uvc_device_brief_info_t *uvc_device_info);

/**
 * @brief     Configure the basic parameters for starting UVC
 *
 * 
 * Execute the command according to the configured parameters, 
 * please fill in the parameters correctly. Prevent parameter 
 * requirements that cannot be executed
 *
 *  parameter: uvc_config 
 *               uint16_t vendor_id;
 *               uint16_t product_id;
 *               uint16_t width;
 *               uint16_t height;
 *               uint32_t fps;
 *               uint16_t frame_index;
 *               uint16_t format_index;
 *               struct s_bk_usb_endpoint_descriptor *ep_desc;
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uvc_set_param(bk_uvc_config_t *uvc_config);

/**
 * @brief     Start UVC stream transmission
 *
 * Execute the command according to the configured parameters, 
 * please fill in the parameters correctly. 
 * Prevent parameter requirements that cannot be executed
 *
 * This API means that the data needs to transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uvc_register_transfer_buffer_ops(void *ops);

/**
 * @brief     Start UVC stream transmission
 *
 * USB video camera data transfer starts:
 *   - configure the required configuration parameters
 *   - Send video stream data transfer instructions
 *
 * This API means that the data starts to transfer and needs to
 * transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_start(void);

bk_err_t bk_uvc_dual_start(void);
/**
 * @brief     Stop UVC stream transmission
 *
 * USB video camera data transfer stops:
 *   - Send the video stream data stop transmission command
 *
 *
 * There will be no more data flow after execution.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uvc_stop(void);

bk_err_t bk_uvc_dual_stop(void);

/**
 * @brief     Check whether the attribute configuration is supported
 *
 * After connecting the UVC device, determine whether the attribute operation
 * is supported by calling this API:
 *   - The Parameters has enumeration variable A planning, please fill in the
 *     attribute parameter correctly.
 *   - attribute:
 *         Processing Unit Control Selectors;
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uvc_check_support_attribute(uint32_t attribute);

/**
 * @brief     Set camera attributes values
 *
 * This API set/get camera attributes values:
 *   - Three parameters ops/attributes/values.
 *   - ops:
 *        E_USB_ATTRIBUTE_OP
 *   - attributes:
 *        Processing Unit Control Selectors
 *   - param:
 *        Incoming parameter pointer
 *
 *    example: Reference cli_usb.c
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uvc_attribute_op(E_USB_ATTRIBUTE_OP ops, uint32_t attribute, uint32_t *param);

/**
 * @brief     UAC connect the callback function registration function registration
 *
 * This API disconnect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is connected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_register_connect_callback(void *param);

/**
 * @brief     UAC disconnect the callback function registration function registration
 *
 * This API disconnect the callback:
 *   - parameter is the registered callback function.
 *   - The callback function needs to be defined.
 *
 *
 *  Notify the registered callback function when the device is disconnected.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_register_disconnect_callback(void *param);

/**
 * @brief     Obtain UAC relate parameters
 *
 * 
 * Including basic parameters,VID PID mic_samples_frequence spk_samples_frequence
 *
 *  parameter: uac_device_info 
 *               uint16_t vendor_id;
 *               uint16_t product_id;
 *               uint16_t mic_format_tag;
 *               uint8_t mic_samples_frequence_num;
 *               uint8_t *mic_samples_frequence;
 *               struct audio_ep_descriptor *mic_ep_desc;
 *               uint16_t spk_format_tag;
 *               uint8_t spk_samples_frequence_num;
 *               uint8_t *spk_samples_frequence;
 *               struct audio_ep_descriptor *spk_ep_desc;
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_get_param(bk_uac_device_brief_info_t *uac_device_info);

/**
 * @brief     Configure the basic parameters for starting UAC
 *
 * 
 * Execute the command according to the configured parameters, 
 * please fill in the parameters correctly. Prevent parameter 
 * requirements that cannot be executed
 *
 *  parameter: uac_config 
 *               uint16_t vendor_id;
 *               uint16_t product_id;
 *               uint16_t mic_format_tag;
 *               uint32_t mic_samples_frequence;
 *               struct audio_ep_descriptor *mic_ep_desc;
 *               uint16_t spk_format_tag;
 *               uint32_t spk_samples_frequence;
 *               struct audio_ep_descriptor *spk_ep_desc;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_set_param(bk_uac_config_t *uac_config);

/**
 * @brief     Configure the basic parameters for starting Mic
 *
 * 
 * Execute the command according to the configured parameters, 
 * please fill in the parameters correctly. Prevent parameter 
 * requirements that cannot be executed
 *
 *  parameter: uac_mic_config 
 *               uint16_t mic_format_tag;
 *               uint32_t mic_samples_frequence;
 *               struct audio_ep_descriptor *mic_ep_desc;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_set_mic_param(bk_uac_mic_config_t *uac_mic_config);

/**
 * @brief     Configure the basic parameters for starting Speaker
 *
 * 
 * Execute the command according to the configured parameters, 
 * please fill in the parameters correctly. Prevent parameter 
 * requirements that cannot be executed
 *
 *  parameter: uac_spk_config 
 *               uint16_t spk_format_tag;
 *               uint32_t spk_samples_frequence;
 *               struct audio_ep_descriptor *spk_ep_desc;
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_set_spk_param(bk_uac_spk_config_t *uac_spk_config);

/**
 * @brief     Start UVC stream transmission
 *
 * Execute the command according to the configured parameters, 
 * please fill in the parameters correctly. 
 * Prevent parameter requirements that cannot be executed
 *
 * This API means that the data needs to transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_uac_register_transfer_buffer_ops(void *ops);

/**
 * @brief     Start UAC mic stream transmission
 *
 * USB audio data transfer starts:
 *   - configure the required configuration parameters
 *   - Send audio stream data transfer instructions
 *
 * This API means that the data starts to transfer and needs to
 * transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_start_mic(void);

/**
 * @brief     Stop UAC mic stream transmission
 *
 * USB audio data transfer stops:
 *   - Send the video stream data stop transmission command
 *
 *
 * There will be no more data flow after execution.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_stop_mic(void);

/**
 * @brief     Start UAC speaker stream transmission
 *
 * USB audio data transfer starts:
 *   - configure the required configuration parameters
 *   - Send audio stream data transfer instructions
 *
 * This API means that the data starts to transfer and needs to
 * transfer and needs to be copied.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_start_speaker(void);

/**
 * @brief     Stop UAC speaker stream transmission
 *
 * USB audio data transfer stops:
 *   - Send the video stream data stop transmission command
 *
 *
 * There will be no more data flow after execution.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uac_stop_speaker(void);

/**
 * @brief     Check whether the attribute configuration is supported
 *
 * After connecting the UAC device, determine whether the attribute operation
 * is supported by calling this API:
 *   - The Parameters has enumeration variable A planning, please fill in the
 *     attribute parameter correctly.
 *   - attribute:
 *         Feature Unit Control Bits;
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uac_check_support_attribute(uint32_t attribute);

/**
 * @brief     Set camera attributes values
 *
 * This API set/get camera attributes values:
 *   - Three parameters ops/attributes/values.
 *   - ops:
 *        E_USB_ATTRIBUTE_OP
 *   - attributes:
 *        Feature Unit Control Bits
 *   - param:
 *        Incoming parameter pointer
 *
 *    example: Reference cli_usb.c
 *
 *   - Valid values needs to be viewed in camera.
 *
 *  Choose to call this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_usb_uac_attribute_op(E_USB_ATTRIBUTE_OP ops, uint32_t attribute, uint32_t *param);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

