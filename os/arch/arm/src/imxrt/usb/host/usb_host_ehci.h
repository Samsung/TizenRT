/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_HOST_CONTROLLER_EHCI_H_
#define _USB_HOST_CONTROLLER_EHCI_H_

#include "usb_host_config.h"

#include <stdint.h>
#include "usb_host.h"

/*!
 * @addtogroup USB_Peripheral_Access_Layer USB Peripheral Access Layer
 * @{
 */

/** USB - Register Layout Typedef */
typedef struct {

	volatile const uint32_t ID;					/**< Identification register, offset: 0x0 */

	volatile const uint32_t HWGENERAL;				/**< Hardware General, offset: 0x4 */

	volatile const uint32_t HWHOST;					/**< Host Hardware Parameters, offset: 0x8 */

	volatile const uint32_t HWDEVICE;				/**< Device Hardware Parameters, offset: 0xC */

	volatile const uint32_t HWTXBUF;				/**< TX Buffer Hardware Parameters, offset: 0x10 */

	volatile const uint32_t HWRXBUF;				/**< RX Buffer Hardware Parameters, offset: 0x14 */

	uint8_t RESERVED_0[104];

	volatile uint32_t GPTIMER0LD;					/**< General Purpose Timer #0 Load, offset: 0x80 */

	volatile uint32_t GPTIMER0CTRL;					/**< General Purpose Timer #0 Controller, offset: 0x84 */

	volatile uint32_t GPTIMER1LD;					/**< General Purpose Timer #1 Load, offset: 0x88 */

	volatile uint32_t GPTIMER1CTRL;					/**< General Purpose Timer #1 Controller, offset: 0x8C */

	volatile uint32_t SBUSCFG;					/**< System Bus Config, offset: 0x90 */

	uint8_t RESERVED_1[108];

	volatile const uint8_t CAPLENGTH;				/**< Capability Registers Length, offset: 0x100 */

	uint8_t RESERVED_2[1];

	volatile const uint16_t HCIVERSION;				/**< Host Controller Interface Version, offset: 0x102 */

	volatile const uint32_t HCSPARAMS;				/**< Host Controller Structural Parameters, offset: 0x104 */

	volatile const uint32_t HCCPARAMS;				/**< Host Controller Capability Parameters, offset: 0x108 */

	uint8_t RESERVED_3[20];

	volatile const uint16_t DCIVERSION;				/**< Device Controller Interface Version, offset: 0x120 */

	uint8_t RESERVED_4[2];
	volatile const uint32_t DCCPARAMS;				/**< Device Controller Capability Parameters, offset: 0x124 */
	uint8_t RESERVED_5[24];
	volatile uint32_t USBCMD;					/**< USB Command Register, offset: 0x140 */
	volatile uint32_t USBSTS;					/**< USB Status Register, offset: 0x144 */
	volatile uint32_t USBINTR;					/**< Interrupt Enable Register, offset: 0x148 */
	volatile uint32_t FRINDEX;					/**< USB Frame Index, offset: 0x14C */
	uint8_t RESERVED_6[4];

	union {
		/* offset: 0x154 */

		volatile uint32_t DEVICEADDR;				/**< Device Address, offset: 0x154 */

		volatile uint32_t PERIODICLISTBASE;			/**< Frame List Base Address, offset: 0x154 */

	};


	union {
		/* offset: 0x158 */

		volatile uint32_t ASYNCLISTADDR;			/**< Next Asynch. Address, offset: 0x158 */

		volatile uint32_t ENDPTLISTADDR;			/**< Endpoint List Address, offset: 0x158 */

	};


	uint8_t RESERVED_7[4];

	volatile uint32_t BURSTSIZE;					/**< Programmable Burst Size, offset: 0x160 */

	volatile uint32_t TXFILLTUNING;					/**< TX FIFO Fill Tuning, offset: 0x164 */

	uint8_t RESERVED_8[16];

	volatile uint32_t ENDPTNAK;					/**< Endpoint NAK, offset: 0x178 */

	volatile uint32_t ENDPTNAKEN;					/**< Endpoint NAK Enable, offset: 0x17C */

	volatile const uint32_t CONFIGFLAG;				/**< Configure Flag Register, offset: 0x180 */

	volatile uint32_t PORTSC1;					/**< Port Status & Control, offset: 0x184 */

	uint8_t RESERVED_9[28];

	volatile uint32_t OTGSC;					/**< On-The-Go Status & control, offset: 0x1A4 */

	volatile uint32_t USBMODE;					/**< USB Device Mode, offset: 0x1A8 */

	volatile uint32_t ENDPTSETUPSTAT;				/**< Endpoint Setup Status, offset: 0x1AC */

	volatile uint32_t ENDPTPRIME;					/**< Endpoint Prime, offset: 0x1B0 */

	volatile uint32_t ENDPTFLUSH;					/**< Endpoint Flush, offset: 0x1B4 */

	volatile const uint32_t ENDPTSTAT;				/**< Endpoint Status, offset: 0x1B8 */

	volatile uint32_t ENDPTCOMPLETE;				/**< Endpoint Complete, offset: 0x1BC */

	volatile uint32_t ENDPTCTRL0;					/**< Endpoint Control0, offset: 0x1C0 */

	volatile uint32_t ENDPTCTRL[7];					/**< Endpoint Control 1..Endpoint Control 7, array offset: 0x1C4, array step: 0x4 */
} imxrt1020_usb;

/** Peripheral USB base address */
#define IMXRT1020_USB_BASE	(0x402E0000u)
#define IMXRT1020_USB		((imxrt1020_usb *)IMXRT1020_USB_BASE)
/** Peripheral USB base pointer */
/* ----------------------------------------------------------------------------
   -- USB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USB_Register_Masks USB Register Masks
 * @{
 */

/*! @name ID - Identification register */
/*! @{ */
#define USB_ID_ID_MASK                           (0x3FU)
#define USB_ID_ID_SHIFT                          (0U)
#define USB_ID_ID(x)                             (((uint32_t)(((uint32_t)(x)) << USB_ID_ID_SHIFT)) & USB_ID_ID_MASK)
#define USB_ID_NID_MASK                          (0x3F00U)
#define USB_ID_NID_SHIFT                         (8U)
#define USB_ID_NID(x)                            (((uint32_t)(((uint32_t)(x)) << USB_ID_NID_SHIFT)) & USB_ID_NID_MASK)
#define USB_ID_REVISION_MASK                     (0xFF0000U)
#define USB_ID_REVISION_SHIFT                    (16U)
#define USB_ID_REVISION(x)                       (((uint32_t)(((uint32_t)(x)) << USB_ID_REVISION_SHIFT)) & USB_ID_REVISION_MASK)
/*! @} */

/*! @name HWGENERAL - Hardware General */
/*! @{ */
#define USB_HWGENERAL_PHYW_MASK                  (0x30U)
#define USB_HWGENERAL_PHYW_SHIFT                 (4U)
/*! PHYW
 *  0b00..8 bit wide data bus Software non-programmable
 *  0b01..16 bit wide data bus Software non-programmable
 *  0b10..Reset to 8 bit wide data bus Software programmable
 *  0b11..Reset to 16 bit wide data bus Software programmable
 */
#define USB_HWGENERAL_PHYW(x)                    (((uint32_t)(((uint32_t)(x)) << USB_HWGENERAL_PHYW_SHIFT)) & USB_HWGENERAL_PHYW_MASK)
#define USB_HWGENERAL_PHYM_MASK                  (0x1C0U)
#define USB_HWGENERAL_PHYM_SHIFT                 (6U)
/*! PHYM
 *  0b000..UTMI/UMTI+
 *  0b001..ULPI DDR
 *  0b010..ULPI
 *  0b011..Serial Only
 *  0b100..Software programmable - reset to UTMI/UTMI+
 *  0b101..Software programmable - reset to ULPI DDR
 *  0b110..Software programmable - reset to ULPI
 *  0b111..Software programmable - reset to Serial
 */
#define USB_HWGENERAL_PHYM(x)                    (((uint32_t)(((uint32_t)(x)) << USB_HWGENERAL_PHYM_SHIFT)) & USB_HWGENERAL_PHYM_MASK)
#define USB_HWGENERAL_SM_MASK                    (0x600U)
#define USB_HWGENERAL_SM_SHIFT                   (9U)
/*! SM
 *  0b00..No Serial Engine, always use parallel signalling.
 *  0b01..Serial Engine present, always use serial signalling for FS/LS.
 *  0b10..Software programmable - Reset to use parallel signalling for FS/LS
 *  0b11..Software programmable - Reset to use serial signalling for FS/LS
 */
#define USB_HWGENERAL_SM(x)                      (((uint32_t)(((uint32_t)(x)) << USB_HWGENERAL_SM_SHIFT)) & USB_HWGENERAL_SM_MASK)
/*! @} */

/*! @name HWHOST - Host Hardware Parameters */
/*! @{ */
#define USB_HWHOST_HC_MASK                       (0x1U)
#define USB_HWHOST_HC_SHIFT                      (0U)
/*! HC
 *  0b1..Supported
 *  0b0..Not supported
 */
#define USB_HWHOST_HC(x)                         (((uint32_t)(((uint32_t)(x)) << USB_HWHOST_HC_SHIFT)) & USB_HWHOST_HC_MASK)
#define USB_HWHOST_NPORT_MASK                    (0xEU)
#define USB_HWHOST_NPORT_SHIFT                   (1U)
#define USB_HWHOST_NPORT(x)                      (((uint32_t)(((uint32_t)(x)) << USB_HWHOST_NPORT_SHIFT)) & USB_HWHOST_NPORT_MASK)
/*! @} */

/*! @name HWDEVICE - Device Hardware Parameters */
/*! @{ */
#define USB_HWDEVICE_DC_MASK                     (0x1U)
#define USB_HWDEVICE_DC_SHIFT                    (0U)
/*! DC
 *  0b1..Supported
 *  0b0..Not supported
 */
#define USB_HWDEVICE_DC(x)                       (((uint32_t)(((uint32_t)(x)) << USB_HWDEVICE_DC_SHIFT)) & USB_HWDEVICE_DC_MASK)
#define USB_HWDEVICE_DEVEP_MASK                  (0x3EU)
#define USB_HWDEVICE_DEVEP_SHIFT                 (1U)
#define USB_HWDEVICE_DEVEP(x)                    (((uint32_t)(((uint32_t)(x)) << USB_HWDEVICE_DEVEP_SHIFT)) & USB_HWDEVICE_DEVEP_MASK)
/*! @} */

/*! @name HWTXBUF - TX Buffer Hardware Parameters */
/*! @{ */
#define USB_HWTXBUF_TXBURST_MASK                 (0xFFU)
#define USB_HWTXBUF_TXBURST_SHIFT                (0U)
#define USB_HWTXBUF_TXBURST(x)                   (((uint32_t)(((uint32_t)(x)) << USB_HWTXBUF_TXBURST_SHIFT)) & USB_HWTXBUF_TXBURST_MASK)
#define USB_HWTXBUF_TXCHANADD_MASK               (0xFF0000U)
#define USB_HWTXBUF_TXCHANADD_SHIFT              (16U)
#define USB_HWTXBUF_TXCHANADD(x)                 (((uint32_t)(((uint32_t)(x)) << USB_HWTXBUF_TXCHANADD_SHIFT)) & USB_HWTXBUF_TXCHANADD_MASK)
/*! @} */

/*! @name HWRXBUF - RX Buffer Hardware Parameters */
/*! @{ */
#define USB_HWRXBUF_RXBURST_MASK                 (0xFFU)
#define USB_HWRXBUF_RXBURST_SHIFT                (0U)
#define USB_HWRXBUF_RXBURST(x)                   (((uint32_t)(((uint32_t)(x)) << USB_HWRXBUF_RXBURST_SHIFT)) & USB_HWRXBUF_RXBURST_MASK)
#define USB_HWRXBUF_RXADD_MASK                   (0xFF00U)
#define USB_HWRXBUF_RXADD_SHIFT                  (8U)
#define USB_HWRXBUF_RXADD(x)                     (((uint32_t)(((uint32_t)(x)) << USB_HWRXBUF_RXADD_SHIFT)) & USB_HWRXBUF_RXADD_MASK)
/*! @} */

/*! @name GPTIMER0LD - General Purpose Timer #0 Load */
/*! @{ */
#define USB_GPTIMER0LD_GPTLD_MASK                (0xFFFFFFU)
#define USB_GPTIMER0LD_GPTLD_SHIFT               (0U)
#define USB_GPTIMER0LD_GPTLD(x)                  (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER0LD_GPTLD_SHIFT)) & USB_GPTIMER0LD_GPTLD_MASK)
/*! @} */

/*! @name GPTIMER0CTRL - General Purpose Timer #0 Controller */
/*! @{ */
#define USB_GPTIMER0CTRL_GPTCNT_MASK             (0xFFFFFFU)
#define USB_GPTIMER0CTRL_GPTCNT_SHIFT            (0U)
#define USB_GPTIMER0CTRL_GPTCNT(x)               (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER0CTRL_GPTCNT_SHIFT)) & USB_GPTIMER0CTRL_GPTCNT_MASK)
#define USB_GPTIMER0CTRL_GPTMODE_MASK            (0x1000000U)
#define USB_GPTIMER0CTRL_GPTMODE_SHIFT           (24U)
/*! GPTMODE
 *  0b0..One Shot Mode
 *  0b1..Repeat Mode
 */
#define USB_GPTIMER0CTRL_GPTMODE(x)              (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER0CTRL_GPTMODE_SHIFT)) & USB_GPTIMER0CTRL_GPTMODE_MASK)
#define USB_GPTIMER0CTRL_GPTRST_MASK             (0x40000000U)
#define USB_GPTIMER0CTRL_GPTRST_SHIFT            (30U)
/*! GPTRST
 *  0b0..No action
 *  0b1..Load counter value from GPTLD bits in n_GPTIMER0LD
 */
#define USB_GPTIMER0CTRL_GPTRST(x)               (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER0CTRL_GPTRST_SHIFT)) & USB_GPTIMER0CTRL_GPTRST_MASK)
#define USB_GPTIMER0CTRL_GPTRUN_MASK             (0x80000000U)
#define USB_GPTIMER0CTRL_GPTRUN_SHIFT            (31U)
/*! GPTRUN
 *  0b0..Stop counting
 *  0b1..Run
 */
#define USB_GPTIMER0CTRL_GPTRUN(x)               (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER0CTRL_GPTRUN_SHIFT)) & USB_GPTIMER0CTRL_GPTRUN_MASK)
/*! @} */

/*! @name GPTIMER1LD - General Purpose Timer #1 Load */
/*! @{ */
#define USB_GPTIMER1LD_GPTLD_MASK                (0xFFFFFFU)
#define USB_GPTIMER1LD_GPTLD_SHIFT               (0U)
#define USB_GPTIMER1LD_GPTLD(x)                  (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER1LD_GPTLD_SHIFT)) & USB_GPTIMER1LD_GPTLD_MASK)
/*! @} */

/*! @name GPTIMER1CTRL - General Purpose Timer #1 Controller */
/*! @{ */
#define USB_GPTIMER1CTRL_GPTCNT_MASK             (0xFFFFFFU)
#define USB_GPTIMER1CTRL_GPTCNT_SHIFT            (0U)
#define USB_GPTIMER1CTRL_GPTCNT(x)               (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER1CTRL_GPTCNT_SHIFT)) & USB_GPTIMER1CTRL_GPTCNT_MASK)
#define USB_GPTIMER1CTRL_GPTMODE_MASK            (0x1000000U)
#define USB_GPTIMER1CTRL_GPTMODE_SHIFT           (24U)
/*! GPTMODE
 *  0b0..One Shot Mode
 *  0b1..Repeat Mode
 */
#define USB_GPTIMER1CTRL_GPTMODE(x)              (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER1CTRL_GPTMODE_SHIFT)) & USB_GPTIMER1CTRL_GPTMODE_MASK)
#define USB_GPTIMER1CTRL_GPTRST_MASK             (0x40000000U)
#define USB_GPTIMER1CTRL_GPTRST_SHIFT            (30U)
/*! GPTRST
 *  0b0..No action
 *  0b1..Load counter value from GPTLD bits in USB_n_GPTIMER0LD
 */
#define USB_GPTIMER1CTRL_GPTRST(x)               (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER1CTRL_GPTRST_SHIFT)) & USB_GPTIMER1CTRL_GPTRST_MASK)
#define USB_GPTIMER1CTRL_GPTRUN_MASK             (0x80000000U)
#define USB_GPTIMER1CTRL_GPTRUN_SHIFT            (31U)
/*! GPTRUN
 *  0b0..Stop counting
 *  0b1..Run
 */
#define USB_GPTIMER1CTRL_GPTRUN(x)               (((uint32_t)(((uint32_t)(x)) << USB_GPTIMER1CTRL_GPTRUN_SHIFT)) & USB_GPTIMER1CTRL_GPTRUN_MASK)
/*! @} */

/*! @name SBUSCFG - System Bus Config */
/*! @{ */
#define USB_SBUSCFG_AHBBRST_MASK                 (0x7U)
#define USB_SBUSCFG_AHBBRST_SHIFT                (0U)
/*! AHBBRST
 *  0b000..Incremental burst of unspecified length only
 *  0b001..INCR4 burst, then single transfer
 *  0b010..INCR8 burst, INCR4 burst, then single transfer
 *  0b011..INCR16 burst, INCR8 burst, INCR4 burst, then single transfer
 *  0b100..Reserved, don't use
 *  0b101..INCR4 burst, then incremental burst of unspecified length
 *  0b110..INCR8 burst, INCR4 burst, then incremental burst of unspecified length
 *  0b111..INCR16 burst, INCR8 burst, INCR4 burst, then incremental burst of unspecified length
 */
#define USB_SBUSCFG_AHBBRST(x)                   (((uint32_t)(((uint32_t)(x)) << USB_SBUSCFG_AHBBRST_SHIFT)) & USB_SBUSCFG_AHBBRST_MASK)
/*! @} */

/*! @name CAPLENGTH - Capability Registers Length */
/*! @{ */
#define USB_CAPLENGTH_CAPLENGTH_MASK             (0xFFU)
#define USB_CAPLENGTH_CAPLENGTH_SHIFT            (0U)
#define USB_CAPLENGTH_CAPLENGTH(x)               (((uint8_t)(((uint8_t)(x)) << USB_CAPLENGTH_CAPLENGTH_SHIFT)) & USB_CAPLENGTH_CAPLENGTH_MASK)
/*! @} */

/*! @name HCIVERSION - Host Controller Interface Version */
/*! @{ */
#define USB_HCIVERSION_HCIVERSION_MASK           (0xFFFFU)
#define USB_HCIVERSION_HCIVERSION_SHIFT          (0U)
#define USB_HCIVERSION_HCIVERSION(x)             (((uint16_t)(((uint16_t)(x)) << USB_HCIVERSION_HCIVERSION_SHIFT)) & USB_HCIVERSION_HCIVERSION_MASK)
/*! @} */

/*! @name HCSPARAMS - Host Controller Structural Parameters */
/*! @{ */
#define USB_HCSPARAMS_N_PORTS_MASK               (0xFU)
#define USB_HCSPARAMS_N_PORTS_SHIFT              (0U)
#define USB_HCSPARAMS_N_PORTS(x)                 (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_N_PORTS_SHIFT)) & USB_HCSPARAMS_N_PORTS_MASK)
#define USB_HCSPARAMS_PPC_MASK                   (0x10U)
#define USB_HCSPARAMS_PPC_SHIFT                  (4U)
#define USB_HCSPARAMS_PPC(x)                     (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_PPC_SHIFT)) & USB_HCSPARAMS_PPC_MASK)
#define USB_HCSPARAMS_N_PCC_MASK                 (0xF00U)
#define USB_HCSPARAMS_N_PCC_SHIFT                (8U)
#define USB_HCSPARAMS_N_PCC(x)                   (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_N_PCC_SHIFT)) & USB_HCSPARAMS_N_PCC_MASK)
#define USB_HCSPARAMS_N_CC_MASK                  (0xF000U)
#define USB_HCSPARAMS_N_CC_SHIFT                 (12U)
/*! N_CC
 *  0b0000..There is no internal Companion Controller and port-ownership hand-off is not supported.
 *  0b0001..There are internal companion controller(s) and port-ownership hand-offs is supported.
 */
#define USB_HCSPARAMS_N_CC(x)                    (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_N_CC_SHIFT)) & USB_HCSPARAMS_N_CC_MASK)
#define USB_HCSPARAMS_PI_MASK                    (0x10000U)
#define USB_HCSPARAMS_PI_SHIFT                   (16U)
#define USB_HCSPARAMS_PI(x)                      (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_PI_SHIFT)) & USB_HCSPARAMS_PI_MASK)
#define USB_HCSPARAMS_N_PTT_MASK                 (0xF00000U)
#define USB_HCSPARAMS_N_PTT_SHIFT                (20U)
#define USB_HCSPARAMS_N_PTT(x)                   (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_N_PTT_SHIFT)) & USB_HCSPARAMS_N_PTT_MASK)
#define USB_HCSPARAMS_N_TT_MASK                  (0xF000000U)
#define USB_HCSPARAMS_N_TT_SHIFT                 (24U)
#define USB_HCSPARAMS_N_TT(x)                    (((uint32_t)(((uint32_t)(x)) << USB_HCSPARAMS_N_TT_SHIFT)) & USB_HCSPARAMS_N_TT_MASK)
/*! @} */

/*! @name HCCPARAMS - Host Controller Capability Parameters */
/*! @{ */
#define USB_HCCPARAMS_ADC_MASK                   (0x1U)
#define USB_HCCPARAMS_ADC_SHIFT                  (0U)
#define USB_HCCPARAMS_ADC(x)                     (((uint32_t)(((uint32_t)(x)) << USB_HCCPARAMS_ADC_SHIFT)) & USB_HCCPARAMS_ADC_MASK)
#define USB_HCCPARAMS_PFL_MASK                   (0x2U)
#define USB_HCCPARAMS_PFL_SHIFT                  (1U)
#define USB_HCCPARAMS_PFL(x)                     (((uint32_t)(((uint32_t)(x)) << USB_HCCPARAMS_PFL_SHIFT)) & USB_HCCPARAMS_PFL_MASK)
#define USB_HCCPARAMS_ASP_MASK                   (0x4U)
#define USB_HCCPARAMS_ASP_SHIFT                  (2U)
#define USB_HCCPARAMS_ASP(x)                     (((uint32_t)(((uint32_t)(x)) << USB_HCCPARAMS_ASP_SHIFT)) & USB_HCCPARAMS_ASP_MASK)
#define USB_HCCPARAMS_IST_MASK                   (0xF0U)
#define USB_HCCPARAMS_IST_SHIFT                  (4U)
#define USB_HCCPARAMS_IST(x)                     (((uint32_t)(((uint32_t)(x)) << USB_HCCPARAMS_IST_SHIFT)) & USB_HCCPARAMS_IST_MASK)
#define USB_HCCPARAMS_EECP_MASK                  (0xFF00U)
#define USB_HCCPARAMS_EECP_SHIFT                 (8U)
#define USB_HCCPARAMS_EECP(x)                    (((uint32_t)(((uint32_t)(x)) << USB_HCCPARAMS_EECP_SHIFT)) & USB_HCCPARAMS_EECP_MASK)
/*! @} */

/*! @name DCIVERSION - Device Controller Interface Version */
/*! @{ */
#define USB_DCIVERSION_DCIVERSION_MASK           (0xFFFFU)
#define USB_DCIVERSION_DCIVERSION_SHIFT          (0U)
#define USB_DCIVERSION_DCIVERSION(x)             (((uint16_t)(((uint16_t)(x)) << USB_DCIVERSION_DCIVERSION_SHIFT)) & USB_DCIVERSION_DCIVERSION_MASK)
/*! @} */

/*! @name DCCPARAMS - Device Controller Capability Parameters */
/*! @{ */
#define USB_DCCPARAMS_DEN_MASK                   (0x1FU)
#define USB_DCCPARAMS_DEN_SHIFT                  (0U)
#define USB_DCCPARAMS_DEN(x)                     (((uint32_t)(((uint32_t)(x)) << USB_DCCPARAMS_DEN_SHIFT)) & USB_DCCPARAMS_DEN_MASK)
#define USB_DCCPARAMS_DC_MASK                    (0x80U)
#define USB_DCCPARAMS_DC_SHIFT                   (7U)
#define USB_DCCPARAMS_DC(x)                      (((uint32_t)(((uint32_t)(x)) << USB_DCCPARAMS_DC_SHIFT)) & USB_DCCPARAMS_DC_MASK)
#define USB_DCCPARAMS_HC_MASK                    (0x100U)
#define USB_DCCPARAMS_HC_SHIFT                   (8U)
#define USB_DCCPARAMS_HC(x)                      (((uint32_t)(((uint32_t)(x)) << USB_DCCPARAMS_HC_SHIFT)) & USB_DCCPARAMS_HC_MASK)
/*! @} */

/*! @name USBCMD - USB Command Register */
/*! @{ */
#define USB_USBCMD_RS_MASK                       (0x1U)
#define USB_USBCMD_RS_SHIFT                      (0U)
#define USB_USBCMD_RS(x)                         (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_RS_SHIFT)) & USB_USBCMD_RS_MASK)
#define USB_USBCMD_RST_MASK                      (0x2U)
#define USB_USBCMD_RST_SHIFT                     (1U)
#define USB_USBCMD_RST(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_RST_SHIFT)) & USB_USBCMD_RST_MASK)
#define USB_USBCMD_FS_1_MASK                     (0xCU)
#define USB_USBCMD_FS_1_SHIFT                    (2U)
#define USB_USBCMD_FS_1(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_FS_1_SHIFT)) & USB_USBCMD_FS_1_MASK)
#define USB_USBCMD_PSE_MASK                      (0x10U)
#define USB_USBCMD_PSE_SHIFT                     (4U)
/*! PSE
 *  0b0..Do not process the Periodic Schedule
 *  0b1..Use the PERIODICLISTBASE register to access the Periodic Schedule.
 */
#define USB_USBCMD_PSE(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_PSE_SHIFT)) & USB_USBCMD_PSE_MASK)
#define USB_USBCMD_ASE_MASK                      (0x20U)
#define USB_USBCMD_ASE_SHIFT                     (5U)
/*! ASE
 *  0b0..Do not process the Asynchronous Schedule.
 *  0b1..Use the ASYNCLISTADDR register to access the Asynchronous Schedule.
 */
#define USB_USBCMD_ASE(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_ASE_SHIFT)) & USB_USBCMD_ASE_MASK)
#define USB_USBCMD_IAA_MASK                      (0x40U)
#define USB_USBCMD_IAA_SHIFT                     (6U)
#define USB_USBCMD_IAA(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_IAA_SHIFT)) & USB_USBCMD_IAA_MASK)
#define USB_USBCMD_ASP_MASK                      (0x300U)
#define USB_USBCMD_ASP_SHIFT                     (8U)
#define USB_USBCMD_ASP(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_ASP_SHIFT)) & USB_USBCMD_ASP_MASK)
#define USB_USBCMD_ASPE_MASK                     (0x800U)
#define USB_USBCMD_ASPE_SHIFT                    (11U)
#define USB_USBCMD_ASPE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_ASPE_SHIFT)) & USB_USBCMD_ASPE_MASK)
#define USB_USBCMD_ATDTW_MASK                    (0x1000U)
#define USB_USBCMD_ATDTW_SHIFT                   (12U)
#define USB_USBCMD_ATDTW(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_ATDTW_SHIFT)) & USB_USBCMD_ATDTW_MASK)
#define USB_USBCMD_SUTW_MASK                     (0x2000U)
#define USB_USBCMD_SUTW_SHIFT                    (13U)
#define USB_USBCMD_SUTW(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_SUTW_SHIFT)) & USB_USBCMD_SUTW_MASK)
#define USB_USBCMD_FS_2_MASK                     (0x8000U)
#define USB_USBCMD_FS_2_SHIFT                    (15U)
/*! FS_2
 *  0b0..1024 elements (4096 bytes) Default value
 *  0b1..512 elements (2048 bytes)
 */
#define USB_USBCMD_FS_2(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_FS_2_SHIFT)) & USB_USBCMD_FS_2_MASK)
#define USB_USBCMD_ITC_MASK                      (0xFF0000U)
#define USB_USBCMD_ITC_SHIFT                     (16U)
/*! ITC
 *  0b00000000..Immediate (no threshold)
 *  0b00000001..1 micro-frame
 *  0b00000010..2 micro-frames
 *  0b00000100..4 micro-frames
 *  0b00001000..8 micro-frames
 *  0b00010000..16 micro-frames
 *  0b00100000..32 micro-frames
 *  0b01000000..64 micro-frames
 */
#define USB_USBCMD_ITC(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBCMD_ITC_SHIFT)) & USB_USBCMD_ITC_MASK)
/*! @} */

/*! @name USBSTS - USB Status Register */
/*! @{ */
#define USB_USBSTS_UI_MASK                       (0x1U)
#define USB_USBSTS_UI_SHIFT                      (0U)
#define USB_USBSTS_UI(x)                         (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_UI_SHIFT)) & USB_USBSTS_UI_MASK)
#define USB_USBSTS_UEI_MASK                      (0x2U)
#define USB_USBSTS_UEI_SHIFT                     (1U)
#define USB_USBSTS_UEI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_UEI_SHIFT)) & USB_USBSTS_UEI_MASK)
#define USB_USBSTS_PCI_MASK                      (0x4U)
#define USB_USBSTS_PCI_SHIFT                     (2U)
#define USB_USBSTS_PCI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_PCI_SHIFT)) & USB_USBSTS_PCI_MASK)
#define USB_USBSTS_FRI_MASK                      (0x8U)
#define USB_USBSTS_FRI_SHIFT                     (3U)
#define USB_USBSTS_FRI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_FRI_SHIFT)) & USB_USBSTS_FRI_MASK)
#define USB_USBSTS_SEI_MASK                      (0x10U)
#define USB_USBSTS_SEI_SHIFT                     (4U)
#define USB_USBSTS_SEI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_SEI_SHIFT)) & USB_USBSTS_SEI_MASK)
#define USB_USBSTS_AAI_MASK                      (0x20U)
#define USB_USBSTS_AAI_SHIFT                     (5U)
#define USB_USBSTS_AAI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_AAI_SHIFT)) & USB_USBSTS_AAI_MASK)
#define USB_USBSTS_URI_MASK                      (0x40U)
#define USB_USBSTS_URI_SHIFT                     (6U)
#define USB_USBSTS_URI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_URI_SHIFT)) & USB_USBSTS_URI_MASK)
#define USB_USBSTS_SRI_MASK                      (0x80U)
#define USB_USBSTS_SRI_SHIFT                     (7U)
#define USB_USBSTS_SRI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_SRI_SHIFT)) & USB_USBSTS_SRI_MASK)
#define USB_USBSTS_SLI_MASK                      (0x100U)
#define USB_USBSTS_SLI_SHIFT                     (8U)
#define USB_USBSTS_SLI(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_SLI_SHIFT)) & USB_USBSTS_SLI_MASK)
#define USB_USBSTS_ULPII_MASK                    (0x400U)
#define USB_USBSTS_ULPII_SHIFT                   (10U)
#define USB_USBSTS_ULPII(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_ULPII_SHIFT)) & USB_USBSTS_ULPII_MASK)
#define USB_USBSTS_HCH_MASK                      (0x1000U)
#define USB_USBSTS_HCH_SHIFT                     (12U)
#define USB_USBSTS_HCH(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_HCH_SHIFT)) & USB_USBSTS_HCH_MASK)
#define USB_USBSTS_RCL_MASK                      (0x2000U)
#define USB_USBSTS_RCL_SHIFT                     (13U)
#define USB_USBSTS_RCL(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_RCL_SHIFT)) & USB_USBSTS_RCL_MASK)
#define USB_USBSTS_PS_MASK                       (0x4000U)
#define USB_USBSTS_PS_SHIFT                      (14U)
#define USB_USBSTS_PS(x)                         (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_PS_SHIFT)) & USB_USBSTS_PS_MASK)
#define USB_USBSTS_AS_MASK                       (0x8000U)
#define USB_USBSTS_AS_SHIFT                      (15U)
#define USB_USBSTS_AS(x)                         (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_AS_SHIFT)) & USB_USBSTS_AS_MASK)
#define USB_USBSTS_NAKI_MASK                     (0x10000U)
#define USB_USBSTS_NAKI_SHIFT                    (16U)
#define USB_USBSTS_NAKI(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_NAKI_SHIFT)) & USB_USBSTS_NAKI_MASK)
#define USB_USBSTS_TI0_MASK                      (0x1000000U)
#define USB_USBSTS_TI0_SHIFT                     (24U)
#define USB_USBSTS_TI0(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_TI0_SHIFT)) & USB_USBSTS_TI0_MASK)
#define USB_USBSTS_TI1_MASK                      (0x2000000U)
#define USB_USBSTS_TI1_SHIFT                     (25U)
#define USB_USBSTS_TI1(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBSTS_TI1_SHIFT)) & USB_USBSTS_TI1_MASK)
/*! @} */

/*! @name USBINTR - Interrupt Enable Register */
/*! @{ */
#define USB_USBINTR_UE_MASK                      (0x1U)
#define USB_USBINTR_UE_SHIFT                     (0U)
#define USB_USBINTR_UE(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_UE_SHIFT)) & USB_USBINTR_UE_MASK)
#define USB_USBINTR_UEE_MASK                     (0x2U)
#define USB_USBINTR_UEE_SHIFT                    (1U)
#define USB_USBINTR_UEE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_UEE_SHIFT)) & USB_USBINTR_UEE_MASK)
#define USB_USBINTR_PCE_MASK                     (0x4U)
#define USB_USBINTR_PCE_SHIFT                    (2U)
#define USB_USBINTR_PCE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_PCE_SHIFT)) & USB_USBINTR_PCE_MASK)
#define USB_USBINTR_FRE_MASK                     (0x8U)
#define USB_USBINTR_FRE_SHIFT                    (3U)
#define USB_USBINTR_FRE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_FRE_SHIFT)) & USB_USBINTR_FRE_MASK)
#define USB_USBINTR_SEE_MASK                     (0x10U)
#define USB_USBINTR_SEE_SHIFT                    (4U)
#define USB_USBINTR_SEE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_SEE_SHIFT)) & USB_USBINTR_SEE_MASK)
#define USB_USBINTR_AAE_MASK                     (0x20U)
#define USB_USBINTR_AAE_SHIFT                    (5U)
#define USB_USBINTR_AAE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_AAE_SHIFT)) & USB_USBINTR_AAE_MASK)
#define USB_USBINTR_URE_MASK                     (0x40U)
#define USB_USBINTR_URE_SHIFT                    (6U)
#define USB_USBINTR_URE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_URE_SHIFT)) & USB_USBINTR_URE_MASK)
#define USB_USBINTR_SRE_MASK                     (0x80U)
#define USB_USBINTR_SRE_SHIFT                    (7U)
#define USB_USBINTR_SRE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_SRE_SHIFT)) & USB_USBINTR_SRE_MASK)
#define USB_USBINTR_SLE_MASK                     (0x100U)
#define USB_USBINTR_SLE_SHIFT                    (8U)
#define USB_USBINTR_SLE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_SLE_SHIFT)) & USB_USBINTR_SLE_MASK)
#define USB_USBINTR_ULPIE_MASK                   (0x400U)
#define USB_USBINTR_ULPIE_SHIFT                  (10U)
#define USB_USBINTR_ULPIE(x)                     (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_ULPIE_SHIFT)) & USB_USBINTR_ULPIE_MASK)
#define USB_USBINTR_NAKE_MASK                    (0x10000U)
#define USB_USBINTR_NAKE_SHIFT                   (16U)
#define USB_USBINTR_NAKE(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_NAKE_SHIFT)) & USB_USBINTR_NAKE_MASK)
#define USB_USBINTR_UAIE_MASK                    (0x40000U)
#define USB_USBINTR_UAIE_SHIFT                   (18U)
#define USB_USBINTR_UAIE(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_UAIE_SHIFT)) & USB_USBINTR_UAIE_MASK)
#define USB_USBINTR_UPIE_MASK                    (0x80000U)
#define USB_USBINTR_UPIE_SHIFT                   (19U)
#define USB_USBINTR_UPIE(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_UPIE_SHIFT)) & USB_USBINTR_UPIE_MASK)
#define USB_USBINTR_TIE0_MASK                    (0x1000000U)
#define USB_USBINTR_TIE0_SHIFT                   (24U)
#define USB_USBINTR_TIE0(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_TIE0_SHIFT)) & USB_USBINTR_TIE0_MASK)
#define USB_USBINTR_TIE1_MASK                    (0x2000000U)
#define USB_USBINTR_TIE1_SHIFT                   (25U)
#define USB_USBINTR_TIE1(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBINTR_TIE1_SHIFT)) & USB_USBINTR_TIE1_MASK)
/*! @} */

/*! @name FRINDEX - USB Frame Index */
/*! @{ */
#define USB_FRINDEX_FRINDEX_MASK                 (0x3FFFU)
#define USB_FRINDEX_FRINDEX_SHIFT                (0U)
/*! FRINDEX
 *  0b00000000000000..(1024) 12
 *  0b00000000000001..(512) 11
 *  0b00000000000010..(256) 10
 *  0b00000000000011..(128) 9
 *  0b00000000000100..(64) 8
 *  0b00000000000101..(32) 7
 *  0b00000000000110..(16) 6
 *  0b00000000000111..(8) 5
 */
#define USB_FRINDEX_FRINDEX(x)                   (((uint32_t)(((uint32_t)(x)) << USB_FRINDEX_FRINDEX_SHIFT)) & USB_FRINDEX_FRINDEX_MASK)
/*! @} */

/*! @name DEVICEADDR - Device Address */
/*! @{ */
#define USB_DEVICEADDR_USBADRA_MASK              (0x1000000U)
#define USB_DEVICEADDR_USBADRA_SHIFT             (24U)
#define USB_DEVICEADDR_USBADRA(x)                (((uint32_t)(((uint32_t)(x)) << USB_DEVICEADDR_USBADRA_SHIFT)) & USB_DEVICEADDR_USBADRA_MASK)
#define USB_DEVICEADDR_USBADR_MASK               (0xFE000000U)
#define USB_DEVICEADDR_USBADR_SHIFT              (25U)
#define USB_DEVICEADDR_USBADR(x)                 (((uint32_t)(((uint32_t)(x)) << USB_DEVICEADDR_USBADR_SHIFT)) & USB_DEVICEADDR_USBADR_MASK)
/*! @} */

/*! @name PERIODICLISTBASE - Frame List Base Address */
/*! @{ */
#define USB_PERIODICLISTBASE_BASEADR_MASK        (0xFFFFF000U)
#define USB_PERIODICLISTBASE_BASEADR_SHIFT       (12U)
#define USB_PERIODICLISTBASE_BASEADR(x)          (((uint32_t)(((uint32_t)(x)) << USB_PERIODICLISTBASE_BASEADR_SHIFT)) & USB_PERIODICLISTBASE_BASEADR_MASK)
/*! @} */

/*! @name ASYNCLISTADDR - Next Asynch. Address */
/*! @{ */
#define USB_ASYNCLISTADDR_ASYBASE_MASK           (0xFFFFFFE0U)
#define USB_ASYNCLISTADDR_ASYBASE_SHIFT          (5U)
#define USB_ASYNCLISTADDR_ASYBASE(x)             (((uint32_t)(((uint32_t)(x)) << USB_ASYNCLISTADDR_ASYBASE_SHIFT)) & USB_ASYNCLISTADDR_ASYBASE_MASK)
/*! @} */

/*! @name ENDPTLISTADDR - Endpoint List Address */
/*! @{ */
#define USB_ENDPTLISTADDR_EPBASE_MASK            (0xFFFFF800U)
#define USB_ENDPTLISTADDR_EPBASE_SHIFT           (11U)
#define USB_ENDPTLISTADDR_EPBASE(x)              (((uint32_t)(((uint32_t)(x)) << USB_ENDPTLISTADDR_EPBASE_SHIFT)) & USB_ENDPTLISTADDR_EPBASE_MASK)
/*! @} */

/*! @name BURSTSIZE - Programmable Burst Size */
/*! @{ */
#define USB_BURSTSIZE_RXPBURST_MASK              (0xFFU)
#define USB_BURSTSIZE_RXPBURST_SHIFT             (0U)
#define USB_BURSTSIZE_RXPBURST(x)                (((uint32_t)(((uint32_t)(x)) << USB_BURSTSIZE_RXPBURST_SHIFT)) & USB_BURSTSIZE_RXPBURST_MASK)
#define USB_BURSTSIZE_TXPBURST_MASK              (0x1FF00U)
#define USB_BURSTSIZE_TXPBURST_SHIFT             (8U)
#define USB_BURSTSIZE_TXPBURST(x)                (((uint32_t)(((uint32_t)(x)) << USB_BURSTSIZE_TXPBURST_SHIFT)) & USB_BURSTSIZE_TXPBURST_MASK)
/*! @} */

/*! @name TXFILLTUNING - TX FIFO Fill Tuning */
/*! @{ */
#define USB_TXFILLTUNING_TXSCHOH_MASK            (0xFFU)
#define USB_TXFILLTUNING_TXSCHOH_SHIFT           (0U)
#define USB_TXFILLTUNING_TXSCHOH(x)              (((uint32_t)(((uint32_t)(x)) << USB_TXFILLTUNING_TXSCHOH_SHIFT)) & USB_TXFILLTUNING_TXSCHOH_MASK)
#define USB_TXFILLTUNING_TXSCHHEALTH_MASK        (0x1F00U)
#define USB_TXFILLTUNING_TXSCHHEALTH_SHIFT       (8U)
#define USB_TXFILLTUNING_TXSCHHEALTH(x)          (((uint32_t)(((uint32_t)(x)) << USB_TXFILLTUNING_TXSCHHEALTH_SHIFT)) & USB_TXFILLTUNING_TXSCHHEALTH_MASK)
#define USB_TXFILLTUNING_TXFIFOTHRES_MASK        (0x3F0000U)
#define USB_TXFILLTUNING_TXFIFOTHRES_SHIFT       (16U)
#define USB_TXFILLTUNING_TXFIFOTHRES(x)          (((uint32_t)(((uint32_t)(x)) << USB_TXFILLTUNING_TXFIFOTHRES_SHIFT)) & USB_TXFILLTUNING_TXFIFOTHRES_MASK)
/*! @} */

/*! @name ENDPTNAK - Endpoint NAK */
/*! @{ */
#define USB_ENDPTNAK_EPRN_MASK                   (0xFFU)
#define USB_ENDPTNAK_EPRN_SHIFT                  (0U)
#define USB_ENDPTNAK_EPRN(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTNAK_EPRN_SHIFT)) & USB_ENDPTNAK_EPRN_MASK)
#define USB_ENDPTNAK_EPTN_MASK                   (0xFF0000U)
#define USB_ENDPTNAK_EPTN_SHIFT                  (16U)
#define USB_ENDPTNAK_EPTN(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTNAK_EPTN_SHIFT)) & USB_ENDPTNAK_EPTN_MASK)
/*! @} */

/*! @name ENDPTNAKEN - Endpoint NAK Enable */
/*! @{ */
#define USB_ENDPTNAKEN_EPRNE_MASK                (0xFFU)
#define USB_ENDPTNAKEN_EPRNE_SHIFT               (0U)
#define USB_ENDPTNAKEN_EPRNE(x)                  (((uint32_t)(((uint32_t)(x)) << USB_ENDPTNAKEN_EPRNE_SHIFT)) & USB_ENDPTNAKEN_EPRNE_MASK)
#define USB_ENDPTNAKEN_EPTNE_MASK                (0xFF0000U)
#define USB_ENDPTNAKEN_EPTNE_SHIFT               (16U)
#define USB_ENDPTNAKEN_EPTNE(x)                  (((uint32_t)(((uint32_t)(x)) << USB_ENDPTNAKEN_EPTNE_SHIFT)) & USB_ENDPTNAKEN_EPTNE_MASK)
/*! @} */

/*! @name CONFIGFLAG - Configure Flag Register */
/*! @{ */
#define USB_CONFIGFLAG_CF_MASK                   (0x1U)
#define USB_CONFIGFLAG_CF_SHIFT                  (0U)
/*! CF
 *  0b0..Port routing control logic default-routes each port to an implementation dependent classic host controller.
 *  0b1..Port routing control logic default-routes all ports to this host controller.
 */
#define USB_CONFIGFLAG_CF(x)                     (((uint32_t)(((uint32_t)(x)) << USB_CONFIGFLAG_CF_SHIFT)) & USB_CONFIGFLAG_CF_MASK)
/*! @} */

/*! @name PORTSC1 - Port Status & Control */
/*! @{ */
#define USB_PORTSC1_CCS_MASK                     (0x1U)
#define USB_PORTSC1_CCS_SHIFT                    (0U)
#define USB_PORTSC1_CCS(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_CCS_SHIFT)) & USB_PORTSC1_CCS_MASK)
#define USB_PORTSC1_CSC_MASK                     (0x2U)
#define USB_PORTSC1_CSC_SHIFT                    (1U)
#define USB_PORTSC1_CSC(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_CSC_SHIFT)) & USB_PORTSC1_CSC_MASK)
#define USB_PORTSC1_PE_MASK                      (0x4U)
#define USB_PORTSC1_PE_SHIFT                     (2U)
#define USB_PORTSC1_PE(x)                        (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PE_SHIFT)) & USB_PORTSC1_PE_MASK)
#define USB_PORTSC1_PEC_MASK                     (0x8U)
#define USB_PORTSC1_PEC_SHIFT                    (3U)
#define USB_PORTSC1_PEC(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PEC_SHIFT)) & USB_PORTSC1_PEC_MASK)
#define USB_PORTSC1_OCA_MASK                     (0x10U)
#define USB_PORTSC1_OCA_SHIFT                    (4U)
/*! OCA
 *  0b1..This port currently has an over-current condition
 *  0b0..This port does not have an over-current condition.
 */
#define USB_PORTSC1_OCA(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_OCA_SHIFT)) & USB_PORTSC1_OCA_MASK)
#define USB_PORTSC1_OCC_MASK                     (0x20U)
#define USB_PORTSC1_OCC_SHIFT                    (5U)
#define USB_PORTSC1_OCC(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_OCC_SHIFT)) & USB_PORTSC1_OCC_MASK)
#define USB_PORTSC1_FPR_MASK                     (0x40U)
#define USB_PORTSC1_FPR_SHIFT                    (6U)
#define USB_PORTSC1_FPR(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_FPR_SHIFT)) & USB_PORTSC1_FPR_MASK)
#define USB_PORTSC1_SUSP_MASK                    (0x80U)
#define USB_PORTSC1_SUSP_SHIFT                   (7U)
#define USB_PORTSC1_SUSP(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_SUSP_SHIFT)) & USB_PORTSC1_SUSP_MASK)
#define USB_PORTSC1_PR_MASK                      (0x100U)
#define USB_PORTSC1_PR_SHIFT                     (8U)
#define USB_PORTSC1_PR(x)                        (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PR_SHIFT)) & USB_PORTSC1_PR_MASK)
#define USB_PORTSC1_HSP_MASK                     (0x200U)
#define USB_PORTSC1_HSP_SHIFT                    (9U)
#define USB_PORTSC1_HSP(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_HSP_SHIFT)) & USB_PORTSC1_HSP_MASK)
#define USB_PORTSC1_LS_MASK                      (0xC00U)
#define USB_PORTSC1_LS_SHIFT                     (10U)
/*! LS
 *  0b00..SE0
 *  0b10..J-state
 *  0b01..K-state
 *  0b11..Undefined
 */
#define USB_PORTSC1_LS(x)                        (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_LS_SHIFT)) & USB_PORTSC1_LS_MASK)
#define USB_PORTSC1_PP_MASK                      (0x1000U)
#define USB_PORTSC1_PP_SHIFT                     (12U)
#define USB_PORTSC1_PP(x)                        (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PP_SHIFT)) & USB_PORTSC1_PP_MASK)
#define USB_PORTSC1_PO_MASK                      (0x2000U)
#define USB_PORTSC1_PO_SHIFT                     (13U)
#define USB_PORTSC1_PO(x)                        (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PO_SHIFT)) & USB_PORTSC1_PO_MASK)
#define USB_PORTSC1_PIC_MASK                     (0xC000U)
#define USB_PORTSC1_PIC_SHIFT                    (14U)
/*! PIC
 *  0b00..Port indicators are off
 *  0b01..Amber
 *  0b10..Green
 *  0b11..Undefined
 */
#define USB_PORTSC1_PIC(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PIC_SHIFT)) & USB_PORTSC1_PIC_MASK)
#define USB_PORTSC1_PTC_MASK                     (0xF0000U)
#define USB_PORTSC1_PTC_SHIFT                    (16U)
/*! PTC
 *  0b0000..TEST_MODE_DISABLE
 *  0b0001..J_STATE
 *  0b0010..K_STATE
 *  0b0011..SE0 (host) / NAK (device)
 *  0b0100..Packet
 *  0b0101..FORCE_ENABLE_HS
 *  0b0110..FORCE_ENABLE_FS
 *  0b0111..FORCE_ENABLE_LS
 */
#define USB_PORTSC1_PTC(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PTC_SHIFT)) & USB_PORTSC1_PTC_MASK)
#define USB_PORTSC1_WKCN_MASK                    (0x100000U)
#define USB_PORTSC1_WKCN_SHIFT                   (20U)
#define USB_PORTSC1_WKCN(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_WKCN_SHIFT)) & USB_PORTSC1_WKCN_MASK)
#define USB_PORTSC1_WKDC_MASK                    (0x200000U)
#define USB_PORTSC1_WKDC_SHIFT                   (21U)
#define USB_PORTSC1_WKDC(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_WKDC_SHIFT)) & USB_PORTSC1_WKDC_MASK)
#define USB_PORTSC1_WKOC_MASK                    (0x400000U)
#define USB_PORTSC1_WKOC_SHIFT                   (22U)
#define USB_PORTSC1_WKOC(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_WKOC_SHIFT)) & USB_PORTSC1_WKOC_MASK)
#define USB_PORTSC1_PHCD_MASK                    (0x800000U)
#define USB_PORTSC1_PHCD_SHIFT                   (23U)
/*! PHCD
 *  0b1..Disable PHY clock
 *  0b0..Enable PHY clock
 */
#define USB_PORTSC1_PHCD(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PHCD_SHIFT)) & USB_PORTSC1_PHCD_MASK)
#define USB_PORTSC1_PFSC_MASK                    (0x1000000U)
#define USB_PORTSC1_PFSC_SHIFT                   (24U)
/*! PFSC
 *  0b1..Forced to full speed
 *  0b0..Normal operation
 */
#define USB_PORTSC1_PFSC(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PFSC_SHIFT)) & USB_PORTSC1_PFSC_MASK)
#define USB_PORTSC1_PTS_2_MASK                   (0x2000000U)
#define USB_PORTSC1_PTS_2_SHIFT                  (25U)
#define USB_PORTSC1_PTS_2(x)                     (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PTS_2_SHIFT)) & USB_PORTSC1_PTS_2_MASK)
#define USB_PORTSC1_PSPD_MASK                    (0xC000000U)
#define USB_PORTSC1_PSPD_SHIFT                   (26U)
/*! PSPD
 *  0b00..Full Speed
 *  0b01..Low Speed
 *  0b10..High Speed
 *  0b11..Undefined
 */
#define USB_PORTSC1_PSPD(x)                      (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PSPD_SHIFT)) & USB_PORTSC1_PSPD_MASK)
#define USB_PORTSC1_PTW_MASK                     (0x10000000U)
#define USB_PORTSC1_PTW_SHIFT                    (28U)
/*! PTW
 *  0b0..Select the 8-bit UTMI interface [60MHz]
 *  0b1..Select the 16-bit UTMI interface [30MHz]
 */
#define USB_PORTSC1_PTW(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PTW_SHIFT)) & USB_PORTSC1_PTW_MASK)
#define USB_PORTSC1_STS_MASK                     (0x20000000U)
#define USB_PORTSC1_STS_SHIFT                    (29U)
#define USB_PORTSC1_STS(x)                       (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_STS_SHIFT)) & USB_PORTSC1_STS_MASK)
#define USB_PORTSC1_PTS_1_MASK                   (0xC0000000U)
#define USB_PORTSC1_PTS_1_SHIFT                  (30U)
#define USB_PORTSC1_PTS_1(x)                     (((uint32_t)(((uint32_t)(x)) << USB_PORTSC1_PTS_1_SHIFT)) & USB_PORTSC1_PTS_1_MASK)
/*! @} */

/*! @name OTGSC - On-The-Go Status & control */
/*! @{ */
#define USB_OTGSC_VD_MASK                        (0x1U)
#define USB_OTGSC_VD_SHIFT                       (0U)
#define USB_OTGSC_VD(x)                          (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_VD_SHIFT)) & USB_OTGSC_VD_MASK)
#define USB_OTGSC_VC_MASK                        (0x2U)
#define USB_OTGSC_VC_SHIFT                       (1U)
#define USB_OTGSC_VC(x)                          (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_VC_SHIFT)) & USB_OTGSC_VC_MASK)
#define USB_OTGSC_OT_MASK                        (0x8U)
#define USB_OTGSC_OT_SHIFT                       (3U)
#define USB_OTGSC_OT(x)                          (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_OT_SHIFT)) & USB_OTGSC_OT_MASK)
#define USB_OTGSC_DP_MASK                        (0x10U)
#define USB_OTGSC_DP_SHIFT                       (4U)
#define USB_OTGSC_DP(x)                          (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_DP_SHIFT)) & USB_OTGSC_DP_MASK)
#define USB_OTGSC_IDPU_MASK                      (0x20U)
#define USB_OTGSC_IDPU_SHIFT                     (5U)
#define USB_OTGSC_IDPU(x)                        (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_IDPU_SHIFT)) & USB_OTGSC_IDPU_MASK)
#define USB_OTGSC_ID_MASK                        (0x100U)
#define USB_OTGSC_ID_SHIFT                       (8U)
#define USB_OTGSC_ID(x)                          (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_ID_SHIFT)) & USB_OTGSC_ID_MASK)
#define USB_OTGSC_AVV_MASK                       (0x200U)
#define USB_OTGSC_AVV_SHIFT                      (9U)
#define USB_OTGSC_AVV(x)                         (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_AVV_SHIFT)) & USB_OTGSC_AVV_MASK)
#define USB_OTGSC_ASV_MASK                       (0x400U)
#define USB_OTGSC_ASV_SHIFT                      (10U)
#define USB_OTGSC_ASV(x)                         (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_ASV_SHIFT)) & USB_OTGSC_ASV_MASK)
#define USB_OTGSC_BSV_MASK                       (0x800U)
#define USB_OTGSC_BSV_SHIFT                      (11U)
#define USB_OTGSC_BSV(x)                         (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_BSV_SHIFT)) & USB_OTGSC_BSV_MASK)
#define USB_OTGSC_BSE_MASK                       (0x1000U)
#define USB_OTGSC_BSE_SHIFT                      (12U)
#define USB_OTGSC_BSE(x)                         (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_BSE_SHIFT)) & USB_OTGSC_BSE_MASK)
#define USB_OTGSC_TOG_1MS_MASK                   (0x2000U)
#define USB_OTGSC_TOG_1MS_SHIFT                  (13U)
#define USB_OTGSC_TOG_1MS(x)                     (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_TOG_1MS_SHIFT)) & USB_OTGSC_TOG_1MS_MASK)
#define USB_OTGSC_DPS_MASK                       (0x4000U)
#define USB_OTGSC_DPS_SHIFT                      (14U)
#define USB_OTGSC_DPS(x)                         (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_DPS_SHIFT)) & USB_OTGSC_DPS_MASK)
#define USB_OTGSC_IDIS_MASK                      (0x10000U)
#define USB_OTGSC_IDIS_SHIFT                     (16U)
#define USB_OTGSC_IDIS(x)                        (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_IDIS_SHIFT)) & USB_OTGSC_IDIS_MASK)
#define USB_OTGSC_AVVIS_MASK                     (0x20000U)
#define USB_OTGSC_AVVIS_SHIFT                    (17U)
#define USB_OTGSC_AVVIS(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_AVVIS_SHIFT)) & USB_OTGSC_AVVIS_MASK)
#define USB_OTGSC_ASVIS_MASK                     (0x40000U)
#define USB_OTGSC_ASVIS_SHIFT                    (18U)
#define USB_OTGSC_ASVIS(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_ASVIS_SHIFT)) & USB_OTGSC_ASVIS_MASK)
#define USB_OTGSC_BSVIS_MASK                     (0x80000U)
#define USB_OTGSC_BSVIS_SHIFT                    (19U)
#define USB_OTGSC_BSVIS(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_BSVIS_SHIFT)) & USB_OTGSC_BSVIS_MASK)
#define USB_OTGSC_BSEIS_MASK                     (0x100000U)
#define USB_OTGSC_BSEIS_SHIFT                    (20U)
#define USB_OTGSC_BSEIS(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_BSEIS_SHIFT)) & USB_OTGSC_BSEIS_MASK)
#define USB_OTGSC_STATUS_1MS_MASK                (0x200000U)
#define USB_OTGSC_STATUS_1MS_SHIFT               (21U)
#define USB_OTGSC_STATUS_1MS(x)                  (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_STATUS_1MS_SHIFT)) & USB_OTGSC_STATUS_1MS_MASK)
#define USB_OTGSC_DPIS_MASK                      (0x400000U)
#define USB_OTGSC_DPIS_SHIFT                     (22U)
#define USB_OTGSC_DPIS(x)                        (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_DPIS_SHIFT)) & USB_OTGSC_DPIS_MASK)
#define USB_OTGSC_IDIE_MASK                      (0x1000000U)
#define USB_OTGSC_IDIE_SHIFT                     (24U)
#define USB_OTGSC_IDIE(x)                        (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_IDIE_SHIFT)) & USB_OTGSC_IDIE_MASK)
#define USB_OTGSC_AVVIE_MASK                     (0x2000000U)
#define USB_OTGSC_AVVIE_SHIFT                    (25U)
#define USB_OTGSC_AVVIE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_AVVIE_SHIFT)) & USB_OTGSC_AVVIE_MASK)
#define USB_OTGSC_ASVIE_MASK                     (0x4000000U)
#define USB_OTGSC_ASVIE_SHIFT                    (26U)
#define USB_OTGSC_ASVIE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_ASVIE_SHIFT)) & USB_OTGSC_ASVIE_MASK)
#define USB_OTGSC_BSVIE_MASK                     (0x8000000U)
#define USB_OTGSC_BSVIE_SHIFT                    (27U)
#define USB_OTGSC_BSVIE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_BSVIE_SHIFT)) & USB_OTGSC_BSVIE_MASK)
#define USB_OTGSC_BSEIE_MASK                     (0x10000000U)
#define USB_OTGSC_BSEIE_SHIFT                    (28U)
#define USB_OTGSC_BSEIE(x)                       (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_BSEIE_SHIFT)) & USB_OTGSC_BSEIE_MASK)
#define USB_OTGSC_EN_1MS_MASK                    (0x20000000U)
#define USB_OTGSC_EN_1MS_SHIFT                   (29U)
#define USB_OTGSC_EN_1MS(x)                      (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_EN_1MS_SHIFT)) & USB_OTGSC_EN_1MS_MASK)
#define USB_OTGSC_DPIE_MASK                      (0x40000000U)
#define USB_OTGSC_DPIE_SHIFT                     (30U)
#define USB_OTGSC_DPIE(x)                        (((uint32_t)(((uint32_t)(x)) << USB_OTGSC_DPIE_SHIFT)) & USB_OTGSC_DPIE_MASK)
/*! @} */

/*! @name USBMODE - USB Device Mode */
/*! @{ */
#define USB_USBMODE_CM_MASK                      (0x3U)
#define USB_USBMODE_CM_SHIFT                     (0U)
/*! CM
 *  0b00..Idle [Default for combination host/device]
 *  0b01..Reserved
 *  0b10..Device Controller [Default for device only controller]
 *  0b11..Host Controller [Default for host only controller]
 */
#define USB_USBMODE_CM(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBMODE_CM_SHIFT)) & USB_USBMODE_CM_MASK)
#define USB_USBMODE_ES_MASK                      (0x4U)
#define USB_USBMODE_ES_SHIFT                     (2U)
/*! ES
 *  0b0..Little Endian [Default]
 *  0b1..Big Endian
 */
#define USB_USBMODE_ES(x)                        (((uint32_t)(((uint32_t)(x)) << USB_USBMODE_ES_SHIFT)) & USB_USBMODE_ES_MASK)
#define USB_USBMODE_SLOM_MASK                    (0x8U)
#define USB_USBMODE_SLOM_SHIFT                   (3U)
/*! SLOM
 *  0b0..Setup Lockouts On (default);
 *  0b1..Setup Lockouts Off (DCD requires use of Setup Data Buffer Tripwire in USBCMDUSB Command Register .
 */
#define USB_USBMODE_SLOM(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBMODE_SLOM_SHIFT)) & USB_USBMODE_SLOM_MASK)
#define USB_USBMODE_SDIS_MASK                    (0x10U)
#define USB_USBMODE_SDIS_SHIFT                   (4U)
#define USB_USBMODE_SDIS(x)                      (((uint32_t)(((uint32_t)(x)) << USB_USBMODE_SDIS_SHIFT)) & USB_USBMODE_SDIS_MASK)
/*! @} */

/*! @name ENDPTSETUPSTAT - Endpoint Setup Status */
/*! @{ */
#define USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT_MASK   (0xFFFFU)
#define USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT_SHIFT  (0U)
#define USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT(x)     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT_SHIFT)) & USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT_MASK)
/*! @} */

/*! @name ENDPTPRIME - Endpoint Prime */
/*! @{ */
#define USB_ENDPTPRIME_PERB_MASK                 (0xFFU)
#define USB_ENDPTPRIME_PERB_SHIFT                (0U)
#define USB_ENDPTPRIME_PERB(x)                   (((uint32_t)(((uint32_t)(x)) << USB_ENDPTPRIME_PERB_SHIFT)) & USB_ENDPTPRIME_PERB_MASK)
#define USB_ENDPTPRIME_PETB_MASK                 (0xFF0000U)
#define USB_ENDPTPRIME_PETB_SHIFT                (16U)
#define USB_ENDPTPRIME_PETB(x)                   (((uint32_t)(((uint32_t)(x)) << USB_ENDPTPRIME_PETB_SHIFT)) & USB_ENDPTPRIME_PETB_MASK)
/*! @} */

/*! @name ENDPTFLUSH - Endpoint Flush */
/*! @{ */
#define USB_ENDPTFLUSH_FERB_MASK                 (0xFFU)
#define USB_ENDPTFLUSH_FERB_SHIFT                (0U)
#define USB_ENDPTFLUSH_FERB(x)                   (((uint32_t)(((uint32_t)(x)) << USB_ENDPTFLUSH_FERB_SHIFT)) & USB_ENDPTFLUSH_FERB_MASK)
#define USB_ENDPTFLUSH_FETB_MASK                 (0xFF0000U)
#define USB_ENDPTFLUSH_FETB_SHIFT                (16U)
#define USB_ENDPTFLUSH_FETB(x)                   (((uint32_t)(((uint32_t)(x)) << USB_ENDPTFLUSH_FETB_SHIFT)) & USB_ENDPTFLUSH_FETB_MASK)
/*! @} */

/*! @name ENDPTSTAT - Endpoint Status */
/*! @{ */
#define USB_ENDPTSTAT_ERBR_MASK                  (0xFFU)
#define USB_ENDPTSTAT_ERBR_SHIFT                 (0U)
#define USB_ENDPTSTAT_ERBR(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTSTAT_ERBR_SHIFT)) & USB_ENDPTSTAT_ERBR_MASK)
#define USB_ENDPTSTAT_ETBR_MASK                  (0xFF0000U)
#define USB_ENDPTSTAT_ETBR_SHIFT                 (16U)
#define USB_ENDPTSTAT_ETBR(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTSTAT_ETBR_SHIFT)) & USB_ENDPTSTAT_ETBR_MASK)
/*! @} */

/*! @name ENDPTCOMPLETE - Endpoint Complete */
/*! @{ */
#define USB_ENDPTCOMPLETE_ERCE_MASK              (0xFFU)
#define USB_ENDPTCOMPLETE_ERCE_SHIFT             (0U)
#define USB_ENDPTCOMPLETE_ERCE(x)                (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCOMPLETE_ERCE_SHIFT)) & USB_ENDPTCOMPLETE_ERCE_MASK)
#define USB_ENDPTCOMPLETE_ETCE_MASK              (0xFF0000U)
#define USB_ENDPTCOMPLETE_ETCE_SHIFT             (16U)
#define USB_ENDPTCOMPLETE_ETCE(x)                (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCOMPLETE_ETCE_SHIFT)) & USB_ENDPTCOMPLETE_ETCE_MASK)
/*! @} */

/*! @name ENDPTCTRL0 - Endpoint Control0 */
/*! @{ */
#define USB_ENDPTCTRL0_RXS_MASK                  (0x1U)
#define USB_ENDPTCTRL0_RXS_SHIFT                 (0U)
#define USB_ENDPTCTRL0_RXS(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL0_RXS_SHIFT)) & USB_ENDPTCTRL0_RXS_MASK)
#define USB_ENDPTCTRL0_RXT_MASK                  (0xCU)
#define USB_ENDPTCTRL0_RXT_SHIFT                 (2U)
#define USB_ENDPTCTRL0_RXT(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL0_RXT_SHIFT)) & USB_ENDPTCTRL0_RXT_MASK)
#define USB_ENDPTCTRL0_RXE_MASK                  (0x80U)
#define USB_ENDPTCTRL0_RXE_SHIFT                 (7U)
#define USB_ENDPTCTRL0_RXE(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL0_RXE_SHIFT)) & USB_ENDPTCTRL0_RXE_MASK)
#define USB_ENDPTCTRL0_TXS_MASK                  (0x10000U)
#define USB_ENDPTCTRL0_TXS_SHIFT                 (16U)
#define USB_ENDPTCTRL0_TXS(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL0_TXS_SHIFT)) & USB_ENDPTCTRL0_TXS_MASK)
#define USB_ENDPTCTRL0_TXT_MASK                  (0xC0000U)
#define USB_ENDPTCTRL0_TXT_SHIFT                 (18U)
#define USB_ENDPTCTRL0_TXT(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL0_TXT_SHIFT)) & USB_ENDPTCTRL0_TXT_MASK)
#define USB_ENDPTCTRL0_TXE_MASK                  (0x800000U)
#define USB_ENDPTCTRL0_TXE_SHIFT                 (23U)
#define USB_ENDPTCTRL0_TXE(x)                    (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL0_TXE_SHIFT)) & USB_ENDPTCTRL0_TXE_MASK)
/*! @} */

/*! @name ENDPTCTRL - Endpoint Control 1..Endpoint Control 7 */
/*! @{ */
#define USB_ENDPTCTRL_RXS_MASK                   (0x1U)
#define USB_ENDPTCTRL_RXS_SHIFT                  (0U)
#define USB_ENDPTCTRL_RXS(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_RXS_SHIFT)) & USB_ENDPTCTRL_RXS_MASK)
#define USB_ENDPTCTRL_RXD_MASK                   (0x2U)
#define USB_ENDPTCTRL_RXD_SHIFT                  (1U)
#define USB_ENDPTCTRL_RXD(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_RXD_SHIFT)) & USB_ENDPTCTRL_RXD_MASK)
#define USB_ENDPTCTRL_RXT_MASK                   (0xCU)
#define USB_ENDPTCTRL_RXT_SHIFT                  (2U)
#define USB_ENDPTCTRL_RXT(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_RXT_SHIFT)) & USB_ENDPTCTRL_RXT_MASK)
#define USB_ENDPTCTRL_RXI_MASK                   (0x20U)
#define USB_ENDPTCTRL_RXI_SHIFT                  (5U)
#define USB_ENDPTCTRL_RXI(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_RXI_SHIFT)) & USB_ENDPTCTRL_RXI_MASK)
#define USB_ENDPTCTRL_RXR_MASK                   (0x40U)
#define USB_ENDPTCTRL_RXR_SHIFT                  (6U)
#define USB_ENDPTCTRL_RXR(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_RXR_SHIFT)) & USB_ENDPTCTRL_RXR_MASK)
#define USB_ENDPTCTRL_RXE_MASK                   (0x80U)
#define USB_ENDPTCTRL_RXE_SHIFT                  (7U)
#define USB_ENDPTCTRL_RXE(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_RXE_SHIFT)) & USB_ENDPTCTRL_RXE_MASK)
#define USB_ENDPTCTRL_TXS_MASK                   (0x10000U)
#define USB_ENDPTCTRL_TXS_SHIFT                  (16U)
#define USB_ENDPTCTRL_TXS(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_TXS_SHIFT)) & USB_ENDPTCTRL_TXS_MASK)
#define USB_ENDPTCTRL_TXD_MASK                   (0x20000U)
#define USB_ENDPTCTRL_TXD_SHIFT                  (17U)
#define USB_ENDPTCTRL_TXD(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_TXD_SHIFT)) & USB_ENDPTCTRL_TXD_MASK)
#define USB_ENDPTCTRL_TXT_MASK                   (0xC0000U)
#define USB_ENDPTCTRL_TXT_SHIFT                  (18U)
#define USB_ENDPTCTRL_TXT(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_TXT_SHIFT)) & USB_ENDPTCTRL_TXT_MASK)
#define USB_ENDPTCTRL_TXI_MASK                   (0x200000U)
#define USB_ENDPTCTRL_TXI_SHIFT                  (21U)
#define USB_ENDPTCTRL_TXI(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_TXI_SHIFT)) & USB_ENDPTCTRL_TXI_MASK)
#define USB_ENDPTCTRL_TXR_MASK                   (0x400000U)
#define USB_ENDPTCTRL_TXR_SHIFT                  (22U)
#define USB_ENDPTCTRL_TXR(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_TXR_SHIFT)) & USB_ENDPTCTRL_TXR_MASK)
#define USB_ENDPTCTRL_TXE_MASK                   (0x800000U)
#define USB_ENDPTCTRL_TXE_SHIFT                  (23U)
#define USB_ENDPTCTRL_TXE(x)                     (((uint32_t)(((uint32_t)(x)) << USB_ENDPTCTRL_TXE_SHIFT)) & USB_ENDPTCTRL_TXE_MASK)
/*! @} */

/* Backward compatibility */
#define GPTIMER0CTL                              GPTIMER0CTRL
#define GPTIMER1CTL                              GPTIMER1CTRL
#define USB_SBUSCFG                              SBUSCFG
#define EPLISTADDR                               ENDPTLISTADDR
#define EPSETUPSR                                ENDPTSETUPSTAT
#define EPPRIME                                  ENDPTPRIME
#define EPFLUSH                                  ENDPTFLUSH
#define EPSR                                     ENDPTSTAT
#define EPCOMPLETE                               ENDPTCOMPLETE
#define EPCR                                     ENDPTCTRL
#define EPCR0                                    ENDPTCTRL0
#define USBHS_ID_ID_MASK                         USB_ID_ID_MASK
#define USBHS_ID_ID_SHIFT                        USB_ID_ID_SHIFT
#define USBHS_ID_ID(x)                           USB_ID_ID(x)
#define USBHS_ID_NID_MASK                        USB_ID_NID_MASK
#define USBHS_ID_NID_SHIFT                       USB_ID_NID_SHIFT
#define USBHS_ID_NID(x)                          USB_ID_NID(x)
#define USBHS_ID_REVISION_MASK                   USB_ID_REVISION_MASK
#define USBHS_ID_REVISION_SHIFT                  USB_ID_REVISION_SHIFT
#define USBHS_ID_REVISION(x)                     USB_ID_REVISION(x)
#define USBHS_HWGENERAL_PHYW_MASK                USB_HWGENERAL_PHYW_MASK
#define USBHS_HWGENERAL_PHYW_SHIFT               USB_HWGENERAL_PHYW_SHIFT
#define USBHS_HWGENERAL_PHYW(x)                  USB_HWGENERAL_PHYW(x)
#define USBHS_HWGENERAL_PHYM_MASK                USB_HWGENERAL_PHYM_MASK
#define USBHS_HWGENERAL_PHYM_SHIFT               USB_HWGENERAL_PHYM_SHIFT
#define USBHS_HWGENERAL_PHYM(x)                  USB_HWGENERAL_PHYM(x)
#define USBHS_HWGENERAL_SM_MASK                  USB_HWGENERAL_SM_MASK
#define USBHS_HWGENERAL_SM_SHIFT                 USB_HWGENERAL_SM_SHIFT
#define USBHS_HWGENERAL_SM(x)                    USB_HWGENERAL_SM(x)
#define USBHS_HWHOST_HC_MASK                     USB_HWHOST_HC_MASK
#define USBHS_HWHOST_HC_SHIFT                    USB_HWHOST_HC_SHIFT
#define USBHS_HWHOST_HC(x)                       USB_HWHOST_HC(x)
#define USBHS_HWHOST_NPORT_MASK                  USB_HWHOST_NPORT_MASK
#define USBHS_HWHOST_NPORT_SHIFT                 USB_HWHOST_NPORT_SHIFT
#define USBHS_HWHOST_NPORT(x)                    USB_HWHOST_NPORT(x)
#define USBHS_HWDEVICE_DC_MASK                   USB_HWDEVICE_DC_MASK
#define USBHS_HWDEVICE_DC_SHIFT                  USB_HWDEVICE_DC_SHIFT
#define USBHS_HWDEVICE_DC(x)                     USB_HWDEVICE_DC(x)
#define USBHS_HWDEVICE_DEVEP_MASK                USB_HWDEVICE_DEVEP_MASK
#define USBHS_HWDEVICE_DEVEP_SHIFT               USB_HWDEVICE_DEVEP_SHIFT
#define USBHS_HWDEVICE_DEVEP(x)                  USB_HWDEVICE_DEVEP(x)
#define USBHS_HWTXBUF_TXBURST_MASK               USB_HWTXBUF_TXBURST_MASK
#define USBHS_HWTXBUF_TXBURST_SHIFT              USB_HWTXBUF_TXBURST_SHIFT
#define USBHS_HWTXBUF_TXBURST(x)                 USB_HWTXBUF_TXBURST(x)
#define USBHS_HWTXBUF_TXCHANADD_MASK             USB_HWTXBUF_TXCHANADD_MASK
#define USBHS_HWTXBUF_TXCHANADD_SHIFT            USB_HWTXBUF_TXCHANADD_SHIFT
#define USBHS_HWTXBUF_TXCHANADD(x)               USB_HWTXBUF_TXCHANADD(x)
#define USBHS_HWRXBUF_RXBURST_MASK               USB_HWRXBUF_RXBURST_MASK
#define USBHS_HWRXBUF_RXBURST_SHIFT              USB_HWRXBUF_RXBURST_SHIFT
#define USBHS_HWRXBUF_RXBURST(x)                 USB_HWRXBUF_RXBURST(x)
#define USBHS_HWRXBUF_RXADD_MASK                 USB_HWRXBUF_RXADD_MASK
#define USBHS_HWRXBUF_RXADD_SHIFT                USB_HWRXBUF_RXADD_SHIFT
#define USBHS_HWRXBUF_RXADD(x)                   USB_HWRXBUF_RXADD(x)
#define USBHS_GPTIMER0LD_GPTLD_MASK              USB_GPTIMER0LD_GPTLD_MASK
#define USBHS_GPTIMER0LD_GPTLD_SHIFT             USB_GPTIMER0LD_GPTLD_SHIFT
#define USBHS_GPTIMER0LD_GPTLD(x)                USB_GPTIMER0LD_GPTLD(x)
#define USBHS_GPTIMER0CTL_GPTCNT_MASK            USB_GPTIMER0CTRL_GPTCNT_MASK
#define USBHS_GPTIMER0CTL_GPTCNT_SHIFT           USB_GPTIMER0CTRL_GPTCNT_SHIFT
#define USBHS_GPTIMER0CTL_GPTCNT(x)              USB_GPTIMER0CTRL_GPTCNT(x)
#define USBHS_GPTIMER0CTL_MODE_MASK              USB_GPTIMER0CTRL_GPTMODE_MASK
#define USBHS_GPTIMER0CTL_MODE_SHIFT             USB_GPTIMER0CTRL_GPTMODE_SHIFT
#define USBHS_GPTIMER0CTL_MODE(x)                USB_GPTIMER0CTRL_GPTMODE(x)
#define USBHS_GPTIMER0CTL_RST_MASK               USB_GPTIMER0CTRL_GPTRST_MASK
#define USBHS_GPTIMER0CTL_RST_SHIFT              USB_GPTIMER0CTRL_GPTRST_SHIFT
#define USBHS_GPTIMER0CTL_RST(x)                 USB_GPTIMER0CTRL_GPTRST(x)
#define USBHS_GPTIMER0CTL_RUN_MASK               USB_GPTIMER0CTRL_GPTRUN_MASK
#define USBHS_GPTIMER0CTL_RUN_SHIFT              USB_GPTIMER0CTRL_GPTRUN_SHIFT
#define USBHS_GPTIMER0CTL_RUN(x)                 USB_GPTIMER0CTRL_GPTRUN(x)
#define USBHS_GPTIMER1LD_GPTLD_MASK              USB_GPTIMER1LD_GPTLD_MASK
#define USBHS_GPTIMER1LD_GPTLD_SHIFT             USB_GPTIMER1LD_GPTLD_SHIFT
#define USBHS_GPTIMER1LD_GPTLD(x)                USB_GPTIMER1LD_GPTLD(x)
#define USBHS_GPTIMER1CTL_GPTCNT_MASK            USB_GPTIMER1CTRL_GPTCNT_MASK
#define USBHS_GPTIMER1CTL_GPTCNT_SHIFT           USB_GPTIMER1CTRL_GPTCNT_SHIFT
#define USBHS_GPTIMER1CTL_GPTCNT(x)              USB_GPTIMER1CTRL_GPTCNT(x)
#define USBHS_GPTIMER1CTL_MODE_MASK              USB_GPTIMER1CTRL_GPTMODE_MASK
#define USBHS_GPTIMER1CTL_MODE_SHIFT             USB_GPTIMER1CTRL_GPTMODE_SHIFT
#define USBHS_GPTIMER1CTL_MODE(x)                USB_GPTIMER1CTRL_GPTMODE(x)
#define USBHS_GPTIMER1CTL_RST_MASK               USB_GPTIMER1CTRL_GPTRST_MASK
#define USBHS_GPTIMER1CTL_RST_SHIFT              USB_GPTIMER1CTRL_GPTRST_SHIFT
#define USBHS_GPTIMER1CTL_RST(x)                 USB_GPTIMER1CTRL_GPTRST(x)
#define USBHS_GPTIMER1CTL_RUN_MASK               USB_GPTIMER1CTRL_GPTRUN_MASK
#define USBHS_GPTIMER1CTL_RUN_SHIFT              USB_GPTIMER1CTRL_GPTRUN_SHIFT
#define USBHS_GPTIMER1CTL_RUN(x)                 USB_GPTIMER1CTRL_GPTRUN(x)
#define USBHS_USB_SBUSCFG_BURSTMODE_MASK         USB_SBUSCFG_AHBBRST_MASK
#define USBHS_USB_SBUSCFG_BURSTMODE_SHIFT        USB_SBUSCFG_AHBBRST_SHIFT
#define USBHS_USB_SBUSCFG_BURSTMODE(x)           USB_SBUSCFG_AHBBRST(x)
#define USBHS_HCIVERSION_CAPLENGTH(x)            USB_HCIVERSION_CAPLENGTH(x)
#define USBHS_HCIVERSION_HCIVERSION_MASK         USB_HCIVERSION_HCIVERSION_MASK
#define USBHS_HCIVERSION_HCIVERSION_SHIFT        USB_HCIVERSION_HCIVERSION_SHIFT
#define USBHS_HCIVERSION_HCIVERSION(x)           USB_HCIVERSION_HCIVERSION(x)
#define USBHS_HCSPARAMS_N_PORTS_MASK             USB_HCSPARAMS_N_PORTS_MASK
#define USBHS_HCSPARAMS_N_PORTS_SHIFT            USB_HCSPARAMS_N_PORTS_SHIFT
#define USBHS_HCSPARAMS_N_PORTS(x)               USB_HCSPARAMS_N_PORTS(x)
#define USBHS_HCSPARAMS_PPC_MASK                 USB_HCSPARAMS_PPC_MASK
#define USBHS_HCSPARAMS_PPC_SHIFT                USB_HCSPARAMS_PPC_SHIFT
#define USBHS_HCSPARAMS_PPC(x)                   USB_HCSPARAMS_PPC(x)
#define USBHS_HCSPARAMS_N_PCC_MASK               USB_HCSPARAMS_N_PCC_MASK
#define USBHS_HCSPARAMS_N_PCC_SHIFT              USB_HCSPARAMS_N_PCC_SHIFT
#define USBHS_HCSPARAMS_N_PCC(x)                 USB_HCSPARAMS_N_PCC(x)
#define USBHS_HCSPARAMS_N_CC_MASK                USB_HCSPARAMS_N_CC_MASK
#define USBHS_HCSPARAMS_N_CC_SHIFT               USB_HCSPARAMS_N_CC_SHIFT
#define USBHS_HCSPARAMS_N_CC(x)                  USB_HCSPARAMS_N_CC(x)
#define USBHS_HCSPARAMS_PI_MASK                  USB_HCSPARAMS_PI_MASK
#define USBHS_HCSPARAMS_PI_SHIFT                 USB_HCSPARAMS_PI_SHIFT
#define USBHS_HCSPARAMS_PI(x)                    USB_HCSPARAMS_PI(x)
#define USBHS_HCSPARAMS_N_PTT_MASK               USB_HCSPARAMS_N_PTT_MASK
#define USBHS_HCSPARAMS_N_PTT_SHIFT              USB_HCSPARAMS_N_PTT_SHIFT
#define USBHS_HCSPARAMS_N_PTT(x)                 USB_HCSPARAMS_N_PTT(x)
#define USBHS_HCSPARAMS_N_TT_MASK                USB_HCSPARAMS_N_TT_MASK
#define USBHS_HCSPARAMS_N_TT_SHIFT               USB_HCSPARAMS_N_TT_SHIFT
#define USBHS_HCSPARAMS_N_TT(x)                  USB_HCSPARAMS_N_TT(x)
#define USBHS_HCCPARAMS_ADC_MASK                 USB_HCCPARAMS_ADC_MASK
#define USBHS_HCCPARAMS_ADC_SHIFT                USB_HCCPARAMS_ADC_SHIFT
#define USBHS_HCCPARAMS_ADC(x)                   USB_HCCPARAMS_ADC(x)
#define USBHS_HCCPARAMS_PFL_MASK                 USB_HCCPARAMS_PFL_MASK
#define USBHS_HCCPARAMS_PFL_SHIFT                USB_HCCPARAMS_PFL_SHIFT
#define USBHS_HCCPARAMS_PFL(x)                   USB_HCCPARAMS_PFL(x)
#define USBHS_HCCPARAMS_ASP_MASK                 USB_HCCPARAMS_ASP_MASK
#define USBHS_HCCPARAMS_ASP_SHIFT                USB_HCCPARAMS_ASP_SHIFT
#define USBHS_HCCPARAMS_ASP(x)                   USB_HCCPARAMS_ASP(x)
#define USBHS_HCCPARAMS_IST_MASK                 USB_HCCPARAMS_IST_MASK
#define USBHS_HCCPARAMS_IST_SHIFT                USB_HCCPARAMS_IST_SHIFT
#define USBHS_HCCPARAMS_IST(x)                   USB_HCCPARAMS_IST(x)
#define USBHS_HCCPARAMS_EECP_MASK                USB_HCCPARAMS_EECP_MASK
#define USBHS_HCCPARAMS_EECP_SHIFT               USB_HCCPARAMS_EECP_SHIFT
#define USBHS_HCCPARAMS_EECP(x)                  USB_HCCPARAMS_EECP(x)
#define USBHS_DCIVERSION_DCIVERSION_MASK         USB_DCIVERSION_DCIVERSION_MASK
#define USBHS_DCIVERSION_DCIVERSION_SHIFT        USB_DCIVERSION_DCIVERSION_SHIFT
#define USBHS_DCIVERSION_DCIVERSION(x)           USB_DCIVERSION_DCIVERSION(x)
#define USBHS_DCCPARAMS_DEN_MASK                 USB_DCCPARAMS_DEN_MASK
#define USBHS_DCCPARAMS_DEN_SHIFT                USB_DCCPARAMS_DEN_SHIFT
#define USBHS_DCCPARAMS_DEN(x)                   USB_DCCPARAMS_DEN(x)
#define USBHS_DCCPARAMS_DC_MASK                  USB_DCCPARAMS_DC_MASK
#define USBHS_DCCPARAMS_DC_SHIFT                 USB_DCCPARAMS_DC_SHIFT
#define USBHS_DCCPARAMS_DC(x)                    USB_DCCPARAMS_DC(x)
#define USBHS_DCCPARAMS_HC_MASK                  USB_DCCPARAMS_HC_MASK
#define USBHS_DCCPARAMS_HC_SHIFT                 USB_DCCPARAMS_HC_SHIFT
#define USBHS_DCCPARAMS_HC(x)                    USB_DCCPARAMS_HC(x)
#define USBHS_USBCMD_RS_MASK                     USB_USBCMD_RS_MASK
#define USBHS_USBCMD_RS_SHIFT                    USB_USBCMD_RS_SHIFT
#define USBHS_USBCMD_RS(x)                       USB_USBCMD_RS(x)
#define USBHS_USBCMD_RST_MASK                    USB_USBCMD_RST_MASK
#define USBHS_USBCMD_RST_SHIFT                   USB_USBCMD_RST_SHIFT
#define USBHS_USBCMD_RST(x)                      USB_USBCMD_RST(x)
#define USBHS_USBCMD_FS_MASK                     USB_USBCMD_FS_1_MASK
#define USBHS_USBCMD_FS_SHIFT                    USB_USBCMD_FS_1_SHIFT
#define USBHS_USBCMD_FS(x)                       USB_USBCMD_FS_1(x)
#define USBHS_USBCMD_PSE_MASK                    USB_USBCMD_PSE_MASK
#define USBHS_USBCMD_PSE_SHIFT                   USB_USBCMD_PSE_SHIFT
#define USBHS_USBCMD_PSE(x)                      USB_USBCMD_PSE(x)
#define USBHS_USBCMD_ASE_MASK                    USB_USBCMD_ASE_MASK
#define USBHS_USBCMD_ASE_SHIFT                   USB_USBCMD_ASE_SHIFT
#define USBHS_USBCMD_ASE(x)                      USB_USBCMD_ASE(x)
#define USBHS_USBCMD_IAA_MASK                    USB_USBCMD_IAA_MASK
#define USBHS_USBCMD_IAA_SHIFT                   USB_USBCMD_IAA_SHIFT
#define USBHS_USBCMD_IAA(x)                      USB_USBCMD_IAA(x)
#define USBHS_USBCMD_ASP_MASK                    USB_USBCMD_ASP_MASK
#define USBHS_USBCMD_ASP_SHIFT                   USB_USBCMD_ASP_SHIFT
#define USBHS_USBCMD_ASP(x)                      USB_USBCMD_ASP(x)
#define USBHS_USBCMD_ASPE_MASK                   USB_USBCMD_ASPE_MASK
#define USBHS_USBCMD_ASPE_SHIFT                  USB_USBCMD_ASPE_SHIFT
#define USBHS_USBCMD_ASPE(x)                     USB_USBCMD_ASPE(x)
#define USBHS_USBCMD_ATDTW_MASK                  USB_USBCMD_ATDTW_MASK
#define USBHS_USBCMD_ATDTW_SHIFT                 USB_USBCMD_ATDTW_SHIFT
#define USBHS_USBCMD_ATDTW(x)                    USB_USBCMD_ATDTW(x)
#define USBHS_USBCMD_SUTW_MASK                   USB_USBCMD_SUTW_MASK
#define USBHS_USBCMD_SUTW_SHIFT                  USB_USBCMD_SUTW_SHIFT
#define USBHS_USBCMD_SUTW(x)                     USB_USBCMD_SUTW(x)
#define USBHS_USBCMD_FS2_MASK                    USB_USBCMD_FS_2_MASK
#define USBHS_USBCMD_FS2_SHIFT                   USB_USBCMD_FS_2_SHIFT
#define USBHS_USBCMD_FS2(x)                      USB_USBCMD_FS_2(x)
#define USBHS_USBCMD_ITC_MASK                    USB_USBCMD_ITC_MASK
#define USBHS_USBCMD_ITC_SHIFT                   USB_USBCMD_ITC_SHIFT
#define USBHS_USBCMD_ITC(x)                      USB_USBCMD_ITC(x)
#define USBHS_USBSTS_UI_MASK                     USB_USBSTS_UI_MASK
#define USBHS_USBSTS_UI_SHIFT                    USB_USBSTS_UI_SHIFT
#define USBHS_USBSTS_UI(x)                       USB_USBSTS_UI(x)
#define USBHS_USBSTS_UEI_MASK                    USB_USBSTS_UEI_MASK
#define USBHS_USBSTS_UEI_SHIFT                   USB_USBSTS_UEI_SHIFT
#define USBHS_USBSTS_UEI(x)                      USB_USBSTS_UEI(x)
#define USBHS_USBSTS_PCI_MASK                    USB_USBSTS_PCI_MASK
#define USBHS_USBSTS_PCI_SHIFT                   USB_USBSTS_PCI_SHIFT
#define USBHS_USBSTS_PCI(x)                      USB_USBSTS_PCI(x)
#define USBHS_USBSTS_FRI_MASK                    USB_USBSTS_FRI_MASK
#define USBHS_USBSTS_FRI_SHIFT                   USB_USBSTS_FRI_SHIFT
#define USBHS_USBSTS_FRI(x)                      USB_USBSTS_FRI(x)
#define USBHS_USBSTS_SEI_MASK                    USB_USBSTS_SEI_MASK
#define USBHS_USBSTS_SEI_SHIFT                   USB_USBSTS_SEI_SHIFT
#define USBHS_USBSTS_SEI(x)                      USB_USBSTS_SEI(x)
#define USBHS_USBSTS_AAI_MASK                    USB_USBSTS_AAI_MASK
#define USBHS_USBSTS_AAI_SHIFT                   USB_USBSTS_AAI_SHIFT
#define USBHS_USBSTS_AAI(x)                      USB_USBSTS_AAI(x)
#define USBHS_USBSTS_URI_MASK                    USB_USBSTS_URI_MASK
#define USBHS_USBSTS_URI_SHIFT                   USB_USBSTS_URI_SHIFT
#define USBHS_USBSTS_URI(x)                      USB_USBSTS_URI(x)
#define USBHS_USBSTS_SRI_MASK                    USB_USBSTS_SRI_MASK
#define USBHS_USBSTS_SRI_SHIFT                   USB_USBSTS_SRI_SHIFT
#define USBHS_USBSTS_SRI(x)                      USB_USBSTS_SRI(x)
#define USBHS_USBSTS_SLI_MASK                    USB_USBSTS_SLI_MASK
#define USBHS_USBSTS_SLI_SHIFT                   USB_USBSTS_SLI_SHIFT
#define USBHS_USBSTS_SLI(x)                      USB_USBSTS_SLI(x)
#define USBHS_USBSTS_ULPII_MASK                  USB_USBSTS_ULPII_MASK
#define USBHS_USBSTS_ULPII_SHIFT                 USB_USBSTS_ULPII_SHIFT
#define USBHS_USBSTS_ULPII(x)                    USB_USBSTS_ULPII(x)
#define USBHS_USBSTS_HCH_MASK                    USB_USBSTS_HCH_MASK
#define USBHS_USBSTS_HCH_SHIFT                   USB_USBSTS_HCH_SHIFT
#define USBHS_USBSTS_HCH(x)                      USB_USBSTS_HCH(x)
#define USBHS_USBSTS_RCL_MASK                    USB_USBSTS_RCL_MASK
#define USBHS_USBSTS_RCL_SHIFT                   USB_USBSTS_RCL_SHIFT
#define USBHS_USBSTS_RCL(x)                      USB_USBSTS_RCL(x)
#define USBHS_USBSTS_PS_MASK                     USB_USBSTS_PS_MASK
#define USBHS_USBSTS_PS_SHIFT                    USB_USBSTS_PS_SHIFT
#define USBHS_USBSTS_PS(x)                       USB_USBSTS_PS(x)
#define USBHS_USBSTS_AS_MASK                     USB_USBSTS_AS_MASK
#define USBHS_USBSTS_AS_SHIFT                    USB_USBSTS_AS_SHIFT
#define USBHS_USBSTS_AS(x)                       USB_USBSTS_AS(x)
#define USBHS_USBSTS_NAKI_MASK                   USB_USBSTS_NAKI_MASK
#define USBHS_USBSTS_NAKI_SHIFT                  USB_USBSTS_NAKI_SHIFT
#define USBHS_USBSTS_NAKI(x)                     USB_USBSTS_NAKI(x)
#define USBHS_USBSTS_TI0_MASK                    USB_USBSTS_TI0_MASK
#define USBHS_USBSTS_TI0_SHIFT                   USB_USBSTS_TI0_SHIFT
#define USBHS_USBSTS_TI0(x)                      USB_USBSTS_TI0(x)
#define USBHS_USBSTS_TI1_MASK                    USB_USBSTS_TI1_MASK
#define USBHS_USBSTS_TI1_SHIFT                   USB_USBSTS_TI1_SHIFT
#define USBHS_USBSTS_TI1(x)                      USB_USBSTS_TI1(x)
#define USBHS_USBINTR_UE_MASK                    USB_USBINTR_UE_MASK
#define USBHS_USBINTR_UE_SHIFT                   USB_USBINTR_UE_SHIFT
#define USBHS_USBINTR_UE(x)                      USB_USBINTR_UE(x)
#define USBHS_USBINTR_UEE_MASK                   USB_USBINTR_UEE_MASK
#define USBHS_USBINTR_UEE_SHIFT                  USB_USBINTR_UEE_SHIFT
#define USBHS_USBINTR_UEE(x)                     USB_USBINTR_UEE(x)
#define USBHS_USBINTR_PCE_MASK                   USB_USBINTR_PCE_MASK
#define USBHS_USBINTR_PCE_SHIFT                  USB_USBINTR_PCE_SHIFT
#define USBHS_USBINTR_PCE(x)                     USB_USBINTR_PCE(x)
#define USBHS_USBINTR_FRE_MASK                   USB_USBINTR_FRE_MASK
#define USBHS_USBINTR_FRE_SHIFT                  USB_USBINTR_FRE_SHIFT
#define USBHS_USBINTR_FRE(x)                     USB_USBINTR_FRE(x)
#define USBHS_USBINTR_SEE_MASK                   USB_USBINTR_SEE_MASK
#define USBHS_USBINTR_SEE_SHIFT                  USB_USBINTR_SEE_SHIFT
#define USBHS_USBINTR_SEE(x)                     USB_USBINTR_SEE(x)
#define USBHS_USBINTR_AAE_MASK                   USB_USBINTR_AAE_MASK
#define USBHS_USBINTR_AAE_SHIFT                  USB_USBINTR_AAE_SHIFT
#define USBHS_USBINTR_AAE(x)                     USB_USBINTR_AAE(x)
#define USBHS_USBINTR_URE_MASK                   USB_USBINTR_URE_MASK
#define USBHS_USBINTR_URE_SHIFT                  USB_USBINTR_URE_SHIFT
#define USBHS_USBINTR_URE(x)                     USB_USBINTR_URE(x)
#define USBHS_USBINTR_SRE_MASK                   USB_USBINTR_SRE_MASK
#define USBHS_USBINTR_SRE_SHIFT                  USB_USBINTR_SRE_SHIFT
#define USBHS_USBINTR_SRE(x)                     USB_USBINTR_SRE(x)
#define USBHS_USBINTR_SLE_MASK                   USB_USBINTR_SLE_MASK
#define USBHS_USBINTR_SLE_SHIFT                  USB_USBINTR_SLE_SHIFT
#define USBHS_USBINTR_SLE(x)                     USB_USBINTR_SLE(x)
#define USBHS_USBINTR_ULPIE_MASK                 USB_USBINTR_ULPIE_MASK
#define USBHS_USBINTR_ULPIE_SHIFT                USB_USBINTR_ULPIE_SHIFT
#define USBHS_USBINTR_ULPIE(x)                   USB_USBINTR_ULPIE(x)
#define USBHS_USBINTR_NAKE_MASK                  USB_USBINTR_NAKE_MASK
#define USBHS_USBINTR_NAKE_SHIFT                 USB_USBINTR_NAKE_SHIFT
#define USBHS_USBINTR_NAKE(x)                    USB_USBINTR_NAKE(x)
#define USBHS_USBINTR_UAIE_MASK                  USB_USBINTR_UAIE_MASK
#define USBHS_USBINTR_UAIE_SHIFT                 USB_USBINTR_UAIE_SHIFT
#define USBHS_USBINTR_UAIE(x)                    USB_USBINTR_UAIE(x)
#define USBHS_USBINTR_UPIE_MASK                  USB_USBINTR_UPIE_MASK
#define USBHS_USBINTR_UPIE_SHIFT                 USB_USBINTR_UPIE_SHIFT
#define USBHS_USBINTR_UPIE(x)                    USB_USBINTR_UPIE(x)
#define USBHS_USBINTR_TIE0_MASK                  USB_USBINTR_TIE0_MASK
#define USBHS_USBINTR_TIE0_SHIFT                 USB_USBINTR_TIE0_SHIFT
#define USBHS_USBINTR_TIE0(x)                    USB_USBINTR_TIE0(x)
#define USBHS_USBINTR_TIE1_MASK                  USB_USBINTR_TIE1_MASK
#define USBHS_USBINTR_TIE1_SHIFT                 USB_USBINTR_TIE1_SHIFT
#define USBHS_USBINTR_TIE1(x)                    USB_USBINTR_TIE1(x)
#define USBHS_FRINDEX_FRINDEX_MASK               USB_FRINDEX_FRINDEX_MASK
#define USBHS_FRINDEX_FRINDEX_SHIFT              USB_FRINDEX_FRINDEX_SHIFT
#define USBHS_FRINDEX_FRINDEX(x)                 USB_FRINDEX_FRINDEX(x)
#define USBHS_DEVICEADDR_USBADRA_MASK            USB_DEVICEADDR_USBADRA_MASK
#define USBHS_DEVICEADDR_USBADRA_SHIFT           USB_DEVICEADDR_USBADRA_SHIFT
#define USBHS_DEVICEADDR_USBADRA(x)              USB_DEVICEADDR_USBADRA(x)
#define USBHS_DEVICEADDR_USBADR_MASK             USB_DEVICEADDR_USBADR_MASK
#define USBHS_DEVICEADDR_USBADR_SHIFT            USB_DEVICEADDR_USBADR_SHIFT
#define USBHS_DEVICEADDR_USBADR(x)               USB_DEVICEADDR_USBADR(x)
#define USBHS_PERIODICLISTBASE_PERBASE_MASK      USB_PERIODICLISTBASE_BASEADR_MASK
#define USBHS_PERIODICLISTBASE_PERBASE_SHIFT     USB_PERIODICLISTBASE_BASEADR_SHIFT
#define USBHS_PERIODICLISTBASE_PERBASE(x)        USB_PERIODICLISTBASE_BASEADR(x)
#define USBHS_ASYNCLISTADDR_ASYBASE_MASK         USB_ASYNCLISTADDR_ASYBASE_MASK
#define USBHS_ASYNCLISTADDR_ASYBASE_SHIFT        USB_ASYNCLISTADDR_ASYBASE_SHIFT
#define USBHS_ASYNCLISTADDR_ASYBASE(x)           USB_ASYNCLISTADDR_ASYBASE(x)
#define USBHS_EPLISTADDR_EPBASE_MASK             USB_ENDPTLISTADDR_EPBASE_MASK
#define USBHS_EPLISTADDR_EPBASE_SHIFT            USB_ENDPTLISTADDR_EPBASE_SHIFT
#define USBHS_EPLISTADDR_EPBASE(x)               USB_ENDPTLISTADDR_EPBASE(x)
#define USBHS_BURSTSIZE_RXPBURST_MASK            USB_BURSTSIZE_RXPBURST_MASK
#define USBHS_BURSTSIZE_RXPBURST_SHIFT           USB_BURSTSIZE_RXPBURST_SHIFT
#define USBHS_BURSTSIZE_RXPBURST(x)              USB_BURSTSIZE_RXPBURST(x)
#define USBHS_BURSTSIZE_TXPBURST_MASK            USB_BURSTSIZE_TXPBURST_MASK
#define USBHS_BURSTSIZE_TXPBURST_SHIFT           USB_BURSTSIZE_TXPBURST_SHIFT
#define USBHS_BURSTSIZE_TXPBURST(x)              USB_BURSTSIZE_TXPBURST(x)
#define USBHS_TXFILLTUNING_TXSCHOH_MASK          USB_TXFILLTUNING_TXSCHOH_MASK
#define USBHS_TXFILLTUNING_TXSCHOH_SHIFT         USB_TXFILLTUNING_TXSCHOH_SHIFT
#define USBHS_TXFILLTUNING_TXSCHOH(x)            USB_TXFILLTUNING_TXSCHOH(x)
#define USBHS_TXFILLTUNING_TXSCHHEALTH_MASK      USB_TXFILLTUNING_TXSCHHEALTH_MASK
#define USBHS_TXFILLTUNING_TXSCHHEALTH_SHIFT     USB_TXFILLTUNING_TXSCHHEALTH_SHIFT
#define USBHS_TXFILLTUNING_TXSCHHEALTH(x)        USB_TXFILLTUNING_TXSCHHEALTH(x)
#define USBHS_TXFILLTUNING_TXFIFOTHRES_MASK      USB_TXFILLTUNING_TXFIFOTHRES_MASK
#define USBHS_TXFILLTUNING_TXFIFOTHRES_SHIFT     USB_TXFILLTUNING_TXFIFOTHRES_SHIFT
#define USBHS_TXFILLTUNING_TXFIFOTHRES(x)        USB_TXFILLTUNING_TXFIFOTHRES(x)
#define USBHS_ENDPTNAK_EPRN_MASK                 USB_ENDPTNAK_EPRN_MASK
#define USBHS_ENDPTNAK_EPRN_SHIFT                USB_ENDPTNAK_EPRN_SHIFT
#define USBHS_ENDPTNAK_EPRN(x)                   USB_ENDPTNAK_EPRN(x)
#define USBHS_ENDPTNAK_EPTN_MASK                 USB_ENDPTNAK_EPTN_MASK
#define USBHS_ENDPTNAK_EPTN_SHIFT                USB_ENDPTNAK_EPTN_SHIFT
#define USBHS_ENDPTNAK_EPTN(x)                   USB_ENDPTNAK_EPTN(x)
#define USBHS_ENDPTNAKEN_EPRNE_MASK              USB_ENDPTNAKEN_EPRNE_MASK
#define USBHS_ENDPTNAKEN_EPRNE_SHIFT             USB_ENDPTNAKEN_EPRNE_SHIFT
#define USBHS_ENDPTNAKEN_EPRNE(x)                USB_ENDPTNAKEN_EPRNE(x)
#define USBHS_ENDPTNAKEN_EPTNE_MASK              USB_ENDPTNAKEN_EPTNE_MASK
#define USBHS_ENDPTNAKEN_EPTNE_SHIFT             USB_ENDPTNAKEN_EPTNE_SHIFT
#define USBHS_ENDPTNAKEN_EPTNE(x)                USB_ENDPTNAKEN_EPTNE(x)
#define USBHS_CONFIGFLAG_CF_MASK                 USB_CONFIGFLAG_CF_MASK
#define USBHS_CONFIGFLAG_CF_SHIFT                USB_CONFIGFLAG_CF_SHIFT
#define USBHS_CONFIGFLAG_CF(x)                   USB_CONFIGFLAG_CF(x)
#define USBHS_PORTSC1_CCS_MASK                   USB_PORTSC1_CCS_MASK
#define USBHS_PORTSC1_CCS_SHIFT                  USB_PORTSC1_CCS_SHIFT
#define USBHS_PORTSC1_CCS(x)                     USB_PORTSC1_CCS(x)
#define USBHS_PORTSC1_CSC_MASK                   USB_PORTSC1_CSC_MASK
#define USBHS_PORTSC1_CSC_SHIFT                  USB_PORTSC1_CSC_SHIFT
#define USBHS_PORTSC1_CSC(x)                     USB_PORTSC1_CSC(x)
#define USBHS_PORTSC1_PE_MASK                    USB_PORTSC1_PE_MASK
#define USBHS_PORTSC1_PE_SHIFT                   USB_PORTSC1_PE_SHIFT
#define USBHS_PORTSC1_PE(x)                      USB_PORTSC1_PE(x)
#define USBHS_PORTSC1_PEC_MASK                   USB_PORTSC1_PEC_MASK
#define USBHS_PORTSC1_PEC_SHIFT                  USB_PORTSC1_PEC_SHIFT
#define USBHS_PORTSC1_PEC(x)                     USB_PORTSC1_PEC(x)
#define USBHS_PORTSC1_OCA_MASK                   USB_PORTSC1_OCA_MASK
#define USBHS_PORTSC1_OCA_SHIFT                  USB_PORTSC1_OCA_SHIFT
#define USBHS_PORTSC1_OCA(x)                     USB_PORTSC1_OCA(x)
#define USBHS_PORTSC1_OCC_MASK                   USB_PORTSC1_OCC_MASK
#define USBHS_PORTSC1_OCC_SHIFT                  USB_PORTSC1_OCC_SHIFT
#define USBHS_PORTSC1_OCC(x)                     USB_PORTSC1_OCC(x)
#define USBHS_PORTSC1_FPR_MASK                   USB_PORTSC1_FPR_MASK
#define USBHS_PORTSC1_FPR_SHIFT                  USB_PORTSC1_FPR_SHIFT
#define USBHS_PORTSC1_FPR(x)                     USB_PORTSC1_FPR(x)
#define USBHS_PORTSC1_SUSP_MASK                  USB_PORTSC1_SUSP_MASK
#define USBHS_PORTSC1_SUSP_SHIFT                 USB_PORTSC1_SUSP_SHIFT
#define USBHS_PORTSC1_SUSP(x)                    USB_PORTSC1_SUSP(x)
#define USBHS_PORTSC1_PR_MASK                    USB_PORTSC1_PR_MASK
#define USBHS_PORTSC1_PR_SHIFT                   USB_PORTSC1_PR_SHIFT
#define USBHS_PORTSC1_PR(x)                      USB_PORTSC1_PR(x)
#define USBHS_PORTSC1_HSP_MASK                   USB_PORTSC1_HSP_MASK
#define USBHS_PORTSC1_HSP_SHIFT                  USB_PORTSC1_HSP_SHIFT
#define USBHS_PORTSC1_HSP(x)                     USB_PORTSC1_HSP(x)
#define USBHS_PORTSC1_LS_MASK                    USB_PORTSC1_LS_MASK
#define USBHS_PORTSC1_LS_SHIFT                   USB_PORTSC1_LS_SHIFT
#define USBHS_PORTSC1_LS(x)                      USB_PORTSC1_LS(x)
#define USBHS_PORTSC1_PP_MASK                    USB_PORTSC1_PP_MASK
#define USBHS_PORTSC1_PP_SHIFT                   USB_PORTSC1_PP_SHIFT
#define USBHS_PORTSC1_PP(x)                      USB_PORTSC1_PP(x)
#define USBHS_PORTSC1_PO_MASK                    USB_PORTSC1_PO_MASK
#define USBHS_PORTSC1_PO_SHIFT                   USB_PORTSC1_PO_SHIFT
#define USBHS_PORTSC1_PO(x)                      USB_PORTSC1_PO(x)
#define USBHS_PORTSC1_PIC_MASK                   USB_PORTSC1_PIC_MASK
#define USBHS_PORTSC1_PIC_SHIFT                  USB_PORTSC1_PIC_SHIFT
#define USBHS_PORTSC1_PIC(x)                     USB_PORTSC1_PIC(x)
#define USBHS_PORTSC1_PTC_MASK                   USB_PORTSC1_PTC_MASK
#define USBHS_PORTSC1_PTC_SHIFT                  USB_PORTSC1_PTC_SHIFT
#define USBHS_PORTSC1_PTC(x)                     USB_PORTSC1_PTC(x)
#define USBHS_PORTSC1_WKCN_MASK                  USB_PORTSC1_WKCN_MASK
#define USBHS_PORTSC1_WKCN_SHIFT                 USB_PORTSC1_WKCN_SHIFT
#define USBHS_PORTSC1_WKCN(x)                    USB_PORTSC1_WKCN(x)
#define USBHS_PORTSC1_WKDS_MASK                  USB_PORTSC1_WKDC_MASK
#define USBHS_PORTSC1_WKDS_SHIFT                 USB_PORTSC1_WKDC_SHIFT
#define USBHS_PORTSC1_WKDS(x)                    USB_PORTSC1_WKDC(x)
#define USBHS_PORTSC1_WKOC_MASK                  USB_PORTSC1_WKOC_MASK
#define USBHS_PORTSC1_WKOC_SHIFT                 USB_PORTSC1_WKOC_SHIFT
#define USBHS_PORTSC1_WKOC(x)                    USB_PORTSC1_WKOC(x)
#define USBHS_PORTSC1_PHCD_MASK                  USB_PORTSC1_PHCD_MASK
#define USBHS_PORTSC1_PHCD_SHIFT                 USB_PORTSC1_PHCD_SHIFT
#define USBHS_PORTSC1_PHCD(x)                    USB_PORTSC1_PHCD(x)
#define USBHS_PORTSC1_PFSC_MASK                  USB_PORTSC1_PFSC_MASK
#define USBHS_PORTSC1_PFSC_SHIFT                 USB_PORTSC1_PFSC_SHIFT
#define USBHS_PORTSC1_PFSC(x)                    USB_PORTSC1_PFSC(x)
#define USBHS_PORTSC1_PTS2_MASK                  USB_PORTSC1_PTS_2_MASK
#define USBHS_PORTSC1_PTS2_SHIFT                 USB_PORTSC1_PTS_2_SHIFT
#define USBHS_PORTSC1_PTS2(x)                    USB_PORTSC1_PTS_2(x)
#define USBHS_PORTSC1_PSPD_MASK                  USB_PORTSC1_PSPD_MASK
#define USBHS_PORTSC1_PSPD_SHIFT                 USB_PORTSC1_PSPD_SHIFT
#define USBHS_PORTSC1_PSPD(x)                    USB_PORTSC1_PSPD(x)
#define USBHS_PORTSC1_PTW_MASK                   USB_PORTSC1_PTW_MASK
#define USBHS_PORTSC1_PTW_SHIFT                  USB_PORTSC1_PTW_SHIFT
#define USBHS_PORTSC1_PTW(x)                     USB_PORTSC1_PTW(x)
#define USBHS_PORTSC1_STS_MASK                   USB_PORTSC1_STS_MASK
#define USBHS_PORTSC1_STS_SHIFT                  USB_PORTSC1_STS_SHIFT
#define USBHS_PORTSC1_STS(x)                     USB_PORTSC1_STS(x)
#define USBHS_PORTSC1_PTS_MASK                   USB_PORTSC1_PTS_1_MASK
#define USBHS_PORTSC1_PTS_SHIFT                  USB_PORTSC1_PTS_1_SHIFT
#define USBHS_PORTSC1_PTS(x)                     USB_PORTSC1_PTS_1(x)
#define USBHS_OTGSC_VD_MASK                      USB_OTGSC_VD_MASK
#define USBHS_OTGSC_VD_SHIFT                     USB_OTGSC_VD_SHIFT
#define USBHS_OTGSC_VD(x)                        USB_OTGSC_VD(x)
#define USBHS_OTGSC_VC_MASK                      USB_OTGSC_VC_MASK
#define USBHS_OTGSC_VC_SHIFT                     USB_OTGSC_VC_SHIFT
#define USBHS_OTGSC_VC(x)                        USB_OTGSC_VC(x)
#define USBHS_OTGSC_OT_MASK                      USB_OTGSC_OT_MASK
#define USBHS_OTGSC_OT_SHIFT                     USB_OTGSC_OT_SHIFT
#define USBHS_OTGSC_OT(x)                        USB_OTGSC_OT(x)
#define USBHS_OTGSC_DP_MASK                      USB_OTGSC_DP_MASK
#define USBHS_OTGSC_DP_SHIFT                     USB_OTGSC_DP_SHIFT
#define USBHS_OTGSC_DP(x)                        USB_OTGSC_DP(x)
#define USBHS_OTGSC_IDPU_MASK                    USB_OTGSC_IDPU_MASK
#define USBHS_OTGSC_IDPU_SHIFT                   USB_OTGSC_IDPU_SHIFT
#define USBHS_OTGSC_IDPU(x)                      USB_OTGSC_IDPU(x)
#define USBHS_OTGSC_ID_MASK                      USB_OTGSC_ID_MASK
#define USBHS_OTGSC_ID_SHIFT                     USB_OTGSC_ID_SHIFT
#define USBHS_OTGSC_ID(x)                        USB_OTGSC_ID(x)
#define USBHS_OTGSC_AVV_MASK                     USB_OTGSC_AVV_MASK
#define USBHS_OTGSC_AVV_SHIFT                    USB_OTGSC_AVV_SHIFT
#define USBHS_OTGSC_AVV(x)                       USB_OTGSC_AVV(x)
#define USBHS_OTGSC_ASV_MASK                     USB_OTGSC_ASV_MASK
#define USBHS_OTGSC_ASV_SHIFT                    USB_OTGSC_ASV_SHIFT
#define USBHS_OTGSC_ASV(x)                       USB_OTGSC_ASV(x)
#define USBHS_OTGSC_BSV_MASK                     USB_OTGSC_BSV_MASK
#define USBHS_OTGSC_BSV_SHIFT                    USB_OTGSC_BSV_SHIFT
#define USBHS_OTGSC_BSV(x)                       USB_OTGSC_BSV(x)
#define USBHS_OTGSC_BSE_MASK                     USB_OTGSC_BSE_MASK
#define USBHS_OTGSC_BSE_SHIFT                    USB_OTGSC_BSE_SHIFT
#define USBHS_OTGSC_BSE(x)                       USB_OTGSC_BSE(x)
#define USBHS_OTGSC_MST_MASK                     USB_OTGSC_TOG_1MS_MASK
#define USBHS_OTGSC_MST_SHIFT                    USB_OTGSC_TOG_1MS_SHIFT
#define USBHS_OTGSC_MST(x)                       USB_OTGSC_TOG_1MS(x)
#define USBHS_OTGSC_DPS_MASK                     USB_OTGSC_DPS_MASK
#define USBHS_OTGSC_DPS_SHIFT                    USB_OTGSC_DPS_SHIFT
#define USBHS_OTGSC_DPS(x)                       USB_OTGSC_DPS(x)
#define USBHS_OTGSC_IDIS_MASK                    USB_OTGSC_IDIS_MASK
#define USBHS_OTGSC_IDIS_SHIFT                   USB_OTGSC_IDIS_SHIFT
#define USBHS_OTGSC_IDIS(x)                      USB_OTGSC_IDIS(x)
#define USBHS_OTGSC_AVVIS_MASK                   USB_OTGSC_AVVIS_MASK
#define USBHS_OTGSC_AVVIS_SHIFT                  USB_OTGSC_AVVIS_SHIFT
#define USBHS_OTGSC_AVVIS(x)                     USB_OTGSC_AVVIS(x)
#define USBHS_OTGSC_ASVIS_MASK                   USB_OTGSC_ASVIS_MASK
#define USBHS_OTGSC_ASVIS_SHIFT                  USB_OTGSC_ASVIS_SHIFT
#define USBHS_OTGSC_ASVIS(x)                     USB_OTGSC_ASVIS(x)
#define USBHS_OTGSC_BSVIS_MASK                   USB_OTGSC_BSVIS_MASK
#define USBHS_OTGSC_BSVIS_SHIFT                  USB_OTGSC_BSVIS_SHIFT
#define USBHS_OTGSC_BSVIS(x)                     USB_OTGSC_BSVIS(x)
#define USBHS_OTGSC_BSEIS_MASK                   USB_OTGSC_BSEIS_MASK
#define USBHS_OTGSC_BSEIS_SHIFT                  USB_OTGSC_BSEIS_SHIFT
#define USBHS_OTGSC_BSEIS(x)                     USB_OTGSC_BSEIS(x)
#define USBHS_OTGSC_MSS_MASK                     USB_OTGSC_STATUS_1MS_MASK
#define USBHS_OTGSC_MSS_SHIFT                    USB_OTGSC_STATUS_1MS_SHIFT
#define USBHS_OTGSC_MSS(x)                       USB_OTGSC_STATUS_1MS(x)
#define USBHS_OTGSC_DPIS_MASK                    USB_OTGSC_DPIS_MASK
#define USBHS_OTGSC_DPIS_SHIFT                   USB_OTGSC_DPIS_SHIFT
#define USBHS_OTGSC_DPIS(x)                      USB_OTGSC_DPIS(x)
#define USBHS_OTGSC_IDIE_MASK                    USB_OTGSC_IDIE_MASK
#define USBHS_OTGSC_IDIE_SHIFT                   USB_OTGSC_IDIE_SHIFT
#define USBHS_OTGSC_IDIE(x)                      USB_OTGSC_IDIE(x)
#define USBHS_OTGSC_AVVIE_MASK                   USB_OTGSC_AVVIE_MASK
#define USBHS_OTGSC_AVVIE_SHIFT                  USB_OTGSC_AVVIE_SHIFT
#define USBHS_OTGSC_AVVIE(x)                     USB_OTGSC_AVVIE(x)
#define USBHS_OTGSC_ASVIE_MASK                   USB_OTGSC_ASVIE_MASK
#define USBHS_OTGSC_ASVIE_SHIFT                  USB_OTGSC_ASVIE_SHIFT
#define USBHS_OTGSC_ASVIE(x)                     USB_OTGSC_ASVIE(x)
#define USBHS_OTGSC_BSVIE_MASK                   USB_OTGSC_BSVIE_MASK
#define USBHS_OTGSC_BSVIE_SHIFT                  USB_OTGSC_BSVIE_SHIFT
#define USBHS_OTGSC_BSVIE(x)                     USB_OTGSC_BSVIE(x)
#define USBHS_OTGSC_BSEIE_MASK                   USB_OTGSC_BSEIE_MASK
#define USBHS_OTGSC_BSEIE_SHIFT                  USB_OTGSC_BSEIE_SHIFT
#define USBHS_OTGSC_BSEIE(x)                     USB_OTGSC_BSEIE(x)
#define USBHS_OTGSC_MSE_MASK                     USB_OTGSC_EN_1MS_MASK
#define USBHS_OTGSC_MSE_SHIFT                    USB_OTGSC_EN_1MS_SHIFT
#define USBHS_OTGSC_MSE(x)                       USB_OTGSC_EN_1MS(x)
#define USBHS_OTGSC_DPIE_MASK                    USB_OTGSC_DPIE_MASK
#define USBHS_OTGSC_DPIE_SHIFT                   USB_OTGSC_DPIE_SHIFT
#define USBHS_OTGSC_DPIE(x)                      USB_OTGSC_DPIE(x)
#define USBHS_USBMODE_CM_MASK                    USB_USBMODE_CM_MASK
#define USBHS_USBMODE_CM_SHIFT                   USB_USBMODE_CM_SHIFT
#define USBHS_USBMODE_CM(x)                      USB_USBMODE_CM(x)
#define USBHS_USBMODE_ES_MASK                    USB_USBMODE_ES_MASK
#define USBHS_USBMODE_ES_SHIFT                   USB_USBMODE_ES_SHIFT
#define USBHS_USBMODE_ES(x)                      USB_USBMODE_ES(x)
#define USBHS_USBMODE_SLOM_MASK                  USB_USBMODE_SLOM_MASK
#define USBHS_USBMODE_SLOM_SHIFT                 USB_USBMODE_SLOM_SHIFT
#define USBHS_USBMODE_SLOM(x)                    USB_USBMODE_SLOM(x)
#define USBHS_USBMODE_SDIS_MASK                  USB_USBMODE_SDIS_MASK
#define USBHS_USBMODE_SDIS_SHIFT                 USB_USBMODE_SDIS_SHIFT
#define USBHS_USBMODE_SDIS(x)                    USB_USBMODE_SDIS(x)
#define USBHS_EPSETUPSR_EPSETUPSTAT_MASK         USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT_MASK
#define USBHS_EPSETUPSR_EPSETUPSTAT_SHIFT        USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT_SHIFT
#define USBHS_EPSETUPSR_EPSETUPSTAT(x)           USB_ENDPTSETUPSTAT_ENDPTSETUPSTAT(x)
#define USBHS_EPPRIME_PERB_MASK                  USB_ENDPTPRIME_PERB_MASK
#define USBHS_EPPRIME_PERB_SHIFT                 USB_ENDPTPRIME_PERB_SHIFT
#define USBHS_EPPRIME_PERB(x)                    USB_ENDPTPRIME_PERB(x)
#define USBHS_EPPRIME_PETB_MASK                  USB_ENDPTPRIME_PETB_MASK
#define USBHS_EPPRIME_PETB_SHIFT                 USB_ENDPTPRIME_PETB_SHIFT
#define USBHS_EPPRIME_PETB(x)                    USB_ENDPTPRIME_PETB(x)
#define USBHS_EPFLUSH_FERB_MASK                  USB_ENDPTFLUSH_FERB_MASK
#define USBHS_EPFLUSH_FERB_SHIFT                 USB_ENDPTFLUSH_FERB_SHIFT
#define USBHS_EPFLUSH_FERB(x)                    USB_ENDPTFLUSH_FERB(x)
#define USBHS_EPFLUSH_FETB_MASK                  USB_ENDPTFLUSH_FETB_MASK
#define USBHS_EPFLUSH_FETB_SHIFT                 USB_ENDPTFLUSH_FETB_SHIFT
#define USBHS_EPFLUSH_FETB(x)                    USB_ENDPTFLUSH_FETB(x)
#define USBHS_EPSR_ERBR_MASK                     USB_ENDPTSTAT_ERBR_MASK
#define USBHS_EPSR_ERBR_SHIFT                    USB_ENDPTSTAT_ERBR_SHIFT
#define USBHS_EPSR_ERBR(x)                       USB_ENDPTSTAT_ERBR(x)
#define USBHS_EPSR_ETBR_MASK                     USB_ENDPTSTAT_ETBR_MASK
#define USBHS_EPSR_ETBR_SHIFT                    USB_ENDPTSTAT_ETBR_SHIFT
#define USBHS_EPSR_ETBR(x)                       USB_ENDPTSTAT_ETBR(x)
#define USBHS_EPCOMPLETE_ERCE_MASK               USB_ENDPTCOMPLETE_ERCE_MASK
#define USBHS_EPCOMPLETE_ERCE_SHIFT              USB_ENDPTCOMPLETE_ERCE_SHIFT
#define USBHS_EPCOMPLETE_ERCE(x)                 USB_ENDPTCOMPLETE_ERCE(x)
#define USBHS_EPCOMPLETE_ETCE_MASK               USB_ENDPTCOMPLETE_ETCE_MASK
#define USBHS_EPCOMPLETE_ETCE_SHIFT              USB_ENDPTCOMPLETE_ETCE_SHIFT
#define USBHS_EPCOMPLETE_ETCE(x)                 USB_ENDPTCOMPLETE_ETCE(x)
#define USBHS_EPCR0_RXS_MASK                     USB_ENDPTCTRL0_RXS_MASK
#define USBHS_EPCR0_RXS_SHIFT                    USB_ENDPTCTRL0_RXS_SHIFT
#define USBHS_EPCR0_RXS(x)                       USB_ENDPTCTRL0_RXS(x)
#define USBHS_EPCR0_RXT_MASK                     USB_ENDPTCTRL0_RXT_MASK
#define USBHS_EPCR0_RXT_SHIFT                    USB_ENDPTCTRL0_RXT_SHIFT
#define USBHS_EPCR0_RXT(x)                       USB_ENDPTCTRL0_RXT(x)
#define USBHS_EPCR0_RXE_MASK                     USB_ENDPTCTRL0_RXE_MASK
#define USBHS_EPCR0_RXE_SHIFT                    USB_ENDPTCTRL0_RXE_SHIFT
#define USBHS_EPCR0_RXE(x)                       USB_ENDPTCTRL0_RXE(x)
#define USBHS_EPCR0_TXS_MASK                     USB_ENDPTCTRL0_TXS_MASK
#define USBHS_EPCR0_TXS_SHIFT                    USB_ENDPTCTRL0_TXS_SHIFT
#define USBHS_EPCR0_TXS(x)                       USB_ENDPTCTRL0_TXS(x)
#define USBHS_EPCR0_TXT_MASK                     USB_ENDPTCTRL0_TXT_MASK
#define USBHS_EPCR0_TXT_SHIFT                    USB_ENDPTCTRL0_TXT_SHIFT
#define USBHS_EPCR0_TXT(x)                       USB_ENDPTCTRL0_TXT(x)
#define USBHS_EPCR0_TXE_MASK                     USB_ENDPTCTRL0_TXE_MASK
#define USBHS_EPCR0_TXE_SHIFT                    USB_ENDPTCTRL0_TXE_SHIFT
#define USBHS_EPCR0_TXE(x)                       USB_ENDPTCTRL0_TXE(x)
#define USBHS_EPCR_RXS_MASK                      USB_ENDPTCTRL_RXS_MASK
#define USBHS_EPCR_RXS_SHIFT                     USB_ENDPTCTRL_RXS_SHIFT
#define USBHS_EPCR_RXS(x)                        USB_ENDPTCTRL_RXS(x)
#define USBHS_EPCR_RXD_MASK                      USB_ENDPTCTRL_RXD_MASK
#define USBHS_EPCR_RXD_SHIFT                     USB_ENDPTCTRL_RXD_SHIFT
#define USBHS_EPCR_RXD(x)                        USB_ENDPTCTRL_RXD(x)
#define USBHS_EPCR_RXT_MASK                      USB_ENDPTCTRL_RXT_MASK
#define USBHS_EPCR_RXT_SHIFT                     USB_ENDPTCTRL_RXT_SHIFT
#define USBHS_EPCR_RXT(x)                        USB_ENDPTCTRL_RXT(x)
#define USBHS_EPCR_RXI_MASK                      USB_ENDPTCTRL_RXI_MASK
#define USBHS_EPCR_RXI_SHIFT                     USB_ENDPTCTRL_RXI_SHIFT
#define USBHS_EPCR_RXI(x)                        USB_ENDPTCTRL_RXI(x)
#define USBHS_EPCR_RXR_MASK                      USB_ENDPTCTRL_RXR_MASK
#define USBHS_EPCR_RXR_SHIFT                     USB_ENDPTCTRL_RXR_SHIFT
#define USBHS_EPCR_RXR(x)                        USB_ENDPTCTRL_RXR(x)
#define USBHS_EPCR_RXE_MASK                      USB_ENDPTCTRL_RXE_MASK
#define USBHS_EPCR_RXE_SHIFT                     USB_ENDPTCTRL_RXE_SHIFT
#define USBHS_EPCR_RXE(x)                        USB_ENDPTCTRL_RXE(x)
#define USBHS_EPCR_TXS_MASK                      USB_ENDPTCTRL_TXS_MASK
#define USBHS_EPCR_TXS_SHIFT                     USB_ENDPTCTRL_TXS_SHIFT
#define USBHS_EPCR_TXS(x)                        USB_ENDPTCTRL_TXS(x)
#define USBHS_EPCR_TXD_MASK                      USB_ENDPTCTRL_TXD_MASK
#define USBHS_EPCR_TXD_SHIFT                     USB_ENDPTCTRL_TXD_SHIFT
#define USBHS_EPCR_TXD(x)                        USB_ENDPTCTRL_TXD(x)
#define USBHS_EPCR_TXT_MASK                      USB_ENDPTCTRL_TXT_MASK
#define USBHS_EPCR_TXT_SHIFT                     USB_ENDPTCTRL_TXT_SHIFT
#define USBHS_EPCR_TXT(x)                        USB_ENDPTCTRL_TXT(x)
#define USBHS_EPCR_TXI_MASK                      USB_ENDPTCTRL_TXI_MASK
#define USBHS_EPCR_TXI_SHIFT                     USB_ENDPTCTRL_TXI_SHIFT
#define USBHS_EPCR_TXI(x)                        USB_ENDPTCTRL_TXI(x)
#define USBHS_EPCR_TXR_MASK                      USB_ENDPTCTRL_TXR_MASK
#define USBHS_EPCR_TXR_SHIFT                     USB_ENDPTCTRL_TXR_SHIFT
#define USBHS_EPCR_TXR(x)                        USB_ENDPTCTRL_TXR(x)
#define USBHS_EPCR_TXE_MASK                      USB_ENDPTCTRL_TXE_MASK
#define USBHS_EPCR_TXE_SHIFT                     USB_ENDPTCTRL_TXE_SHIFT
#define USBHS_EPCR_TXE(x)                        USB_ENDPTCTRL_TXE(x)
#define USBHS_BASE_ADDRS                         { IMXRT1020_USB_BASE }
#define USBHS_IRQS                               { USB_OTG1_IRQn }
#define USBHS_IRQHandler                         USB_OTG1_IRQHandler

/* ----------------------------------------------------------------------------
   -- USBNC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
* @addtogroup USBNC_Peripheral_Access_Layer USBNC Peripheral Access Layer
* @{
*/

/** USBNC - Register Layout Typedef */
typedef struct {

	uint8_t RESERVED_0[2048];

	volatile uint32_t USB_OTGn_CTRL;			/**< USB OTG1 Control Register, offset: 0x800 */

	uint8_t RESERVED_1[20];

	volatile uint32_t USB_OTGn_PHY_CTRL_0;			/**< OTG1 UTMI PHY Control 0 Register, offset: 0x818 */
} imxrt1020_usb_nc;

/* USBNC - Peripheral instance base addresses */
/** Peripheral USBNC base address */
#define IMXRT1020_USB_NC_BASE			(0x402E0000u)
/** Peripheral USBNC base pointer */
#define IMXRT1020_USB_NC			((imxrt1020_usb_nc *)IMXRT1020_USB_NC_BASE)
/** Array initializer of USBNC peripheral base addresses */
#define IIMXRT1020_USBNC_BASE_ADDRS		{ 0u, IMXRT1020_USB_NC_BASE }

/* ----------------------------------------------------------------------------
   -- USBNC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USBNC_Register_Masks USBNC Register Masks
 * @{
 */

/*! @name USB_OTGn_CTRL - USB OTG1 Control Register */
/*! @{ */
#define USBNC_USB_OTGn_CTRL_OVER_CUR_DIS_MASK    (0x80U)
#define USBNC_USB_OTGn_CTRL_OVER_CUR_DIS_SHIFT   (7U)
/*! OVER_CUR_DIS
 *  0b1..Disables overcurrent detection
 *  0b0..Enables overcurrent detection
 */
#define USBNC_USB_OTGn_CTRL_OVER_CUR_DIS(x)      (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_OVER_CUR_DIS_SHIFT)) & USBNC_USB_OTGn_CTRL_OVER_CUR_DIS_MASK)
#define USBNC_USB_OTGn_CTRL_OVER_CUR_POL_MASK    (0x100U)
#define USBNC_USB_OTGn_CTRL_OVER_CUR_POL_SHIFT   (8U)
/*! OVER_CUR_POL
 *  0b1..Low active (low on this signal represents an overcurrent condition)
 *  0b0..High active (high on this signal represents an overcurrent condition)
 */
#define USBNC_USB_OTGn_CTRL_OVER_CUR_POL(x)      (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_OVER_CUR_POL_SHIFT)) & USBNC_USB_OTGn_CTRL_OVER_CUR_POL_MASK)
#define USBNC_USB_OTGn_CTRL_PWR_POL_MASK         (0x200U)
#define USBNC_USB_OTGn_CTRL_PWR_POL_SHIFT        (9U)
/*! PWR_POL
 *  0b1..PMIC Power Pin is High active.
 *  0b0..PMIC Power Pin is Low active.
 */
#define USBNC_USB_OTGn_CTRL_PWR_POL(x)           (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_PWR_POL_SHIFT)) & USBNC_USB_OTGn_CTRL_PWR_POL_MASK)
#define USBNC_USB_OTGn_CTRL_WIE_MASK             (0x400U)
#define USBNC_USB_OTGn_CTRL_WIE_SHIFT            (10U)
/*! WIE
 *  0b1..Interrupt Enabled
 *  0b0..Interrupt Disabled
 */
#define USBNC_USB_OTGn_CTRL_WIE(x)               (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WIE_SHIFT)) & USBNC_USB_OTGn_CTRL_WIE_MASK)
#define USBNC_USB_OTGn_CTRL_WKUP_SW_EN_MASK      (0x4000U)
#define USBNC_USB_OTGn_CTRL_WKUP_SW_EN_SHIFT     (14U)
/*! WKUP_SW_EN
 *  0b1..Enable
 *  0b0..Disable
 */
#define USBNC_USB_OTGn_CTRL_WKUP_SW_EN(x)        (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WKUP_SW_EN_SHIFT)) & USBNC_USB_OTGn_CTRL_WKUP_SW_EN_MASK)
#define USBNC_USB_OTGn_CTRL_WKUP_SW_MASK         (0x8000U)
#define USBNC_USB_OTGn_CTRL_WKUP_SW_SHIFT        (15U)
/*! WKUP_SW
 *  0b1..Force wake-up
 *  0b0..Inactive
 */
#define USBNC_USB_OTGn_CTRL_WKUP_SW(x)           (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WKUP_SW_SHIFT)) & USBNC_USB_OTGn_CTRL_WKUP_SW_MASK)
#define USBNC_USB_OTGn_CTRL_WKUP_ID_EN_MASK      (0x10000U)
#define USBNC_USB_OTGn_CTRL_WKUP_ID_EN_SHIFT     (16U)
/*! WKUP_ID_EN
 *  0b1..Enable
 *  0b0..Disable
 */
#define USBNC_USB_OTGn_CTRL_WKUP_ID_EN(x)        (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WKUP_ID_EN_SHIFT)) & USBNC_USB_OTGn_CTRL_WKUP_ID_EN_MASK)
#define USBNC_USB_OTGn_CTRL_WKUP_VBUS_EN_MASK    (0x20000U)
#define USBNC_USB_OTGn_CTRL_WKUP_VBUS_EN_SHIFT   (17U)
/*! WKUP_VBUS_EN
 *  0b1..Enable
 *  0b0..Disable
 */
#define USBNC_USB_OTGn_CTRL_WKUP_VBUS_EN(x)      (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WKUP_VBUS_EN_SHIFT)) & USBNC_USB_OTGn_CTRL_WKUP_VBUS_EN_MASK)
#define USBNC_USB_OTGn_CTRL_WKUP_DPDM_EN_MASK    (0x20000000U)
#define USBNC_USB_OTGn_CTRL_WKUP_DPDM_EN_SHIFT   (29U)
/*! WKUP_DPDM_EN
 *  0b1..(Default) DPDM changes wake-up to be enabled, it is for device only.
 *  0b0..DPDM changes wake-up to be disabled only when VBUS is 0.
 */
#define USBNC_USB_OTGn_CTRL_WKUP_DPDM_EN(x)      (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WKUP_DPDM_EN_SHIFT)) & USBNC_USB_OTGn_CTRL_WKUP_DPDM_EN_MASK)
#define USBNC_USB_OTGn_CTRL_WIR_MASK             (0x80000000U)
#define USBNC_USB_OTGn_CTRL_WIR_SHIFT            (31U)
/*! WIR
 *  0b1..Wake-up Interrupt Request received
 *  0b0..No wake-up interrupt request received
 */
#define USBNC_USB_OTGn_CTRL_WIR(x)               (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_CTRL_WIR_SHIFT)) & USBNC_USB_OTGn_CTRL_WIR_MASK)
/*! @} */

/*! @name USB_OTGn_PHY_CTRL_0 - OTG1 UTMI PHY Control 0 Register */
/*! @{ */
#define USBNC_USB_OTGn_PHY_CTRL_0_UTMI_CLK_VLD_MASK (0x80000000U)
#define USBNC_USB_OTGn_PHY_CTRL_0_UTMI_CLK_VLD_SHIFT (31U)
/*! UTMI_CLK_VLD
 *  0b1..Valid
 *  0b0..Invalid
 */
#define USBNC_USB_OTGn_PHY_CTRL_0_UTMI_CLK_VLD(x) (((uint32_t)(((uint32_t)(x)) << USBNC_USB_OTGn_PHY_CTRL_0_UTMI_CLK_VLD_SHIFT)) & USBNC_USB_OTGn_PHY_CTRL_0_UTMI_CLK_VLD_MASK)
/*! @} */


/*!
 * @}
	 *//* end of group USBNC_Register_Masks */
/*!
 * @}
	 *//* end of group USB_Register_Masks */

/* ----------------------------------------------------------------------------
   -- USBPHY Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USBPHY_Peripheral_Access_Layer USBPHY Peripheral Access Layer
 * @{
 */
/** USBPHY - Register Layout Typedef */
typedef struct {

	volatile uint32_t PWD;						/**< USB PHY Power-Down Register, offset: 0x0 */

	volatile uint32_t PWD_SET;					/**< USB PHY Power-Down Register, offset: 0x4 */

	volatile uint32_t PWD_CLR;					/**< USB PHY Power-Down Register, offset: 0x8 */

	volatile uint32_t PWD_TOG;					/**< USB PHY Power-Down Register, offset: 0xC */

	volatile uint32_t TX;						/**< USB PHY Transmitter Control Register, offset: 0x10 */

	volatile uint32_t TX_SET;					/**< USB PHY Transmitter Control Register, offset: 0x14 */

	volatile uint32_t TX_CLR;					/**< USB PHY Transmitter Control Register, offset: 0x18 */

	volatile uint32_t TX_TOG;					/**< USB PHY Transmitter Control Register, offset: 0x1C */

	volatile uint32_t RX;						/**< USB PHY Receiver Control Register, offset: 0x20 */

	volatile uint32_t RX_SET;					/**< USB PHY Receiver Control Register, offset: 0x24 */

	volatile uint32_t RX_CLR;					/**< USB PHY Receiver Control Register, offset: 0x28 */

	volatile uint32_t RX_TOG;					/**< USB PHY Receiver Control Register, offset: 0x2C */

	volatile uint32_t CTRL;						/**< USB PHY General Control Register, offset: 0x30 */

	volatile uint32_t CTRL_SET;					/**< USB PHY General Control Register, offset: 0x34 */

	volatile uint32_t CTRL_CLR;					/**< USB PHY General Control Register, offset: 0x38 */

	volatile uint32_t CTRL_TOG;					/**< USB PHY General Control Register, offset: 0x3C */

	volatile uint32_t STATUS;					/**< USB PHY Status Register, offset: 0x40 */

	uint8_t RESERVED_0[12];

	volatile uint32_t DEBUGr;					/**< USB PHY Debug Register, offset: 0x50 */

	volatile uint32_t DEBUG_SET;					/**< USB PHY Debug Register, offset: 0x54 */

	volatile uint32_t DEBUG_CLR;					/**< USB PHY Debug Register, offset: 0x58 */

	volatile uint32_t DEBUG_TOG;					/**< USB PHY Debug Register, offset: 0x5C */

	volatile const uint32_t DEBUG0_STATUS;				/**< UTMI Debug Status Register 0, offset: 0x60 */

	uint8_t RESERVED_1[12];

	volatile uint32_t DEBUG1;					/**< UTMI Debug Status Register 1, offset: 0x70 */

	volatile uint32_t DEBUG1_SET;					/**< UTMI Debug Status Register 1, offset: 0x74 */

	volatile uint32_t DEBUG1_CLR;					/**< UTMI Debug Status Register 1, offset: 0x78 */

	volatile uint32_t DEBUG1_TOG;					/**< UTMI Debug Status Register 1, offset: 0x7C */

	volatile const uint32_t VERSION;				/**< UTMI RTL Version, offset: 0x80 */
} imxrt1020_usb_phy;

/* USBPHY - Peripheral instance base addresses */
/** Peripheral USBPHY base address */

#define IMXRT1020_USB_PHY_IRQn					(65)											/**< USBPHY (OTG1 UTMI), Interrupt */
#define IMXRT1020_USB_PHY_BASE					(0x400D9000u)
/** Peripheral USBPHY base pointer */
#define IMXRT1020_USB_PHY					((imxrt1020_usb_phy *)IMXRT1020_USB_PHY_BASE)
/** Array initializer of USBPHY peripheral base addresses */
#define IMXRT1020_USB_PHY_BASE_ADDRS				{ 0u, IMXRT1020_USB_PHY_BASE }
/** Array initializer of USBPHY peripheral base pointers */
#define IMXRT1020_USBPHY_BASE_PTRS				{ (imxrt1020_usb_phy *)0u, IMXRT1020_USB_PHY }
/** Interrupt vectors for the USBPHY peripheral type */
#define IMXRT1020_USBPHY_IRQS					{ NotAvail_IRQn, IMXRT1020_USB_PHY_IRQn }

/* ----------------------------------------------------------------------------
   -- USBPHY Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USBPHY_Register_Masks USBPHY Register Masks
 * @{
 */

/*! @name PWD - USB PHY Power-Down Register */
/*! @{ */
#define USBPHY_PWD_RSVD0_MASK                    (0x3FFU)
#define USBPHY_PWD_RSVD0_SHIFT                   (0U)
#define USBPHY_PWD_RSVD0(x)                      (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RSVD0_SHIFT)) & USBPHY_PWD_RSVD0_MASK)
#define USBPHY_PWD_TXPWDFS_MASK                  (0x400U)
#define USBPHY_PWD_TXPWDFS_SHIFT                 (10U)
#define USBPHY_PWD_TXPWDFS(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TXPWDFS_SHIFT)) & USBPHY_PWD_TXPWDFS_MASK)
#define USBPHY_PWD_TXPWDIBIAS_MASK               (0x800U)
#define USBPHY_PWD_TXPWDIBIAS_SHIFT              (11U)
#define USBPHY_PWD_TXPWDIBIAS(x)                 (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TXPWDIBIAS_SHIFT)) & USBPHY_PWD_TXPWDIBIAS_MASK)
#define USBPHY_PWD_TXPWDV2I_MASK                 (0x1000U)
#define USBPHY_PWD_TXPWDV2I_SHIFT                (12U)
#define USBPHY_PWD_TXPWDV2I(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TXPWDV2I_SHIFT)) & USBPHY_PWD_TXPWDV2I_MASK)
#define USBPHY_PWD_RSVD1_MASK                    (0x1E000U)
#define USBPHY_PWD_RSVD1_SHIFT                   (13U)
#define USBPHY_PWD_RSVD1(x)                      (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RSVD1_SHIFT)) & USBPHY_PWD_RSVD1_MASK)
#define USBPHY_PWD_RXPWDENV_MASK                 (0x20000U)
#define USBPHY_PWD_RXPWDENV_SHIFT                (17U)
#define USBPHY_PWD_RXPWDENV(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RXPWDENV_SHIFT)) & USBPHY_PWD_RXPWDENV_MASK)
#define USBPHY_PWD_RXPWD1PT1_MASK                (0x40000U)
#define USBPHY_PWD_RXPWD1PT1_SHIFT               (18U)
#define USBPHY_PWD_RXPWD1PT1(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RXPWD1PT1_SHIFT)) & USBPHY_PWD_RXPWD1PT1_MASK)
#define USBPHY_PWD_RXPWDDIFF_MASK                (0x80000U)
#define USBPHY_PWD_RXPWDDIFF_SHIFT               (19U)
#define USBPHY_PWD_RXPWDDIFF(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RXPWDDIFF_SHIFT)) & USBPHY_PWD_RXPWDDIFF_MASK)
#define USBPHY_PWD_RXPWDRX_MASK                  (0x100000U)
#define USBPHY_PWD_RXPWDRX_SHIFT                 (20U)
#define USBPHY_PWD_RXPWDRX(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RXPWDRX_SHIFT)) & USBPHY_PWD_RXPWDRX_MASK)
#define USBPHY_PWD_RSVD2_MASK                    (0xFFE00000U)
#define USBPHY_PWD_RSVD2_SHIFT                   (21U)
#define USBPHY_PWD_RSVD2(x)                      (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_RSVD2_SHIFT)) & USBPHY_PWD_RSVD2_MASK)
/*! @} */

/*! @name PWD_SET - USB PHY Power-Down Register */
/*! @{ */
#define USBPHY_PWD_SET_RSVD0_MASK                (0x3FFU)
#define USBPHY_PWD_SET_RSVD0_SHIFT               (0U)
#define USBPHY_PWD_SET_RSVD0(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RSVD0_SHIFT)) & USBPHY_PWD_SET_RSVD0_MASK)
#define USBPHY_PWD_SET_TXPWDFS_MASK              (0x400U)
#define USBPHY_PWD_SET_TXPWDFS_SHIFT             (10U)
#define USBPHY_PWD_SET_TXPWDFS(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_TXPWDFS_SHIFT)) & USBPHY_PWD_SET_TXPWDFS_MASK)
#define USBPHY_PWD_SET_TXPWDIBIAS_MASK           (0x800U)
#define USBPHY_PWD_SET_TXPWDIBIAS_SHIFT          (11U)
#define USBPHY_PWD_SET_TXPWDIBIAS(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_TXPWDIBIAS_SHIFT)) & USBPHY_PWD_SET_TXPWDIBIAS_MASK)
#define USBPHY_PWD_SET_TXPWDV2I_MASK             (0x1000U)
#define USBPHY_PWD_SET_TXPWDV2I_SHIFT            (12U)
#define USBPHY_PWD_SET_TXPWDV2I(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_TXPWDV2I_SHIFT)) & USBPHY_PWD_SET_TXPWDV2I_MASK)
#define USBPHY_PWD_SET_RSVD1_MASK                (0x1E000U)
#define USBPHY_PWD_SET_RSVD1_SHIFT               (13U)
#define USBPHY_PWD_SET_RSVD1(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RSVD1_SHIFT)) & USBPHY_PWD_SET_RSVD1_MASK)
#define USBPHY_PWD_SET_RXPWDENV_MASK             (0x20000U)
#define USBPHY_PWD_SET_RXPWDENV_SHIFT            (17U)
#define USBPHY_PWD_SET_RXPWDENV(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RXPWDENV_SHIFT)) & USBPHY_PWD_SET_RXPWDENV_MASK)
#define USBPHY_PWD_SET_RXPWD1PT1_MASK            (0x40000U)
#define USBPHY_PWD_SET_RXPWD1PT1_SHIFT           (18U)
#define USBPHY_PWD_SET_RXPWD1PT1(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RXPWD1PT1_SHIFT)) & USBPHY_PWD_SET_RXPWD1PT1_MASK)
#define USBPHY_PWD_SET_RXPWDDIFF_MASK            (0x80000U)
#define USBPHY_PWD_SET_RXPWDDIFF_SHIFT           (19U)
#define USBPHY_PWD_SET_RXPWDDIFF(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RXPWDDIFF_SHIFT)) & USBPHY_PWD_SET_RXPWDDIFF_MASK)
#define USBPHY_PWD_SET_RXPWDRX_MASK              (0x100000U)
#define USBPHY_PWD_SET_RXPWDRX_SHIFT             (20U)
#define USBPHY_PWD_SET_RXPWDRX(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RXPWDRX_SHIFT)) & USBPHY_PWD_SET_RXPWDRX_MASK)
#define USBPHY_PWD_SET_RSVD2_MASK                (0xFFE00000U)
#define USBPHY_PWD_SET_RSVD2_SHIFT               (21U)
#define USBPHY_PWD_SET_RSVD2(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_SET_RSVD2_SHIFT)) & USBPHY_PWD_SET_RSVD2_MASK)
/*! @} */

/*! @name PWD_CLR - USB PHY Power-Down Register */
/*! @{ */
#define USBPHY_PWD_CLR_RSVD0_MASK                (0x3FFU)
#define USBPHY_PWD_CLR_RSVD0_SHIFT               (0U)
#define USBPHY_PWD_CLR_RSVD0(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RSVD0_SHIFT)) & USBPHY_PWD_CLR_RSVD0_MASK)
#define USBPHY_PWD_CLR_TXPWDFS_MASK              (0x400U)
#define USBPHY_PWD_CLR_TXPWDFS_SHIFT             (10U)
#define USBPHY_PWD_CLR_TXPWDFS(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_TXPWDFS_SHIFT)) & USBPHY_PWD_CLR_TXPWDFS_MASK)
#define USBPHY_PWD_CLR_TXPWDIBIAS_MASK           (0x800U)
#define USBPHY_PWD_CLR_TXPWDIBIAS_SHIFT          (11U)
#define USBPHY_PWD_CLR_TXPWDIBIAS(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_TXPWDIBIAS_SHIFT)) & USBPHY_PWD_CLR_TXPWDIBIAS_MASK)
#define USBPHY_PWD_CLR_TXPWDV2I_MASK             (0x1000U)
#define USBPHY_PWD_CLR_TXPWDV2I_SHIFT            (12U)
#define USBPHY_PWD_CLR_TXPWDV2I(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_TXPWDV2I_SHIFT)) & USBPHY_PWD_CLR_TXPWDV2I_MASK)
#define USBPHY_PWD_CLR_RSVD1_MASK                (0x1E000U)
#define USBPHY_PWD_CLR_RSVD1_SHIFT               (13U)
#define USBPHY_PWD_CLR_RSVD1(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RSVD1_SHIFT)) & USBPHY_PWD_CLR_RSVD1_MASK)
#define USBPHY_PWD_CLR_RXPWDENV_MASK             (0x20000U)
#define USBPHY_PWD_CLR_RXPWDENV_SHIFT            (17U)
#define USBPHY_PWD_CLR_RXPWDENV(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RXPWDENV_SHIFT)) & USBPHY_PWD_CLR_RXPWDENV_MASK)
#define USBPHY_PWD_CLR_RXPWD1PT1_MASK            (0x40000U)
#define USBPHY_PWD_CLR_RXPWD1PT1_SHIFT           (18U)
#define USBPHY_PWD_CLR_RXPWD1PT1(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RXPWD1PT1_SHIFT)) & USBPHY_PWD_CLR_RXPWD1PT1_MASK)
#define USBPHY_PWD_CLR_RXPWDDIFF_MASK            (0x80000U)
#define USBPHY_PWD_CLR_RXPWDDIFF_SHIFT           (19U)
#define USBPHY_PWD_CLR_RXPWDDIFF(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RXPWDDIFF_SHIFT)) & USBPHY_PWD_CLR_RXPWDDIFF_MASK)
#define USBPHY_PWD_CLR_RXPWDRX_MASK              (0x100000U)
#define USBPHY_PWD_CLR_RXPWDRX_SHIFT             (20U)
#define USBPHY_PWD_CLR_RXPWDRX(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RXPWDRX_SHIFT)) & USBPHY_PWD_CLR_RXPWDRX_MASK)
#define USBPHY_PWD_CLR_RSVD2_MASK                (0xFFE00000U)
#define USBPHY_PWD_CLR_RSVD2_SHIFT               (21U)
#define USBPHY_PWD_CLR_RSVD2(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_CLR_RSVD2_SHIFT)) & USBPHY_PWD_CLR_RSVD2_MASK)
/*! @} */

/*! @name PWD_TOG - USB PHY Power-Down Register */
/*! @{ */
#define USBPHY_PWD_TOG_RSVD0_MASK                (0x3FFU)
#define USBPHY_PWD_TOG_RSVD0_SHIFT               (0U)
#define USBPHY_PWD_TOG_RSVD0(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RSVD0_SHIFT)) & USBPHY_PWD_TOG_RSVD0_MASK)
#define USBPHY_PWD_TOG_TXPWDFS_MASK              (0x400U)
#define USBPHY_PWD_TOG_TXPWDFS_SHIFT             (10U)
#define USBPHY_PWD_TOG_TXPWDFS(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_TXPWDFS_SHIFT)) & USBPHY_PWD_TOG_TXPWDFS_MASK)
#define USBPHY_PWD_TOG_TXPWDIBIAS_MASK           (0x800U)
#define USBPHY_PWD_TOG_TXPWDIBIAS_SHIFT          (11U)
#define USBPHY_PWD_TOG_TXPWDIBIAS(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_TXPWDIBIAS_SHIFT)) & USBPHY_PWD_TOG_TXPWDIBIAS_MASK)
#define USBPHY_PWD_TOG_TXPWDV2I_MASK             (0x1000U)
#define USBPHY_PWD_TOG_TXPWDV2I_SHIFT            (12U)
#define USBPHY_PWD_TOG_TXPWDV2I(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_TXPWDV2I_SHIFT)) & USBPHY_PWD_TOG_TXPWDV2I_MASK)
#define USBPHY_PWD_TOG_RSVD1_MASK                (0x1E000U)
#define USBPHY_PWD_TOG_RSVD1_SHIFT               (13U)
#define USBPHY_PWD_TOG_RSVD1(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RSVD1_SHIFT)) & USBPHY_PWD_TOG_RSVD1_MASK)
#define USBPHY_PWD_TOG_RXPWDENV_MASK             (0x20000U)
#define USBPHY_PWD_TOG_RXPWDENV_SHIFT            (17U)
#define USBPHY_PWD_TOG_RXPWDENV(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RXPWDENV_SHIFT)) & USBPHY_PWD_TOG_RXPWDENV_MASK)
#define USBPHY_PWD_TOG_RXPWD1PT1_MASK            (0x40000U)
#define USBPHY_PWD_TOG_RXPWD1PT1_SHIFT           (18U)
#define USBPHY_PWD_TOG_RXPWD1PT1(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RXPWD1PT1_SHIFT)) & USBPHY_PWD_TOG_RXPWD1PT1_MASK)
#define USBPHY_PWD_TOG_RXPWDDIFF_MASK            (0x80000U)
#define USBPHY_PWD_TOG_RXPWDDIFF_SHIFT           (19U)
#define USBPHY_PWD_TOG_RXPWDDIFF(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RXPWDDIFF_SHIFT)) & USBPHY_PWD_TOG_RXPWDDIFF_MASK)
#define USBPHY_PWD_TOG_RXPWDRX_MASK              (0x100000U)
#define USBPHY_PWD_TOG_RXPWDRX_SHIFT             (20U)
#define USBPHY_PWD_TOG_RXPWDRX(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RXPWDRX_SHIFT)) & USBPHY_PWD_TOG_RXPWDRX_MASK)
#define USBPHY_PWD_TOG_RSVD2_MASK                (0xFFE00000U)
#define USBPHY_PWD_TOG_RSVD2_SHIFT               (21U)
#define USBPHY_PWD_TOG_RSVD2(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_PWD_TOG_RSVD2_SHIFT)) & USBPHY_PWD_TOG_RSVD2_MASK)
/*! @} */

/*! @name TX - USB PHY Transmitter Control Register */
/*! @{ */
#define USBPHY_TX_D_CAL_MASK                     (0xFU)
#define USBPHY_TX_D_CAL_SHIFT                    (0U)
#define USBPHY_TX_D_CAL(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_D_CAL_SHIFT)) & USBPHY_TX_D_CAL_MASK)
#define USBPHY_TX_RSVD0_MASK                     (0xF0U)
#define USBPHY_TX_RSVD0_SHIFT                    (4U)
#define USBPHY_TX_RSVD0(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_RSVD0_SHIFT)) & USBPHY_TX_RSVD0_MASK)
#define USBPHY_TX_TXCAL45DN_MASK                 (0xF00U)
#define USBPHY_TX_TXCAL45DN_SHIFT                (8U)
#define USBPHY_TX_TXCAL45DN(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TXCAL45DN_SHIFT)) & USBPHY_TX_TXCAL45DN_MASK)
#define USBPHY_TX_RSVD1_MASK                     (0xF000U)
#define USBPHY_TX_RSVD1_SHIFT                    (12U)
#define USBPHY_TX_RSVD1(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_RSVD1_SHIFT)) & USBPHY_TX_RSVD1_MASK)
#define USBPHY_TX_TXCAL45DP_MASK                 (0xF0000U)
#define USBPHY_TX_TXCAL45DP_SHIFT                (16U)
#define USBPHY_TX_TXCAL45DP(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TXCAL45DP_SHIFT)) & USBPHY_TX_TXCAL45DP_MASK)
#define USBPHY_TX_RSVD2_MASK                     (0x3F00000U)
#define USBPHY_TX_RSVD2_SHIFT                    (20U)
#define USBPHY_TX_RSVD2(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_RSVD2_SHIFT)) & USBPHY_TX_RSVD2_MASK)
#define USBPHY_TX_USBPHY_TX_EDGECTRL_MASK        (0x1C000000U)
#define USBPHY_TX_USBPHY_TX_EDGECTRL_SHIFT       (26U)
#define USBPHY_TX_USBPHY_TX_EDGECTRL(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_USBPHY_TX_EDGECTRL_SHIFT)) & USBPHY_TX_USBPHY_TX_EDGECTRL_MASK)
#define USBPHY_TX_RSVD5_MASK                     (0xE0000000U)
#define USBPHY_TX_RSVD5_SHIFT                    (29U)
#define USBPHY_TX_RSVD5(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_RSVD5_SHIFT)) & USBPHY_TX_RSVD5_MASK)
/*! @} */

/*! @name TX_SET - USB PHY Transmitter Control Register */
/*! @{ */
#define USBPHY_TX_SET_D_CAL_MASK                 (0xFU)
#define USBPHY_TX_SET_D_CAL_SHIFT                (0U)
#define USBPHY_TX_SET_D_CAL(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_D_CAL_SHIFT)) & USBPHY_TX_SET_D_CAL_MASK)
#define USBPHY_TX_SET_RSVD0_MASK                 (0xF0U)
#define USBPHY_TX_SET_RSVD0_SHIFT                (4U)
#define USBPHY_TX_SET_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_RSVD0_SHIFT)) & USBPHY_TX_SET_RSVD0_MASK)
#define USBPHY_TX_SET_TXCAL45DN_MASK             (0xF00U)
#define USBPHY_TX_SET_TXCAL45DN_SHIFT            (8U)
#define USBPHY_TX_SET_TXCAL45DN(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_TXCAL45DN_SHIFT)) & USBPHY_TX_SET_TXCAL45DN_MASK)
#define USBPHY_TX_SET_RSVD1_MASK                 (0xF000U)
#define USBPHY_TX_SET_RSVD1_SHIFT                (12U)
#define USBPHY_TX_SET_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_RSVD1_SHIFT)) & USBPHY_TX_SET_RSVD1_MASK)
#define USBPHY_TX_SET_TXCAL45DP_MASK             (0xF0000U)
#define USBPHY_TX_SET_TXCAL45DP_SHIFT            (16U)
#define USBPHY_TX_SET_TXCAL45DP(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_TXCAL45DP_SHIFT)) & USBPHY_TX_SET_TXCAL45DP_MASK)
#define USBPHY_TX_SET_RSVD2_MASK                 (0x3F00000U)
#define USBPHY_TX_SET_RSVD2_SHIFT                (20U)
#define USBPHY_TX_SET_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_RSVD2_SHIFT)) & USBPHY_TX_SET_RSVD2_MASK)
#define USBPHY_TX_SET_USBPHY_TX_EDGECTRL_MASK    (0x1C000000U)
#define USBPHY_TX_SET_USBPHY_TX_EDGECTRL_SHIFT   (26U)
#define USBPHY_TX_SET_USBPHY_TX_EDGECTRL(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_USBPHY_TX_EDGECTRL_SHIFT)) & USBPHY_TX_SET_USBPHY_TX_EDGECTRL_MASK)
#define USBPHY_TX_SET_RSVD5_MASK                 (0xE0000000U)
#define USBPHY_TX_SET_RSVD5_SHIFT                (29U)
#define USBPHY_TX_SET_RSVD5(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_SET_RSVD5_SHIFT)) & USBPHY_TX_SET_RSVD5_MASK)
/*! @} */

/*! @name TX_CLR - USB PHY Transmitter Control Register */
/*! @{ */
#define USBPHY_TX_CLR_D_CAL_MASK                 (0xFU)
#define USBPHY_TX_CLR_D_CAL_SHIFT                (0U)
#define USBPHY_TX_CLR_D_CAL(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_D_CAL_SHIFT)) & USBPHY_TX_CLR_D_CAL_MASK)
#define USBPHY_TX_CLR_RSVD0_MASK                 (0xF0U)
#define USBPHY_TX_CLR_RSVD0_SHIFT                (4U)
#define USBPHY_TX_CLR_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_RSVD0_SHIFT)) & USBPHY_TX_CLR_RSVD0_MASK)
#define USBPHY_TX_CLR_TXCAL45DN_MASK             (0xF00U)
#define USBPHY_TX_CLR_TXCAL45DN_SHIFT            (8U)
#define USBPHY_TX_CLR_TXCAL45DN(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_TXCAL45DN_SHIFT)) & USBPHY_TX_CLR_TXCAL45DN_MASK)
#define USBPHY_TX_CLR_RSVD1_MASK                 (0xF000U)
#define USBPHY_TX_CLR_RSVD1_SHIFT                (12U)
#define USBPHY_TX_CLR_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_RSVD1_SHIFT)) & USBPHY_TX_CLR_RSVD1_MASK)
#define USBPHY_TX_CLR_TXCAL45DP_MASK             (0xF0000U)
#define USBPHY_TX_CLR_TXCAL45DP_SHIFT            (16U)
#define USBPHY_TX_CLR_TXCAL45DP(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_TXCAL45DP_SHIFT)) & USBPHY_TX_CLR_TXCAL45DP_MASK)
#define USBPHY_TX_CLR_RSVD2_MASK                 (0x3F00000U)
#define USBPHY_TX_CLR_RSVD2_SHIFT                (20U)
#define USBPHY_TX_CLR_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_RSVD2_SHIFT)) & USBPHY_TX_CLR_RSVD2_MASK)
#define USBPHY_TX_CLR_USBPHY_TX_EDGECTRL_MASK    (0x1C000000U)
#define USBPHY_TX_CLR_USBPHY_TX_EDGECTRL_SHIFT   (26U)
#define USBPHY_TX_CLR_USBPHY_TX_EDGECTRL(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_USBPHY_TX_EDGECTRL_SHIFT)) & USBPHY_TX_CLR_USBPHY_TX_EDGECTRL_MASK)
#define USBPHY_TX_CLR_RSVD5_MASK                 (0xE0000000U)
#define USBPHY_TX_CLR_RSVD5_SHIFT                (29U)
#define USBPHY_TX_CLR_RSVD5(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_CLR_RSVD5_SHIFT)) & USBPHY_TX_CLR_RSVD5_MASK)
/*! @} */

/*! @name TX_TOG - USB PHY Transmitter Control Register */
/*! @{ */
#define USBPHY_TX_TOG_D_CAL_MASK                 (0xFU)
#define USBPHY_TX_TOG_D_CAL_SHIFT                (0U)
#define USBPHY_TX_TOG_D_CAL(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_D_CAL_SHIFT)) & USBPHY_TX_TOG_D_CAL_MASK)
#define USBPHY_TX_TOG_RSVD0_MASK                 (0xF0U)
#define USBPHY_TX_TOG_RSVD0_SHIFT                (4U)
#define USBPHY_TX_TOG_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_RSVD0_SHIFT)) & USBPHY_TX_TOG_RSVD0_MASK)
#define USBPHY_TX_TOG_TXCAL45DN_MASK             (0xF00U)
#define USBPHY_TX_TOG_TXCAL45DN_SHIFT            (8U)
#define USBPHY_TX_TOG_TXCAL45DN(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_TXCAL45DN_SHIFT)) & USBPHY_TX_TOG_TXCAL45DN_MASK)
#define USBPHY_TX_TOG_RSVD1_MASK                 (0xF000U)
#define USBPHY_TX_TOG_RSVD1_SHIFT                (12U)
#define USBPHY_TX_TOG_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_RSVD1_SHIFT)) & USBPHY_TX_TOG_RSVD1_MASK)
#define USBPHY_TX_TOG_TXCAL45DP_MASK             (0xF0000U)
#define USBPHY_TX_TOG_TXCAL45DP_SHIFT            (16U)
#define USBPHY_TX_TOG_TXCAL45DP(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_TXCAL45DP_SHIFT)) & USBPHY_TX_TOG_TXCAL45DP_MASK)
#define USBPHY_TX_TOG_RSVD2_MASK                 (0x3F00000U)
#define USBPHY_TX_TOG_RSVD2_SHIFT                (20U)
#define USBPHY_TX_TOG_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_RSVD2_SHIFT)) & USBPHY_TX_TOG_RSVD2_MASK)
#define USBPHY_TX_TOG_USBPHY_TX_EDGECTRL_MASK    (0x1C000000U)
#define USBPHY_TX_TOG_USBPHY_TX_EDGECTRL_SHIFT   (26U)
#define USBPHY_TX_TOG_USBPHY_TX_EDGECTRL(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_USBPHY_TX_EDGECTRL_SHIFT)) & USBPHY_TX_TOG_USBPHY_TX_EDGECTRL_MASK)
#define USBPHY_TX_TOG_RSVD5_MASK                 (0xE0000000U)
#define USBPHY_TX_TOG_RSVD5_SHIFT                (29U)
#define USBPHY_TX_TOG_RSVD5(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_TX_TOG_RSVD5_SHIFT)) & USBPHY_TX_TOG_RSVD5_MASK)
/*! @} */

/*! @name RX - USB PHY Receiver Control Register */
/*! @{ */
#define USBPHY_RX_ENVADJ_MASK                    (0x7U)
#define USBPHY_RX_ENVADJ_SHIFT                   (0U)
#define USBPHY_RX_ENVADJ(x)                      (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_ENVADJ_SHIFT)) & USBPHY_RX_ENVADJ_MASK)
#define USBPHY_RX_RSVD0_MASK                     (0x8U)
#define USBPHY_RX_RSVD0_SHIFT                    (3U)
#define USBPHY_RX_RSVD0(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_RSVD0_SHIFT)) & USBPHY_RX_RSVD0_MASK)
#define USBPHY_RX_DISCONADJ_MASK                 (0x70U)
#define USBPHY_RX_DISCONADJ_SHIFT                (4U)
#define USBPHY_RX_DISCONADJ(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_DISCONADJ_SHIFT)) & USBPHY_RX_DISCONADJ_MASK)
#define USBPHY_RX_RSVD1_MASK                     (0x3FFF80U)
#define USBPHY_RX_RSVD1_SHIFT                    (7U)
#define USBPHY_RX_RSVD1(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_RSVD1_SHIFT)) & USBPHY_RX_RSVD1_MASK)
#define USBPHY_RX_RXDBYPASS_MASK                 (0x400000U)
#define USBPHY_RX_RXDBYPASS_SHIFT                (22U)
#define USBPHY_RX_RXDBYPASS(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_RXDBYPASS_SHIFT)) & USBPHY_RX_RXDBYPASS_MASK)
#define USBPHY_RX_RSVD2_MASK                     (0xFF800000U)
#define USBPHY_RX_RSVD2_SHIFT                    (23U)
#define USBPHY_RX_RSVD2(x)                       (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_RSVD2_SHIFT)) & USBPHY_RX_RSVD2_MASK)
/*! @} */

/*! @name RX_SET - USB PHY Receiver Control Register */
/*! @{ */
#define USBPHY_RX_SET_ENVADJ_MASK                (0x7U)
#define USBPHY_RX_SET_ENVADJ_SHIFT               (0U)
#define USBPHY_RX_SET_ENVADJ(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_SET_ENVADJ_SHIFT)) & USBPHY_RX_SET_ENVADJ_MASK)
#define USBPHY_RX_SET_RSVD0_MASK                 (0x8U)
#define USBPHY_RX_SET_RSVD0_SHIFT                (3U)
#define USBPHY_RX_SET_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_SET_RSVD0_SHIFT)) & USBPHY_RX_SET_RSVD0_MASK)
#define USBPHY_RX_SET_DISCONADJ_MASK             (0x70U)
#define USBPHY_RX_SET_DISCONADJ_SHIFT            (4U)
#define USBPHY_RX_SET_DISCONADJ(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_SET_DISCONADJ_SHIFT)) & USBPHY_RX_SET_DISCONADJ_MASK)
#define USBPHY_RX_SET_RSVD1_MASK                 (0x3FFF80U)
#define USBPHY_RX_SET_RSVD1_SHIFT                (7U)
#define USBPHY_RX_SET_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_SET_RSVD1_SHIFT)) & USBPHY_RX_SET_RSVD1_MASK)
#define USBPHY_RX_SET_RXDBYPASS_MASK             (0x400000U)
#define USBPHY_RX_SET_RXDBYPASS_SHIFT            (22U)
#define USBPHY_RX_SET_RXDBYPASS(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_SET_RXDBYPASS_SHIFT)) & USBPHY_RX_SET_RXDBYPASS_MASK)
#define USBPHY_RX_SET_RSVD2_MASK                 (0xFF800000U)
#define USBPHY_RX_SET_RSVD2_SHIFT                (23U)
#define USBPHY_RX_SET_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_SET_RSVD2_SHIFT)) & USBPHY_RX_SET_RSVD2_MASK)
/*! @} */

/*! @name RX_CLR - USB PHY Receiver Control Register */
/*! @{ */
#define USBPHY_RX_CLR_ENVADJ_MASK                (0x7U)
#define USBPHY_RX_CLR_ENVADJ_SHIFT               (0U)
#define USBPHY_RX_CLR_ENVADJ(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_CLR_ENVADJ_SHIFT)) & USBPHY_RX_CLR_ENVADJ_MASK)
#define USBPHY_RX_CLR_RSVD0_MASK                 (0x8U)
#define USBPHY_RX_CLR_RSVD0_SHIFT                (3U)
#define USBPHY_RX_CLR_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_CLR_RSVD0_SHIFT)) & USBPHY_RX_CLR_RSVD0_MASK)
#define USBPHY_RX_CLR_DISCONADJ_MASK             (0x70U)
#define USBPHY_RX_CLR_DISCONADJ_SHIFT            (4U)
#define USBPHY_RX_CLR_DISCONADJ(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_CLR_DISCONADJ_SHIFT)) & USBPHY_RX_CLR_DISCONADJ_MASK)
#define USBPHY_RX_CLR_RSVD1_MASK                 (0x3FFF80U)
#define USBPHY_RX_CLR_RSVD1_SHIFT                (7U)
#define USBPHY_RX_CLR_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_CLR_RSVD1_SHIFT)) & USBPHY_RX_CLR_RSVD1_MASK)
#define USBPHY_RX_CLR_RXDBYPASS_MASK             (0x400000U)
#define USBPHY_RX_CLR_RXDBYPASS_SHIFT            (22U)
#define USBPHY_RX_CLR_RXDBYPASS(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_CLR_RXDBYPASS_SHIFT)) & USBPHY_RX_CLR_RXDBYPASS_MASK)
#define USBPHY_RX_CLR_RSVD2_MASK                 (0xFF800000U)
#define USBPHY_RX_CLR_RSVD2_SHIFT                (23U)
#define USBPHY_RX_CLR_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_CLR_RSVD2_SHIFT)) & USBPHY_RX_CLR_RSVD2_MASK)
/*! @} */

/*! @name RX_TOG - USB PHY Receiver Control Register */
/*! @{ */
#define USBPHY_RX_TOG_ENVADJ_MASK                (0x7U)
#define USBPHY_RX_TOG_ENVADJ_SHIFT               (0U)
#define USBPHY_RX_TOG_ENVADJ(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_TOG_ENVADJ_SHIFT)) & USBPHY_RX_TOG_ENVADJ_MASK)
#define USBPHY_RX_TOG_RSVD0_MASK                 (0x8U)
#define USBPHY_RX_TOG_RSVD0_SHIFT                (3U)
#define USBPHY_RX_TOG_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_TOG_RSVD0_SHIFT)) & USBPHY_RX_TOG_RSVD0_MASK)
#define USBPHY_RX_TOG_DISCONADJ_MASK             (0x70U)
#define USBPHY_RX_TOG_DISCONADJ_SHIFT            (4U)
#define USBPHY_RX_TOG_DISCONADJ(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_TOG_DISCONADJ_SHIFT)) & USBPHY_RX_TOG_DISCONADJ_MASK)
#define USBPHY_RX_TOG_RSVD1_MASK                 (0x3FFF80U)
#define USBPHY_RX_TOG_RSVD1_SHIFT                (7U)
#define USBPHY_RX_TOG_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_TOG_RSVD1_SHIFT)) & USBPHY_RX_TOG_RSVD1_MASK)
#define USBPHY_RX_TOG_RXDBYPASS_MASK             (0x400000U)
#define USBPHY_RX_TOG_RXDBYPASS_SHIFT            (22U)
#define USBPHY_RX_TOG_RXDBYPASS(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_TOG_RXDBYPASS_SHIFT)) & USBPHY_RX_TOG_RXDBYPASS_MASK)
#define USBPHY_RX_TOG_RSVD2_MASK                 (0xFF800000U)
#define USBPHY_RX_TOG_RSVD2_SHIFT                (23U)
#define USBPHY_RX_TOG_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_RX_TOG_RSVD2_SHIFT)) & USBPHY_RX_TOG_RSVD2_MASK)
/*! @} */

/*! @name CTRL - USB PHY General Control Register */
/*! @{ */
#define USBPHY_CTRL_ENOTG_ID_CHG_IRQ_MASK        (0x1U)
#define USBPHY_CTRL_ENOTG_ID_CHG_IRQ_SHIFT       (0U)
#define USBPHY_CTRL_ENOTG_ID_CHG_IRQ(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENOTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_ENOTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_ENHOSTDISCONDETECT_MASK      (0x2U)
#define USBPHY_CTRL_ENHOSTDISCONDETECT_SHIFT     (1U)
#define USBPHY_CTRL_ENHOSTDISCONDETECT(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENHOSTDISCONDETECT_SHIFT)) & USBPHY_CTRL_ENHOSTDISCONDETECT_MASK)
#define USBPHY_CTRL_ENIRQHOSTDISCON_MASK         (0x4U)
#define USBPHY_CTRL_ENIRQHOSTDISCON_SHIFT        (2U)
#define USBPHY_CTRL_ENIRQHOSTDISCON(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENIRQHOSTDISCON_SHIFT)) & USBPHY_CTRL_ENIRQHOSTDISCON_MASK)
#define USBPHY_CTRL_HOSTDISCONDETECT_IRQ_MASK    (0x8U)
#define USBPHY_CTRL_HOSTDISCONDETECT_IRQ_SHIFT   (3U)
#define USBPHY_CTRL_HOSTDISCONDETECT_IRQ(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_HOSTDISCONDETECT_IRQ_SHIFT)) & USBPHY_CTRL_HOSTDISCONDETECT_IRQ_MASK)
#define USBPHY_CTRL_ENDEVPLUGINDETECT_MASK       (0x10U)
#define USBPHY_CTRL_ENDEVPLUGINDETECT_SHIFT      (4U)
#define USBPHY_CTRL_ENDEVPLUGINDETECT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENDEVPLUGINDETECT_SHIFT)) & USBPHY_CTRL_ENDEVPLUGINDETECT_MASK)
#define USBPHY_CTRL_DEVPLUGIN_POLARITY_MASK      (0x20U)
#define USBPHY_CTRL_DEVPLUGIN_POLARITY_SHIFT     (5U)
#define USBPHY_CTRL_DEVPLUGIN_POLARITY(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_DEVPLUGIN_POLARITY_SHIFT)) & USBPHY_CTRL_DEVPLUGIN_POLARITY_MASK)
#define USBPHY_CTRL_OTG_ID_CHG_IRQ_MASK          (0x40U)
#define USBPHY_CTRL_OTG_ID_CHG_IRQ_SHIFT         (6U)
#define USBPHY_CTRL_OTG_ID_CHG_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_OTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_OTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_ENOTGIDDETECT_MASK           (0x80U)
#define USBPHY_CTRL_ENOTGIDDETECT_SHIFT          (7U)
#define USBPHY_CTRL_ENOTGIDDETECT(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENOTGIDDETECT_SHIFT)) & USBPHY_CTRL_ENOTGIDDETECT_MASK)
#define USBPHY_CTRL_RESUMEIRQSTICKY_MASK         (0x100U)
#define USBPHY_CTRL_RESUMEIRQSTICKY_SHIFT        (8U)
#define USBPHY_CTRL_RESUMEIRQSTICKY(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_RESUMEIRQSTICKY_SHIFT)) & USBPHY_CTRL_RESUMEIRQSTICKY_MASK)
#define USBPHY_CTRL_ENIRQRESUMEDETECT_MASK       (0x200U)
#define USBPHY_CTRL_ENIRQRESUMEDETECT_SHIFT      (9U)
#define USBPHY_CTRL_ENIRQRESUMEDETECT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENIRQRESUMEDETECT_SHIFT)) & USBPHY_CTRL_ENIRQRESUMEDETECT_MASK)
#define USBPHY_CTRL_RESUME_IRQ_MASK              (0x400U)
#define USBPHY_CTRL_RESUME_IRQ_SHIFT             (10U)
#define USBPHY_CTRL_RESUME_IRQ(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_RESUME_IRQ_SHIFT)) & USBPHY_CTRL_RESUME_IRQ_MASK)
#define USBPHY_CTRL_ENIRQDEVPLUGIN_MASK          (0x800U)
#define USBPHY_CTRL_ENIRQDEVPLUGIN_SHIFT         (11U)
#define USBPHY_CTRL_ENIRQDEVPLUGIN(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENIRQDEVPLUGIN_SHIFT)) & USBPHY_CTRL_ENIRQDEVPLUGIN_MASK)
#define USBPHY_CTRL_DEVPLUGIN_IRQ_MASK           (0x1000U)
#define USBPHY_CTRL_DEVPLUGIN_IRQ_SHIFT          (12U)
#define USBPHY_CTRL_DEVPLUGIN_IRQ(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_DEVPLUGIN_IRQ_SHIFT)) & USBPHY_CTRL_DEVPLUGIN_IRQ_MASK)
#define USBPHY_CTRL_DATA_ON_LRADC_MASK           (0x2000U)
#define USBPHY_CTRL_DATA_ON_LRADC_SHIFT          (13U)
#define USBPHY_CTRL_DATA_ON_LRADC(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_DATA_ON_LRADC_SHIFT)) & USBPHY_CTRL_DATA_ON_LRADC_MASK)
#define USBPHY_CTRL_ENUTMILEVEL2_MASK            (0x4000U)
#define USBPHY_CTRL_ENUTMILEVEL2_SHIFT           (14U)
#define USBPHY_CTRL_ENUTMILEVEL2(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENUTMILEVEL2_SHIFT)) & USBPHY_CTRL_ENUTMILEVEL2_MASK)
#define USBPHY_CTRL_ENUTMILEVEL3_MASK            (0x8000U)
#define USBPHY_CTRL_ENUTMILEVEL3_SHIFT           (15U)
#define USBPHY_CTRL_ENUTMILEVEL3(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENUTMILEVEL3_SHIFT)) & USBPHY_CTRL_ENUTMILEVEL3_MASK)
#define USBPHY_CTRL_ENIRQWAKEUP_MASK             (0x10000U)
#define USBPHY_CTRL_ENIRQWAKEUP_SHIFT            (16U)
#define USBPHY_CTRL_ENIRQWAKEUP(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENIRQWAKEUP_SHIFT)) & USBPHY_CTRL_ENIRQWAKEUP_MASK)
#define USBPHY_CTRL_WAKEUP_IRQ_MASK              (0x20000U)
#define USBPHY_CTRL_WAKEUP_IRQ_SHIFT             (17U)
#define USBPHY_CTRL_WAKEUP_IRQ(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_WAKEUP_IRQ_SHIFT)) & USBPHY_CTRL_WAKEUP_IRQ_MASK)
#define USBPHY_CTRL_ENAUTO_PWRON_PLL_MASK        (0x40000U)
#define USBPHY_CTRL_ENAUTO_PWRON_PLL_SHIFT       (18U)
#define USBPHY_CTRL_ENAUTO_PWRON_PLL(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENAUTO_PWRON_PLL_SHIFT)) & USBPHY_CTRL_ENAUTO_PWRON_PLL_MASK)
#define USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK       (0x80000U)
#define USBPHY_CTRL_ENAUTOCLR_CLKGATE_SHIFT      (19U)
#define USBPHY_CTRL_ENAUTOCLR_CLKGATE(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENAUTOCLR_CLKGATE_SHIFT)) & USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK)
#define USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK       (0x100000U)
#define USBPHY_CTRL_ENAUTOCLR_PHY_PWD_SHIFT      (20U)
#define USBPHY_CTRL_ENAUTOCLR_PHY_PWD(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENAUTOCLR_PHY_PWD_SHIFT)) & USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK)
#define USBPHY_CTRL_ENDPDMCHG_WKUP_MASK          (0x200000U)
#define USBPHY_CTRL_ENDPDMCHG_WKUP_SHIFT         (21U)
#define USBPHY_CTRL_ENDPDMCHG_WKUP(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENDPDMCHG_WKUP_SHIFT)) & USBPHY_CTRL_ENDPDMCHG_WKUP_MASK)
#define USBPHY_CTRL_ENIDCHG_WKUP_MASK            (0x400000U)
#define USBPHY_CTRL_ENIDCHG_WKUP_SHIFT           (22U)
#define USBPHY_CTRL_ENIDCHG_WKUP(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENIDCHG_WKUP_SHIFT)) & USBPHY_CTRL_ENIDCHG_WKUP_MASK)
#define USBPHY_CTRL_ENVBUSCHG_WKUP_MASK          (0x800000U)
#define USBPHY_CTRL_ENVBUSCHG_WKUP_SHIFT         (23U)
#define USBPHY_CTRL_ENVBUSCHG_WKUP(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_ENVBUSCHG_WKUP_SHIFT)) & USBPHY_CTRL_ENVBUSCHG_WKUP_MASK)
#define USBPHY_CTRL_FSDLL_RST_EN_MASK            (0x1000000U)
#define USBPHY_CTRL_FSDLL_RST_EN_SHIFT           (24U)
#define USBPHY_CTRL_FSDLL_RST_EN(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_FSDLL_RST_EN_SHIFT)) & USBPHY_CTRL_FSDLL_RST_EN_MASK)
#define USBPHY_CTRL_RSVD1_MASK                   (0x6000000U)
#define USBPHY_CTRL_RSVD1_SHIFT                  (25U)
#define USBPHY_CTRL_RSVD1(x)                     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_RSVD1_SHIFT)) & USBPHY_CTRL_RSVD1_MASK)
#define USBPHY_CTRL_OTG_ID_VALUE_MASK            (0x8000000U)
#define USBPHY_CTRL_OTG_ID_VALUE_SHIFT           (27U)
#define USBPHY_CTRL_OTG_ID_VALUE(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_OTG_ID_VALUE_SHIFT)) & USBPHY_CTRL_OTG_ID_VALUE_MASK)
#define USBPHY_CTRL_HOST_FORCE_LS_SE0_MASK       (0x10000000U)
#define USBPHY_CTRL_HOST_FORCE_LS_SE0_SHIFT      (28U)
#define USBPHY_CTRL_HOST_FORCE_LS_SE0(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_HOST_FORCE_LS_SE0_SHIFT)) & USBPHY_CTRL_HOST_FORCE_LS_SE0_MASK)
#define USBPHY_CTRL_UTMI_SUSPENDM_MASK           (0x20000000U)
#define USBPHY_CTRL_UTMI_SUSPENDM_SHIFT          (29U)
#define USBPHY_CTRL_UTMI_SUSPENDM(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_UTMI_SUSPENDM_SHIFT)) & USBPHY_CTRL_UTMI_SUSPENDM_MASK)
#define USBPHY_CTRL_CLKGATE_MASK                 (0x40000000U)
#define USBPHY_CTRL_CLKGATE_SHIFT                (30U)
#define USBPHY_CTRL_CLKGATE(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLKGATE_SHIFT)) & USBPHY_CTRL_CLKGATE_MASK)
#define USBPHY_CTRL_SFTRST_MASK                  (0x80000000U)
#define USBPHY_CTRL_SFTRST_SHIFT                 (31U)
#define USBPHY_CTRL_SFTRST(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SFTRST_SHIFT)) & USBPHY_CTRL_SFTRST_MASK)
/*! @} */

/*! @name CTRL_SET - USB PHY General Control Register */
/*! @{ */
#define USBPHY_CTRL_SET_ENOTG_ID_CHG_IRQ_MASK    (0x1U)
#define USBPHY_CTRL_SET_ENOTG_ID_CHG_IRQ_SHIFT   (0U)
#define USBPHY_CTRL_SET_ENOTG_ID_CHG_IRQ(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENOTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_SET_ENOTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_SET_ENHOSTDISCONDETECT_MASK  (0x2U)
#define USBPHY_CTRL_SET_ENHOSTDISCONDETECT_SHIFT (1U)
#define USBPHY_CTRL_SET_ENHOSTDISCONDETECT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENHOSTDISCONDETECT_SHIFT)) & USBPHY_CTRL_SET_ENHOSTDISCONDETECT_MASK)
#define USBPHY_CTRL_SET_ENIRQHOSTDISCON_MASK     (0x4U)
#define USBPHY_CTRL_SET_ENIRQHOSTDISCON_SHIFT    (2U)
#define USBPHY_CTRL_SET_ENIRQHOSTDISCON(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENIRQHOSTDISCON_SHIFT)) & USBPHY_CTRL_SET_ENIRQHOSTDISCON_MASK)
#define USBPHY_CTRL_SET_HOSTDISCONDETECT_IRQ_MASK (0x8U)
#define USBPHY_CTRL_SET_HOSTDISCONDETECT_IRQ_SHIFT (3U)
#define USBPHY_CTRL_SET_HOSTDISCONDETECT_IRQ(x)  (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_HOSTDISCONDETECT_IRQ_SHIFT)) & USBPHY_CTRL_SET_HOSTDISCONDETECT_IRQ_MASK)
#define USBPHY_CTRL_SET_ENDEVPLUGINDETECT_MASK   (0x10U)
#define USBPHY_CTRL_SET_ENDEVPLUGINDETECT_SHIFT  (4U)
#define USBPHY_CTRL_SET_ENDEVPLUGINDETECT(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENDEVPLUGINDETECT_SHIFT)) & USBPHY_CTRL_SET_ENDEVPLUGINDETECT_MASK)
#define USBPHY_CTRL_SET_DEVPLUGIN_POLARITY_MASK  (0x20U)
#define USBPHY_CTRL_SET_DEVPLUGIN_POLARITY_SHIFT (5U)
#define USBPHY_CTRL_SET_DEVPLUGIN_POLARITY(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_DEVPLUGIN_POLARITY_SHIFT)) & USBPHY_CTRL_SET_DEVPLUGIN_POLARITY_MASK)
#define USBPHY_CTRL_SET_OTG_ID_CHG_IRQ_MASK      (0x40U)
#define USBPHY_CTRL_SET_OTG_ID_CHG_IRQ_SHIFT     (6U)
#define USBPHY_CTRL_SET_OTG_ID_CHG_IRQ(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_OTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_SET_OTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_SET_ENOTGIDDETECT_MASK       (0x80U)
#define USBPHY_CTRL_SET_ENOTGIDDETECT_SHIFT      (7U)
#define USBPHY_CTRL_SET_ENOTGIDDETECT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENOTGIDDETECT_SHIFT)) & USBPHY_CTRL_SET_ENOTGIDDETECT_MASK)
#define USBPHY_CTRL_SET_RESUMEIRQSTICKY_MASK     (0x100U)
#define USBPHY_CTRL_SET_RESUMEIRQSTICKY_SHIFT    (8U)
#define USBPHY_CTRL_SET_RESUMEIRQSTICKY(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_RESUMEIRQSTICKY_SHIFT)) & USBPHY_CTRL_SET_RESUMEIRQSTICKY_MASK)
#define USBPHY_CTRL_SET_ENIRQRESUMEDETECT_MASK   (0x200U)
#define USBPHY_CTRL_SET_ENIRQRESUMEDETECT_SHIFT  (9U)
#define USBPHY_CTRL_SET_ENIRQRESUMEDETECT(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENIRQRESUMEDETECT_SHIFT)) & USBPHY_CTRL_SET_ENIRQRESUMEDETECT_MASK)
#define USBPHY_CTRL_SET_RESUME_IRQ_MASK          (0x400U)
#define USBPHY_CTRL_SET_RESUME_IRQ_SHIFT         (10U)
#define USBPHY_CTRL_SET_RESUME_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_RESUME_IRQ_SHIFT)) & USBPHY_CTRL_SET_RESUME_IRQ_MASK)
#define USBPHY_CTRL_SET_ENIRQDEVPLUGIN_MASK      (0x800U)
#define USBPHY_CTRL_SET_ENIRQDEVPLUGIN_SHIFT     (11U)
#define USBPHY_CTRL_SET_ENIRQDEVPLUGIN(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENIRQDEVPLUGIN_SHIFT)) & USBPHY_CTRL_SET_ENIRQDEVPLUGIN_MASK)
#define USBPHY_CTRL_SET_DEVPLUGIN_IRQ_MASK       (0x1000U)
#define USBPHY_CTRL_SET_DEVPLUGIN_IRQ_SHIFT      (12U)
#define USBPHY_CTRL_SET_DEVPLUGIN_IRQ(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_DEVPLUGIN_IRQ_SHIFT)) & USBPHY_CTRL_SET_DEVPLUGIN_IRQ_MASK)
#define USBPHY_CTRL_SET_DATA_ON_LRADC_MASK       (0x2000U)
#define USBPHY_CTRL_SET_DATA_ON_LRADC_SHIFT      (13U)
#define USBPHY_CTRL_SET_DATA_ON_LRADC(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_DATA_ON_LRADC_SHIFT)) & USBPHY_CTRL_SET_DATA_ON_LRADC_MASK)
#define USBPHY_CTRL_SET_ENUTMILEVEL2_MASK        (0x4000U)
#define USBPHY_CTRL_SET_ENUTMILEVEL2_SHIFT       (14U)
#define USBPHY_CTRL_SET_ENUTMILEVEL2(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENUTMILEVEL2_SHIFT)) & USBPHY_CTRL_SET_ENUTMILEVEL2_MASK)
#define USBPHY_CTRL_SET_ENUTMILEVEL3_MASK        (0x8000U)
#define USBPHY_CTRL_SET_ENUTMILEVEL3_SHIFT       (15U)
#define USBPHY_CTRL_SET_ENUTMILEVEL3(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENUTMILEVEL3_SHIFT)) & USBPHY_CTRL_SET_ENUTMILEVEL3_MASK)
#define USBPHY_CTRL_SET_ENIRQWAKEUP_MASK         (0x10000U)
#define USBPHY_CTRL_SET_ENIRQWAKEUP_SHIFT        (16U)
#define USBPHY_CTRL_SET_ENIRQWAKEUP(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENIRQWAKEUP_SHIFT)) & USBPHY_CTRL_SET_ENIRQWAKEUP_MASK)
#define USBPHY_CTRL_SET_WAKEUP_IRQ_MASK          (0x20000U)
#define USBPHY_CTRL_SET_WAKEUP_IRQ_SHIFT         (17U)
#define USBPHY_CTRL_SET_WAKEUP_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_WAKEUP_IRQ_SHIFT)) & USBPHY_CTRL_SET_WAKEUP_IRQ_MASK)
#define USBPHY_CTRL_SET_ENAUTO_PWRON_PLL_MASK    (0x40000U)
#define USBPHY_CTRL_SET_ENAUTO_PWRON_PLL_SHIFT   (18U)
#define USBPHY_CTRL_SET_ENAUTO_PWRON_PLL(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENAUTO_PWRON_PLL_SHIFT)) & USBPHY_CTRL_SET_ENAUTO_PWRON_PLL_MASK)
#define USBPHY_CTRL_SET_ENAUTOCLR_CLKGATE_MASK   (0x80000U)
#define USBPHY_CTRL_SET_ENAUTOCLR_CLKGATE_SHIFT  (19U)
#define USBPHY_CTRL_SET_ENAUTOCLR_CLKGATE(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENAUTOCLR_CLKGATE_SHIFT)) & USBPHY_CTRL_SET_ENAUTOCLR_CLKGATE_MASK)
#define USBPHY_CTRL_SET_ENAUTOCLR_PHY_PWD_MASK   (0x100000U)
#define USBPHY_CTRL_SET_ENAUTOCLR_PHY_PWD_SHIFT  (20U)
#define USBPHY_CTRL_SET_ENAUTOCLR_PHY_PWD(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENAUTOCLR_PHY_PWD_SHIFT)) & USBPHY_CTRL_SET_ENAUTOCLR_PHY_PWD_MASK)
#define USBPHY_CTRL_SET_ENDPDMCHG_WKUP_MASK      (0x200000U)
#define USBPHY_CTRL_SET_ENDPDMCHG_WKUP_SHIFT     (21U)
#define USBPHY_CTRL_SET_ENDPDMCHG_WKUP(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENDPDMCHG_WKUP_SHIFT)) & USBPHY_CTRL_SET_ENDPDMCHG_WKUP_MASK)
#define USBPHY_CTRL_SET_ENIDCHG_WKUP_MASK        (0x400000U)
#define USBPHY_CTRL_SET_ENIDCHG_WKUP_SHIFT       (22U)
#define USBPHY_CTRL_SET_ENIDCHG_WKUP(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENIDCHG_WKUP_SHIFT)) & USBPHY_CTRL_SET_ENIDCHG_WKUP_MASK)
#define USBPHY_CTRL_SET_ENVBUSCHG_WKUP_MASK      (0x800000U)
#define USBPHY_CTRL_SET_ENVBUSCHG_WKUP_SHIFT     (23U)
#define USBPHY_CTRL_SET_ENVBUSCHG_WKUP(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_ENVBUSCHG_WKUP_SHIFT)) & USBPHY_CTRL_SET_ENVBUSCHG_WKUP_MASK)
#define USBPHY_CTRL_SET_FSDLL_RST_EN_MASK        (0x1000000U)
#define USBPHY_CTRL_SET_FSDLL_RST_EN_SHIFT       (24U)
#define USBPHY_CTRL_SET_FSDLL_RST_EN(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_FSDLL_RST_EN_SHIFT)) & USBPHY_CTRL_SET_FSDLL_RST_EN_MASK)
#define USBPHY_CTRL_SET_RSVD1_MASK               (0x6000000U)
#define USBPHY_CTRL_SET_RSVD1_SHIFT              (25U)
#define USBPHY_CTRL_SET_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_RSVD1_SHIFT)) & USBPHY_CTRL_SET_RSVD1_MASK)
#define USBPHY_CTRL_SET_OTG_ID_VALUE_MASK        (0x8000000U)
#define USBPHY_CTRL_SET_OTG_ID_VALUE_SHIFT       (27U)
#define USBPHY_CTRL_SET_OTG_ID_VALUE(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_OTG_ID_VALUE_SHIFT)) & USBPHY_CTRL_SET_OTG_ID_VALUE_MASK)
#define USBPHY_CTRL_SET_HOST_FORCE_LS_SE0_MASK   (0x10000000U)
#define USBPHY_CTRL_SET_HOST_FORCE_LS_SE0_SHIFT  (28U)
#define USBPHY_CTRL_SET_HOST_FORCE_LS_SE0(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_HOST_FORCE_LS_SE0_SHIFT)) & USBPHY_CTRL_SET_HOST_FORCE_LS_SE0_MASK)
#define USBPHY_CTRL_SET_UTMI_SUSPENDM_MASK       (0x20000000U)
#define USBPHY_CTRL_SET_UTMI_SUSPENDM_SHIFT      (29U)
#define USBPHY_CTRL_SET_UTMI_SUSPENDM(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_UTMI_SUSPENDM_SHIFT)) & USBPHY_CTRL_SET_UTMI_SUSPENDM_MASK)
#define USBPHY_CTRL_SET_CLKGATE_MASK             (0x40000000U)
#define USBPHY_CTRL_SET_CLKGATE_SHIFT            (30U)
#define USBPHY_CTRL_SET_CLKGATE(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_CLKGATE_SHIFT)) & USBPHY_CTRL_SET_CLKGATE_MASK)
#define USBPHY_CTRL_SET_SFTRST_MASK              (0x80000000U)
#define USBPHY_CTRL_SET_SFTRST_SHIFT             (31U)
#define USBPHY_CTRL_SET_SFTRST(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_SET_SFTRST_SHIFT)) & USBPHY_CTRL_SET_SFTRST_MASK)
/*! @} */

/*! @name CTRL_CLR - USB PHY General Control Register */
/*! @{ */
#define USBPHY_CTRL_CLR_ENOTG_ID_CHG_IRQ_MASK    (0x1U)
#define USBPHY_CTRL_CLR_ENOTG_ID_CHG_IRQ_SHIFT   (0U)
#define USBPHY_CTRL_CLR_ENOTG_ID_CHG_IRQ(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENOTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_CLR_ENOTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_CLR_ENHOSTDISCONDETECT_MASK  (0x2U)
#define USBPHY_CTRL_CLR_ENHOSTDISCONDETECT_SHIFT (1U)
#define USBPHY_CTRL_CLR_ENHOSTDISCONDETECT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENHOSTDISCONDETECT_SHIFT)) & USBPHY_CTRL_CLR_ENHOSTDISCONDETECT_MASK)
#define USBPHY_CTRL_CLR_ENIRQHOSTDISCON_MASK     (0x4U)
#define USBPHY_CTRL_CLR_ENIRQHOSTDISCON_SHIFT    (2U)
#define USBPHY_CTRL_CLR_ENIRQHOSTDISCON(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENIRQHOSTDISCON_SHIFT)) & USBPHY_CTRL_CLR_ENIRQHOSTDISCON_MASK)
#define USBPHY_CTRL_CLR_HOSTDISCONDETECT_IRQ_MASK (0x8U)
#define USBPHY_CTRL_CLR_HOSTDISCONDETECT_IRQ_SHIFT (3U)
#define USBPHY_CTRL_CLR_HOSTDISCONDETECT_IRQ(x)  (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_HOSTDISCONDETECT_IRQ_SHIFT)) & USBPHY_CTRL_CLR_HOSTDISCONDETECT_IRQ_MASK)
#define USBPHY_CTRL_CLR_ENDEVPLUGINDETECT_MASK   (0x10U)
#define USBPHY_CTRL_CLR_ENDEVPLUGINDETECT_SHIFT  (4U)
#define USBPHY_CTRL_CLR_ENDEVPLUGINDETECT(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENDEVPLUGINDETECT_SHIFT)) & USBPHY_CTRL_CLR_ENDEVPLUGINDETECT_MASK)
#define USBPHY_CTRL_CLR_DEVPLUGIN_POLARITY_MASK  (0x20U)
#define USBPHY_CTRL_CLR_DEVPLUGIN_POLARITY_SHIFT (5U)
#define USBPHY_CTRL_CLR_DEVPLUGIN_POLARITY(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_DEVPLUGIN_POLARITY_SHIFT)) & USBPHY_CTRL_CLR_DEVPLUGIN_POLARITY_MASK)
#define USBPHY_CTRL_CLR_OTG_ID_CHG_IRQ_MASK      (0x40U)
#define USBPHY_CTRL_CLR_OTG_ID_CHG_IRQ_SHIFT     (6U)
#define USBPHY_CTRL_CLR_OTG_ID_CHG_IRQ(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_OTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_CLR_OTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_CLR_ENOTGIDDETECT_MASK       (0x80U)
#define USBPHY_CTRL_CLR_ENOTGIDDETECT_SHIFT      (7U)
#define USBPHY_CTRL_CLR_ENOTGIDDETECT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENOTGIDDETECT_SHIFT)) & USBPHY_CTRL_CLR_ENOTGIDDETECT_MASK)
#define USBPHY_CTRL_CLR_RESUMEIRQSTICKY_MASK     (0x100U)
#define USBPHY_CTRL_CLR_RESUMEIRQSTICKY_SHIFT    (8U)
#define USBPHY_CTRL_CLR_RESUMEIRQSTICKY(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_RESUMEIRQSTICKY_SHIFT)) & USBPHY_CTRL_CLR_RESUMEIRQSTICKY_MASK)
#define USBPHY_CTRL_CLR_ENIRQRESUMEDETECT_MASK   (0x200U)
#define USBPHY_CTRL_CLR_ENIRQRESUMEDETECT_SHIFT  (9U)
#define USBPHY_CTRL_CLR_ENIRQRESUMEDETECT(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENIRQRESUMEDETECT_SHIFT)) & USBPHY_CTRL_CLR_ENIRQRESUMEDETECT_MASK)
#define USBPHY_CTRL_CLR_RESUME_IRQ_MASK          (0x400U)
#define USBPHY_CTRL_CLR_RESUME_IRQ_SHIFT         (10U)
#define USBPHY_CTRL_CLR_RESUME_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_RESUME_IRQ_SHIFT)) & USBPHY_CTRL_CLR_RESUME_IRQ_MASK)
#define USBPHY_CTRL_CLR_ENIRQDEVPLUGIN_MASK      (0x800U)
#define USBPHY_CTRL_CLR_ENIRQDEVPLUGIN_SHIFT     (11U)
#define USBPHY_CTRL_CLR_ENIRQDEVPLUGIN(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENIRQDEVPLUGIN_SHIFT)) & USBPHY_CTRL_CLR_ENIRQDEVPLUGIN_MASK)
#define USBPHY_CTRL_CLR_DEVPLUGIN_IRQ_MASK       (0x1000U)
#define USBPHY_CTRL_CLR_DEVPLUGIN_IRQ_SHIFT      (12U)
#define USBPHY_CTRL_CLR_DEVPLUGIN_IRQ(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_DEVPLUGIN_IRQ_SHIFT)) & USBPHY_CTRL_CLR_DEVPLUGIN_IRQ_MASK)
#define USBPHY_CTRL_CLR_DATA_ON_LRADC_MASK       (0x2000U)
#define USBPHY_CTRL_CLR_DATA_ON_LRADC_SHIFT      (13U)
#define USBPHY_CTRL_CLR_DATA_ON_LRADC(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_DATA_ON_LRADC_SHIFT)) & USBPHY_CTRL_CLR_DATA_ON_LRADC_MASK)
#define USBPHY_CTRL_CLR_ENUTMILEVEL2_MASK        (0x4000U)
#define USBPHY_CTRL_CLR_ENUTMILEVEL2_SHIFT       (14U)
#define USBPHY_CTRL_CLR_ENUTMILEVEL2(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENUTMILEVEL2_SHIFT)) & USBPHY_CTRL_CLR_ENUTMILEVEL2_MASK)
#define USBPHY_CTRL_CLR_ENUTMILEVEL3_MASK        (0x8000U)
#define USBPHY_CTRL_CLR_ENUTMILEVEL3_SHIFT       (15U)
#define USBPHY_CTRL_CLR_ENUTMILEVEL3(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENUTMILEVEL3_SHIFT)) & USBPHY_CTRL_CLR_ENUTMILEVEL3_MASK)
#define USBPHY_CTRL_CLR_ENIRQWAKEUP_MASK         (0x10000U)
#define USBPHY_CTRL_CLR_ENIRQWAKEUP_SHIFT        (16U)
#define USBPHY_CTRL_CLR_ENIRQWAKEUP(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENIRQWAKEUP_SHIFT)) & USBPHY_CTRL_CLR_ENIRQWAKEUP_MASK)
#define USBPHY_CTRL_CLR_WAKEUP_IRQ_MASK          (0x20000U)
#define USBPHY_CTRL_CLR_WAKEUP_IRQ_SHIFT         (17U)
#define USBPHY_CTRL_CLR_WAKEUP_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_WAKEUP_IRQ_SHIFT)) & USBPHY_CTRL_CLR_WAKEUP_IRQ_MASK)
#define USBPHY_CTRL_CLR_ENAUTO_PWRON_PLL_MASK    (0x40000U)
#define USBPHY_CTRL_CLR_ENAUTO_PWRON_PLL_SHIFT   (18U)
#define USBPHY_CTRL_CLR_ENAUTO_PWRON_PLL(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENAUTO_PWRON_PLL_SHIFT)) & USBPHY_CTRL_CLR_ENAUTO_PWRON_PLL_MASK)
#define USBPHY_CTRL_CLR_ENAUTOCLR_CLKGATE_MASK   (0x80000U)
#define USBPHY_CTRL_CLR_ENAUTOCLR_CLKGATE_SHIFT  (19U)
#define USBPHY_CTRL_CLR_ENAUTOCLR_CLKGATE(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENAUTOCLR_CLKGATE_SHIFT)) & USBPHY_CTRL_CLR_ENAUTOCLR_CLKGATE_MASK)
#define USBPHY_CTRL_CLR_ENAUTOCLR_PHY_PWD_MASK   (0x100000U)
#define USBPHY_CTRL_CLR_ENAUTOCLR_PHY_PWD_SHIFT  (20U)
#define USBPHY_CTRL_CLR_ENAUTOCLR_PHY_PWD(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENAUTOCLR_PHY_PWD_SHIFT)) & USBPHY_CTRL_CLR_ENAUTOCLR_PHY_PWD_MASK)
#define USBPHY_CTRL_CLR_ENDPDMCHG_WKUP_MASK      (0x200000U)
#define USBPHY_CTRL_CLR_ENDPDMCHG_WKUP_SHIFT     (21U)
#define USBPHY_CTRL_CLR_ENDPDMCHG_WKUP(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENDPDMCHG_WKUP_SHIFT)) & USBPHY_CTRL_CLR_ENDPDMCHG_WKUP_MASK)
#define USBPHY_CTRL_CLR_ENIDCHG_WKUP_MASK        (0x400000U)
#define USBPHY_CTRL_CLR_ENIDCHG_WKUP_SHIFT       (22U)
#define USBPHY_CTRL_CLR_ENIDCHG_WKUP(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENIDCHG_WKUP_SHIFT)) & USBPHY_CTRL_CLR_ENIDCHG_WKUP_MASK)
#define USBPHY_CTRL_CLR_ENVBUSCHG_WKUP_MASK      (0x800000U)
#define USBPHY_CTRL_CLR_ENVBUSCHG_WKUP_SHIFT     (23U)
#define USBPHY_CTRL_CLR_ENVBUSCHG_WKUP(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_ENVBUSCHG_WKUP_SHIFT)) & USBPHY_CTRL_CLR_ENVBUSCHG_WKUP_MASK)
#define USBPHY_CTRL_CLR_FSDLL_RST_EN_MASK        (0x1000000U)
#define USBPHY_CTRL_CLR_FSDLL_RST_EN_SHIFT       (24U)
#define USBPHY_CTRL_CLR_FSDLL_RST_EN(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_FSDLL_RST_EN_SHIFT)) & USBPHY_CTRL_CLR_FSDLL_RST_EN_MASK)
#define USBPHY_CTRL_CLR_RSVD1_MASK               (0x6000000U)
#define USBPHY_CTRL_CLR_RSVD1_SHIFT              (25U)
#define USBPHY_CTRL_CLR_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_RSVD1_SHIFT)) & USBPHY_CTRL_CLR_RSVD1_MASK)
#define USBPHY_CTRL_CLR_OTG_ID_VALUE_MASK        (0x8000000U)
#define USBPHY_CTRL_CLR_OTG_ID_VALUE_SHIFT       (27U)
#define USBPHY_CTRL_CLR_OTG_ID_VALUE(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_OTG_ID_VALUE_SHIFT)) & USBPHY_CTRL_CLR_OTG_ID_VALUE_MASK)
#define USBPHY_CTRL_CLR_HOST_FORCE_LS_SE0_MASK   (0x10000000U)
#define USBPHY_CTRL_CLR_HOST_FORCE_LS_SE0_SHIFT  (28U)
#define USBPHY_CTRL_CLR_HOST_FORCE_LS_SE0(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_HOST_FORCE_LS_SE0_SHIFT)) & USBPHY_CTRL_CLR_HOST_FORCE_LS_SE0_MASK)
#define USBPHY_CTRL_CLR_UTMI_SUSPENDM_MASK       (0x20000000U)
#define USBPHY_CTRL_CLR_UTMI_SUSPENDM_SHIFT      (29U)
#define USBPHY_CTRL_CLR_UTMI_SUSPENDM(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_UTMI_SUSPENDM_SHIFT)) & USBPHY_CTRL_CLR_UTMI_SUSPENDM_MASK)
#define USBPHY_CTRL_CLR_CLKGATE_MASK             (0x40000000U)
#define USBPHY_CTRL_CLR_CLKGATE_SHIFT            (30U)
#define USBPHY_CTRL_CLR_CLKGATE(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_CLKGATE_SHIFT)) & USBPHY_CTRL_CLR_CLKGATE_MASK)
#define USBPHY_CTRL_CLR_SFTRST_MASK              (0x80000000U)
#define USBPHY_CTRL_CLR_SFTRST_SHIFT             (31U)
#define USBPHY_CTRL_CLR_SFTRST(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_CLR_SFTRST_SHIFT)) & USBPHY_CTRL_CLR_SFTRST_MASK)
/*! @} */

/*! @name CTRL_TOG - USB PHY General Control Register */
/*! @{ */
#define USBPHY_CTRL_TOG_ENOTG_ID_CHG_IRQ_MASK    (0x1U)
#define USBPHY_CTRL_TOG_ENOTG_ID_CHG_IRQ_SHIFT   (0U)
#define USBPHY_CTRL_TOG_ENOTG_ID_CHG_IRQ(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENOTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_TOG_ENOTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_TOG_ENHOSTDISCONDETECT_MASK  (0x2U)
#define USBPHY_CTRL_TOG_ENHOSTDISCONDETECT_SHIFT (1U)
#define USBPHY_CTRL_TOG_ENHOSTDISCONDETECT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENHOSTDISCONDETECT_SHIFT)) & USBPHY_CTRL_TOG_ENHOSTDISCONDETECT_MASK)
#define USBPHY_CTRL_TOG_ENIRQHOSTDISCON_MASK     (0x4U)
#define USBPHY_CTRL_TOG_ENIRQHOSTDISCON_SHIFT    (2U)
#define USBPHY_CTRL_TOG_ENIRQHOSTDISCON(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENIRQHOSTDISCON_SHIFT)) & USBPHY_CTRL_TOG_ENIRQHOSTDISCON_MASK)
#define USBPHY_CTRL_TOG_HOSTDISCONDETECT_IRQ_MASK (0x8U)
#define USBPHY_CTRL_TOG_HOSTDISCONDETECT_IRQ_SHIFT (3U)
#define USBPHY_CTRL_TOG_HOSTDISCONDETECT_IRQ(x)  (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_HOSTDISCONDETECT_IRQ_SHIFT)) & USBPHY_CTRL_TOG_HOSTDISCONDETECT_IRQ_MASK)
#define USBPHY_CTRL_TOG_ENDEVPLUGINDETECT_MASK   (0x10U)
#define USBPHY_CTRL_TOG_ENDEVPLUGINDETECT_SHIFT  (4U)
#define USBPHY_CTRL_TOG_ENDEVPLUGINDETECT(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENDEVPLUGINDETECT_SHIFT)) & USBPHY_CTRL_TOG_ENDEVPLUGINDETECT_MASK)
#define USBPHY_CTRL_TOG_DEVPLUGIN_POLARITY_MASK  (0x20U)
#define USBPHY_CTRL_TOG_DEVPLUGIN_POLARITY_SHIFT (5U)
#define USBPHY_CTRL_TOG_DEVPLUGIN_POLARITY(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_DEVPLUGIN_POLARITY_SHIFT)) & USBPHY_CTRL_TOG_DEVPLUGIN_POLARITY_MASK)
#define USBPHY_CTRL_TOG_OTG_ID_CHG_IRQ_MASK      (0x40U)
#define USBPHY_CTRL_TOG_OTG_ID_CHG_IRQ_SHIFT     (6U)
#define USBPHY_CTRL_TOG_OTG_ID_CHG_IRQ(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_OTG_ID_CHG_IRQ_SHIFT)) & USBPHY_CTRL_TOG_OTG_ID_CHG_IRQ_MASK)
#define USBPHY_CTRL_TOG_ENOTGIDDETECT_MASK       (0x80U)
#define USBPHY_CTRL_TOG_ENOTGIDDETECT_SHIFT      (7U)
#define USBPHY_CTRL_TOG_ENOTGIDDETECT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENOTGIDDETECT_SHIFT)) & USBPHY_CTRL_TOG_ENOTGIDDETECT_MASK)
#define USBPHY_CTRL_TOG_RESUMEIRQSTICKY_MASK     (0x100U)
#define USBPHY_CTRL_TOG_RESUMEIRQSTICKY_SHIFT    (8U)
#define USBPHY_CTRL_TOG_RESUMEIRQSTICKY(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_RESUMEIRQSTICKY_SHIFT)) & USBPHY_CTRL_TOG_RESUMEIRQSTICKY_MASK)
#define USBPHY_CTRL_TOG_ENIRQRESUMEDETECT_MASK   (0x200U)
#define USBPHY_CTRL_TOG_ENIRQRESUMEDETECT_SHIFT  (9U)
#define USBPHY_CTRL_TOG_ENIRQRESUMEDETECT(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENIRQRESUMEDETECT_SHIFT)) & USBPHY_CTRL_TOG_ENIRQRESUMEDETECT_MASK)
#define USBPHY_CTRL_TOG_RESUME_IRQ_MASK          (0x400U)
#define USBPHY_CTRL_TOG_RESUME_IRQ_SHIFT         (10U)
#define USBPHY_CTRL_TOG_RESUME_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_RESUME_IRQ_SHIFT)) & USBPHY_CTRL_TOG_RESUME_IRQ_MASK)
#define USBPHY_CTRL_TOG_ENIRQDEVPLUGIN_MASK      (0x800U)
#define USBPHY_CTRL_TOG_ENIRQDEVPLUGIN_SHIFT     (11U)
#define USBPHY_CTRL_TOG_ENIRQDEVPLUGIN(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENIRQDEVPLUGIN_SHIFT)) & USBPHY_CTRL_TOG_ENIRQDEVPLUGIN_MASK)
#define USBPHY_CTRL_TOG_DEVPLUGIN_IRQ_MASK       (0x1000U)
#define USBPHY_CTRL_TOG_DEVPLUGIN_IRQ_SHIFT      (12U)
#define USBPHY_CTRL_TOG_DEVPLUGIN_IRQ(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_DEVPLUGIN_IRQ_SHIFT)) & USBPHY_CTRL_TOG_DEVPLUGIN_IRQ_MASK)
#define USBPHY_CTRL_TOG_DATA_ON_LRADC_MASK       (0x2000U)
#define USBPHY_CTRL_TOG_DATA_ON_LRADC_SHIFT      (13U)
#define USBPHY_CTRL_TOG_DATA_ON_LRADC(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_DATA_ON_LRADC_SHIFT)) & USBPHY_CTRL_TOG_DATA_ON_LRADC_MASK)
#define USBPHY_CTRL_TOG_ENUTMILEVEL2_MASK        (0x4000U)
#define USBPHY_CTRL_TOG_ENUTMILEVEL2_SHIFT       (14U)
#define USBPHY_CTRL_TOG_ENUTMILEVEL2(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENUTMILEVEL2_SHIFT)) & USBPHY_CTRL_TOG_ENUTMILEVEL2_MASK)
#define USBPHY_CTRL_TOG_ENUTMILEVEL3_MASK        (0x8000U)
#define USBPHY_CTRL_TOG_ENUTMILEVEL3_SHIFT       (15U)
#define USBPHY_CTRL_TOG_ENUTMILEVEL3(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENUTMILEVEL3_SHIFT)) & USBPHY_CTRL_TOG_ENUTMILEVEL3_MASK)
#define USBPHY_CTRL_TOG_ENIRQWAKEUP_MASK         (0x10000U)
#define USBPHY_CTRL_TOG_ENIRQWAKEUP_SHIFT        (16U)
#define USBPHY_CTRL_TOG_ENIRQWAKEUP(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENIRQWAKEUP_SHIFT)) & USBPHY_CTRL_TOG_ENIRQWAKEUP_MASK)
#define USBPHY_CTRL_TOG_WAKEUP_IRQ_MASK          (0x20000U)
#define USBPHY_CTRL_TOG_WAKEUP_IRQ_SHIFT         (17U)
#define USBPHY_CTRL_TOG_WAKEUP_IRQ(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_WAKEUP_IRQ_SHIFT)) & USBPHY_CTRL_TOG_WAKEUP_IRQ_MASK)
#define USBPHY_CTRL_TOG_ENAUTO_PWRON_PLL_MASK    (0x40000U)
#define USBPHY_CTRL_TOG_ENAUTO_PWRON_PLL_SHIFT   (18U)
#define USBPHY_CTRL_TOG_ENAUTO_PWRON_PLL(x)      (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENAUTO_PWRON_PLL_SHIFT)) & USBPHY_CTRL_TOG_ENAUTO_PWRON_PLL_MASK)
#define USBPHY_CTRL_TOG_ENAUTOCLR_CLKGATE_MASK   (0x80000U)
#define USBPHY_CTRL_TOG_ENAUTOCLR_CLKGATE_SHIFT  (19U)
#define USBPHY_CTRL_TOG_ENAUTOCLR_CLKGATE(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENAUTOCLR_CLKGATE_SHIFT)) & USBPHY_CTRL_TOG_ENAUTOCLR_CLKGATE_MASK)
#define USBPHY_CTRL_TOG_ENAUTOCLR_PHY_PWD_MASK   (0x100000U)
#define USBPHY_CTRL_TOG_ENAUTOCLR_PHY_PWD_SHIFT  (20U)
#define USBPHY_CTRL_TOG_ENAUTOCLR_PHY_PWD(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENAUTOCLR_PHY_PWD_SHIFT)) & USBPHY_CTRL_TOG_ENAUTOCLR_PHY_PWD_MASK)
#define USBPHY_CTRL_TOG_ENDPDMCHG_WKUP_MASK      (0x200000U)
#define USBPHY_CTRL_TOG_ENDPDMCHG_WKUP_SHIFT     (21U)
#define USBPHY_CTRL_TOG_ENDPDMCHG_WKUP(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENDPDMCHG_WKUP_SHIFT)) & USBPHY_CTRL_TOG_ENDPDMCHG_WKUP_MASK)
#define USBPHY_CTRL_TOG_ENIDCHG_WKUP_MASK        (0x400000U)
#define USBPHY_CTRL_TOG_ENIDCHG_WKUP_SHIFT       (22U)
#define USBPHY_CTRL_TOG_ENIDCHG_WKUP(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENIDCHG_WKUP_SHIFT)) & USBPHY_CTRL_TOG_ENIDCHG_WKUP_MASK)
#define USBPHY_CTRL_TOG_ENVBUSCHG_WKUP_MASK      (0x800000U)
#define USBPHY_CTRL_TOG_ENVBUSCHG_WKUP_SHIFT     (23U)
#define USBPHY_CTRL_TOG_ENVBUSCHG_WKUP(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_ENVBUSCHG_WKUP_SHIFT)) & USBPHY_CTRL_TOG_ENVBUSCHG_WKUP_MASK)
#define USBPHY_CTRL_TOG_FSDLL_RST_EN_MASK        (0x1000000U)
#define USBPHY_CTRL_TOG_FSDLL_RST_EN_SHIFT       (24U)
#define USBPHY_CTRL_TOG_FSDLL_RST_EN(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_FSDLL_RST_EN_SHIFT)) & USBPHY_CTRL_TOG_FSDLL_RST_EN_MASK)
#define USBPHY_CTRL_TOG_RSVD1_MASK               (0x6000000U)
#define USBPHY_CTRL_TOG_RSVD1_SHIFT              (25U)
#define USBPHY_CTRL_TOG_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_RSVD1_SHIFT)) & USBPHY_CTRL_TOG_RSVD1_MASK)
#define USBPHY_CTRL_TOG_OTG_ID_VALUE_MASK        (0x8000000U)
#define USBPHY_CTRL_TOG_OTG_ID_VALUE_SHIFT       (27U)
#define USBPHY_CTRL_TOG_OTG_ID_VALUE(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_OTG_ID_VALUE_SHIFT)) & USBPHY_CTRL_TOG_OTG_ID_VALUE_MASK)
#define USBPHY_CTRL_TOG_HOST_FORCE_LS_SE0_MASK   (0x10000000U)
#define USBPHY_CTRL_TOG_HOST_FORCE_LS_SE0_SHIFT  (28U)
#define USBPHY_CTRL_TOG_HOST_FORCE_LS_SE0(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_HOST_FORCE_LS_SE0_SHIFT)) & USBPHY_CTRL_TOG_HOST_FORCE_LS_SE0_MASK)
#define USBPHY_CTRL_TOG_UTMI_SUSPENDM_MASK       (0x20000000U)
#define USBPHY_CTRL_TOG_UTMI_SUSPENDM_SHIFT      (29U)
#define USBPHY_CTRL_TOG_UTMI_SUSPENDM(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_UTMI_SUSPENDM_SHIFT)) & USBPHY_CTRL_TOG_UTMI_SUSPENDM_MASK)
#define USBPHY_CTRL_TOG_CLKGATE_MASK             (0x40000000U)
#define USBPHY_CTRL_TOG_CLKGATE_SHIFT            (30U)
#define USBPHY_CTRL_TOG_CLKGATE(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_CLKGATE_SHIFT)) & USBPHY_CTRL_TOG_CLKGATE_MASK)
#define USBPHY_CTRL_TOG_SFTRST_MASK              (0x80000000U)
#define USBPHY_CTRL_TOG_SFTRST_SHIFT             (31U)
#define USBPHY_CTRL_TOG_SFTRST(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_CTRL_TOG_SFTRST_SHIFT)) & USBPHY_CTRL_TOG_SFTRST_MASK)
/*! @} */

/*! @name STATUS - USB PHY Status Register */
/*! @{ */
#define USBPHY_STATUS_RSVD0_MASK                 (0x7U)
#define USBPHY_STATUS_RSVD0_SHIFT                (0U)
#define USBPHY_STATUS_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_RSVD0_SHIFT)) & USBPHY_STATUS_RSVD0_MASK)
#define USBPHY_STATUS_HOSTDISCONDETECT_STATUS_MASK (0x8U)
#define USBPHY_STATUS_HOSTDISCONDETECT_STATUS_SHIFT (3U)
#define USBPHY_STATUS_HOSTDISCONDETECT_STATUS(x) (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_HOSTDISCONDETECT_STATUS_SHIFT)) & USBPHY_STATUS_HOSTDISCONDETECT_STATUS_MASK)
#define USBPHY_STATUS_RSVD1_MASK                 (0x30U)
#define USBPHY_STATUS_RSVD1_SHIFT                (4U)
#define USBPHY_STATUS_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_RSVD1_SHIFT)) & USBPHY_STATUS_RSVD1_MASK)
#define USBPHY_STATUS_DEVPLUGIN_STATUS_MASK      (0x40U)
#define USBPHY_STATUS_DEVPLUGIN_STATUS_SHIFT     (6U)
#define USBPHY_STATUS_DEVPLUGIN_STATUS(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_DEVPLUGIN_STATUS_SHIFT)) & USBPHY_STATUS_DEVPLUGIN_STATUS_MASK)
#define USBPHY_STATUS_RSVD2_MASK                 (0x80U)
#define USBPHY_STATUS_RSVD2_SHIFT                (7U)
#define USBPHY_STATUS_RSVD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_RSVD2_SHIFT)) & USBPHY_STATUS_RSVD2_MASK)
#define USBPHY_STATUS_OTGID_STATUS_MASK          (0x100U)
#define USBPHY_STATUS_OTGID_STATUS_SHIFT         (8U)
#define USBPHY_STATUS_OTGID_STATUS(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_OTGID_STATUS_SHIFT)) & USBPHY_STATUS_OTGID_STATUS_MASK)
#define USBPHY_STATUS_RSVD3_MASK                 (0x200U)
#define USBPHY_STATUS_RSVD3_SHIFT                (9U)
#define USBPHY_STATUS_RSVD3(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_RSVD3_SHIFT)) & USBPHY_STATUS_RSVD3_MASK)
#define USBPHY_STATUS_RESUME_STATUS_MASK         (0x400U)
#define USBPHY_STATUS_RESUME_STATUS_SHIFT        (10U)
#define USBPHY_STATUS_RESUME_STATUS(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_RESUME_STATUS_SHIFT)) & USBPHY_STATUS_RESUME_STATUS_MASK)
#define USBPHY_STATUS_RSVD4_MASK                 (0xFFFFF800U)
#define USBPHY_STATUS_RSVD4_SHIFT                (11U)
#define USBPHY_STATUS_RSVD4(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_STATUS_RSVD4_SHIFT)) & USBPHY_STATUS_RSVD4_MASK)
/*! @} */

/*! @name DEBUG - USB PHY Debug Register */
/*! @{ */
#define USBPHY_DEBUG_OTGIDPIOLOCK_MASK           (0x1U)
#define USBPHY_DEBUG_OTGIDPIOLOCK_SHIFT          (0U)
#define USBPHY_DEBUG_OTGIDPIOLOCK(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_OTGIDPIOLOCK_SHIFT)) & USBPHY_DEBUG_OTGIDPIOLOCK_MASK)
#define USBPHY_DEBUG_DEBUG_INTERFACE_HOLD_MASK   (0x2U)
#define USBPHY_DEBUG_DEBUG_INTERFACE_HOLD_SHIFT  (1U)
#define USBPHY_DEBUG_DEBUG_INTERFACE_HOLD(x)     (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_DEBUG_INTERFACE_HOLD_SHIFT)) & USBPHY_DEBUG_DEBUG_INTERFACE_HOLD_MASK)
#define USBPHY_DEBUG_HSTPULLDOWN_MASK            (0xCU)
#define USBPHY_DEBUG_HSTPULLDOWN_SHIFT           (2U)
#define USBPHY_DEBUG_HSTPULLDOWN(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_HSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_HSTPULLDOWN_MASK)
#define USBPHY_DEBUG_ENHSTPULLDOWN_MASK          (0x30U)
#define USBPHY_DEBUG_ENHSTPULLDOWN_SHIFT         (4U)
#define USBPHY_DEBUG_ENHSTPULLDOWN(x)            (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_ENHSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_ENHSTPULLDOWN_MASK)
#define USBPHY_DEBUG_RSVD0_MASK                  (0xC0U)
#define USBPHY_DEBUG_RSVD0_SHIFT                 (6U)
#define USBPHY_DEBUG_RSVD0(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_RSVD0_SHIFT)) & USBPHY_DEBUG_RSVD0_MASK)
#define USBPHY_DEBUG_TX2RXCOUNT_MASK             (0xF00U)
#define USBPHY_DEBUG_TX2RXCOUNT_SHIFT            (8U)
#define USBPHY_DEBUG_TX2RXCOUNT(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_TX2RXCOUNT_MASK)
#define USBPHY_DEBUG_ENTX2RXCOUNT_MASK           (0x1000U)
#define USBPHY_DEBUG_ENTX2RXCOUNT_SHIFT          (12U)
#define USBPHY_DEBUG_ENTX2RXCOUNT(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_ENTX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_ENTX2RXCOUNT_MASK)
#define USBPHY_DEBUG_RSVD1_MASK                  (0xE000U)
#define USBPHY_DEBUG_RSVD1_SHIFT                 (13U)
#define USBPHY_DEBUG_RSVD1(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_RSVD1_SHIFT)) & USBPHY_DEBUG_RSVD1_MASK)
#define USBPHY_DEBUG_SQUELCHRESETCOUNT_MASK      (0x1F0000U)
#define USBPHY_DEBUG_SQUELCHRESETCOUNT_SHIFT     (16U)
#define USBPHY_DEBUG_SQUELCHRESETCOUNT(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SQUELCHRESETCOUNT_SHIFT)) & USBPHY_DEBUG_SQUELCHRESETCOUNT_MASK)
#define USBPHY_DEBUG_RSVD2_MASK                  (0xE00000U)
#define USBPHY_DEBUG_RSVD2_SHIFT                 (21U)
#define USBPHY_DEBUG_RSVD2(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_RSVD2_SHIFT)) & USBPHY_DEBUG_RSVD2_MASK)
#define USBPHY_DEBUG_ENSQUELCHRESET_MASK         (0x1000000U)
#define USBPHY_DEBUG_ENSQUELCHRESET_SHIFT        (24U)
#define USBPHY_DEBUG_ENSQUELCHRESET(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_ENSQUELCHRESET_SHIFT)) & USBPHY_DEBUG_ENSQUELCHRESET_MASK)
#define USBPHY_DEBUG_SQUELCHRESETLENGTH_MASK     (0x1E000000U)
#define USBPHY_DEBUG_SQUELCHRESETLENGTH_SHIFT    (25U)
#define USBPHY_DEBUG_SQUELCHRESETLENGTH(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SQUELCHRESETLENGTH_SHIFT)) & USBPHY_DEBUG_SQUELCHRESETLENGTH_MASK)
#define USBPHY_DEBUG_HOST_RESUME_DEBUG_MASK      (0x20000000U)
#define USBPHY_DEBUG_HOST_RESUME_DEBUG_SHIFT     (29U)
#define USBPHY_DEBUG_HOST_RESUME_DEBUG(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_HOST_RESUME_DEBUG_SHIFT)) & USBPHY_DEBUG_HOST_RESUME_DEBUG_MASK)
#define USBPHY_DEBUG_CLKGATE_MASK                (0x40000000U)
#define USBPHY_DEBUG_CLKGATE_SHIFT               (30U)
#define USBPHY_DEBUG_CLKGATE(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLKGATE_SHIFT)) & USBPHY_DEBUG_CLKGATE_MASK)
#define USBPHY_DEBUG_RSVD3_MASK                  (0x80000000U)
#define USBPHY_DEBUG_RSVD3_SHIFT                 (31U)
#define USBPHY_DEBUG_RSVD3(x)                    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_RSVD3_SHIFT)) & USBPHY_DEBUG_RSVD3_MASK)
/*! @} */

/*! @name DEBUG_SET - USB PHY Debug Register */
/*! @{ */
#define USBPHY_DEBUG_SET_OTGIDPIOLOCK_MASK       (0x1U)
#define USBPHY_DEBUG_SET_OTGIDPIOLOCK_SHIFT      (0U)
#define USBPHY_DEBUG_SET_OTGIDPIOLOCK(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_OTGIDPIOLOCK_SHIFT)) & USBPHY_DEBUG_SET_OTGIDPIOLOCK_MASK)
#define USBPHY_DEBUG_SET_DEBUG_INTERFACE_HOLD_MASK (0x2U)
#define USBPHY_DEBUG_SET_DEBUG_INTERFACE_HOLD_SHIFT (1U)
#define USBPHY_DEBUG_SET_DEBUG_INTERFACE_HOLD(x) (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_DEBUG_INTERFACE_HOLD_SHIFT)) & USBPHY_DEBUG_SET_DEBUG_INTERFACE_HOLD_MASK)
#define USBPHY_DEBUG_SET_HSTPULLDOWN_MASK        (0xCU)
#define USBPHY_DEBUG_SET_HSTPULLDOWN_SHIFT       (2U)
#define USBPHY_DEBUG_SET_HSTPULLDOWN(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_HSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_SET_HSTPULLDOWN_MASK)
#define USBPHY_DEBUG_SET_ENHSTPULLDOWN_MASK      (0x30U)
#define USBPHY_DEBUG_SET_ENHSTPULLDOWN_SHIFT     (4U)
#define USBPHY_DEBUG_SET_ENHSTPULLDOWN(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_ENHSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_SET_ENHSTPULLDOWN_MASK)
#define USBPHY_DEBUG_SET_RSVD0_MASK              (0xC0U)
#define USBPHY_DEBUG_SET_RSVD0_SHIFT             (6U)
#define USBPHY_DEBUG_SET_RSVD0(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_RSVD0_SHIFT)) & USBPHY_DEBUG_SET_RSVD0_MASK)
#define USBPHY_DEBUG_SET_TX2RXCOUNT_MASK         (0xF00U)
#define USBPHY_DEBUG_SET_TX2RXCOUNT_SHIFT        (8U)
#define USBPHY_DEBUG_SET_TX2RXCOUNT(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_TX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_SET_TX2RXCOUNT_MASK)
#define USBPHY_DEBUG_SET_ENTX2RXCOUNT_MASK       (0x1000U)
#define USBPHY_DEBUG_SET_ENTX2RXCOUNT_SHIFT      (12U)
#define USBPHY_DEBUG_SET_ENTX2RXCOUNT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_ENTX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_SET_ENTX2RXCOUNT_MASK)
#define USBPHY_DEBUG_SET_RSVD1_MASK              (0xE000U)
#define USBPHY_DEBUG_SET_RSVD1_SHIFT             (13U)
#define USBPHY_DEBUG_SET_RSVD1(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_RSVD1_SHIFT)) & USBPHY_DEBUG_SET_RSVD1_MASK)
#define USBPHY_DEBUG_SET_SQUELCHRESETCOUNT_MASK  (0x1F0000U)
#define USBPHY_DEBUG_SET_SQUELCHRESETCOUNT_SHIFT (16U)
#define USBPHY_DEBUG_SET_SQUELCHRESETCOUNT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_SQUELCHRESETCOUNT_SHIFT)) & USBPHY_DEBUG_SET_SQUELCHRESETCOUNT_MASK)
#define USBPHY_DEBUG_SET_RSVD2_MASK              (0xE00000U)
#define USBPHY_DEBUG_SET_RSVD2_SHIFT             (21U)
#define USBPHY_DEBUG_SET_RSVD2(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_RSVD2_SHIFT)) & USBPHY_DEBUG_SET_RSVD2_MASK)
#define USBPHY_DEBUG_SET_ENSQUELCHRESET_MASK     (0x1000000U)
#define USBPHY_DEBUG_SET_ENSQUELCHRESET_SHIFT    (24U)
#define USBPHY_DEBUG_SET_ENSQUELCHRESET(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_ENSQUELCHRESET_SHIFT)) & USBPHY_DEBUG_SET_ENSQUELCHRESET_MASK)
#define USBPHY_DEBUG_SET_SQUELCHRESETLENGTH_MASK (0x1E000000U)
#define USBPHY_DEBUG_SET_SQUELCHRESETLENGTH_SHIFT (25U)
#define USBPHY_DEBUG_SET_SQUELCHRESETLENGTH(x)   (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_SQUELCHRESETLENGTH_SHIFT)) & USBPHY_DEBUG_SET_SQUELCHRESETLENGTH_MASK)
#define USBPHY_DEBUG_SET_HOST_RESUME_DEBUG_MASK  (0x20000000U)
#define USBPHY_DEBUG_SET_HOST_RESUME_DEBUG_SHIFT (29U)
#define USBPHY_DEBUG_SET_HOST_RESUME_DEBUG(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_HOST_RESUME_DEBUG_SHIFT)) & USBPHY_DEBUG_SET_HOST_RESUME_DEBUG_MASK)
#define USBPHY_DEBUG_SET_CLKGATE_MASK            (0x40000000U)
#define USBPHY_DEBUG_SET_CLKGATE_SHIFT           (30U)
#define USBPHY_DEBUG_SET_CLKGATE(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_CLKGATE_SHIFT)) & USBPHY_DEBUG_SET_CLKGATE_MASK)
#define USBPHY_DEBUG_SET_RSVD3_MASK              (0x80000000U)
#define USBPHY_DEBUG_SET_RSVD3_SHIFT             (31U)
#define USBPHY_DEBUG_SET_RSVD3(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_SET_RSVD3_SHIFT)) & USBPHY_DEBUG_SET_RSVD3_MASK)
/*! @} */

/*! @name DEBUG_CLR - USB PHY Debug Register */
/*! @{ */
#define USBPHY_DEBUG_CLR_OTGIDPIOLOCK_MASK       (0x1U)
#define USBPHY_DEBUG_CLR_OTGIDPIOLOCK_SHIFT      (0U)
#define USBPHY_DEBUG_CLR_OTGIDPIOLOCK(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_OTGIDPIOLOCK_SHIFT)) & USBPHY_DEBUG_CLR_OTGIDPIOLOCK_MASK)
#define USBPHY_DEBUG_CLR_DEBUG_INTERFACE_HOLD_MASK (0x2U)
#define USBPHY_DEBUG_CLR_DEBUG_INTERFACE_HOLD_SHIFT (1U)
#define USBPHY_DEBUG_CLR_DEBUG_INTERFACE_HOLD(x) (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_DEBUG_INTERFACE_HOLD_SHIFT)) & USBPHY_DEBUG_CLR_DEBUG_INTERFACE_HOLD_MASK)
#define USBPHY_DEBUG_CLR_HSTPULLDOWN_MASK        (0xCU)
#define USBPHY_DEBUG_CLR_HSTPULLDOWN_SHIFT       (2U)
#define USBPHY_DEBUG_CLR_HSTPULLDOWN(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_HSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_CLR_HSTPULLDOWN_MASK)
#define USBPHY_DEBUG_CLR_ENHSTPULLDOWN_MASK      (0x30U)
#define USBPHY_DEBUG_CLR_ENHSTPULLDOWN_SHIFT     (4U)
#define USBPHY_DEBUG_CLR_ENHSTPULLDOWN(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_ENHSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_CLR_ENHSTPULLDOWN_MASK)
#define USBPHY_DEBUG_CLR_RSVD0_MASK              (0xC0U)
#define USBPHY_DEBUG_CLR_RSVD0_SHIFT             (6U)
#define USBPHY_DEBUG_CLR_RSVD0(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_RSVD0_SHIFT)) & USBPHY_DEBUG_CLR_RSVD0_MASK)
#define USBPHY_DEBUG_CLR_TX2RXCOUNT_MASK         (0xF00U)
#define USBPHY_DEBUG_CLR_TX2RXCOUNT_SHIFT        (8U)
#define USBPHY_DEBUG_CLR_TX2RXCOUNT(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_TX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_CLR_TX2RXCOUNT_MASK)
#define USBPHY_DEBUG_CLR_ENTX2RXCOUNT_MASK       (0x1000U)
#define USBPHY_DEBUG_CLR_ENTX2RXCOUNT_SHIFT      (12U)
#define USBPHY_DEBUG_CLR_ENTX2RXCOUNT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_ENTX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_CLR_ENTX2RXCOUNT_MASK)
#define USBPHY_DEBUG_CLR_RSVD1_MASK              (0xE000U)
#define USBPHY_DEBUG_CLR_RSVD1_SHIFT             (13U)
#define USBPHY_DEBUG_CLR_RSVD1(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_RSVD1_SHIFT)) & USBPHY_DEBUG_CLR_RSVD1_MASK)
#define USBPHY_DEBUG_CLR_SQUELCHRESETCOUNT_MASK  (0x1F0000U)
#define USBPHY_DEBUG_CLR_SQUELCHRESETCOUNT_SHIFT (16U)
#define USBPHY_DEBUG_CLR_SQUELCHRESETCOUNT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_SQUELCHRESETCOUNT_SHIFT)) & USBPHY_DEBUG_CLR_SQUELCHRESETCOUNT_MASK)
#define USBPHY_DEBUG_CLR_RSVD2_MASK              (0xE00000U)
#define USBPHY_DEBUG_CLR_RSVD2_SHIFT             (21U)
#define USBPHY_DEBUG_CLR_RSVD2(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_RSVD2_SHIFT)) & USBPHY_DEBUG_CLR_RSVD2_MASK)
#define USBPHY_DEBUG_CLR_ENSQUELCHRESET_MASK     (0x1000000U)
#define USBPHY_DEBUG_CLR_ENSQUELCHRESET_SHIFT    (24U)
#define USBPHY_DEBUG_CLR_ENSQUELCHRESET(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_ENSQUELCHRESET_SHIFT)) & USBPHY_DEBUG_CLR_ENSQUELCHRESET_MASK)
#define USBPHY_DEBUG_CLR_SQUELCHRESETLENGTH_MASK (0x1E000000U)
#define USBPHY_DEBUG_CLR_SQUELCHRESETLENGTH_SHIFT (25U)
#define USBPHY_DEBUG_CLR_SQUELCHRESETLENGTH(x)   (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_SQUELCHRESETLENGTH_SHIFT)) & USBPHY_DEBUG_CLR_SQUELCHRESETLENGTH_MASK)
#define USBPHY_DEBUG_CLR_HOST_RESUME_DEBUG_MASK  (0x20000000U)
#define USBPHY_DEBUG_CLR_HOST_RESUME_DEBUG_SHIFT (29U)
#define USBPHY_DEBUG_CLR_HOST_RESUME_DEBUG(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_HOST_RESUME_DEBUG_SHIFT)) & USBPHY_DEBUG_CLR_HOST_RESUME_DEBUG_MASK)
#define USBPHY_DEBUG_CLR_CLKGATE_MASK            (0x40000000U)
#define USBPHY_DEBUG_CLR_CLKGATE_SHIFT           (30U)
#define USBPHY_DEBUG_CLR_CLKGATE(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_CLKGATE_SHIFT)) & USBPHY_DEBUG_CLR_CLKGATE_MASK)
#define USBPHY_DEBUG_CLR_RSVD3_MASK              (0x80000000U)
#define USBPHY_DEBUG_CLR_RSVD3_SHIFT             (31U)
#define USBPHY_DEBUG_CLR_RSVD3(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_CLR_RSVD3_SHIFT)) & USBPHY_DEBUG_CLR_RSVD3_MASK)
/*! @} */

/*! @name DEBUG_TOG - USB PHY Debug Register */
/*! @{ */
#define USBPHY_DEBUG_TOG_OTGIDPIOLOCK_MASK       (0x1U)
#define USBPHY_DEBUG_TOG_OTGIDPIOLOCK_SHIFT      (0U)
#define USBPHY_DEBUG_TOG_OTGIDPIOLOCK(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_OTGIDPIOLOCK_SHIFT)) & USBPHY_DEBUG_TOG_OTGIDPIOLOCK_MASK)
#define USBPHY_DEBUG_TOG_DEBUG_INTERFACE_HOLD_MASK (0x2U)
#define USBPHY_DEBUG_TOG_DEBUG_INTERFACE_HOLD_SHIFT (1U)
#define USBPHY_DEBUG_TOG_DEBUG_INTERFACE_HOLD(x) (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_DEBUG_INTERFACE_HOLD_SHIFT)) & USBPHY_DEBUG_TOG_DEBUG_INTERFACE_HOLD_MASK)
#define USBPHY_DEBUG_TOG_HSTPULLDOWN_MASK        (0xCU)
#define USBPHY_DEBUG_TOG_HSTPULLDOWN_SHIFT       (2U)
#define USBPHY_DEBUG_TOG_HSTPULLDOWN(x)          (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_HSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_TOG_HSTPULLDOWN_MASK)
#define USBPHY_DEBUG_TOG_ENHSTPULLDOWN_MASK      (0x30U)
#define USBPHY_DEBUG_TOG_ENHSTPULLDOWN_SHIFT     (4U)
#define USBPHY_DEBUG_TOG_ENHSTPULLDOWN(x)        (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_ENHSTPULLDOWN_SHIFT)) & USBPHY_DEBUG_TOG_ENHSTPULLDOWN_MASK)
#define USBPHY_DEBUG_TOG_RSVD0_MASK              (0xC0U)
#define USBPHY_DEBUG_TOG_RSVD0_SHIFT             (6U)
#define USBPHY_DEBUG_TOG_RSVD0(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_RSVD0_SHIFT)) & USBPHY_DEBUG_TOG_RSVD0_MASK)
#define USBPHY_DEBUG_TOG_TX2RXCOUNT_MASK         (0xF00U)
#define USBPHY_DEBUG_TOG_TX2RXCOUNT_SHIFT        (8U)
#define USBPHY_DEBUG_TOG_TX2RXCOUNT(x)           (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_TX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_TOG_TX2RXCOUNT_MASK)
#define USBPHY_DEBUG_TOG_ENTX2RXCOUNT_MASK       (0x1000U)
#define USBPHY_DEBUG_TOG_ENTX2RXCOUNT_SHIFT      (12U)
#define USBPHY_DEBUG_TOG_ENTX2RXCOUNT(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_ENTX2RXCOUNT_SHIFT)) & USBPHY_DEBUG_TOG_ENTX2RXCOUNT_MASK)
#define USBPHY_DEBUG_TOG_RSVD1_MASK              (0xE000U)
#define USBPHY_DEBUG_TOG_RSVD1_SHIFT             (13U)
#define USBPHY_DEBUG_TOG_RSVD1(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_RSVD1_SHIFT)) & USBPHY_DEBUG_TOG_RSVD1_MASK)
#define USBPHY_DEBUG_TOG_SQUELCHRESETCOUNT_MASK  (0x1F0000U)
#define USBPHY_DEBUG_TOG_SQUELCHRESETCOUNT_SHIFT (16U)
#define USBPHY_DEBUG_TOG_SQUELCHRESETCOUNT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_SQUELCHRESETCOUNT_SHIFT)) & USBPHY_DEBUG_TOG_SQUELCHRESETCOUNT_MASK)
#define USBPHY_DEBUG_TOG_RSVD2_MASK              (0xE00000U)
#define USBPHY_DEBUG_TOG_RSVD2_SHIFT             (21U)
#define USBPHY_DEBUG_TOG_RSVD2(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_RSVD2_SHIFT)) & USBPHY_DEBUG_TOG_RSVD2_MASK)
#define USBPHY_DEBUG_TOG_ENSQUELCHRESET_MASK     (0x1000000U)
#define USBPHY_DEBUG_TOG_ENSQUELCHRESET_SHIFT    (24U)
#define USBPHY_DEBUG_TOG_ENSQUELCHRESET(x)       (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_ENSQUELCHRESET_SHIFT)) & USBPHY_DEBUG_TOG_ENSQUELCHRESET_MASK)
#define USBPHY_DEBUG_TOG_SQUELCHRESETLENGTH_MASK (0x1E000000U)
#define USBPHY_DEBUG_TOG_SQUELCHRESETLENGTH_SHIFT (25U)
#define USBPHY_DEBUG_TOG_SQUELCHRESETLENGTH(x)   (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_SQUELCHRESETLENGTH_SHIFT)) & USBPHY_DEBUG_TOG_SQUELCHRESETLENGTH_MASK)
#define USBPHY_DEBUG_TOG_HOST_RESUME_DEBUG_MASK  (0x20000000U)
#define USBPHY_DEBUG_TOG_HOST_RESUME_DEBUG_SHIFT (29U)
#define USBPHY_DEBUG_TOG_HOST_RESUME_DEBUG(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_HOST_RESUME_DEBUG_SHIFT)) & USBPHY_DEBUG_TOG_HOST_RESUME_DEBUG_MASK)
#define USBPHY_DEBUG_TOG_CLKGATE_MASK            (0x40000000U)
#define USBPHY_DEBUG_TOG_CLKGATE_SHIFT           (30U)
#define USBPHY_DEBUG_TOG_CLKGATE(x)              (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_CLKGATE_SHIFT)) & USBPHY_DEBUG_TOG_CLKGATE_MASK)
#define USBPHY_DEBUG_TOG_RSVD3_MASK              (0x80000000U)
#define USBPHY_DEBUG_TOG_RSVD3_SHIFT             (31U)
#define USBPHY_DEBUG_TOG_RSVD3(x)                (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG_TOG_RSVD3_SHIFT)) & USBPHY_DEBUG_TOG_RSVD3_MASK)
/*! @} */

/*! @name DEBUG0_STATUS - UTMI Debug Status Register 0 */
/*! @{ */
#define USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT_MASK (0xFFFFU)
#define USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT_SHIFT (0U)
#define USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT(x) (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT_SHIFT)) & USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT_MASK)
#define USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT_MASK (0x3FF0000U)
#define USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT_SHIFT (16U)
#define USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT(x) (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT_SHIFT)) & USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT_MASK)
#define USBPHY_DEBUG0_STATUS_SQUELCH_COUNT_MASK  (0xFC000000U)
#define USBPHY_DEBUG0_STATUS_SQUELCH_COUNT_SHIFT (26U)
#define USBPHY_DEBUG0_STATUS_SQUELCH_COUNT(x)    (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG0_STATUS_SQUELCH_COUNT_SHIFT)) & USBPHY_DEBUG0_STATUS_SQUELCH_COUNT_MASK)
/*! @} */

/*! @name DEBUG1 - UTMI Debug Status Register 1 */
/*! @{ */
#define USBPHY_DEBUG1_RSVD0_MASK                 (0x1FFFU)
#define USBPHY_DEBUG1_RSVD0_SHIFT                (0U)
#define USBPHY_DEBUG1_RSVD0(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_RSVD0_SHIFT)) & USBPHY_DEBUG1_RSVD0_MASK)
#define USBPHY_DEBUG1_ENTAILADJVD_MASK           (0x6000U)
#define USBPHY_DEBUG1_ENTAILADJVD_SHIFT          (13U)
#define USBPHY_DEBUG1_ENTAILADJVD(x)             (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_ENTAILADJVD_SHIFT)) & USBPHY_DEBUG1_ENTAILADJVD_MASK)
#define USBPHY_DEBUG1_RSVD1_MASK                 (0xFFFF8000U)
#define USBPHY_DEBUG1_RSVD1_SHIFT                (15U)
#define USBPHY_DEBUG1_RSVD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_RSVD1_SHIFT)) & USBPHY_DEBUG1_RSVD1_MASK)
/*! @} */

/*! @name DEBUG1_SET - UTMI Debug Status Register 1 */
/*! @{ */
#define USBPHY_DEBUG1_SET_RSVD0_MASK             (0x1FFFU)
#define USBPHY_DEBUG1_SET_RSVD0_SHIFT            (0U)
#define USBPHY_DEBUG1_SET_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_SET_RSVD0_SHIFT)) & USBPHY_DEBUG1_SET_RSVD0_MASK)
#define USBPHY_DEBUG1_SET_ENTAILADJVD_MASK       (0x6000U)
#define USBPHY_DEBUG1_SET_ENTAILADJVD_SHIFT      (13U)
#define USBPHY_DEBUG1_SET_ENTAILADJVD(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_SET_ENTAILADJVD_SHIFT)) & USBPHY_DEBUG1_SET_ENTAILADJVD_MASK)
#define USBPHY_DEBUG1_SET_RSVD1_MASK             (0xFFFF8000U)
#define USBPHY_DEBUG1_SET_RSVD1_SHIFT            (15U)
#define USBPHY_DEBUG1_SET_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_SET_RSVD1_SHIFT)) & USBPHY_DEBUG1_SET_RSVD1_MASK)
/*! @} */

/*! @name DEBUG1_CLR - UTMI Debug Status Register 1 */
/*! @{ */
#define USBPHY_DEBUG1_CLR_RSVD0_MASK             (0x1FFFU)
#define USBPHY_DEBUG1_CLR_RSVD0_SHIFT            (0U)
#define USBPHY_DEBUG1_CLR_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_CLR_RSVD0_SHIFT)) & USBPHY_DEBUG1_CLR_RSVD0_MASK)
#define USBPHY_DEBUG1_CLR_ENTAILADJVD_MASK       (0x6000U)
#define USBPHY_DEBUG1_CLR_ENTAILADJVD_SHIFT      (13U)
#define USBPHY_DEBUG1_CLR_ENTAILADJVD(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_CLR_ENTAILADJVD_SHIFT)) & USBPHY_DEBUG1_CLR_ENTAILADJVD_MASK)
#define USBPHY_DEBUG1_CLR_RSVD1_MASK             (0xFFFF8000U)
#define USBPHY_DEBUG1_CLR_RSVD1_SHIFT            (15U)
#define USBPHY_DEBUG1_CLR_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_CLR_RSVD1_SHIFT)) & USBPHY_DEBUG1_CLR_RSVD1_MASK)
/*! @} */

/*! @name DEBUG1_TOG - UTMI Debug Status Register 1 */
/*! @{ */
#define USBPHY_DEBUG1_TOG_RSVD0_MASK             (0x1FFFU)
#define USBPHY_DEBUG1_TOG_RSVD0_SHIFT            (0U)
#define USBPHY_DEBUG1_TOG_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_TOG_RSVD0_SHIFT)) & USBPHY_DEBUG1_TOG_RSVD0_MASK)
#define USBPHY_DEBUG1_TOG_ENTAILADJVD_MASK       (0x6000U)
#define USBPHY_DEBUG1_TOG_ENTAILADJVD_SHIFT      (13U)
#define USBPHY_DEBUG1_TOG_ENTAILADJVD(x)         (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_TOG_ENTAILADJVD_SHIFT)) & USBPHY_DEBUG1_TOG_ENTAILADJVD_MASK)
#define USBPHY_DEBUG1_TOG_RSVD1_MASK             (0xFFFF8000U)
#define USBPHY_DEBUG1_TOG_RSVD1_SHIFT            (15U)
#define USBPHY_DEBUG1_TOG_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << USBPHY_DEBUG1_TOG_RSVD1_SHIFT)) & USBPHY_DEBUG1_TOG_RSVD1_MASK)
/*! @} */

/*! @name VERSION - UTMI RTL Version */
/*! @{ */
#define USBPHY_VERSION_STEP_MASK                 (0xFFFFU)
#define USBPHY_VERSION_STEP_SHIFT                (0U)
#define USBPHY_VERSION_STEP(x)                   (((uint32_t)(((uint32_t)(x)) << USBPHY_VERSION_STEP_SHIFT)) & USBPHY_VERSION_STEP_MASK)
#define USBPHY_VERSION_MINOR_MASK                (0xFF0000U)
#define USBPHY_VERSION_MINOR_SHIFT               (16U)
#define USBPHY_VERSION_MINOR(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_VERSION_MINOR_SHIFT)) & USBPHY_VERSION_MINOR_MASK)
#define USBPHY_VERSION_MAJOR_MASK                (0xFF000000U)
#define USBPHY_VERSION_MAJOR_SHIFT               (24U)
#define USBPHY_VERSION_MAJOR(x)                  (((uint32_t)(((uint32_t)(x)) << USBPHY_VERSION_MAJOR_SHIFT)) & USBPHY_VERSION_MAJOR_MASK)

/* Backward compatibility */
#define USBPHY_CTRL_ENDEVPLUGINDET_MASK     USBPHY_CTRL_ENDEVPLUGINDETECT_MASK
#define USBPHY_CTRL_ENDEVPLUGINDET_SHIFT    USBPHY_CTRL_ENDEVPLUGINDETECT_SHIFT
#define USBPHY_CTRL_ENDEVPLUGINDET(x)       USBPHY_CTRL_ENDEVPLUGINDETECT(x)
#define USBPHY_TX_TXCAL45DM_MASK            USBPHY_TX_TXCAL45DN_MASK
#define USBPHY_TX_TXCAL45DM(x)              USBPHY_TX_TXCAL45DN(x)

/*! @} */

/* ----------------------------------------------------------------------------
   -- USB_ANALOG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USB_ANALOG_Peripheral_Access_Layer USB_ANALOG Peripheral Access Layer
 * @{
 */

/** USB_ANALOG - Register Layout Typedef */
typedef struct {

	uint8_t RESERVED_0[416];

	struct {
		/* offset: 0x1A0, array step: 0x60 */

		volatile uint32_t VBUS_DETECT;									/**< USB VBUS Detect Register, array offset: 0x1A0, array step: 0x60 */

		volatile uint32_t VBUS_DETECT_SET;							/**< USB VBUS Detect Register, array offset: 0x1A4, array step: 0x60 */

		volatile uint32_t VBUS_DETECT_CLR;							/**< USB VBUS Detect Register, array offset: 0x1A8, array step: 0x60 */

		volatile uint32_t VBUS_DETECT_TOG;							/**< USB VBUS Detect Register, array offset: 0x1AC, array step: 0x60 */

		volatile uint32_t CHRG_DETECT;								/**< USB Charger Detect Register, array offset: 0x1B0, array step: 0x60 */

		volatile uint32_t CHRG_DETECT_SET;							/**< USB Charger Detect Register, array offset: 0x1B4, array step: 0x60 */

		volatile uint32_t CHRG_DETECT_CLR;							/**< USB Charger Detect Register, array offset: 0x1B8, array step: 0x60 */

		volatile uint32_t CHRG_DETECT_TOG;							/**< USB Charger Detect Register, array offset: 0x1BC, array step: 0x60 */

		volatile const uint32_t VBUS_DETECT_STAT;						/**< USB VBUS Detect Status Register, array offset: 0x1C0, array step: 0x60 */

		uint8_t RESERVED_0[12];

		volatile const uint32_t CHRG_DETECT_STAT;						/**< USB Charger Detect Status Register, array offset: 0x1D0, array step: 0x60 */

		uint8_t RESERVED_1[12];

		volatile uint32_t LOOPBACK;									/**< USB Loopback Test Register, array offset: 0x1E0, array step: 0x60 */

		volatile uint32_t LOOPBACK_SET;								/**< USB Loopback Test Register, array offset: 0x1E4, array step: 0x60 */

		volatile uint32_t LOOPBACK_CLR;								/**< USB Loopback Test Register, array offset: 0x1E8, array step: 0x60 */

		volatile uint32_t LOOPBACK_TOG;								/**< USB Loopback Test Register, array offset: 0x1EC, array step: 0x60 */

		volatile uint32_t MISC;										/**< USB Misc Register, array offset: 0x1F0, array step: 0x60 */

		volatile uint32_t MISC_SET;									/**< USB Misc Register, array offset: 0x1F4, array step: 0x60 */

		volatile uint32_t MISC_CLR;									/**< USB Misc Register, array offset: 0x1F8, array step: 0x60 */

		volatile uint32_t MISC_TOG;									/**< USB Misc Register, array offset: 0x1FC, array step: 0x60 */

	} INSTANCE[1];

	uint8_t RESERVED_1[96];

	volatile const uint32_t DIGPROG;									/**< Chip Silicon Version, offset: 0x260 */
} imxrt1020_usb_analog;

/* USB_ANALOG - Peripheral instance base addresses */
/** Peripheral USB_ANALOG base address */
#define IMXRT1020_USB_ANALOG_BASE							(0x400D8000u)
/** Peripheral IMXRT1020_USB_ANALOG base pointer */
#define IMXRT1020_USB_ANALOG								((imxrt1020_usb_analog *)IMXRT1020_USB_ANALOG_BASE)
/** Array initializer of IMXRT1020_USB_ANALOG peripheral base addresses */
#define IMXRT1020_USB_ANALOG_BASE_ADDRS						{ IMXRT1020_USB_ANALOG_BASE }
/** Array initializer of IMXRT1020_USB_ANALOG peripheral base pointers */
#define IMXRT1020_USB_ANALOG_BASE_PTRS						{ IMXRT1020_USB_ANALOG }

/* ----------------------------------------------------------------------------
   -- USB_ANALOG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USB_ANALOG_Register_Masks USB_ANALOG Register Masks
 * @{
 */

/*! @name VBUS_DETECT - USB VBUS Detect Register */
/*! @{ */
#define USB_ANALOG_VBUS_DETECT_VBUSVALID_THRESH_MASK (0x7U)
#define USB_ANALOG_VBUS_DETECT_VBUSVALID_THRESH_SHIFT (0U)
/*! VBUSVALID_THRESH
 *  0b000..4.0V
 *  0b001..4.1V
 *  0b010..4.2V
 *  0b011..4.3V
 *  0b100..4.4V (default)
 *  0b101..4.5V
 *  0b110..4.6V
 *  0b111..4.7V
 */
#define USB_ANALOG_VBUS_DETECT_VBUSVALID_THRESH(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_VBUSVALID_THRESH_SHIFT)) & USB_ANALOG_VBUS_DETECT_VBUSVALID_THRESH_MASK)
#define USB_ANALOG_VBUS_DETECT_VBUSVALID_PWRUP_CMPS_MASK		(0x100000U)
#define USB_ANALOG_VBUS_DETECT_VBUSVALID_PWRUP_CMPS_SHIFT		(20U)
#define USB_ANALOG_VBUS_DETECT_VBUSVALID_PWRUP_CMPS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_VBUSVALID_PWRUP_CMPS_SHIFT)) & USB_ANALOG_VBUS_DETECT_VBUSVALID_PWRUP_CMPS_MASK)
#define USB_ANALOG_VBUS_DETECT_DISCHARGE_VBUS_MASK			(0x4000000U)
#define USB_ANALOG_VBUS_DETECT_DISCHARGE_VBUS_SHIFT			(26U)
#define USB_ANALOG_VBUS_DETECT_DISCHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_DISCHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_DISCHARGE_VBUS_MASK)
#define USB_ANALOG_VBUS_DETECT_CHARGE_VBUS_MASK				(0x8000000U)
#define USB_ANALOG_VBUS_DETECT_CHARGE_VBUS_SHIFT			(27U)
#define USB_ANALOG_VBUS_DETECT_CHARGE_VBUS(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_CHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_CHARGE_VBUS_MASK)
/*! @} */

/* The count of USB_ANALOG_VBUS_DETECT */
#define USB_ANALOG_VBUS_DETECT_COUNT					(1U)

/*! @name VBUS_DETECT_SET - USB VBUS Detect Register */
/*! @{ */
#define USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_THRESH_MASK		(0x7U)
#define USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_THRESH_SHIFT		(0U)
/*! VBUSVALID_THRESH
 *  0b000..4.0V
 *  0b001..4.1V
 *  0b010..4.2V
 *  0b011..4.3V
 *  0b100..4.4V (default)
 *  0b101..4.5V
 *  0b110..4.6V
 *  0b111..4.7V
 */
#define USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_THRESH(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_THRESH_SHIFT)) & USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_THRESH_MASK)
#define USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_PWRUP_CMPS_MASK		(0x100000U)
#define USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_PWRUP_CMPS_SHIFT		(20U)
#define USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_PWRUP_CMPS(x)		(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_PWRUP_CMPS_SHIFT)) & USB_ANALOG_VBUS_DETECT_SET_VBUSVALID_PWRUP_CMPS_MASK)
#define USB_ANALOG_VBUS_DETECT_SET_DISCHARGE_VBUS_MASK			(0x4000000U)
#define USB_ANALOG_VBUS_DETECT_SET_DISCHARGE_VBUS_SHIFT			(26U)
#define USB_ANALOG_VBUS_DETECT_SET_DISCHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_SET_DISCHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_SET_DISCHARGE_VBUS_MASK)
#define USB_ANALOG_VBUS_DETECT_SET_CHARGE_VBUS_MASK			(0x8000000U)
#define USB_ANALOG_VBUS_DETECT_SET_CHARGE_VBUS_SHIFT			(27U)
#define USB_ANALOG_VBUS_DETECT_SET_CHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_SET_CHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_SET_CHARGE_VBUS_MASK)
/*! @} */

/* The count of USB_ANALOG_VBUS_DETECT_SET */
#define USB_ANALOG_VBUS_DETECT_SET_COUNT				(1U)

/*! @name VBUS_DETECT_CLR - USB VBUS Detect Register */
/*! @{ */
#define USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_THRESH_MASK		(0x7U)
#define USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_THRESH_SHIFT		(0U)
/*! VBUSVALID_THRESH
 *  0b000..4.0V
 *  0b001..4.1V
 *  0b010..4.2V
 *  0b011..4.3V
 *  0b100..4.4V (default)
 *  0b101..4.5V
 *  0b110..4.6V
 *  0b111..4.7V
 */
#define USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_THRESH(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_THRESH_SHIFT)) & USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_THRESH_MASK)
#define USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_PWRUP_CMPS_MASK		(0x100000U)
#define USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_PWRUP_CMPS_SHIFT		(20U)
#define USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_PWRUP_CMPS(x)		(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_PWRUP_CMPS_SHIFT)) & USB_ANALOG_VBUS_DETECT_CLR_VBUSVALID_PWRUP_CMPS_MASK)
#define USB_ANALOG_VBUS_DETECT_CLR_DISCHARGE_VBUS_MASK			(0x4000000U)
#define USB_ANALOG_VBUS_DETECT_CLR_DISCHARGE_VBUS_SHIFT			(26U)
#define USB_ANALOG_VBUS_DETECT_CLR_DISCHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_CLR_DISCHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_CLR_DISCHARGE_VBUS_MASK)
#define USB_ANALOG_VBUS_DETECT_CLR_CHARGE_VBUS_MASK			(0x8000000U)
#define USB_ANALOG_VBUS_DETECT_CLR_CHARGE_VBUS_SHIFT			(27U)
#define USB_ANALOG_VBUS_DETECT_CLR_CHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_CLR_CHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_CLR_CHARGE_VBUS_MASK)
/*! @} */

/* The count of USB_ANALOG_VBUS_DETECT_CLR */
#define USB_ANALOG_VBUS_DETECT_CLR_COUNT				(1U)

/*! @name VBUS_DETECT_TOG - USB VBUS Detect Register */
/*! @{ */
#define USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_THRESH_MASK		(0x7U)
#define USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_THRESH_SHIFT		(0U)
/*! VBUSVALID_THRESH
 *  0b000..4.0V
 *  0b001..4.1V
 *  0b010..4.2V
 *  0b011..4.3V
 *  0b100..4.4V (default)
 *  0b101..4.5V
 *  0b110..4.6V
 *  0b111..4.7V
 */
#define USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_THRESH(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_THRESH_SHIFT)) & USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_THRESH_MASK)
#define USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_PWRUP_CMPS_MASK		(0x100000U)
#define USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_PWRUP_CMPS_SHIFT		(20U)
#define USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_PWRUP_CMPS(x)		(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_PWRUP_CMPS_SHIFT)) & USB_ANALOG_VBUS_DETECT_TOG_VBUSVALID_PWRUP_CMPS_MASK)
#define USB_ANALOG_VBUS_DETECT_TOG_DISCHARGE_VBUS_MASK			(0x4000000U)
#define USB_ANALOG_VBUS_DETECT_TOG_DISCHARGE_VBUS_SHIFT			(26U)
#define USB_ANALOG_VBUS_DETECT_TOG_DISCHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_TOG_DISCHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_TOG_DISCHARGE_VBUS_MASK)
#define USB_ANALOG_VBUS_DETECT_TOG_CHARGE_VBUS_MASK			(0x8000000U)
#define USB_ANALOG_VBUS_DETECT_TOG_CHARGE_VBUS_SHIFT			(27U)
#define USB_ANALOG_VBUS_DETECT_TOG_CHARGE_VBUS(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_TOG_CHARGE_VBUS_SHIFT)) & USB_ANALOG_VBUS_DETECT_TOG_CHARGE_VBUS_MASK)
/*! @} */

/* The count of USB_ANALOG_VBUS_DETECT_TOG */
#define USB_ANALOG_VBUS_DETECT_TOG_COUNT				(1U)

/*! @name CHRG_DETECT - USB Charger Detect Register */
/*! @{ */
#define USB_ANALOG_CHRG_DETECT_CHK_CONTACT_MASK				(0x40000U)
#define USB_ANALOG_CHRG_DETECT_CHK_CONTACT_SHIFT			(18U)
/*! CHK_CONTACT - Check the contact of USB plug
 *  0b0..Do not check the contact of USB plug.
 *  0b1..Check whether the USB plug has been in contact with each other
 */
#define USB_ANALOG_CHRG_DETECT_CHK_CONTACT(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_CHK_CONTACT_SHIFT)) & USB_ANALOG_CHRG_DETECT_CHK_CONTACT_MASK)
#define USB_ANALOG_CHRG_DETECT_CHK_CHRG_B_MASK				(0x80000U)
#define USB_ANALOG_CHRG_DETECT_CHK_CHRG_B_SHIFT				(19U)
/*! CHK_CHRG_B - Check the charger connection
 *  0b0..Check whether a charger (either a dedicated charger or a host charger) is connected to USB port.
 *  0b1..Do not check whether a charger is connected to the USB port.
 */
#define USB_ANALOG_CHRG_DETECT_CHK_CHRG_B(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_CHK_CHRG_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_CHK_CHRG_B_MASK)
#define USB_ANALOG_CHRG_DETECT_EN_B_MASK				(0x100000U)
#define USB_ANALOG_CHRG_DETECT_EN_B_SHIFT				(20U)
/*! EN_B
 *  0b0..Enable the charger detector.
 *  0b1..Disable the charger detector.
 */
#define USB_ANALOG_CHRG_DETECT_EN_B(x)					(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_EN_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_EN_B_MASK)
/*! @} */

/* The count of USB_ANALOG_CHRG_DETECT */
#define USB_ANALOG_CHRG_DETECT_COUNT					(1U)

/*! @name CHRG_DETECT_SET - USB Charger Detect Register */
/*! @{ */
#define USB_ANALOG_CHRG_DETECT_SET_CHK_CONTACT_MASK			(0x40000U)
#define USB_ANALOG_CHRG_DETECT_SET_CHK_CONTACT_SHIFT			(18U)
/*! CHK_CONTACT - Check the contact of USB plug
 *  0b0..Do not check the contact of USB plug.
 *  0b1..Check whether the USB plug has been in contact with each other
 */
#define USB_ANALOG_CHRG_DETECT_SET_CHK_CONTACT(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_SET_CHK_CONTACT_SHIFT)) & USB_ANALOG_CHRG_DETECT_SET_CHK_CONTACT_MASK)
#define USB_ANALOG_CHRG_DETECT_SET_CHK_CHRG_B_MASK			(0x80000U)
#define USB_ANALOG_CHRG_DETECT_SET_CHK_CHRG_B_SHIFT			(19U)
/*! CHK_CHRG_B - Check the charger connection
 *  0b0..Check whether a charger (either a dedicated charger or a host charger) is connected to USB port.
 *  0b1..Do not check whether a charger is connected to the USB port.
 */
#define USB_ANALOG_CHRG_DETECT_SET_CHK_CHRG_B(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_SET_CHK_CHRG_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_SET_CHK_CHRG_B_MASK)
#define USB_ANALOG_CHRG_DETECT_SET_EN_B_MASK				(0x100000U)
#define USB_ANALOG_CHRG_DETECT_SET_EN_B_SHIFT				(20U)
/*! EN_B
 *  0b0..Enable the charger detector.
 *  0b1..Disable the charger detector.
 */
#define USB_ANALOG_CHRG_DETECT_SET_EN_B(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_SET_EN_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_SET_EN_B_MASK)
/*! @} */

/* The count of USB_ANALOG_CHRG_DETECT_SET */
#define USB_ANALOG_CHRG_DETECT_SET_COUNT				(1U)

/*! @name CHRG_DETECT_CLR - USB Charger Detect Register */
/*! @{ */
#define USB_ANALOG_CHRG_DETECT_CLR_CHK_CONTACT_MASK			(0x40000U)
#define USB_ANALOG_CHRG_DETECT_CLR_CHK_CONTACT_SHIFT			(18U)
/*! CHK_CONTACT - Check the contact of USB plug
 *  0b0..Do not check the contact of USB plug.
 *  0b1..Check whether the USB plug has been in contact with each other
 */
#define USB_ANALOG_CHRG_DETECT_CLR_CHK_CONTACT(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_CLR_CHK_CONTACT_SHIFT)) & USB_ANALOG_CHRG_DETECT_CLR_CHK_CONTACT_MASK)
#define USB_ANALOG_CHRG_DETECT_CLR_CHK_CHRG_B_MASK			(0x80000U)
#define USB_ANALOG_CHRG_DETECT_CLR_CHK_CHRG_B_SHIFT			(19U)
/*! CHK_CHRG_B - Check the charger connection
 *  0b0..Check whether a charger (either a dedicated charger or a host charger) is connected to USB port.
 *  0b1..Do not check whether a charger is connected to the USB port.
 */
#define USB_ANALOG_CHRG_DETECT_CLR_CHK_CHRG_B(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_CLR_CHK_CHRG_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_CLR_CHK_CHRG_B_MASK)
#define USB_ANALOG_CHRG_DETECT_CLR_EN_B_MASK				(0x100000U)
#define USB_ANALOG_CHRG_DETECT_CLR_EN_B_SHIFT				(20U)
/*! EN_B
 *  0b0..Enable the charger detector.
 *  0b1..Disable the charger detector.
 */
#define USB_ANALOG_CHRG_DETECT_CLR_EN_B(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_CLR_EN_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_CLR_EN_B_MASK)
/*! @} */

/* The count of USB_ANALOG_CHRG_DETECT_CLR */
#define USB_ANALOG_CHRG_DETECT_CLR_COUNT				(1U)

/*! @name CHRG_DETECT_TOG - USB Charger Detect Register */
/*! @{ */
#define USB_ANALOG_CHRG_DETECT_TOG_CHK_CONTACT_MASK			(0x40000U)
#define USB_ANALOG_CHRG_DETECT_TOG_CHK_CONTACT_SHIFT			(18U)
/*! CHK_CONTACT - Check the contact of USB plug
 *  0b0..Do not check the contact of USB plug.
 *  0b1..Check whether the USB plug has been in contact with each other
 */
#define USB_ANALOG_CHRG_DETECT_TOG_CHK_CONTACT(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_TOG_CHK_CONTACT_SHIFT)) & USB_ANALOG_CHRG_DETECT_TOG_CHK_CONTACT_MASK)
#define USB_ANALOG_CHRG_DETECT_TOG_CHK_CHRG_B_MASK			(0x80000U)
#define USB_ANALOG_CHRG_DETECT_TOG_CHK_CHRG_B_SHIFT			(19U)
/*! CHK_CHRG_B - Check the charger connection
 *  0b0..Check whether a charger (either a dedicated charger or a host charger) is connected to USB port.
 *  0b1..Do not check whether a charger is connected to the USB port.
 */
#define USB_ANALOG_CHRG_DETECT_TOG_CHK_CHRG_B(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_TOG_CHK_CHRG_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_TOG_CHK_CHRG_B_MASK)
#define USB_ANALOG_CHRG_DETECT_TOG_EN_B_MASK				(0x100000U)
#define USB_ANALOG_CHRG_DETECT_TOG_EN_B_SHIFT				(20U)
/*! EN_B
 *  0b0..Enable the charger detector.
 *  0b1..Disable the charger detector.
 */
#define USB_ANALOG_CHRG_DETECT_TOG_EN_B(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_TOG_EN_B_SHIFT)) & USB_ANALOG_CHRG_DETECT_TOG_EN_B_MASK)
/*! @} */

/* The count of USB_ANALOG_CHRG_DETECT_TOG */
#define USB_ANALOG_CHRG_DETECT_TOG_COUNT				(1U)

/*! @name VBUS_DETECT_STAT - USB VBUS Detect Status Register */
/*! @{ */
#define USB_ANALOG_VBUS_DETECT_STAT_SESSEND_MASK			(0x1U)
#define USB_ANALOG_VBUS_DETECT_STAT_SESSEND_SHIFT			(0U)
#define USB_ANALOG_VBUS_DETECT_STAT_SESSEND(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_STAT_SESSEND_SHIFT)) & USB_ANALOG_VBUS_DETECT_STAT_SESSEND_MASK)
#define USB_ANALOG_VBUS_DETECT_STAT_BVALID_MASK				(0x2U)
#define USB_ANALOG_VBUS_DETECT_STAT_BVALID_SHIFT			(1U)
#define USB_ANALOG_VBUS_DETECT_STAT_BVALID(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_STAT_BVALID_SHIFT)) & USB_ANALOG_VBUS_DETECT_STAT_BVALID_MASK)
#define USB_ANALOG_VBUS_DETECT_STAT_AVALID_MASK				(0x4U)
#define USB_ANALOG_VBUS_DETECT_STAT_AVALID_SHIFT			(2U)
#define USB_ANALOG_VBUS_DETECT_STAT_AVALID(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_STAT_AVALID_SHIFT)) & USB_ANALOG_VBUS_DETECT_STAT_AVALID_MASK)
#define USB_ANALOG_VBUS_DETECT_STAT_VBUS_VALID_MASK			(0x8U)
#define USB_ANALOG_VBUS_DETECT_STAT_VBUS_VALID_SHIFT			(3U)
#define USB_ANALOG_VBUS_DETECT_STAT_VBUS_VALID(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_VBUS_DETECT_STAT_VBUS_VALID_SHIFT)) & USB_ANALOG_VBUS_DETECT_STAT_VBUS_VALID_MASK)
/*! @} */

/* The count of USB_ANALOG_VBUS_DETECT_STAT */
#define USB_ANALOG_VBUS_DETECT_STAT_COUNT				(1U)

/*! @name CHRG_DETECT_STAT - USB Charger Detect Status Register */
/*! @{ */
#define USB_ANALOG_CHRG_DETECT_STAT_PLUG_CONTACT_MASK			(0x1U)
#define USB_ANALOG_CHRG_DETECT_STAT_PLUG_CONTACT_SHIFT			(0U)
/*! PLUG_CONTACT
 *  0b0..The USB plug has not made contact.
 *  0b1..The USB plug has made good contact.
 */
#define USB_ANALOG_CHRG_DETECT_STAT_PLUG_CONTACT(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_STAT_PLUG_CONTACT_SHIFT)) & USB_ANALOG_CHRG_DETECT_STAT_PLUG_CONTACT_MASK)
#define USB_ANALOG_CHRG_DETECT_STAT_CHRG_DETECTED_MASK			(0x2U)
#define USB_ANALOG_CHRG_DETECT_STAT_CHRG_DETECTED_SHIFT			(1U)
/*! CHRG_DETECTED
 *  0b0..The USB port is not connected to a charger.
 *  0b1..A charger (either a dedicated charger or a host charger) is connected to the USB port.
 */
#define USB_ANALOG_CHRG_DETECT_STAT_CHRG_DETECTED(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_STAT_CHRG_DETECTED_SHIFT)) & USB_ANALOG_CHRG_DETECT_STAT_CHRG_DETECTED_MASK)
#define USB_ANALOG_CHRG_DETECT_STAT_DM_STATE_MASK			(0x4U)
#define USB_ANALOG_CHRG_DETECT_STAT_DM_STATE_SHIFT			(2U)
#define USB_ANALOG_CHRG_DETECT_STAT_DM_STATE(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_STAT_DM_STATE_SHIFT)) & USB_ANALOG_CHRG_DETECT_STAT_DM_STATE_MASK)
#define USB_ANALOG_CHRG_DETECT_STAT_DP_STATE_MASK			(0x8U)
#define USB_ANALOG_CHRG_DETECT_STAT_DP_STATE_SHIFT			(3U)
#define USB_ANALOG_CHRG_DETECT_STAT_DP_STATE(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_CHRG_DETECT_STAT_DP_STATE_SHIFT)) & USB_ANALOG_CHRG_DETECT_STAT_DP_STATE_MASK)
/*! @} */

/* The count of USB_ANALOG_CHRG_DETECT_STAT */
#define USB_ANALOG_CHRG_DETECT_STAT_COUNT				(1U)

/*! @name LOOPBACK - USB Loopback Test Register */
/*! @{ */
#define USB_ANALOG_LOOPBACK_UTMI_TESTSTART_MASK				(0x1U)
#define USB_ANALOG_LOOPBACK_UTMI_TESTSTART_SHIFT			(0U)
#define USB_ANALOG_LOOPBACK_UTMI_TESTSTART(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_LOOPBACK_UTMI_TESTSTART_SHIFT)) & USB_ANALOG_LOOPBACK_UTMI_TESTSTART_MASK)
/*! @} */

/* The count of USB_ANALOG_LOOPBACK */
#define USB_ANALOG_LOOPBACK_COUNT					(1U)

/*! @name LOOPBACK_SET - USB Loopback Test Register */
/*! @{ */
#define USB_ANALOG_LOOPBACK_SET_UTMI_TESTSTART_MASK			(0x1U)
#define USB_ANALOG_LOOPBACK_SET_UTMI_TESTSTART_SHIFT			(0U)
#define USB_ANALOG_LOOPBACK_SET_UTMI_TESTSTART(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_LOOPBACK_SET_UTMI_TESTSTART_SHIFT)) & USB_ANALOG_LOOPBACK_SET_UTMI_TESTSTART_MASK)
/*! @} */

/* The count of USB_ANALOG_LOOPBACK_SET */
#define USB_ANALOG_LOOPBACK_SET_COUNT					(1U)

/*! @name LOOPBACK_CLR - USB Loopback Test Register */
/*! @{ */
#define USB_ANALOG_LOOPBACK_CLR_UTMI_TESTSTART_MASK			(0x1U)
#define USB_ANALOG_LOOPBACK_CLR_UTMI_TESTSTART_SHIFT			(0U)
#define USB_ANALOG_LOOPBACK_CLR_UTMI_TESTSTART(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_LOOPBACK_CLR_UTMI_TESTSTART_SHIFT)) & USB_ANALOG_LOOPBACK_CLR_UTMI_TESTSTART_MASK)
/*! @} */

/* The count of USB_ANALOG_LOOPBACK_CLR */
#define USB_ANALOG_LOOPBACK_CLR_COUNT					(1U)

/*! @name LOOPBACK_TOG - USB Loopback Test Register */
/*! @{ */
#define USB_ANALOG_LOOPBACK_TOG_UTMI_TESTSTART_MASK			(0x1U)
#define USB_ANALOG_LOOPBACK_TOG_UTMI_TESTSTART_SHIFT			(0U)
#define USB_ANALOG_LOOPBACK_TOG_UTMI_TESTSTART(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_LOOPBACK_TOG_UTMI_TESTSTART_SHIFT)) & USB_ANALOG_LOOPBACK_TOG_UTMI_TESTSTART_MASK)
/*! @} */

/* The count of USB_ANALOG_LOOPBACK_TOG */
#define USB_ANALOG_LOOPBACK_TOG_COUNT					(1U)

/*! @name MISC - USB Misc Register */
/*! @{ */
#define USB_ANALOG_MISC_HS_USE_EXTERNAL_R_MASK				(0x1U)
#define USB_ANALOG_MISC_HS_USE_EXTERNAL_R_SHIFT				(0U)
#define USB_ANALOG_MISC_HS_USE_EXTERNAL_R(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_HS_USE_EXTERNAL_R_SHIFT)) & USB_ANALOG_MISC_HS_USE_EXTERNAL_R_MASK)
#define USB_ANALOG_MISC_EN_DEGLITCH_MASK				(0x2U)
#define USB_ANALOG_MISC_EN_DEGLITCH_SHIFT				(1U)
#define USB_ANALOG_MISC_EN_DEGLITCH(x)					(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_EN_DEGLITCH_SHIFT)) & USB_ANALOG_MISC_EN_DEGLITCH_MASK)
#define USB_ANALOG_MISC_EN_CLK_UTMI_MASK				(0x40000000U)
#define USB_ANALOG_MISC_EN_CLK_UTMI_SHIFT				(30U)
#define USB_ANALOG_MISC_EN_CLK_UTMI(x)					(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_EN_CLK_UTMI_SHIFT)) & USB_ANALOG_MISC_EN_CLK_UTMI_MASK)
/*! @} */

/* The count of USB_ANALOG_MISC */
#define USB_ANALOG_MISC_COUNT						(1U)

/*! @name MISC_SET - USB Misc Register */
/*! @{ */
#define USB_ANALOG_MISC_SET_HS_USE_EXTERNAL_R_MASK			(0x1U)
#define USB_ANALOG_MISC_SET_HS_USE_EXTERNAL_R_SHIFT			(0U)
#define USB_ANALOG_MISC_SET_HS_USE_EXTERNAL_R(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_SET_HS_USE_EXTERNAL_R_SHIFT)) & USB_ANALOG_MISC_SET_HS_USE_EXTERNAL_R_MASK)
#define USB_ANALOG_MISC_SET_EN_DEGLITCH_MASK				(0x2U)
#define USB_ANALOG_MISC_SET_EN_DEGLITCH_SHIFT				(1U)
#define USB_ANALOG_MISC_SET_EN_DEGLITCH(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_SET_EN_DEGLITCH_SHIFT)) & USB_ANALOG_MISC_SET_EN_DEGLITCH_MASK)
#define USB_ANALOG_MISC_SET_EN_CLK_UTMI_MASK				(0x40000000U)
#define USB_ANALOG_MISC_SET_EN_CLK_UTMI_SHIFT				(30U)
#define USB_ANALOG_MISC_SET_EN_CLK_UTMI(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_SET_EN_CLK_UTMI_SHIFT)) & USB_ANALOG_MISC_SET_EN_CLK_UTMI_MASK)
/*! @} */

/* The count of USB_ANALOG_MISC_SET */
#define USB_ANALOG_MISC_SET_COUNT					(1U)

/*! @name MISC_CLR - USB Misc Register */
/*! @{ */
#define USB_ANALOG_MISC_CLR_HS_USE_EXTERNAL_R_MASK			(0x1U)
#define USB_ANALOG_MISC_CLR_HS_USE_EXTERNAL_R_SHIFT			(0U)
#define USB_ANALOG_MISC_CLR_HS_USE_EXTERNAL_R(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_CLR_HS_USE_EXTERNAL_R_SHIFT)) & USB_ANALOG_MISC_CLR_HS_USE_EXTERNAL_R_MASK)
#define USB_ANALOG_MISC_CLR_EN_DEGLITCH_MASK				(0x2U)
#define USB_ANALOG_MISC_CLR_EN_DEGLITCH_SHIFT				(1U)
#define USB_ANALOG_MISC_CLR_EN_DEGLITCH(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_CLR_EN_DEGLITCH_SHIFT)) & USB_ANALOG_MISC_CLR_EN_DEGLITCH_MASK)
#define USB_ANALOG_MISC_CLR_EN_CLK_UTMI_MASK				(0x40000000U)
#define USB_ANALOG_MISC_CLR_EN_CLK_UTMI_SHIFT				(30U)
#define USB_ANALOG_MISC_CLR_EN_CLK_UTMI(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_CLR_EN_CLK_UTMI_SHIFT)) & USB_ANALOG_MISC_CLR_EN_CLK_UTMI_MASK)
/*! @} */

/* The count of USB_ANALOG_MISC_CLR */
#define USB_ANALOG_MISC_CLR_COUNT					(1U)

/*! @name MISC_TOG - USB Misc Register */
/*! @{ */
#define USB_ANALOG_MISC_TOG_HS_USE_EXTERNAL_R_MASK			(0x1U)
#define USB_ANALOG_MISC_TOG_HS_USE_EXTERNAL_R_SHIFT			(0U)
#define USB_ANALOG_MISC_TOG_HS_USE_EXTERNAL_R(x)			(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_TOG_HS_USE_EXTERNAL_R_SHIFT)) & USB_ANALOG_MISC_TOG_HS_USE_EXTERNAL_R_MASK)
#define USB_ANALOG_MISC_TOG_EN_DEGLITCH_MASK				(0x2U)
#define USB_ANALOG_MISC_TOG_EN_DEGLITCH_SHIFT				(1U)
#define USB_ANALOG_MISC_TOG_EN_DEGLITCH(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_TOG_EN_DEGLITCH_SHIFT)) & USB_ANALOG_MISC_TOG_EN_DEGLITCH_MASK)
#define USB_ANALOG_MISC_TOG_EN_CLK_UTMI_MASK				(0x40000000U)
#define USB_ANALOG_MISC_TOG_EN_CLK_UTMI_SHIFT				(30U)
#define USB_ANALOG_MISC_TOG_EN_CLK_UTMI(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_MISC_TOG_EN_CLK_UTMI_SHIFT)) & USB_ANALOG_MISC_TOG_EN_CLK_UTMI_MASK)
/*! @} */

/* The count of USB_ANALOG_MISC_TOG */
#define USB_ANALOG_MISC_TOG_COUNT					(1U)

/*! @name DIGPROG - Chip Silicon Version */
/*! @{ */
#define USB_ANALOG_DIGPROG_SILICON_REVISION_MASK			(0xFFFFFFFFU)
#define USB_ANALOG_DIGPROG_SILICON_REVISION_SHIFT			(0U)
/*! SILICON_REVISION
 *  0b00000000011010110000000000000000..Silicon revision 1.0
 */
#define USB_ANALOG_DIGPROG_SILICON_REVISION(x)				(((uint32_t)(((uint32_t)(x)) << USB_ANALOG_DIGPROG_SILICON_REVISION_SHIFT)) & USB_ANALOG_DIGPROG_SILICON_REVISION_MASK)
/*! @} */
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* EHCI host macros */
#define EHCI_HOST_T_INVALID_VALUE					(1U)
#define EHCI_HOST_POINTER_TYPE_ITD					(0x00U)
#define EHCI_HOST_POINTER_TYPE_QH					(0x00000002U)
#define EHCI_HOST_POINTER_TYPE_SITD					(0x00000004U)
#define EHCI_HOST_POINTER_TYPE_FSTN					(0x00000006U)
#define EHCI_HOST_POINTER_TYPE_MASK					(0x00000006U)
#define EHCI_HOST_POINTER_ADDRESS_MASK					(0xFFFFFFE0U)
#define EHCI_HOST_PID_OUT						(0U)
#define EHCI_HOST_PID_IN						(1U)
#define EHCI_HOST_PID_SETUP						(2U)

#define EHCI_HOST_QH_RL_SHIFT						(28U)
#define EHCI_HOST_QH_RL_MASK						(0xF0000000U)
#define EHCI_HOST_QH_C_SHIFT						(27U)
#define EHCI_HOST_QH_MAX_PACKET_LENGTH_SHIFT				(16U)
#define EHCI_HOST_QH_MAX_PACKET_LENGTH_MASK				(0x07FF0000U)
#define EHCI_HOST_QH_H_SHIFT						(15U)
#define EHCI_HOST_QH_DTC_SHIFT						(14U)
#define EHCI_HOST_QH_EPS_SHIFT						(12U)
#define EHCI_HOST_QH_ENDPT_SHIFT					(8U)
#define EHCI_HOST_QH_I_SHIFT						(7U)
#define EHCI_HOST_QH_DEVICE_ADDRESS_SHIFT				(0U)
#define EHCI_HOST_QH_DEVICE_ADDRESS_MASK				(0x0000007FU)
#define EHCI_HOST_QH_MULT_SHIFT						(30U)
#define EHCI_HOST_QH_PORT_NUMBER_SHIFT					(23U)
#define EHCI_HOST_QH_HUB_ADDR_SHIFT					(16U)
#define EHCI_HOST_QH_UFRAME_CMASK_SHIFT					(8U)
#define EHCI_HOST_QH_UFRAME_SMASK_SHIFT					(0U)
#define EHCI_HOST_QH_STATUS_ERROR_MASK					(0x0000007EU)
#define EHCI_HOST_QH_STATUS_NOSTALL_ERROR_MASK				(0x0000003EU)

#define EHCI_HOST_QTD_DT_SHIFT						(31U)
#define EHCI_HOST_QTD_DT_MASK						(0x80000000U)
#define EHCI_HOST_QTD_TOTAL_BYTES_SHIFT					(16U)
#define EHCI_HOST_QTD_TOTAL_BYTES_MASK					(0x7FFF0000U)
#define EHCI_HOST_QTD_IOC_MASK						(0x00008000U)
#define EHCI_HOST_QTD_C_PAGE_SHIFT					(12U)
#define EHCI_HOST_QTD_CERR_SHIFT					(10U)
#define EHCI_HOST_QTD_CERR_MAX_VALUE					(0x00000003U)
#define EHCI_HOST_QTD_PID_CODE_SHIFT					(8U)
#define EHCI_HOST_QTD_STATUS_SHIFT					(0U)
#define EHCI_HOST_QTD_CURRENT_OFFSET_MASK				(0x00000FFFU)
#define EHCI_HOST_QTD_BUFFER_POINTER_SHIFT				(12U)
#define EHCI_HOST_QTD_STATUS_ACTIVE_MASK				(0x00000080U)
#define EHCI_HOST_QTD_STATUS_MASK					(0x000000ffU)
#define EHCI_HOST_QTD_STATUS_ERROR_MASK					(0x0000007EU)
#define EHCI_HOST_QTD_STATUS_STALL_ERROR_MASK				(0x00000040U)

#define EHCI_HOST_ITD_STATUS_ACTIVE_MASK				(0x80000000U)
#define EHCI_HOST_ITD_TRANSACTION_LEN_SHIFT				(16U)
#define EHCI_HOST_ITD_TRANSACTION_LEN_MASK				(0x0FFF0000U)
#define EHCI_HOST_ITD_IOC_SHIFT						(15U)
#define EHCI_HOST_ITD_PG_SHIFT						(12U)
#define EHCI_HOST_ITD_TRANSACTION_OFFSET_SHIFT				(0U)
#define EHCI_HOST_ITD_TRANSACTION_OFFSET_MASK				(0x00000FFFU)
#define EHCI_HOST_ITD_BUFFER_POINTER_SHIFT				(12U)
#define EHCI_HOST_ITD_ENDPT_SHIFT					(8U)
#define EHCI_HOST_ITD_DEVICE_ADDRESS_SHIFT				(0U)
#define EHCI_HOST_ITD_MAX_PACKET_SIZE_SHIFT				(0U)
#define EHCI_HOST_ITD_MULT_SHIFT					(0U)
#define EHCI_HOST_ITD_DIRECTION_SHIFT					(11U)

#define EHCI_HOST_SITD_STATUS_ACTIVE_MASK				(0x00000080U)
#define EHCI_HOST_SITD_DIRECTION_SHIFT					(31U)
#define EHCI_HOST_SITD_PORT_NUMBER_SHIFT				(24U)
#define EHCI_HOST_SITD_HUB_ADDR_SHIFT					(16U)
#define EHCI_HOST_SITD_ENDPT_SHIFT					(8U)
#define EHCI_HOST_SITD_DEVICE_ADDRESS_SHIFT				(0U)
#define EHCI_HOST_SITD_CMASK_SHIFT					(8U)
#define EHCI_HOST_SITD_SMASK_SHIFT					(0U)
#define EHCI_HOST_SITD_TOTAL_BYTES_SHIFT				(16U)
#define EHCI_HOST_SITD_TOTAL_BYTES_MASK					(0x03FF0000U)
#define EHCI_HOST_SITD_TP_SHIFT						(3U)
#define EHCI_HOST_SITD_TCOUNT_SHIFT					(0U)
#define EHCI_HOST_SITD_IOC_SHIFT					(31U)

/* register related MACROs */
#define EHCI_PORTSC1_W1_BITS						(0x0000002AU)
#define EHCI_MAX_UFRAME_VALUE						(0x00003FFFU)

/* task event */
#define EHCI_TASK_EVENT_DEVICE_ATTACH					(0x01U)
#define EHCI_TASK_EVENT_TRANSACTION_DONE				(0x02U)
#define EHCI_TASK_EVENT_DEVICE_DETACH					(0x04U)
#define EHCI_TASK_EVENT_PORT_CHANGE					(0x08U)
#define EHCI_TASK_EVENT_TIMER0						(0x10U)
#define EHCI_TASK_EVENT_TIMER1						(0x20U)

#define USB_HostEhciLock()						USB_ENTER_CRITICAL()
#define USB_HostEhciUnlock()						USB_EXIT_CRITICAL()

/*******************************************************************************
 * KHCI driver public structures, enumerations, macros, functions
 ******************************************************************************/

/*!
 * @addtogroup usb_host_controller_ehci
 * @{
 */

/*! @brief The maximum supported ISO pipe number */
#define USB_HOST_EHCI_ISO_NUMBER					USB_HOST_CONFIG_EHCI_MAX_ITD
/*! @brief Check the port connect state delay if the state is unstable */
#define USB_HOST_EHCI_PORT_CONNECT_DEBOUNCE_DELAY			(101U)
/*! @brief Delay for port reset */
#define USB_HOST_EHCI_PORT_RESET_DELAY					(11U)
/*! @brief The SITD inserts a frame interval for putting more SITD continuously.
 * There is an interval when an application sends two FS/LS ISO transfers.
 * When the interval is less than the macro, the two transfers are continuous in the frame list. Otherwise, the two
 * transfers
 * are not continuous.
 * For example:
 * - Use case 1: when inserting the SITD first, the inserted frame = the current frame value + this MACRO value.
 * - Use case 2: when inserting SITD is not first, choose between the last inserted frame value and the
 * current frame value according to the following criteria:
 *           If the interval is less than the MACRO value, the new SITD is continuous with the last SITD.
 *           If not, the new SITD inserting frame = the current frame value + this MACRO value.
 */
#define USB_HOST_EHCI_ISO_BOUNCE_FRAME_NUMBER				(2U)
/*! @brief The ITD inserts a micro-frame interval for putting more ITD continuously.
 * There is an interval when an application sends two HS ISO transfers.
 * When the interval is less than the macro, the two transfers are continuous in the frame list. Otherwise, the two
 * transfers
 * are not continuous.
 * For example:
 * - Use case 1: when inserting ITD first, the inserted micro-frame = the current micro-frame value + this MACRO value.
 * - Use case 2: when inserting ITD is not first, choose between the last inserted micro-frame value and the
 * current micro-frame value according to the following criteria:
 *           If the interval is less than this MACRO value, the new ITD is continuous with the last ITD.
 *           If not, the new ITD inserting micro-frame = the current micro-frame value + this MACRO value.
 */
#define USB_HOST_EHCI_ISO_BOUNCE_UFRAME_NUMBER				(16U)
/*! @brief Control or bulk transaction timeout value (unit: 100 ms) */
#define USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE			(50U)

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
typedef enum _bus_ehci_suspend_request_state {
	kBus_EhciIdle = 0U,
	kBus_EhciStartSuspend,
	kBus_EhciSuspended,
	kBus_EhciStartResume,
} bus_ehci_suspend_request_state_t;

#endif	/* 
 */

/*! @brief EHCI state for device attachment/detachment. */
typedef enum _host_ehci_device_state_ {
	kEHCIDevicePhyAttached = 1, /*!< Device is physically attached */
	kEHCIDeviceAttached, /*!< Device is attached and initialized */
	kEHCIDeviceDetached, /*!< Device is detached and de-initialized */
} host_ehci_device_state_t;

/*! @brief EHCI pipe structure */
typedef struct _usb_host_ehci_pipe {

	usb_host_pipe_t pipeCommon;	/*!< Common pipe information */

	void *ehciQh;				/*!< Control/bulk/interrupt: QH; ISO: usb_host_ehci_iso_t */


	/* bandwidth */
	uint16_t uframeInterval;	/*!< Micro-frame interval value */

	uint16_t startFrame;		/*!<
								   Bandwidth start frame: its value is from 0 to frame_list.
								 */

	uint16_t dataTime;		/*!<
								   Bandwidth time value:
								   - When the host works as HS: it's the data bandwidth value.
								   - When the host works as FS/LS:
								   - For FS/LS device, it's the data bandwidth value when transferring the data by FS/LS.
								   - For HS device, it's the data bandwidth value when transferring the data by HS.
								 */

	uint16_t startSplitTime;	/*!<
								   Start splitting the bandwidth time value:
								   - When the host works as HS, it is the start split bandwidth value.
								 */

	uint16_t completeSplitTime;	/*!<
									   Complete splitting the bandwidth time value:
									   - When host works as HS, it is the complete split bandwidth value.
									 */

	uint8_t startUframe;		/*!<
								   Bandwidth start micro-frame: its value is from 0 to 7.
								 */

	uint8_t uframeSmask;		/*!<
								   Start micro-frame.
								   - When host works as an HS:
								   - For FS/LS device, it's the interrupt or ISO transfer start-split mask.
								   - For HS device, it's the interrupt transfer start micro-frame mask.
								   - When host works as FS/LS, it's the interrupt and ISO start micro-frame mask
								 */

	uint8_t uframeCmask;		/*!<
								   Complete micro-frame
								   - When host works as HS:
								   - For FS/LS device, it's the interrupt or ISO transfer complete-split mask.
								 */
} usb_host_ehci_pipe_t;

/*! @brief EHCI QH structure. See the USB EHCI specification */
typedef struct _usb_host_ehci_qh {

	uint32_t horizontalLinkPointer;	/*!< QH specification filed, queue head a horizontal link pointer */

	uint32_t staticEndpointStates[2];	/*!< QH specification filed, static endpoint state and configuration information */

	uint32_t currentQtdPointer;	/*!< QH specification filed, current qTD pointer */

	uint32_t nextQtdPointer;	/*!< QH specification filed, next qTD pointer */

	uint32_t alternateNextQtdPointer;	/*!< QH specification filed, alternate next qTD pointer */

	uint32_t transferOverlayResults[6];	/*!< QH specification filed, transfer overlay configuration and transfer results */


	/* reserved space */
	usb_host_ehci_pipe_t *ehciPipePointer;	/*!< EHCI pipe pointer */

	usb_host_transfer_t *ehciTransferHead;	/*!< Transfer list head on this QH */

	usb_host_transfer_t *ehciTransferTail;	/*!< Transfer list tail on this QH */

	uint16_t timeOutValue;		/*!< Its maximum value is USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE. When the value is zero, the transfer times out. */

	uint16_t timeOutLabel;		/*!< It's used to judge the transfer timeout. The EHCI driver maintain the value */
} usb_host_ehci_qh_t;

/*! @brief EHCI QTD structure. See the USB EHCI specification. */
typedef struct _usb_host_ehci_qtd {

	uint32_t nextQtdPointer;	/*!< QTD specification filed, the next QTD pointer */

	uint32_t alternateNextQtdPointer;	/*!< QTD specification filed, alternate next QTD pointer */

	uint32_t transferResults[2];	/*!< QTD specification filed, transfer results fields */

	uint32_t bufferPointers[4];	/*!< QTD specification filed, transfer buffer fields */
} usb_host_ehci_qtd_t;

/*! @brief EHCI ITD structure. See the USB EHCI specification. */
typedef struct _usb_host_ehci_itd {

	uint32_t nextLinkPointer;	/*!< ITD specification filed, the next linker pointer */

	uint32_t transactions[8];	/*!< ITD specification filed, transactions information */

	uint32_t bufferPointers[7];	/*!< ITD specification filed, transfer buffer fields */


	/* add space */
	struct _usb_host_ehci_itd *nextItdPointer;	/*!< Next ITD pointer */

	uint32_t frameEntryIndex;	/*!< The ITD inserted frame value */

	uint32_t reserved[6];		/*!< Reserved fields for 32 bytes align */
} usb_host_ehci_itd_t;

/*! @brief EHCI SITD structure. See the USB EHCI specification. */
typedef struct _usb_host_ehci_sitd {

	uint32_t nextLinkPointer;	/*!< SITD specification filed, the next linker pointer */

	uint32_t endpointStates[2];	/*!< SITD specification filed, endpoint configuration information */

	uint32_t transferResults[3];	/*!< SITD specification filed, transfer result fields */

	uint32_t backPointer;		/*!< SITD specification filed, back pointer */


	/* reserved space */
	uint16_t frameEntryIndex;	/*!< The SITD inserted frame value */

	uint8_t nextSitdIndex;		/*!< The next SITD index; Get the next SITD pointer through adding base address with the
								   index. 0xFF means invalid. */

	uint8_t reserved;			/*!< Reserved fields for 32 bytes align */
} usb_host_ehci_sitd_t;

/*! @brief EHCI ISO structure; An ISO pipe has an instance of this structure to keep the ISO pipe-specific information.
 */
typedef struct _usb_host_ehci_iso {

	struct _usb_host_ehci_iso *next;	/*!< Next instance pointer */

	usb_host_pipe_t *ehciPipePointer;	/*!< This ISO's EHCI pipe pointer */

	usb_host_transfer_t *ehciTransferHead;	/*!< Transfer list head on this ISO pipe */

	usb_host_transfer_t *ehciTransferTail;	/*!< Transfer list head on this ISO pipe */


	uint16_t lastLinkFrame;	/*!< It means that the inserted frame for ISO ITD/SITD. 0xFFFF is invalid. For ITD, it is a
								   micro-frame value. For SITD, it is a frame value */
} usb_host_ehci_iso_t;

/*! @brief EHCI data structure */
typedef struct _usb_host_ehci_data {

#if ((defined(USB_HOST_CONFIG_EHCI_MAX_QH)) && (USB_HOST_CONFIG_EHCI_MAX_QH > 0U))
	usb_host_ehci_qh_t ehciQh[USB_HOST_CONFIG_EHCI_MAX_QH];	/*!< Idle QH list array */

#endif	/* 
 */
#if ((defined(USB_HOST_CONFIG_EHCI_MAX_QTD)) && (USB_HOST_CONFIG_EHCI_MAX_QTD > 0U))
	usb_host_ehci_qtd_t ehciQtd[USB_HOST_CONFIG_EHCI_MAX_QTD];	/*!< Idle QTD list array */

#endif	/* 
 */
#if ((defined(USB_HOST_CONFIG_EHCI_MAX_ITD)) && (USB_HOST_CONFIG_EHCI_MAX_ITD > 0U))
	usb_host_ehci_itd_t ehciItd[USB_HOST_CONFIG_EHCI_MAX_ITD];	/*!< Idle ITD list array */

#endif	/* 
 */
#if ((defined(USB_HOST_CONFIG_EHCI_MAX_SITD)) && (USB_HOST_CONFIG_EHCI_MAX_SITD > 0U))
	usb_host_ehci_sitd_t ehciSitd[USB_HOST_CONFIG_EHCI_MAX_SITD];	/*!< Idle SITD list array */

#endif	/* 
 */
#if ((defined(USB_HOST_EHCI_ISO_NUMBER)) && (USB_HOST_EHCI_ISO_NUMBER > 0U))
	usb_host_ehci_iso_t ehciIso[USB_HOST_EHCI_ISO_NUMBER];	/*!< Idle ISO list array */

#endif	/* 
 */
#if ((defined(USB_HOST_CONFIG_MAX_PIPES)) && (USB_HOST_CONFIG_MAX_PIPES > 0U))
	usb_host_ehci_pipe_t ehciPipe[USB_HOST_CONFIG_MAX_PIPES];	/*!< Idle pipe list array */

#endif	/* 
 */
} usb_host_ehci_data_t;

/*! @brief EHCI instance structure */
typedef struct _IMXRT1020_USB_HOST_ {

	uint32_t usb_analog_index;

	uint32_t irqNumber;

	uint32_t irqPriority;

	uint8_t *ehciFrameList;	/*!< The frame list of the current ehci instance */

	usb_host_ehci_qh_t *ehciQhList;	/*!< Idle QH list pointer */

	usb_host_ehci_qtd_t *ehciQtdHead;	/*!< Idle QTD list pointer head */

	usb_host_ehci_qtd_t *ehciQtdTail;	/*!< Idle QTD list pointer tail (recently used qTD will be used) */


	usb_host_ehci_itd_t *ehciItdList;	/*!< Idle ITD list pointer */

	usb_host_ehci_sitd_t *ehciSitdIndexBase;	/*!< SITD buffer's start pointer */

	usb_host_ehci_sitd_t *ehciSitdList;	/*!< Idle SITD list pointer */

	usb_host_ehci_iso_t *ehciIsoList;	/*!< Idle ISO list pointer */

	imxrt1020_usb *usb;		/*!< EHCI IP base address */

	imxrt1020_usb_nc *usb_nc;	/*!< EHCI IP non-core base address */

	imxrt1020_usb_phy *usb_phy;

	imxrt1020_usb_analog *usb_analog;

	usb_host_ehci_qh_t *shedFirstQh;	/*!< First async QH */

	usb_host_ehci_pipe_t *ehciPipeIndexBase;	/*!< Pipe buffer's start pointer */

	usb_host_ehci_pipe_t *ehciPipeList;	/*!< Idle pipe list pointer */

	usb_host_ehci_pipe_t *ehciRunningPipeList;	/*!< Running pipe list pointer */


#ifdef USB_TIZEN_RT
	struct work_s ehciEventTrigger;	/* Supports interrupt bottom half */

#else	/* 
 */
	usb_osa_event_handle ehciEventTrigger;	/*!< EHCI task trigger */

#ifdef FSL_RTOS_FREE_RTOS
	osa_event_struct_t ehciEvent;

#else	/* 
 */
	event_t ehciEvent;

#endif	/* 
 */
	usb_osa_task_handle ehciEventTask;

#endif	/* 
 */

	USB_EVENT ehciEventFifo;


	////////////////////////////////////////////////////////////////////////////
	usb_osa_event_handle ehciStatusChangeTrigger;

#ifndef USB_TIZEN_RT
#ifdef FSL_RTOS_FREE_RTOS
	osa_event_struct_t ehciStatusChangeEvent;

#else	/* 
 */
	event_t ehciStatusChangeEvent;

#endif	/* 
 */
#endif	/* 
 */
	USB_EVENT ehciStatusChangeFifo;

	usb_osa_task_handle ehciStatusMonitorTask;


	uint8_t deviceAttached;	/*!< Device attach/detach state, see #host_ehci_device_state_t */

	uint8_t firstDeviceSpeed;	/*!< The first device's speed, the controller's work speed */

	uint8_t ehciItdNumber;		/*!< Idle ITD number */

	uint8_t ehciSitdNumber;	/*!< Idle SITD number */

	uint8_t ehciQtdNumber;		/*!< Idle QTD number */


	// from usb_host_instance_t
	host_callback_t deviceCallback;	/*!< Device attach/detach callback */


	usb_host_transfer_t transferList[USB_HOST_CONFIG_MAX_TRANSFERS];	/*!< Transfer resource */

	usb_host_transfer_t *transferHead;	/*!< Idle transfer head */

	void *deviceList;			/*!< Device list */


	uint8_t addressBitMap[16];	/*!< Used for address allocation. The first bit is the address 1, second bit is the address 2 */

	uint8_t occupied;			/*!< 0 - the instance is not occupied; 1 - the instance is occupied */


#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
	uint64_t matchTick;

	bus_ehci_suspend_request_state_t busSuspendStatus;	/*!< Bus Suspend Status */

	void *suspendedDevice;	/*!< Suspended device handle */

	volatile uint64_t hwTick;	/*!< Current hw tick(ms) */


	usb_host_pipe_handle controlPipe;	/*!< Device's control pipe */

#endif	/* 
 */
} IMXRT1020_USB_HOST;

extern IMXRT1020_USB_HOST gIMXRT1020_USB_HOST;

typedef struct _usb_host_ehci_sync_result {

	usb_status_t result;

	uint32_t dataLength;

	usb_osa_event_handle finished;	/*!< waiting for transaction end */

#ifndef USB_TIZEN_RT
#ifdef FSL_RTOS_FREE_RTOS
	osa_event_struct_t finishedEvent;

#else	/* 
 */
	event_t finishedEvent;

#endif	/* 
 */
#endif	/* 
 */
} usb_host_ehci_sync_result_t;


/*!
 * @name USB host EHCI APIs
 * @{
 */

/*!
 * @brief Creates the USB host EHCI instance.
 *
 * This function initializes the USB host EHCI controller driver.
 *
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_AllocFail            Allocating memory failed.
 * @retval kStatus_USB_Error                Host mutex create fail, KHCI/EHCI mutex or KHCI/EHCI event create fail.
 *                                          Or, KHCI/EHCI IP initialize fail.
 */
extern usb_status_t USB_HostEhciCreate();

/*!
 * @brief Destroys the USB host EHCI instance.
 *
 * This function de-initializes The USB host EHCI controller driver.
 *
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 */
extern usb_status_t USB_HostEhciDestroy();

/*!
 * @brief Opens the USB host pipe.
 *
 * This function opens a pipe according to the pipe_init_ptr parameter.
 *
 * @param[in] controllerHandle The controller handle.
 * @param[out] pipeHandle      The pipe handle pointer, it is used to return the pipe handle.
 * @param[in] pipeInit         It is used to initialize the pipe.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_Error                There is no idle pipe.
 *                                          Or, there is no idle QH for EHCI.
 *                                          Or, bandwidth allocate fail for EHCI.
 */
extern usb_status_t USB_HostEhciOpenPipe(usb_host_pipe_handle *pipeHandle,
		usb_host_pipe_init_t *pipeInit);

/*!
 * @brief Closes the USB host pipe.
 *
 * This function closes a pipe and releases related resources.
 *
 * @param[in] controllerHandle The controller handle.
 * @param[in] pipeHandle       The closing pipe handle.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 */
extern usb_status_t USB_HostEhciClosePipe(usb_host_pipe_handle pipeHandle);

/*!
 * @brief Sends data to the pipe.
 *
 * This function requests to send the transfer to the specified pipe.
 *
 * @param[in] controllerHandle The controller handle.
 * @param[in] pipeHandle       The sending pipe handle.
 * @param[in] transfer          The transfer information.
 *
 * @retval kStatus_USB_Success              Sent successfully.
 * @retval kStatus_USB_LackSwapBuffer       There is no swap buffer for KHCI.
 * @retval kStatus_USB_Error                There is no idle QTD/ITD/SITD for EHCI.
 */
extern usb_status_t USB_HostEhciWritePipe(usb_host_pipe_handle pipeHandle,
		usb_host_transfer_t *transfer);

/*!
 * @brief Receives data from the pipe.
 *
 * This function requests to receive the transfer from the specified pipe.
 *
 * @param[in] controllerHandle The controller handle.
 * @param[in] pipeHandle       The receiving pipe handle.
 * @param[in] transfer         The transfer information.

 * @retval kStatus_USB_Success              Send successfully.
 * @retval kStatus_USB_LackSwapBuffer       There is no swap buffer for KHCI.
 * @retval kStatus_USB_Error                There is no idle QTD/ITD/SITD for EHCI.
 */
extern usb_status_t USB_HostEhciReadpipe(usb_host_pipe_handle pipeHandle,
		usb_host_transfer_t *transfer);

/*!
 * @brief Controls the EHCI.
 *
 * This function controls the EHCI.
 *
 * @param[in] controllerHandle The controller handle.
 * @param[in] ioctlEvent       See enumeration host_bus_control_t.
 * @param[in] ioctlParam       The control parameter.
 *
 * @retval kStatus_USB_Success              Cancel successfully.
 * @retval kStatus_USB_InvalidHandle        The controllerHandle is a NULL pointer.
 */
extern usb_status_t USB_HostEhciIoctl(uint32_t ioctlEvent, void *ioctlParam);

#endif	/* _USB_HOST_CONTROLLER_EHCI_H_ */
