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

/***********************************************************************************************************************************
* This file is generated from BK7236_v6_ADDR Mapping_s.xlsm automatically
* Modify it manually is not recommended
* CHIP ID:BK7236,GENARATE TIME:2022-06-29 11:16:19
************************************************************************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	/* REG_0x00 */
	uint32_t dev_id; /**< 0x0,ASCII "PPRO", default value 0x5050524f, R */

	/* REG_0x01 */
	uint32_t dev_version;

	/* REG_0x02 */
	union {
		struct {
			uint32_t soft_reset:      1; /**< bit[0] PRRO soft reset */
			uint32_t clk_gate_bypass: 1; /**< bit[1] bypass PRRO clock gate */
			uint32_t reserved:       30; /**< bit[2:31] reserved */
		};
		uint32_t v;
	} global_ctrl;

	/* REG_0x03 */
	uint32_t dev_status;

	/* REG_0x04 */
	union {
		struct {
			uint32_t aonp_ahb_aon_ap   : 1;  //0x4[0],aonp_ahb_aon privilege cfg,0x1,R/W
			uint32_t aonp_ahb_sys_ap   : 1;  //0x4[1],aonp_ahb_system privilege cfg,0x1,R/W
			uint32_t aonp_ahb_flash_ap : 1;  //0x4[2],aonp_ahb flash_privilege cfg,0x1,R/W
			uint32_t bakp_ahb_gdma_ap  : 1;  //0x4[3],bakp_ahb_gdma privilege cfg,0x1,R/W
			uint32_t bakp_ahb_la_ap    : 1;  //0x4[4],bakp_ahb_la privilege cfg,0x1,R/W
			uint32_t ahbp_ahb_usb_ap   : 1;  //0x4[5],ahbp_ahb_usb privilege cfg,0x1,R/W
			uint32_t ahbp_ahb_can_ap   : 1;  //0x4[6],ahbp_ahb_can privilege cfg,0x1,R/W
			uint32_t ahbp_ahb_qspi0_ap : 1;  //0x4[7],ahbp_ahb_qspi0 privilege cfg,0x1,R/W
			uint32_t ahbp_ahb_qspi1_ap : 1;  //0x4[8],ahbp_ahb_qspi1 privilege cfg,0x1,R/W
			uint32_t ahbp_ahb_psram_ap : 1;  //0x4[9],ahbp_ahb_psram privilege cfg,0x1,R/W
			uint32_t audp_ahb_fft_ap   : 1;  //0x4[10],audp_ahb_fft privilege cfg,0x1,R/W
			uint32_t audp_ahb_sbc_ap   : 1;  //0x4[11],audp_ahb_sbc privilege cfg,0x1,R/W
			uint32_t vidp_ahb_jpge_ap  : 1;  //0x4[12],vidp_ahb jpg encode_privilege cfg,0x1,R/W
			uint32_t vidp_ahb_yuv_ap   : 1;  //0x4[13],vidp_ahb yuv_privilege cfg,0x1,R/W
			uint32_t vidp_ahb_jpgd_ap  : 1;  //0x4[14],vidp_ahb jpg decoder_privilege cfg,0x1,R/W
			uint32_t vidp_ahb_disp_ap  : 1;  //0x4[15],vidp_ahb display_privilege cfg,0x1,R/W
			uint32_t vidp_ahb_dmad_ap  : 1;  //0x4[16],vidp_ahb dmad_privilege cfg,0x1,R/W
			uint32_t vidp_ahb_rott_ap  : 1;  //0x4[17],vidp_ahb_rott_ap dmad_privilege cfg,0x1,R/W
			uint32_t wifi_mac_ahb_ap   : 1;  //0x4[18],wifi_ahb_privilege cfg,0x3,R/W
			uint32_t wifi_modem_ahb_ap : 1;  //0x4[19],wifi_ahb_privilege cfg,0x3,R/W
			uint32_t btdm_ahb_ap       : 1;  //0x4[20],btdm_ahb_privilege cfg,0x1,R/W
			uint32_t mbox0_ahb_ap      : 1;  //0x4[21],mbox0_ahb_privilege cfg,0x1,R/W
			uint32_t mbox1_ahb_ap      : 1;  //0x4[22],mbox1_ahb_privilege cfg,0x1,R/W
			uint32_t dma1_ahb_ap       : 1;  //0x4[23],dma1_ahb_privilege cfg,0x1,R/W
			uint32_t reserved          : 8;  //0x4[31:24],Reserved,0,R
		};
		uint32_t v;
	} ahb_cfg_ap;

	/* REG_0x05 */
	union {
		struct {
			uint32_t  aonp_ahb_aon_nsec              : 1;  //0x4[0],aonp_ahb_aon nonsecure cfg,0x0,R/W
			uint32_t  aonp_ahb_sys_nsec              : 1;  //0x4[1],aonp_ahb_system nonsecure cfg,0x0,R/W
			uint32_t  aonp_ahb_flash_nsec            : 1;  //0x4[2],aonp_ahb flash_nonsecure cfg,0x0,R/W
			uint32_t  bakp_ahb_gdma_nsec             : 1;  //0x4[3],bakp_ahb_gdma nonsecure cfg,0x0,R/W
			uint32_t  bakp_ahb_la_nsec               : 1;  //0x4[4],bakp_ahb_la nonsecure cfg,0x0,R/W
			uint32_t  ahbp_ahb_usb_nsec              : 1;  //0x4[5],ahbp_ahb_usb nonsecure cfg,0x0,R/W
			uint32_t  ahbp_ahb_can_nsec              : 1;  //0x4[6],ahbp_ahb_can nonsecure cfg,0x0,R/W
			uint32_t  ahbp_ahb_qspi0_nsec            : 1;  //0x4[7],ahbp_ahb_qspi0 nonsecure cfg,0x0,R/W
			uint32_t  ahbp_ahb_qspi1_nsec            : 1;  //0x4[8],ahbp_ahb_qspi1 nonsecure cfg,0x0,R/W
			uint32_t  ahbp_ahb_psram_nsec            : 1;  //0x4[9],ahbp_ahb_psram nonsecure cfg,0x0,R/W
			uint32_t  audp_ahb_fft_nsec              : 1;  //0x4[10],audp_ahb_fft nonsecure cfg,0x0,R/W
			uint32_t  audp_ahb_sbc_nsec              : 1;  //0x4[11],audp_ahb_sbc nonsecure cfg,0x0,R/W
			uint32_t  vidp_ahb_jpge_nsec             : 1;  //0x4[12],vidp_ahb jpg encode_nonsecure cfg,0x0,R/W
			uint32_t  vidp_ahb_yuv_nsec              : 1;  //0x4[13],vidp_ahb yuv_nonsecure cfg,0x0,R/W
			uint32_t  vidp_ahb_jpgd_nsec             : 1;  //0x4[14],vidp_ahb jpg decoder_nonsecure cfg,0x0,R/W
			uint32_t  vidp_ahb_disp_nsec             : 1;  //0x4[15],vidp_ahb display_nonsecure cfg,0x0,R/W
			uint32_t  vidp_ahb_dmad_nsec             : 1;  //0x4[16],vidp_ahb dmad_nonsecure cfg,0x0,R/W
			uint32_t  vidp_ahb_rott_nsec             : 1;  //0x4[17],vidp_ahb_rott_nsec dmad_nonsecure cfg,0x0,R/W
			uint32_t  wifi_mac_ahb_nsec              : 1;  //0x4[18],wifi_ahb_nonsecure cfg,0x0,R/W
			uint32_t  wifi_modem_ahb_nsec            : 1;  //0x4[19],wifi_ahb_nonsecure cfg,0x0,R/W
			uint32_t  btdm_ahb_nsec                  : 1;  //0x4[20],btdm_ahb_nonsecure cfg,0x0,R/W
			uint32_t  mbox0_ahb_nsec                 : 1;  //0x4[21],mbox0_ahb_nonsecure cfg,0x0,R/W
			uint32_t  mbox1_ahb_nsec                 : 1;  //0x4[22],mbox1_ahb_nonsecure cfg,0x0,R/W
			uint32_t  dma1_ahb_nsec                  : 1;  //0x4[23],dma1_ahb_nonsecure cfg,0x0,R/W
			uint32_t  reserved                       : 8;  //0x4[24:31],Reserved,0,R
		};
		uint32_t v;
	} ahb_cfg_nsec;

	/* REG_0x06 */
	union {
		struct {
			uint32_t  aonp_ahb_sresp                 : 1;  //0x6[0],aonp_ahb_response config,0x1,R/W
			uint32_t  bakp_ahb_sresp                 : 1;  //0x6[1],bakp_ahb_response config,0x1,R/W
			uint32_t  ahbp_ahb_sresp                 : 1;  //0x6[2],ahbp_ahb_response config,0x1,R/W
			uint32_t  audp_ahb_sresp                 : 1;  //0x6[3],audp_ahb_response config,0x1,R/W
			uint32_t  vidp_ahb_sresp                 : 1;  //0x6[4],vidp_ahb_response config,0x1,R/W
			uint32_t  wifi_ahb_sresp                 : 1;  //0x6[5],wifi_ahb_response config,0x1,R/W
			uint32_t  btdm_ahb_sresp                 : 1;  //0x6[6],btdm_ahb_response config,0x1,R/W
			uint32_t  mbox_ahb_sresp                 : 1;  //0x6[7],mbox_ahb_response config,0x1,R/W
			uint32_t  reserved                       :24;  //0x6[31:8],Reserved,0,R
		};
		uint32_t v;
	} ahb_cfg_sresp;

	/* REG_0x07 */
	union {
		struct {
			uint32_t  aonp_apb_wdt_ap                : 1;  //0x8[0]  ,aonp_apb_wdt privilege,0x1,R/W
			uint32_t  aonp_apb_timr_ap               : 1;  //0x8[1]  ,aonp_apb_timr privilege,0x1,R/W
			uint32_t  aonp_apb_uart_ap               : 1;  //0x8[2],aonp_apb_uart privilege,0x1,R/W
			uint32_t  aonp_apb_spi0_ap               : 1;  //0x8[3],aonp_apb_spi0 privilege,0x1,R/W
			uint32_t  aonp_apb_efs_ap                : 1;  //0x8[4],aonp_apb_efs privilege,0x1,R/W
			uint32_t  aonp_apb_mbic_ap               : 1;  //0x8[5],aonp_apb_mbic privilege,0x1,R/W
			uint32_t  bakp_apb_timer1_ap             : 1;  //0x8[6] ,bakp_apb_privilege,0x1,R/W
			uint32_t  bakp_apb_sdmadc_ap             : 1;  //0x8[7],bakp_apb_sdmadc_privilege,0x1,R/W
			uint32_t  bakp_apb_uart1_ap              : 1;  //0x8[8],bakp_apb_uart1_privilege,0x1,R/W
			uint32_t  bakp_apb_uart2_ap              : 1;  //0x8[9],bakp_apb_uart2_privilege,0x1,R/W
			uint32_t  bakp_apb_i2c0_ap               : 1;  //0x8[10],bakp_apb_i2c0_privilege,0x1,R/W
			uint32_t  bakp_apb_i2c1_ap               : 1;  //0x8[11],bakp_apb_i2c1_privilege,0x1,R/W
			uint32_t  bakp_apb_spi1_ap               : 1;  //0x8[12],bakp_apb_spi1_privilege,0x1,R/W
			uint32_t  bakp_apb_sadc_ap               : 1;  //0x8[13],bakp_apb_sadc_privilege,0x1,R/W
			uint32_t  bakp_apb_pwm_ap                : 1;  //0x8[14],bakp_apb_pwm_privilege,0x1,R/W
			uint32_t  bakp_apb_irda_ap               : 1;  //0x8[15],bakp_apb_irda_privilege,0x1,R/W
			uint32_t  bakp_apb_trng_ap               : 1;  //0x8[16],bakp_apb_trng_privilege,0x1,R/W
			uint32_t  bakp_apb_sdio_ap               : 1;  //0x8[17],bakp_apb_sdio_privilege,0x1,R/W
			uint32_t  bakp_apb_slcd_ap               : 1;  //0x8[18],bakp_apb_slcd_privilege,0x1,R/W
			uint32_t  bakp_apb_pwm1_ap               : 1;  //0x8[19],bakp_apb_pwm1_privilege,0x1,R/W
			uint32_t  audp_apb_aud_ap                : 1;  //0x8[20],audp_apb_aud_privilege,0x1,R/W
			uint32_t  audp_apb_i2s0_ap               : 1;  //0x8[21],audp_apb_i2s0_privilege,0x1,R/W
			uint32_t  audp_apb_i2s1_ap               : 1;  //0x8[22],audp_apb_i2s1_privilege,0x1,R/W
			uint32_t  audp_apb_i2s2_ap               : 1;  //0x8[23],audp_apb_i2s2_privilege,0x1,R/W
			uint32_t  vidp_apb_ap                    : 1;  //0x8[24]   ,vidp_apb_privilege,0x1,R/W
			uint32_t  btdm_apb_ap                    : 1;  //0x8[25]   ,btdm_apb_privilege,0x1,R/W
			uint32_t  reserved                       : 6;  //0x8[31:26],Reserved,0,R
		};
		uint32_t v;
	} apb_cfg_ap;

	/* REG_0x08 */
	union {
		struct {
			uint32_t  aonp_apb_wdt_nsec              : 1;  //0x8[0]  ,aonp_apb_wdt nsec,0x0,R/W
			uint32_t  aonp_apb_timr_nsec             : 1;  //0x8[1]  ,aonp_apb_timr nsec,0x0,R/W
			uint32_t  aonp_apb_uart_nsec             : 1;  //0x8[2],aonp_apb_uart nsec,0x0,R/W
			uint32_t  aonp_apb_spi0_nsec             : 1;  //0x8[3],aonp_apb_spi0 nsec,0x0,R/W
			uint32_t  aonp_apb_efs_nsec              : 1;  //0x8[4],aonp_apb_efs nsec,0x0,R/W
			uint32_t  aonp_apb_mbic_nsec             : 1;  //0x8[5],aonp_apb_mbic nsec,0x0,R/W
			uint32_t  bakp_apb_timer1_nsec           : 1;  //0x8[6] ,bakp_apb_nsec,0x0,R/W
			uint32_t  bakp_apb_sdmadc_nsec           : 1;  //0x8[7],bakp_apb_sdmadc_nsec,0x0,R/W
			uint32_t  bakp_apb_uart1_nsec            : 1;  //0x8[8],bakp_apb_uart1_nsec,0x0,R/W
			uint32_t  bakp_apb_uart2_nsec            : 1;  //0x8[9],bakp_apb_uart2_nsec,0x0,R/W
			uint32_t  bakp_apb_i2c0_nsec             : 1;  //0x8[10],bakp_apb_i2c0_nsec,0x0,R/W
			uint32_t  bakp_apb_i2c1_nsec             : 1;  //0x8[11],bakp_apb_i2c1_nsec,0x0,R/W
			uint32_t  bakp_apb_spi1_nsec             : 1;  //0x8[12],bakp_apb_spi1_nsec,0x0,R/W
			uint32_t  bakp_apb_sadc_nsec             : 1;  //0x8[13],bakp_apb_sadc_nsec,0x0,R/W
			uint32_t  bakp_apb_pwm_nsec              : 1;  //0x8[14],bakp_apb_pwm_nsec,0x0,R/W
			uint32_t  bakp_apb_irda_nsec             : 1;  //0x8[15],bakp_apb_irda_nsec,0x0,R/W
			uint32_t  bakp_apb_trng_nsec             : 1;  //0x8[16],bakp_apb_trng_nsec,0x0,R/W
			uint32_t  bakp_apb_sdio_nsec             : 1;  //0x8[17],bakp_apb_sdio_nsec,0x0,R/W
			uint32_t  bakp_apb_slcd_nsec             : 1;  //0x8[18],bakp_apb_slcd_nsec,0x0,R/W
			uint32_t  bakp_apb_pwm1_nsec             : 1;  //0x8[19],bakp_apb_pwm1_nsec,0x0,R/W
			uint32_t  audp_apb_aud_nsec              : 1;  //0x8[20],audp_apb_aud_nsec,0x0,R/W
			uint32_t  audp_apb_i2s0_nsec             : 1;  //0x8[21],audp_apb_i2s0_nsec,0x0,R/W
			uint32_t  audp_apb_i2s1_nsec             : 1;  //0x8[22],audp_apb_aud_nsec,0x0,R/W
			uint32_t  audp_apb_i2s2_nsec             : 1;  //0x8[23],audp_apb_i2s1_nsec,0x0,R/W
			uint32_t  vidp_apb_nsec                  : 1;  //0x8[24]   ,vidp_apb_nsec,0x0,R/W
			uint32_t  btdm_apb_nsec                  : 1;  //0x8[25]   ,btdm_apb_nsec,0x0,R/W
			uint32_t  reserved                       : 6;  //0x8[26:31],Reserved,0,R
		};
		uint32_t v;
	} apb_cfg_nsec;

	/* REG_0x09 */
	union {
		struct {
			uint32_t  aonp_apb_sresp                 : 1;  //0x9[0],aonp_apb_sresp,0x1,R/W
			uint32_t  bakp_apb_sresp                 : 1;  //0x9[1],bakp_apb_sresp,0x1,R/W
			uint32_t  audp_apb_sresp                 : 1;  //0x9[2],audp_apb_sresp,0x1,R/W
			uint32_t  vidp_apb_sresp                 : 1;  //0x9[3],vidp_apb_sresp,0x1,R/W
			uint32_t  btdm_apb_sresp                 : 1;  //0x9[4],btdm_apb_sresp,0x1,R/W
			uint32_t  reserved                       :27;  //0x9[31:5],Reserved,0,R
		};
		uint32_t v;
	} apb_cfg_sresp;

	/* REG_0x0a */
	union {
		struct {
			uint32_t  aonp_apb_nonsec                : 4;  //0xa[3:0],aonp_apb_nonsec,0x0,R/W
			uint32_t  reserved                       :28;  //0xa[31:4],Reserved,0,R
		};
		uint32_t v;
	} aonp_apb_nonsec;

	/* REG_0x0b */
	uint32_t aon_gpio_nonsec0;  //0xb[31:0],aon_gpio_nonsec0,0x0,R/W

	/* REG_0x0c */
	uint32_t aon_gpio_nonsec1;  //0xc[31:0],aon_gpio_nonsec1,0x0,R/W

	/* REG_0x0d */
	union {
		struct {
			uint32_t  jpenc_hnonsec                  : 1;  //0xd[0],jpenc_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0xd[31:1],Reserved,0,R
		};
		uint32_t v;
	} jpenc_hnonsec;

	/* REG_0x0e */
	union {
		struct {
			uint32_t  jpdec_hnonsec                  : 1;  //0xe[0],jpdec_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0xe[31:1],Reserved,0,R
		};
		uint32_t v;
	} jpdec_hnonsec;

	/* REG_0x0f */
	union {
		struct {
			uint32_t  dmad_m_hnonsec                 : 1;  //0xf[0],dmad_m_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0xf[31:1],Reserved,0,R
		};
		uint32_t v;
	} dmad_m_hnonsec;

	/* REG_0x10 */
	union {
		struct {
			uint32_t  mac_m_hnonsec                  : 1;  //0x10[0],mac_m_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0x10[31:1],Reserved,0,R
		};
		uint32_t v;
	} mac_m_hnonsec;

	/* REG_0x11 */
	union {
		struct {
			uint32_t  hsu_m_hnonsec                  : 1;  //0x11[0],hsu_m_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0x11[31:1],Reserved,0,R
		};
		uint32_t v;
	} hsu_m_hnonsec;

	/* REG_0x12 */
	union {
		struct {
			uint32_t  btdm_m_hnonsec                 : 1;  //0x12[0],btdm_m_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0x12[31:1],Reserved,0,R
		};
		uint32_t v;
	} btdm_m_hnonsec;

	/* REG_0x13 */
	union {
		struct {
			uint32_t  la_m_hnonsec                   : 1;  //0x13[0],la_m_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0x13[31:1],Reserved,0,R
		};
		uint32_t v;
	} la_m_hnonsec;

	/* REG_0x14 */
	union {
		struct {
			uint32_t  disp_m_hnonsec                 : 1;  //0x14[0],disp_m_hnonsec,0x0,R/W
			uint32_t  reserved                       :31;  //0x14[31:1],Reserved,0,R
		};
		uint32_t v;
	} disp_m_hnonsec;

	/* REG_0x15 */
	union {
		struct {
			uint32_t  enc_m_hnonsec                 : 1;  //0x15[0],enc_m_hnonsec,0x0,R/W
			uint32_t  reserved                      :31;  //0x15[31:1],Reserved,0,R
		};
		uint32_t v;
	} enc_m_hnonsec;

	uint32_t reg22_reserved;

	/* REG_0x17~REG_0x22 */
	union {
		uint32_t start;
		uint32_t end;
		uint32_t src;
		uint32_t dst;
	} cmp_addr[3];

	/* REG_0x23 */
	union {
		struct {
			uint32_t  cmp0_mismatch_int         : 1;  //0x23[0], compare group0 mismatch interrupt triggered
			uint32_t  cmp1_mismatch_int         : 1;  //0x23[1], compare group1 mismatch interrupt triggered
			uint32_t  cmp2_mismatch_int         : 1;  //0x23[2], compare group2 mismatch interrupt triggered
			uint32_t  reserved                  :31;  //0x23[31:3]
		};
		uint32_t v;
	} cmp_int_status;
} prro_hw_t;

#ifdef __cplusplus
}
#endif

