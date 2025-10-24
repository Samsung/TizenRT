// Copyright 2022-2023 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef volatile union {
	struct {
		uint32_t deviceid                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_device_id_t;


typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_version_id_t;


typedef volatile union {
	struct {
		uint32_t soft_reset               :  1; /**<bit[0 : 0] */
		uint32_t clkg_bps                 :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} prro_gbl_control_t;


typedef volatile union {
	struct {
		uint32_t devstatus                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_gbl_status_t;


typedef volatile union {
	struct {
		uint32_t aon_gpio_nsec0           : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_aonp_gpio_nsec0_t;


typedef volatile union {
	struct {
		uint32_t aon_gpio_nsec1           : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_aonp_gpio_nsec1_t;


typedef volatile union {
	struct {
		uint32_t aon_reg_nsec             :  1; /**<bit[0 : 0] */
		uint32_t aon_rtc_nsec             :  1; /**<bit[1 : 1] */
		uint32_t aon_gpiohig_nsec         :  1; /**<bit[2 : 2] */
		uint32_t aon_wdt_nsec             :  1; /**<bit[3 : 3] */
		uint32_t aon_ahb_aon_nsec         :  1; /**<bit[4 : 4] */
		uint32_t aon_ahb_sys_nsec         :  1; /**<bit[5 : 5] */
		uint32_t aon_ahb_flsh_nsec        :  1; /**<bit[6 : 6] */
		uint32_t aon_ahb_apb_nsec         :  1; /**<bit[7 : 7] */
		uint32_t aon_ahb_aon_ap           :  1; /**<bit[8 : 8] */
		uint32_t aon_ahb_sys_ap           :  1; /**<bit[9 : 9] */
		uint32_t aon_ahb_flsh_ap          :  1; /**<bit[10 : 10] */
		uint32_t aon_ahb_apb_ap           :  1; /**<bit[11 : 11] */
		uint32_t aon_ahb_sresp            :  1; /**<bit[12 : 12] */
		uint32_t aon_apb_wdt_nsec         :  1; /**<bit[13 : 13] */
		uint32_t aon_apb_timr0_nsec       :  1; /**<bit[14 : 14] */
		uint32_t aon_apb_uart0_nsec       :  1; /**<bit[15 : 15] */
		uint32_t aon_apb_spi0_nsec        :  1; /**<bit[16 : 16] */
		uint32_t aon_apb_efs_nsec         :  1; /**<bit[17 : 17] */
		uint32_t aon_apb_mcheck_nsec      :  1; /**<bit[18 : 18] */
		uint32_t aon_apb_ckmn_nsec        :  1; /**<bit[19 : 19] */
		uint32_t aon_apb_wdt_ap           :  1; /**<bit[20 : 20] */
		uint32_t aon_apb_timr0_ap         :  1; /**<bit[21 : 21] */
		uint32_t aon_apb_uart0_ap         :  1; /**<bit[22 : 22] */
		uint32_t aon_apb_spi0_ap          :  1; /**<bit[23 : 23] */
		uint32_t aon_apb_efs_ap           :  1; /**<bit[24 : 24] */
		uint32_t aon_apb_mcheck_ap        :  1; /**<bit[25 : 25] */
		uint32_t aon_apb_ckmn_ap          :  1; /**<bit[26 : 26] */
		uint32_t aon_apb_sresp            :  1; /**<bit[27 : 27] */
		uint32_t reserved_bit_28_31       :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} prro_aonp_sap_t;


typedef volatile union {
	struct {
		uint32_t bak_ahb_dma0_nsec        :  1; /**<bit[0 : 0] */
		uint32_t bak_ahb_dma1_nsec        :  1; /**<bit[1 : 1] */
		uint32_t bak_ahb_la_nsec          :  1; /**<bit[2 : 2] */
		uint32_t bak_ahb_apb_nsec         :  1; /**<bit[3 : 3] */
		uint32_t bak_ahb_dma0_ap          :  1; /**<bit[4 : 4] */
		uint32_t bak_ahb_dma1_ap          :  1; /**<bit[5 : 5] */
		uint32_t bak_ahb_la_ap            :  1; /**<bit[6 : 6] */
		uint32_t bak_ahb_apb_ap           :  1; /**<bit[7 : 7] */
		uint32_t bak_ahb_sresp            :  1; /**<bit[8 : 8] */
		uint32_t reserved_bit_9_31        : 23; /**<bit[9 : 31] */
	};
	uint32_t v;
} prro_bakp_ahb_sap_t;


typedef volatile union {
	struct {
		uint32_t bak_apb_timer1_nsec      :  1; /**<bit[0 : 0] */
		uint32_t bak_apb_sdmadc_nsec      :  1; /**<bit[1 : 1] */
		uint32_t bak_apb_uart1_nsec       :  1; /**<bit[2 : 2] */
		uint32_t bak_apb_uart2_nsec       :  1; /**<bit[3 : 3] */
		uint32_t bak_apb_i2c0_nsec        :  1; /**<bit[4 : 4] */
		uint32_t bak_apb_i2c1_nsec        :  1; /**<bit[5 : 5] */
		uint32_t bak_apb_spi1_nsec        :  1; /**<bit[6 : 6] */
		uint32_t bak_apb_sadc_nsec        :  1; /**<bit[7 : 7] */
		uint32_t bak_apb_pwm0_nsec        :  1; /**<bit[8 : 8] */
		uint32_t bak_apb_irda_nsec        :  1; /**<bit[9 : 9] */
		uint32_t bak_apb_trng_nsec        :  1; /**<bit[10 : 10] */
		uint32_t bak_apb_sdio_nsec        :  1; /**<bit[11 : 11] */
		uint32_t bak_apb_scld_nsec        :  1; /**<bit[12 : 12] */
		uint32_t bak_apb_pwm1_nsec        :  1; /**<bit[13 : 13] */
		uint32_t bak_apb_timer1_ap        :  1; /**<bit[14 : 14] */
		uint32_t bak_apb_sdmadc_ap        :  1; /**<bit[15 : 15] */
		uint32_t bak_apb_uart1_ap         :  1; /**<bit[16 : 16] */
		uint32_t bak_apb_uart2_ap         :  1; /**<bit[17 : 17] */
		uint32_t bak_apb_i2c0_ap          :  1; /**<bit[18 : 18] */
		uint32_t bak_apb_i2c1_ap          :  1; /**<bit[19 : 19] */
		uint32_t bak_apb_spi1_ap          :  1; /**<bit[20 : 20] */
		uint32_t bak_apb_sadc_ap          :  1; /**<bit[21 : 21] */
		uint32_t bak_apb_pwm0_ap          :  1; /**<bit[22 : 22] */
		uint32_t bak_apb_irda_ap          :  1; /**<bit[23 : 23] */
		uint32_t bak_apb_trng_ap          :  1; /**<bit[24 : 24] */
		uint32_t bak_apb_sdio_ap          :  1; /**<bit[25 : 25] */
		uint32_t bak_apb_scld_ap          :  1; /**<bit[26 : 26] */
		uint32_t bak_apb_pwm1_ap          :  1; /**<bit[27 : 27] */
		uint32_t bak_apb_sresp            :  1; /**<bit[28 : 28] */
		uint32_t reserved_bit_29_31       :  3; /**<bit[29 : 31] */
	};
	uint32_t v;
} prro_bakp_apb_sap_t;


typedef volatile union {
	struct {
		uint32_t ahb_ahb_usb_nsec         :  1; /**<bit[0 : 0] */
		uint32_t ahb_ahb_can_nsec         :  1; /**<bit[1 : 1] */
		uint32_t ahb_ahb_qspi0_nsec       :  1; /**<bit[2 : 2] */
		uint32_t ahb_ahb_qspi1_nsec       :  1; /**<bit[3 : 3] */
		uint32_t ahb_ahb_psram_nsec       :  1; /**<bit[4 : 4] */
		uint32_t ahb_ahb_enet_nsec        :  1; /**<bit[5 : 5] */
		uint32_t ahb_ahb_apb_nsec         :  1; /**<bit[6 : 6] */
		uint32_t ahb_ahb_usb_ap           :  1; /**<bit[7 : 7] */
		uint32_t ahb_ahb_can_ap           :  1; /**<bit[8 : 8] */
		uint32_t ahb_ahb_qspi0_ap         :  1; /**<bit[9 : 9] */
		uint32_t ahb_ahb_qspi1_ap         :  1; /**<bit[10 : 10] */
		uint32_t ahb_ahb_psram_ap         :  1; /**<bit[11 : 11] */
		uint32_t ahb_ahb_enet_ap          :  1; /**<bit[12 : 12] */
		uint32_t ahb_ahb_apb_ap           :  1; /**<bit[13 : 13] */
		uint32_t ahb_ahb_sresp            :  1; /**<bit[14 : 14] */
		uint32_t ahb_apb_scr_nsec         :  1; /**<bit[15 : 15] */
		uint32_t ahb_apb_lin_nsec         :  1; /**<bit[16 : 16] */
		uint32_t ahb_apb_scr_ap           :  1; /**<bit[17 : 17] */
		uint32_t ahb_apb_lin_ap           :  1; /**<bit[18 : 18] */
		uint32_t ahb_apb_sresp            :  1; /**<bit[19 : 19] */
		uint32_t reserved_bit_20_31       : 12; /**<bit[20 : 31] */
	};
	uint32_t v;
} prro_ahbp_sap_t;


typedef volatile union {
	struct {
		uint32_t aud_ahb_sbc_nsec         :  1; /**<bit[0 : 0] */
		uint32_t aud_ahb_apb_nsec         :  1; /**<bit[1 : 1] */
		uint32_t aud_ahb_sbc_ap           :  1; /**<bit[2 : 2] */
		uint32_t aud_ahb_apb_ap           :  1; /**<bit[3 : 3] */
		uint32_t aud_ahb_sresp            :  1; /**<bit[4 : 4] */
		uint32_t aud_apb_aud_nses         :  1; /**<bit[5 : 5] */
		uint32_t aud_apb_i2s0_nses        :  1; /**<bit[6 : 6] */
		uint32_t aud_apb_i2s1_nses        :  1; /**<bit[7 : 7] */
		uint32_t aud_apb_i2s2_nses        :  1; /**<bit[8 : 8] */
		uint32_t aud_apb_aud_ap           :  1; /**<bit[9 : 9] */
		uint32_t aud_apb_i2s0_ap          :  1; /**<bit[10 : 10] */
		uint32_t aud_apb_i2s1_ap          :  1; /**<bit[11 : 11] */
		uint32_t aud_apb_i2s2_ap          :  1; /**<bit[12 : 12] */
		uint32_t aud_apb_sresp            :  1; /**<bit[13 : 13] */
		uint32_t reserved_bit_14_31       : 18; /**<bit[14 : 31] */
	};
	uint32_t v;
} prro_audp_sap_t;


typedef volatile union {
	struct {
		uint32_t vid_ahb_jpge_nsec        :  1; /**<bit[0 : 0] */
		uint32_t vid_ahb_yuv_nsec         :  1; /**<bit[1 : 1] */
		uint32_t vid_ahb_jpgd_nsec        :  1; /**<bit[2 : 2] */
		uint32_t vid_ahb_disp_nsec        :  1; /**<bit[3 : 3] */
		uint32_t vid_ahb_dmad_nsec        :  1; /**<bit[4 : 4] */
		uint32_t vid_ahb_rott_nsec        :  1; /**<bit[5 : 5] */
		uint32_t vid_ahb_scal0_nsec       :  1; /**<bit[6 : 6] */
		uint32_t vid_ahb_scal1_nsec       :  1; /**<bit[7 : 7] */
		uint32_t vid_ahb_apb_nsec         :  1; /**<bit[8 : 8] */
		uint32_t vid_ahb_jpge_ap          :  1; /**<bit[9 : 9] */
		uint32_t vid_ahb_yuv_ap           :  1; /**<bit[10 : 10] */
		uint32_t vid_ahb_jpgd_ap          :  1; /**<bit[11 : 11] */
		uint32_t vid_ahb_disp_ap          :  1; /**<bit[12 : 12] */
		uint32_t vid_ahb_dmad_ap          :  1; /**<bit[13 : 13] */
		uint32_t vid_ahb_rott_ap          :  1; /**<bit[14 : 14] */
		uint32_t vid_ahb_scal0_ap         :  1; /**<bit[15 : 15] */
		uint32_t vid_ahb_scal1_ap         :  1; /**<bit[16 : 16] */
		uint32_t vid_ahb_apb_ap           :  1; /**<bit[17 : 17] */
		uint32_t vid_ahb_sresp            :  1; /**<bit[18 : 18] */
		uint32_t vid_apb_h264_nsec        :  1; /**<bit[19 : 19] */
		uint32_t vid_apb_h264_ap          :  1; /**<bit[20 : 20] */
		uint32_t vid_apb_h264_sresp       :  1; /**<bit[21 : 21] */
		uint32_t reserved_bit_22_31       : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} prro_vidp_sap_t;


typedef volatile union {
	struct {
		uint32_t wifi_ahb_mac_nsec        :  1; /**<bit[0 : 0] */
		uint32_t wifi_ahb_mod_nsec        :  1; /**<bit[1 : 1] */
		uint32_t wifi_ahb_mac_ap          :  1; /**<bit[2 : 2] */
		uint32_t wifi_ahb_mod_ap          :  1; /**<bit[3 : 3] */
		uint32_t wifi_ahb_sresp           :  1; /**<bit[4 : 4] */
		uint32_t reserved_bit_5_31        : 27; /**<bit[5 : 31] */
	};
	uint32_t v;
} prro_wifp_sap_t;


typedef volatile union {
	struct {
		uint32_t btdm_ahb_btdm_nsec       :  1; /**<bit[0 : 0] */
		uint32_t btdm_ahb_apb_nsec        :  1; /**<bit[1 : 1] */
		uint32_t btdm_ahb_btdm_ap         :  1; /**<bit[2 : 2] */
		uint32_t btdm_ahb_apb_ap          :  1; /**<bit[3 : 3] */
		uint32_t btdm_ahb_sresp           :  1; /**<bit[4 : 4] */
		uint32_t btdm_apb_xvr_nsec        :  1; /**<bit[5 : 5] */
		uint32_t btdm_apb_xvr_ap          :  1; /**<bit[6 : 6] */
		uint32_t btdm_apb_sresp           :  1; /**<bit[7 : 7] */
		uint32_t reserved_bit_8_31        : 24; /**<bit[8 : 31] */
	};
	uint32_t v;
} prro_btsp_sap_t;


typedef volatile union {
	struct {
		uint32_t mbox_ahb_nsec            :  1; /**<bit[0 : 0] */
		uint32_t mbox_ahb_ap              :  1; /**<bit[1 : 1] */
		uint32_t ppro_mpc_ap              :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_31        : 29; /**<bit[3 : 31] */
	};
	uint32_t v;
} prro_mbox_sap_t;


typedef volatile union {
	struct {
		uint32_t jpenc_m_hnsec            :  1; /**<bit[0 : 0] */
		uint32_t jpdec_m_hnsec            :  1; /**<bit[1 : 1] */
		uint32_t dmad_m_hnsec             :  1; /**<bit[2 : 2] */
		uint32_t mac_m_hnsec              :  1; /**<bit[3 : 3] */
		uint32_t hsu_m_hnsec              :  1; /**<bit[4 : 4] */
		uint32_t btdm_m_hnsec             :  1; /**<bit[5 : 5] */
		uint32_t la_m_hnsec               :  1; /**<bit[6 : 6] */
		uint32_t disp_m_hnsec             :  1; /**<bit[7 : 7] */
		uint32_t rott_m_hnsec             :  1; /**<bit[8 : 8] */
		uint32_t usb_m_hnsec              :  1; /**<bit[9 : 9] */
		uint32_t enet_m_hnsec             :  1; /**<bit[10 : 10] */
		uint32_t scal0_m_hnsec            :  1; /**<bit[11 : 11] */
		uint32_t scal1_m_hnsec            :  1; /**<bit[12 : 12] */
		uint32_t reserved_bit_13_31       : 19; /**<bit[13 : 31] */
	};
	uint32_t v;
} prro_master_hnsec_t;


typedef volatile union {
	struct {
		uint32_t cmp0_addr_start          : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp0_addr_start_t;


typedef volatile union {
	struct {
		uint32_t cmp0_addr_end            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp0_end_start_t;


typedef volatile union {
	struct {
		uint32_t cmp0_data_src            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp0_data_rega_t;


typedef volatile union {
	struct {
		uint32_t cmp0_data_dst            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp0_data_regb_t;


typedef volatile union {
	struct {
		uint32_t cmp1_addr_start          : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp1_addr_start_t;


typedef volatile union {
	struct {
		uint32_t cmp1_addr_end            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp1_end_start_t;


typedef volatile union {
	struct {
		uint32_t cmp1_data_src            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp1_data_rega_t;


typedef volatile union {
	struct {
		uint32_t cmp1_data_dst            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp1_data_regb_t;


typedef volatile union {
	struct {
		uint32_t cmp2_addr_start          : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp2_addr_start_t;


typedef volatile union {
	struct {
		uint32_t cmp2_addr_end            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp2_end_start_t;


typedef volatile union {
	struct {
		uint32_t cmp2_data_src            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp2_data_rega_t;


typedef volatile union {
	struct {
		uint32_t cmp2_data_dst            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} prro_cmp2_data_regb_t;


typedef volatile union {
	struct {
		uint32_t cmp0_mismatch_int        :  1; /**<bit[0 : 0] */
		uint32_t cmp1_mismatch_int        :  1; /**<bit[1 : 1] */
		uint32_t cmp2_mismatch_int        :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_31        : 29; /**<bit[3 : 31] */
	};
	uint32_t v;
} prro_cmp_int_status_t;

typedef volatile struct {
	volatile prro_device_id_t device_id;
	volatile prro_version_id_t version_id;
	volatile prro_gbl_control_t gbl_control;
	volatile prro_gbl_status_t gbl_status;
	volatile prro_aonp_gpio_nsec0_t aonp_gpio_nsec0;
	volatile prro_aonp_gpio_nsec1_t aonp_gpio_nsec1;
	volatile prro_aonp_sap_t aonp_sap;
	volatile prro_bakp_ahb_sap_t bakp_ahb_sap;
	volatile prro_bakp_apb_sap_t bakp_apb_sap;
	volatile prro_ahbp_sap_t ahbp_sap;
	volatile prro_audp_sap_t audp_sap;
	volatile prro_vidp_sap_t vidp_sap;
	volatile prro_wifp_sap_t wifp_sap;
	volatile prro_btsp_sap_t btsp_sap;
	volatile prro_mbox_sap_t mbox_sap;
	volatile prro_master_hnsec_t master_hnsec;
	volatile uint32_t rsv_10_16[7];
	volatile prro_cmp0_addr_start_t cmp0_addr_start;
	volatile prro_cmp0_end_start_t cmp0_end_start;
	volatile prro_cmp0_data_rega_t cmp0_data_rega;
	volatile prro_cmp0_data_regb_t cmp0_data_regb;
	volatile prro_cmp1_addr_start_t cmp1_addr_start;
	volatile prro_cmp1_end_start_t cmp1_end_start;
	volatile prro_cmp1_data_rega_t cmp1_data_rega;
	volatile prro_cmp1_data_regb_t cmp1_data_regb;
	volatile prro_cmp2_addr_start_t cmp2_addr_start;
	volatile prro_cmp2_end_start_t cmp2_end_start;
	volatile prro_cmp2_data_rega_t cmp2_data_rega;
	volatile prro_cmp2_data_regb_t cmp2_data_regb;
	volatile prro_cmp_int_status_t cmp_int_status;
} prro_hw_t;

#ifdef __cplusplus
}
#endif
