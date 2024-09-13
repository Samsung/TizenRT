/**
  ******************************************************************************
  * @file    usbd_hal.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbd_hal.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_HAL_H
#define USBD_HAL_H

/* Includes ------------------------------------------------------------------*/

#include "usb_hal.h"
#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

/*
	The device wakes up the host remotely:
	While the device remote wakeup the host,the WkUpInt interrupt will not be triggered
	Therefore, enable the sof interrupt to check that the host has indeed recovered the device
*/
#define USBD_SWITCH_TO_ACTIVE_BY_SOF_INTR      0U

/* USB ISR thread stack size */
#define USBD_ISR_THREAD_STACK_SIZE             4096U

/* USB EPMIS cnt */
#define USBD_EPMIS_CNT                         8U

/* USB setup packet buffer, 3 back-to-back setup packets */
#define USBD_SETUP_PACKET_BUF_LEN              (3U * 8U)
/* USB setup packet cnt, temporaly only for one packet */
#define USBD_SETUP_PACKET_CNT              (1U)

/* Enable in token prediction workaround */
#define IN_TOKEN_PREDICT_WA_EN                 1U

/* USB transfer error detect, set it to 1 only if IN_TOKEN_PREDICT_WA_EN set to 1 */
#define USBD_XFER_ERROR_DETECT_EN              1U
#define USBD_NPTX_DEF_ERR_CNT                  0xFFFFFFFFU

/* USB Core Turnaround Timeout Value */
#define USBD_HS_TRDT_VALUE                     5U
#define USBD_FS_TRDT_VALUE                     9U

/* USB Core MPS */
#define USB_OTG_HS_MAX_PACKET_SIZE             512U
#define USB_OTG_FS_MAX_PACKET_SIZE             64U
#define USB_OTG_MAX_EP0_SIZE                   64U

/* USB Core PHY Frequency */
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     (0U << 1)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     (1U << 1)
#define DSTS_ENUMSPD_LS_PHY_6MHZ               (2U << 1)
#define DSTS_ENUMSPD_FS_PHY_48MHZ              (3U << 1)

/* USB Core Frame Interval */
#define DCFG_FRAME_INTERVAL_80                 0U
#define DCFG_FRAME_INTERVAL_85                 1U
#define DCFG_FRAME_INTERVAL_90                 2U
#define DCFG_FRAME_INTERVAL_95                 3U

/* USB Core EP0 MPS */
#define DEP0CTL_MPS_64                         0U
#define DEP0CTL_MPS_32                         1U
#define DEP0CTL_MPS_16                         2U
#define DEP0CTL_MPS_8                          3U

/* USB EP Address Mask */
#define EP_ADDR_MSK                            0xFU

/* USB STS Defines */
#define STS_GOUT_NAK                           1U
#define STS_DATA_UPDT                          2U
#define STS_XFER_COMP                          3U
#define STS_SETUP_COMP                         4U
#define STS_SETUP_UPDT                         6U

/* Exported types ------------------------------------------------------------*/

/* USB PCD endpoint structure */
typedef struct {
	u32  dma_addr;             /*!< 32 bits aligned transfer buffer address */
	u32  xfer_len;             /*!< Current total transfer length */
	u32  xfer_count;           /*!< Partial transfer length in case of multi packet transfer,
                                    The length of data that has been written to the FIFO,
                                    including no tx success and tx success */
#ifdef USBD_XFER_ERROR_DETECT_EN
	u32  nptx_err_cnt;         /*!< transfer error count */
#endif
	u32  max_packet_len;       /*!< Endpoint Max packet size, 0~64KB */
	u8  *xfer_buff;            /*!< Pointer to transfer buffer */
	u16  tx_fifo_num;          /*!< Transmission FIFO number, 0~max tx fifo num */
	u8   addr;                 /*!< Endpoint address */
	u8   type;                 /*!< Endpoint type, USB_EP_Type_XXX */
	u8   even_odd_frame : 1;       /*!< IFrame parity, 0~1 */
	u8   is_initialized : 1;       /*!< Flag indicating whether the endpoint is initialized */
	u8   is_stall : 1;             /*!< Endpoint stall condition */
	u8   is_zlp : 1;               /*!< ZLP packet */
	u8   is_ptx : 1;               /*!< Periodic TX Transfer 0~1 */
} usbd_pcd_ep_t;

/* USB device LPM suspend state */
typedef enum {
	LPM_L0 = 0x00, /* on */
	LPM_L1 = 0x01, /* LPM L1 sleep */
	LPM_L2 = 0x02, /* suspend */
	LPM_L3 = 0x03, /* off */
} usbd_lpm_state_t;

/* PCD state */
typedef enum {
	HAL_PCD_STATE_RESET   = 0x00,
	HAL_PCD_STATE_READY   = 0x01,
	HAL_PCD_STATE_ERROR   = 0x02,
	HAL_PCD_STATE_BUSY    = 0x03,
	HAL_PCD_STATE_TIMEOUT = 0x04,
	HAL_PCD_STATE_STOP    = 0x05
} usbd_pcd_state_t;

/* PCD handle structure */
typedef struct {
	/*!< PCD required parameters  */
	usbd_config_t config;
	/*!< IN endpoint parameters */
	usbd_pcd_ep_t in_ep[USB_MAX_ENDPOINTS];
	/*!< OUT endpoint parameters */
	usbd_pcd_ep_t out_ep[USB_MAX_ENDPOINTS];
	/*!< PCD communication state */
	__IO usbd_pcd_state_t pcd_state;
	/*!< LPM State */
	usbd_lpm_state_t lpm_state;
	_sema isr_sema;
	struct task_struct isr_task;
	/*!< Pointer to upper stack Handler */
	usb_dev_t *dev;
	/*!< NP TX ep mispatch count */
	u32 nptx_epmis_cnt;
	/*!< Setup packet buffer */
	u8 *setup;
	/*!< PCD lock */
	void *lock;
	/*!< NextEp sequence, including EP0: nextep_seq[] = EP if non-periodic and active, 0xff otherwise */
	u8 nextep_seq[USB_MAX_ENDPOINTS];
	/*!< IN EP sequence from In token queue */
	u8 in_ep_sequence[USB_MAX_ENDPOINTS];
	/*!< Start predict NextEP based on Learning Queue if equal 1, also used as counter of disabled NP IN EP's */
	u8 start_predict;
	/*!< Index of fisrt EP in nextep_seq array which should be re-enabled */
	u8 first_in_nextep_seq;
	/*!< USB Address */
	__IO u8 address;
	/*!< PHY initialized */
	u8 phy_initialized : 1;
	/*!< ISR initialized */
	u8 isr_initialized : 1;
	u8 ded_tx_fifo_en : 1;
} usbd_pcd_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_hal_device_init(usbd_pcd_t *pcd);
u8 usbd_hal_set_turnaround_time(usbd_pcd_t *pcd);
u8 usbd_hal_set_device_speed(usbd_pcd_t *pcd, u8 speed);
u8 usbd_hal_ep_activate(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_deactivate(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_start_transfer(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep0_start_transfer(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_set_stall(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_ep_clear_stall(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_set_device_address(usbd_pcd_t *pcd, u8 address);
u8 usbd_hal_connect(usbd_pcd_t *pcd);
u8 usbd_hal_disconnect(usbd_pcd_t *pcd);
u8 usbd_hal_device_stop(usbd_pcd_t *pcd);
u8 usbd_hal_config_dfifo(usbd_pcd_t *pcd);
u8 usbd_hal_ep0_setup_activate(usbd_pcd_t *pcd);
u8 usbd_hal_ep0_out_start(usbd_pcd_t *pcd);
u8 usbd_hal_reset_in_token_queue(usbd_pcd_t *pcd);
u8 usbd_hal_predict_next_ep(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u8 usbd_hal_get_device_speed(usbd_pcd_t *pcd);
u32 usbd_hal_get_tx_fifo_num(usbd_pcd_t *pcd, usbd_pcd_ep_t *ep);
u32 usbd_hal_read_all_out_ep_interrupts(usbd_pcd_t *pcd);
u32 usbd_hal_read_out_ep_interrupts(usbd_pcd_t *pcd, u8 ep_num);
u32 usbd_hal_read_all_in_ep_interrupts(usbd_pcd_t *pcd);
u32 usbd_hal_read_in_ep_interrupts(usbd_pcd_t *pcd, u8 ep_num);
u8 usbd_hal_test_mode(u8 mode);
u8 usbd_hal_get_bus_status(usbd_pcd_t *pcd, u32 *bus_status);
u8 usbd_hal_wake_host(usbd_pcd_t *pcd);
void usbd_hal_dump_registers(void);

#endif /* USBD_HAL_H */

