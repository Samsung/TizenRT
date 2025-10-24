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

#include "hal_config.h"
#include "prro_hw.h"
#include "prro_hal.h"
#include "prro_ll.h"

#if CONFIG_PRRO_TEST

void prro_struct_dump(void)
{
	prro_hw_t *hw = (prro_hw_t *)PRRO_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);
	SOC_LOGI("dev_id[0]=%x\r\n", hw->dev_id);
	SOC_LOGI("dev_version[1]=%x\r\n", hw->dev_version);

	SOC_LOGI("global_ctrl[2]=%x\r\n", hw->global_ctrl.v);
	SOC_LOGI("\tsoft_reset=%x\r\n", hw->global_ctrl.soft_reset);
	SOC_LOGI("\tclk_gate_bypass=%x\r\n", hw->global_ctrl.clk_gate_bypass);
	SOC_LOGI("\treserved=%x\r\n", hw->global_ctrl.reserved);

	SOC_LOGI("dev_status[3]=%x\r\n", hw->dev_status);

	SOC_LOGI("ahb_cfg_ap[4]=%x\r\n", hw->ahb_cfg_ap.v);
	SOC_LOGI("\taonp_ahb_aon_ap=%x\r\n", hw->ahb_cfg_ap.aonp_ahb_aon_ap);
	SOC_LOGI("\taonp_ahb_sys_ap=%x\r\n", hw->ahb_cfg_ap.aonp_ahb_sys_ap);
	SOC_LOGI("\taonp_ahb_flash_ap=%x\r\n", hw->ahb_cfg_ap.aonp_ahb_flash_ap);
	SOC_LOGI("\tbakp_ahb_gdma_ap=%x\r\n", hw->ahb_cfg_ap.bakp_ahb_gdma_ap);
	SOC_LOGI("\tbakp_ahb_la_ap=%x\r\n", hw->ahb_cfg_ap.bakp_ahb_la_ap);
	SOC_LOGI("\tahbp_ahb_usb_ap=%x\r\n", hw->ahb_cfg_ap.ahbp_ahb_usb_ap);
	SOC_LOGI("\tahbp_ahb_can_ap=%x\r\n", hw->ahb_cfg_ap.ahbp_ahb_can_ap);
	SOC_LOGI("\tahbp_ahb_qspi0_ap=%x\r\n", hw->ahb_cfg_ap.ahbp_ahb_qspi0_ap);
	SOC_LOGI("\tahbp_ahb_qspi1_ap=%x\r\n", hw->ahb_cfg_ap.ahbp_ahb_qspi1_ap);
	SOC_LOGI("\tahbp_ahb_psram_ap=%x\r\n", hw->ahb_cfg_ap.ahbp_ahb_psram_ap);
	SOC_LOGI("\tvidp_ahb_yuv_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_yuv_ap);
	SOC_LOGI("\taudp_ahb_fft_ap=%x\r\n", hw->ahb_cfg_ap.audp_ahb_fft_ap);
	SOC_LOGI("\taudp_ahb_sbc_ap=%x\r\n", hw->ahb_cfg_ap.audp_ahb_sbc_ap);
	SOC_LOGI("\tvidp_ahb_jpge_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_jpge_ap);
	SOC_LOGI("\tvidp_ahb_yuv_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_yuv_ap);
	SOC_LOGI("\tvidp_ahb_jpge_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_jpge_ap);
	SOC_LOGI("\tvidp_ahb_yuv_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_yuv_ap);
	SOC_LOGI("\tvidp_ahb_jpgd_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_jpgd_ap);
	SOC_LOGI("\tvidp_ahb_disp_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_disp_ap);
	SOC_LOGI("\tvidp_ahb_dmad_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_dmad_ap);
	SOC_LOGI("\tvidp_ahb_rott_ap=%x\r\n", hw->ahb_cfg_ap.vidp_ahb_rott_ap);
	SOC_LOGI("\twifi_mac_ahb_ap=%x\r\n", hw->ahb_cfg_ap.wifi_mac_ahb_ap);
	SOC_LOGI("\twifi_modem_ahb_ap=%x\r\n", hw->ahb_cfg_ap.wifi_modem_ahb_ap);
	SOC_LOGI("\tbtdm_ahb_ap=%x\r\n", hw->ahb_cfg_ap.btdm_ahb_ap);
	SOC_LOGI("\tmbox0_ahb_ap=%x\r\n", hw->ahb_cfg_ap.mbox0_ahb_ap);
	SOC_LOGI("\tmbox1_ahb_ap=%x\r\n", hw->ahb_cfg_ap.mbox1_ahb_ap);
	SOC_LOGI("\tdma1_ahb_ap=%x\r\n", hw->ahb_cfg_ap.dma1_ahb_ap);
	SOC_LOGI("\treserved=%x\r\n", hw->ahb_cfg_ap.reserved);

	SOC_LOGI("ahb_cfg_nsec[5]=%x\r\n", hw->ahb_cfg_nsec.v);
	SOC_LOGI("\taonp_ahb_aon_nsec=%x\r\n", hw->ahb_cfg_nsec.aonp_ahb_aon_nsec);
	SOC_LOGI("\taonp_ahb_sys_nsec=%x\r\n", hw->ahb_cfg_nsec.aonp_ahb_sys_nsec);
	SOC_LOGI("\taonp_ahb_flash_nsec=%x\r\n", hw->ahb_cfg_nsec.aonp_ahb_flash_nsec);
	SOC_LOGI("\tbakp_ahb_gdma_nsec=%x\r\n", hw->ahb_cfg_nsec.bakp_ahb_gdma_nsec);
	SOC_LOGI("\tbakp_ahb_la_nsec=%x\r\n", hw->ahb_cfg_nsec.bakp_ahb_la_nsec);
	SOC_LOGI("\tahbp_ahb_usb_nsec=%x\r\n", hw->ahb_cfg_nsec.ahbp_ahb_usb_nsec);
	SOC_LOGI("\tahbp_ahb_can_nsec=%x\r\n", hw->ahb_cfg_nsec.ahbp_ahb_can_nsec);
	SOC_LOGI("\tahbp_ahb_qspi0_nsec=%x\r\n", hw->ahb_cfg_nsec.ahbp_ahb_qspi0_nsec);
	SOC_LOGI("\tahbp_ahb_qspi1_nsec=%x\r\n", hw->ahb_cfg_nsec.ahbp_ahb_qspi1_nsec);
	SOC_LOGI("\tahbp_ahb_psram_nsec=%x\r\n", hw->ahb_cfg_nsec.ahbp_ahb_psram_nsec);
	SOC_LOGI("\tvidp_ahb_yuv_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_yuv_nsec);
	SOC_LOGI("\taudp_ahb_fft_nsec=%x\r\n", hw->ahb_cfg_nsec.audp_ahb_fft_nsec);
	SOC_LOGI("\taudp_ahb_sbc_nsec=%x\r\n", hw->ahb_cfg_nsec.audp_ahb_sbc_nsec);
	SOC_LOGI("\tvidp_ahb_jpge_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_jpge_nsec);
	SOC_LOGI("\tvidp_ahb_yuv_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_yuv_nsec);
	SOC_LOGI("\tvidp_ahb_jpge_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_jpge_nsec);
	SOC_LOGI("\tvidp_ahb_yuv_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_yuv_nsec);
	SOC_LOGI("\tvidp_ahb_jpgd_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_jpgd_nsec);
	SOC_LOGI("\tvidp_ahb_disp_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_disp_nsec);
	SOC_LOGI("\tvidp_ahb_dmad_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_dmad_nsec);
	SOC_LOGI("\tvidp_ahb_rott_nsec=%x\r\n", hw->ahb_cfg_nsec.vidp_ahb_rott_nsec);
	SOC_LOGI("\twifi_mac_ahb_nsec=%x\r\n", hw->ahb_cfg_nsec.wifi_mac_ahb_nsec);
	SOC_LOGI("\twifi_modem_ahb_nsec=%x\r\n", hw->ahb_cfg_nsec.wifi_modem_ahb_nsec);
	SOC_LOGI("\tbtdm_ahb_nsec=%x\r\n", hw->ahb_cfg_nsec.btdm_ahb_nsec);
	SOC_LOGI("\tmbox0_ahb_nsec=%x\r\n", hw->ahb_cfg_nsec.mbox0_ahb_nsec);
	SOC_LOGI("\tmbox1_ahb_nsec=%x\r\n", hw->ahb_cfg_nsec.mbox1_ahb_nsec);
	SOC_LOGI("\tdma1_ahb_nsec=%x\r\n", hw->ahb_cfg_nsec.dma1_ahb_nsec);
	SOC_LOGI("\treserved=%x\r\n", hw->ahb_cfg_nsec.reserved);

	SOC_LOGI("ahb_cfg_sresp[6]=%x\r\n", hw->ahb_cfg_sresp.v);
	SOC_LOGI("\taonp_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.aonp_ahb_sresp);
	SOC_LOGI("\tbakp_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.ahbp_ahb_sresp);
	SOC_LOGI("\taudp_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.audp_ahb_sresp);
	SOC_LOGI("\tvidp_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.vidp_ahb_sresp);
	SOC_LOGI("\twifi_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.wifi_ahb_sresp);
	SOC_LOGI("\tbtdm_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.btdm_ahb_sresp);
	SOC_LOGI("\tmbox_ahb_sresp=%x\r\n", hw->ahb_cfg_sresp.mbox_ahb_sresp);
	SOC_LOGI("\treserved=%x\r\n", hw->ahb_cfg_sresp.reserved);

	SOC_LOGI("apb_cfg_ap[7]=%x\r\n", hw->apb_cfg_ap.v);
	SOC_LOGI("\taonp_apb_timr_ap=%x\r\n", hw->apb_cfg_ap.aonp_apb_timr_ap);
	SOC_LOGI("\taonp_apb_uart_ap=%x\r\n", hw->apb_cfg_ap.aonp_apb_uart_ap);
	SOC_LOGI("\taonp_apb_spi0_ap=%x\r\n", hw->apb_cfg_ap.aonp_apb_spi0_ap);
	SOC_LOGI("\taonp_apb_efs_ap=%x\r\n", hw->apb_cfg_ap.aonp_apb_efs_ap);
	SOC_LOGI("\taonp_apb_mbic_ap=%x\r\n", hw->apb_cfg_ap.aonp_apb_mbic_ap);
	SOC_LOGI("\tbakp_apb_timer1_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_timer1_ap);
	SOC_LOGI("\tbakp_apb_sdmadc_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_sdmadc_ap);
	SOC_LOGI("\tbakp_apb_uart1_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_uart1_ap);
	SOC_LOGI("\tbakp_apb_uart2_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_uart2_ap);
	SOC_LOGI("\tbakp_apb_i2c0_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_i2c0_ap);
	SOC_LOGI("\tbakp_apb_i2c1_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_i2c1_ap);
	SOC_LOGI("\tbakp_apb_spi1_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_spi1_ap);
	SOC_LOGI("\tbakp_apb_sadc_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_sadc_ap);
	SOC_LOGI("\tbakp_apb_pwm_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_pwm_ap);
	SOC_LOGI("\tbakp_apb_irda_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_irda_ap);
	SOC_LOGI("\tbakp_apb_trng_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_trng_ap);
	SOC_LOGI("\tbakp_apb_sdio_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_sdio_ap);
	SOC_LOGI("\tbakp_apb_slcd_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_slcd_ap);
	SOC_LOGI("\tbakp_apb_pwm1_ap=%x\r\n", hw->apb_cfg_ap.bakp_apb_pwm1_ap);
	SOC_LOGI("\taudp_apb_aud_ap=%x\r\n", hw->apb_cfg_ap.audp_apb_aud_ap);
	SOC_LOGI("\taudp_apb_i2s0_ap=%x\r\n", hw->apb_cfg_ap.audp_apb_i2s0_ap);
	SOC_LOGI("\taudp_apb_i2s1_ap=%x\r\n", hw->apb_cfg_ap.audp_apb_i2s1_ap);
	SOC_LOGI("\taudp_apb_i2s2_ap=%x\r\n", hw->apb_cfg_ap.audp_apb_i2s2_ap);
	SOC_LOGI("\tvidp_apb_ap=%x\r\n", hw->apb_cfg_ap.vidp_apb_ap);
	SOC_LOGI("\tbtdm_apb_ap=%x\r\n", hw->apb_cfg_ap.btdm_apb_ap);
	SOC_LOGI("\treserved=%x\r\n", hw->apb_cfg_ap.reserved);

	SOC_LOGI("apb_cfg_nsec[8]=%x\r\n", hw->apb_cfg_nsec.v);
	SOC_LOGI("\taonp_apb_timr_nsec=%x\r\n", hw->apb_cfg_nsec.aonp_apb_timr_nsec);
	SOC_LOGI("\taonp_apb_uart_nsec=%x\r\n", hw->apb_cfg_nsec.aonp_apb_uart_nsec);
	SOC_LOGI("\taonp_apb_spi0_nsec=%x\r\n", hw->apb_cfg_nsec.aonp_apb_spi0_nsec);
	SOC_LOGI("\taonp_apb_efs_nsec=%x\r\n", hw->apb_cfg_nsec.aonp_apb_efs_nsec);
	SOC_LOGI("\taonp_apb_mbic_nsec=%x\r\n", hw->apb_cfg_nsec.aonp_apb_mbic_nsec);
	SOC_LOGI("\tbakp_apb_timer1_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_timer1_nsec);
	SOC_LOGI("\tbakp_apb_sdmadc_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_sdmadc_nsec);
	SOC_LOGI("\tbakp_apb_uart1_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_uart1_nsec);
	SOC_LOGI("\tbakp_apb_uart2_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_uart2_nsec);
	SOC_LOGI("\tbakp_apb_i2c0_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_i2c0_nsec);
	SOC_LOGI("\tbakp_apb_i2c1_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_i2c1_nsec);
	SOC_LOGI("\tbakp_apb_spi1_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_spi1_nsec);
	SOC_LOGI("\tbakp_apb_sadc_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_sadc_nsec);
	SOC_LOGI("\tbakp_apb_pwm_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_pwm_nsec);
	SOC_LOGI("\tbakp_apb_irda_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_irda_nsec);
	SOC_LOGI("\tbakp_apb_trng_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_trng_nsec);
	SOC_LOGI("\tbakp_apb_sdio_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_sdio_nsec);
	SOC_LOGI("\tbakp_apb_slcd_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_slcd_nsec);
	SOC_LOGI("\tbakp_apb_pwm1_nsec=%x\r\n", hw->apb_cfg_nsec.bakp_apb_pwm1_nsec);
	SOC_LOGI("\taudp_apb_aud_nsec=%x\r\n", hw->apb_cfg_nsec.audp_apb_aud_nsec);
	SOC_LOGI("\taudp_apb_i2s0_nsec=%x\r\n", hw->apb_cfg_nsec.audp_apb_i2s0_nsec);
	SOC_LOGI("\taudp_apb_i2s1_nsec=%x\r\n", hw->apb_cfg_nsec.audp_apb_i2s1_nsec);
	SOC_LOGI("\taudp_apb_i2s2_nsec=%x\r\n", hw->apb_cfg_nsec.audp_apb_i2s2_nsec);
	SOC_LOGI("\tvidp_apb_nsec=%x\r\n", hw->apb_cfg_nsec.vidp_apb_nsec);
	SOC_LOGI("\tbtdm_apb_nsec=%x\r\n", hw->apb_cfg_nsec.btdm_apb_nsec);
	SOC_LOGI("\treserved=%x\r\n", hw->apb_cfg_nsec.reserved);

	SOC_LOGI("apb_cfg_sresp[9]=%x\r\n", hw->apb_cfg_sresp.v);
	SOC_LOGI("\taonp_apb_sresp=%x\r\n", hw->apb_cfg_sresp.aonp_apb_sresp);
	SOC_LOGI("\tbakp_apb_sresp=%x\r\n", hw->apb_cfg_sresp.bakp_apb_sresp);
	SOC_LOGI("\taudp_apb_sresp=%x\r\n", hw->apb_cfg_sresp.audp_apb_sresp);
	SOC_LOGI("\tvidp_apb_sresp=%x\r\n", hw->apb_cfg_sresp.vidp_apb_sresp);
	SOC_LOGI("\tbtdm_apb_sresp=%x\r\n", hw->apb_cfg_sresp.btdm_apb_sresp);
	SOC_LOGI("\treserved=%x\r\n", hw->apb_cfg_sresp.reserved);

	SOC_LOGI("aonp_apb_nonsec[a]=%x\r\n", hw->aonp_apb_nonsec.v);
	SOC_LOGI("\taonp_apb_nonsec=%x\r\n", hw->aonp_apb_nonsec.aonp_apb_nonsec);
	SOC_LOGI("\treserved=%x\r\n", hw->aonp_apb_nonsec.reserved);

	SOC_LOGI("aon_gpio_nonsec0[b]=%x\r\n", hw->aon_gpio_nonsec0);
	SOC_LOGI("aon_gpio_nonsec1[c]=%x\r\n", hw->aon_gpio_nonsec1);
	SOC_LOGI("jpenc_hnonsec[d]=%x\r\n", hw->jpenc_hnonsec.v);
	SOC_LOGI("jpdec_hnonsec[e]=%x\r\n", hw->jpdec_hnonsec.v);
	SOC_LOGI("dmad_m_hnonsec[f]=%x\r\n", hw->dmad_m_hnonsec.v);
	SOC_LOGI("mac_m_hnonsec[10]=%x\r\n", hw->mac_m_hnonsec.v);
	SOC_LOGI("hsu_m_hnonsec[11]=%x\r\n", hw->hsu_m_hnonsec.v);
	SOC_LOGI("btdm_m_hnonsec[12]=%x\r\n", hw->btdm_m_hnonsec.v);
	SOC_LOGI("la_m_hnonsec[13]=%x\r\n", hw->la_m_hnonsec.v);
	SOC_LOGI("disp_m_hnonsec[14]=%x\r\n", hw->disp_m_hnonsec.v);
	SOC_LOGI("enc_m_hnonsec[15]=%x\r\n", hw->enc_m_hnonsec.v);
	SOC_LOGI("reg22_reserved[15]=%x\r\n", hw->reg22_reserved);

	for (int i = 0; i < 3; i++) {
		SOC_LOGI("cmp_addr%d\r\n", i);
		SOC_LOGI("\tstart[%x]=%x%d\r\n", 0x17+i, hw->cmp_addr[i].start);
		SOC_LOGI("\tend[%x]=%x%d\r\n", 0x17+i, hw->cmp_addr[i].end);
		SOC_LOGI("\tsrc[%x]=%x%d\r\n", 0x18+i, hw->cmp_addr[i].src);
		SOC_LOGI("\tdst[%x]=%x%d\r\n", 0x19+i, hw->cmp_addr[i].dst);
	}

	SOC_LOGI("cmp_int_status[23]=%x\r\n", hw->cmp_int_status.v);
	SOC_LOGI("\tcmp0_mismatch_int=%x\r\n", hw->cmp_int_status.cmp0_mismatch_int);
	SOC_LOGI("\tcmp1_mismatch_int=%x\r\n", hw->cmp_int_status.cmp1_mismatch_int);
	SOC_LOGI("\tcmp2_mismatch_int=%x\r\n", hw->cmp_int_status.cmp2_mismatch_int);
}

#endif

