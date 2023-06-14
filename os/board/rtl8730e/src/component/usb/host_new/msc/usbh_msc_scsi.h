/**
  ******************************************************************************
  * @file    usbh_msc_scsi.h
  * @brief   Header file for usbh_msc_scsi.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef USBH_MSC_SCSI_H
#define USBH_MSC_SCSI_H

/* Includes ------------------------------------------------------------------*/

#include "usbh_msc.h"

/* Exported defines ----------------------------------------------------------*/

/* SCSI Command */
#define USBH_MSC_TEST_UNIT_READY            0x00U
#define USBH_MSC_READ_CAPACITY10            0x25U
#define USBH_MSC_READ10                     0x28U
#define USBH_MSC_WRITE10                    0x2AU
#define USBH_MSC_REQUEST_SENSE              0x03U
#define USBH_MSC_INQUIRY                    0x12U

/* SCSI Command Data Length */
#define USBH_MSC_TEST_UNIT_READY_LEN        0U
#define USBH_MSC_READ_CAPACITY10_LEN             8U
#define USBH_MSC_INQUIRY_LEN                    36U
#define USBH_MSC_REQUEST_SENSE_LEN              14U

#define CBW_CB_LENGTH                       16U
#define CBW_LENGTH                          10U

/* SCSI Sense Code */
#define SCSI_SENSE_KEY_NO_SENSE                          0x00U
#define SCSI_SENSE_KEY_RECOVERED_ERROR                   0x01U
#define SCSI_SENSE_KEY_NOT_READY                         0x02U
#define SCSI_SENSE_KEY_MEDIUM_ERROR                      0x03U
#define SCSI_SENSE_KEY_HARDWARE_ERROR                    0x04U
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                   0x05U
#define SCSI_SENSE_KEY_UNIT_ATTENTION                    0x06U
#define SCSI_SENSE_KEY_DATA_PROTECT                      0x07U
#define SCSI_SENSE_KEY_BLANK_CHECK                       0x08U
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                   0x09U
#define SCSI_SENSE_KEY_COPY_ABORTED                      0x0AU
#define SCSI_SENSE_KEY_ABORTED_COMMAND                   0x0BU
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                   0x0DU
#define SCSI_SENSE_KEY_MISCOMPARE                        0x0EU

/* SCSI ASC Code */
#define SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION         0x00
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY                  0x04
#define SCSI_ASC_INVALID_FIELD_IN_CDB                    0x24
#define SCSI_ASC_WRITE_PROTECTED                         0x27
#define SCSI_ASC_FORMAT_ERROR                            0x31
#define SCSI_ASC_INVALID_COMMAND_OPERATION_CODE          0x20
#define SCSI_ASC_NOT_READY_TO_READY_CHANGE               0x28
#define SCSI_ASC_MEDIUM_NOT_PRESENT                      0x3A

/* SCSI ASCQ */
#define SCSI_ASCQ_FORMAT_COMMAND_FAILED                  0x01
#define SCSI_ASCQ_INITIALIZING_COMMAND_REQUIRED          0x02
#define SCSI_ASCQ_OPERATION_IN_PROGRESS                  0x07

/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_scsi_test_unit_ready(usbh_msc_host_t *msc, u8 lun);
u8 usbh_scsi_read_capacity(usbh_msc_host_t *msc, u8 lun, usbh_scsi_capacity_t *capacity);
u8 usbh_scsi_inquiry(usbh_msc_host_t *msc, u8 lun, usbh_scsi_inquiry_t *inquiry);
u8 usbh_scsi_request_sense(usbh_msc_host_t *msc, u8 lun, usbh_scsi_sense_t *sense_data);
u8 usbh_scsi_write(usbh_msc_host_t *msc, u8 lun, u32 address, u8 *pbuf, u32 length);
u8 usbh_scsi_read(usbh_msc_host_t *msc, u8 lun, u32 address, u8 *pbuf, u32 length);

#endif  /* USBH_MSC_SCSI_H */

