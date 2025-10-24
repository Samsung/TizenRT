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

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_types.h"
#include "uvc_uac_api_common.h"

/*******************************************************************************
*                      Function Declarations
*******************************************************************************/

/**
 * @brief     USB Driver initialization
 *
 * Register the adaptation layer and pass the function pointers used by 
 * the closed source library. Register the current USB Class type to be 
 * supported, and it can be uninstalled and reinitialized during execution.
 * But be sure to do so before calling other interfaces.
 *
 *   - Configure Interrupt handling function registration
 *   - Configure Class Driver Registration
 *
 * This API should be called before any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_driver_init(void);

/**
 * @brief     USB Driver uninstallation
 *
 * Turn off the USB main interrupt switch
 * Unregister Total Interrupt Handling Function
 * Unregistered Class Driver
 *
 * This API should be called after any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_driver_deinit(void);

/**
 * @brief     Pin controlled USB Vbus
 *
 * This API controls VBUS for USB:
 *   - Determine whether to output voltage through the 
 *    high and low levels of a GPIO output
 *  parameter: gpio_id 
 *               Select the corresponding pin number
 *             ops
 *               Choose to power on or off
 *
 *   - Configure whether USB is power on
 *
 * When the fusion device is working, if there is a device voting for use,
 * it cannot be directly turned off.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_power_ops(uint32_t gpio_id, bool ops);

/**
 * @brief     open the USB
 *
 * This API configure the resoure common to USB:
 *   - Select the identity as host or slave
 *  parameter: usb_mode
 *            USB_HOST_MODE   = 0
 *            USB_DEVICE_MODE = 1
 *
 *   - Configure USB common clock
 *   - Configure USB common registers
 *
 * This API should be called before any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_open (uint32_t usb_mode);

/**
 * @brief     close the USB
 *
 * This API release USB resources:
 *   - Restore register
 *   - Turn off the power and clock
 *
 * This API should be called after any other USB APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_close (void);

/**
 * @brief     Initializes the control transfer request resource
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
*/
bk_err_t bk_usb_control_transfer_init(void);

/**
 * @brief     Unmount control transfer to release resource
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
*/
bk_err_t bk_usb_control_transfer_deinit(void);

/**
 * @brief     Control command and data interface fucntion to be passed
 *
 * This API is used to send commands and transfer data:
 *   - Two parameters pSetup/buffer_info.
 *   - s_usb_device_request:
 *        The command format is specified by the USB protocol
 *   - s_usb_transfer_buffer_info:
 *        Buffer informataion for sending and receiving data.
 *
 *    example: Reference UVC_OTA demo
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_usb_control_transfer(s_usb_device_request *pSetup, s_usb_transfer_buffer_info *buffer_info);

/**
 * @brief     A voting function is currently in use
 *
 * This API requires two parameters to be filled in for voting to prevent
 * it from being closed
 *  parameter: use_or_no
 *               TRUE USBING
 *               FALSE NOT used
 *            dev
 *               USB_UVC_DEVICE
 *               USB_UAC_MIC_DEVICE
 *               USB_UAC_SPEAKER_DEVICE
 *               USB_MSD_DEVICE
 *               USB_OTA_DEVICE
 *               USB_USER_CHECK_DEVICE
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
void bk_usb_device_set_using_status(bool use_or_no, E_USB_DEVICE_T dev);

/**
 * @brief     Obtain whether there is currently a device connection
 *
 *
 * @return
 *    - TRUE: connect
 *    - FALSE: disconnect
*/
bool bk_usb_get_device_connect_status(void);

/**
 * @brief     Check if the corresponding device class is supported
 *
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
*/
bk_err_t bk_usb_check_device_supported(E_USB_DEVICE_T usb_dev);

/**
 * @brief     Obtain device descriptor
 *
 *  dev_descriptor:
 *   uint8_t bLength;            Descriptor size in bytes = 18
 *   uint8_t bDescriptorType;    DEVICE descriptor type = 1
 *   uint16_t bcdUSB;            USB spec in BCD, e.g. 0x0200
 *   uint8_t bDeviceClass;       Class code, if 0 see interface
 *   uint8_t bDeviceSubClass;    Sub-Class code, 0 if class = 0
 *   uint8_t bDeviceProtocol;    Protocol, if 0 see interface
 *   uint8_t bMaxPacketSize0;    Endpoint 0 max. size
 *   uint16_t idVendor;          Vendor ID per USB-IF
 *   uint16_t idProduct;         Product ID per manufacture
 *   uint16_t bcdDevice;         Device release # in BCD
 *   uint8_t iManufacturer;      Index to manufacturer string
 *   uint8_t iProduct;           Index to product string
 *   uint8_t iSerialNumber;      Index to serial number string
 *   uint8_t bNumConfigurations; Number of possible configurations
 *
*/
bk_err_t bk_usb_get_device_descriptor(struct usb_device_descriptor_t *dev_descriptor);

/**
 * @brief     Initialize the USB master mailbox software
 *
 *
*/
void bk_usb_mailbox_sw_master_init(void);

/**
 * @brief     Initialize the USB slave mailbox software
 *
 *
*/
void bk_usb_mailbox_sw_slave_init(void);

/**
 * @brief     Uninstall the USB master mailbox software
 *
 *
*/
void bk_usb_mailbox_sw_master_deinit(void);

/**
 * @brief     Uninstall the USB slave mailbox software
 *
 *
*/
void bk_usb_mailbox_sw_slave_deinit(void);

#ifdef __cplusplus
}
#endif