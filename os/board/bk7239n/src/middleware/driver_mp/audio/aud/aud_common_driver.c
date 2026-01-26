// Copyright 2023-2025 Beken
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



#include <common/bk_include.h>
#include "aud_hal.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <modules/pm.h>
#include "sys_hal.h"

#include <driver/aud_common.h>

#include <driver/aud_adc.h>
#include <driver/aud_adc_types.h>
#include <driver/aud_dtmf.h>
#include <driver/aud_dtmf_types.h>
#include <driver/aud_dmic.h>
#include <driver/aud_dmic_types.h>
#include <driver/aud_dac.h>
#include <driver/aud_dac_types.h>


#if defined(CONFIG_SOC_BK7236XX)
#define SYS_ANA_REG18_ISELAUD_DEFAULT_VAL                      (0x01)
#define SYS_ANA_REG18_AUDCK_RLCEN1V_DEFAULT_VAL                (0x00)
#define SYS_ANA_REG18_LCHCKINVEN1V_DEFAULT_VAL                 (0x01)
#define SYS_ANA_REG18_ENAUDBIAS_DEFAULT_VAL                    (0x00)
#define SYS_ANA_REG18_ENADCBIAS_DEFAULT_VAL                    (0x00)
#define SYS_ANA_REG18_ENMICBIAS_DEFAULT_VAL                    (0x00)
#define SYS_ANA_REG18_ADCCKINVEN1V_DEFAULT_VAL                 (0x00)
#define SYS_ANA_REG18_DACFB2ST0V9_DEFAULT_VAL                  (0x01)
#define SYS_ANA_REG18_NC1_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG18_MICBIAS_TRM_DEFAULT_VAL                  (0x00)
#define SYS_ANA_REG18_MICBIAS_VOC_DEFAULT_VAL                  (0x10)
#define SYS_ANA_REG18_VREFSEL1V_DEFAULT_VAL                    (0x01)
#define SYS_ANA_REG18_CAPSWSPI_DEFAULT_VAL                     (0x1f)
#define SYS_ANA_REG18_ADREF_SEL_DEFAULT_VAL                    (0x02)
#define SYS_ANA_REG18_NC0_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG18_RESERVED_BIT_26_30_DEFAULT_VAL           (0x00)
#define SYS_ANA_REG18_SPI_DACCKPSSEL_DEFAULT_VAL               (0x00)

#define SYS_ANA_REG19_ISEL_DEFAULT_VAL                         (0x02)
#define SYS_ANA_REG19_MICIRSEL1_DEFAULT_VAL                    (0x01)
#define SYS_ANA_REG19_MICDACIT_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICDACIH_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICSINGLEEN_DEFAULT_VAL                  (0x00)
#define SYS_ANA_REG19_DCCOMPEN_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICGAIN_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG19_MICDACEN_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_STG2LSEN1V_DEFAULT_VAL                   (0x00)
#define SYS_ANA_REG19_OPENLOOPCAL1V_DEFAULT_VAL                (0x00)
#define SYS_ANA_REG19_CALLATCH_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_VCMSEL_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG19_DWAMODE_DEFAULT_VAL                      (0x01)
#define SYS_ANA_REG19_R2REN_DEFAULT_VAL                        (0x00)
#define SYS_ANA_REG19_NC_26_27_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICEN_DEFAULT_VAL                        (0x00)
#define SYS_ANA_REG19_RST_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG19_BPDWA1V_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG19_HCEN1STG_DEFAULT_VAL                     (0x01)

#define SYS_ANA_REG20_HPDAC_DEFAULT_VAL                        (0x01)
#define SYS_ANA_REG20_CALCON_SEL_DEFAULT_VAL                   (0x01)
#define SYS_ANA_REG20_OSCDAC_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG20_OCENDAC_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_VCMSEL_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG20_ADJDACREF_DEFAULT_VAL                    (0x10)
#define SYS_ANA_REG20_DCOCHG_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG20_DIFFEN_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG20_ENDACCAL_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG20_NC2_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG20_LENDCOC_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_NC1_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG20_LENVCMD_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_DACDRVEN_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG20_NC0_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG20_DACLEN_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG20_DACG_DEFAULT_VAL                         (0x0f)
#define SYS_ANA_REG20_DACMUTE_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_DACDWAMODE_SEL_DEFAULT_VAL               (0x01)
#define SYS_ANA_REG20_DACSEL_DEFAULT_VAL                       (0x0f)

#define SYS_ANA_REG21_LMDCIN_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG21_NC1_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG21_SPIRST_OVC_DEFAULT_VAL                   (0x00)
#define SYS_ANA_REG21_NC0_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG21_ENIDACL_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG21_DAC3RDHC0V9_DEFAULT_VAL                  (0x00)
#define SYS_ANA_REG21_HC2S_DEFAULT_VAL                         (0x01)
#define SYS_ANA_REG21_RFB_CTRL_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG21_VCMSEL_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG21_ENBS_DEFAULT_VAL                         (0x00)
#define SYS_ANA_REG21_CALCK_SEL0V9_DEFAULT_VAL                 (0x00)
#define SYS_ANA_REG21_BPDWA0V9_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG21_LOOPRST0V9_DEFAULT_VAL                   (0x00)
#define SYS_ANA_REG21_OCT0V9_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG21_SOUT0V9_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG21_HC0V9_DEFAULT_VAL                        (0x00)
#endif //#if CONFIG_SOC_BK7236XX


#define AUD_RETURN_ON_NOT_INIT() do {\
		if (!s_aud_driver_is_init) {\
			return BK_ERR_AUD_NOT_INIT;\
		}\
	} while(0)

static bool s_aud_driver_is_init = false;
static aud_module_init_sta_t s_aud_module_init_sta = {0};
#if (defined(CONFIG_AUDIO_ADC) || defined(CONFIG_AUDIO_DMIC) || defined(CONFIG_AUDIO_DTMF) || defined(CONFIG_AUDIO_DAC))
static aud_isr_handle_t s_aud_isr = {NULL};
#endif
static void aud_isr(void);
extern void delay(int num);

#if defined(CONFIG_SOC_BK7236XX)
static uint32_t ana_reg18_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG18_ISELAUD_DEFAULT_VAL & SYS_ANA_REG18_ISELAUD_MASK) << SYS_ANA_REG18_ISELAUD_POS);
	value |= ((SYS_ANA_REG18_AUDCK_RLCEN1V_DEFAULT_VAL & SYS_ANA_REG18_AUDCK_RLCEN1V_MASK) << SYS_ANA_REG18_AUDCK_RLCEN1V_POS);
	value |= ((SYS_ANA_REG18_LCHCKINVEN1V_DEFAULT_VAL & SYS_ANA_REG18_LCHCKINVEN1V_MASK) << SYS_ANA_REG18_LCHCKINVEN1V_POS);
	value |= ((SYS_ANA_REG18_ENAUDBIAS_DEFAULT_VAL & SYS_ANA_REG18_ENAUDBIAS_MASK) << SYS_ANA_REG18_ENAUDBIAS_POS);
	value |= ((SYS_ANA_REG18_ENADCBIAS_DEFAULT_VAL & SYS_ANA_REG18_ENADCBIAS_MASK) << SYS_ANA_REG18_ENADCBIAS_POS);
	value |= ((SYS_ANA_REG18_ENMICBIAS_DEFAULT_VAL & SYS_ANA_REG18_ENMICBIAS_MASK) << SYS_ANA_REG18_ENMICBIAS_POS);
	value |= ((SYS_ANA_REG18_ADCCKINVEN1V_DEFAULT_VAL & SYS_ANA_REG18_ADCCKINVEN1V_MASK) << SYS_ANA_REG18_ADCCKINVEN1V_POS);
	value |= ((SYS_ANA_REG18_DACFB2ST0V9_DEFAULT_VAL & SYS_ANA_REG18_DACFB2ST0V9_MASK) << SYS_ANA_REG18_DACFB2ST0V9_POS);
	value |= ((SYS_ANA_REG18_NC1_DEFAULT_VAL & SYS_ANA_REG18_NC1_MASK) << SYS_ANA_REG18_NC1_POS);
	value |= ((SYS_ANA_REG18_MICBIAS_TRM_DEFAULT_VAL & SYS_ANA_REG18_MICBIAS_TRM_MASK) << SYS_ANA_REG18_MICBIAS_TRM_POS);
	value |= ((SYS_ANA_REG18_MICBIAS_VOC_DEFAULT_VAL & SYS_ANA_REG18_MICBIAS_VOC_MASK) << SYS_ANA_REG18_MICBIAS_VOC_POS);
	value |= ((SYS_ANA_REG18_VREFSEL1V_DEFAULT_VAL & SYS_ANA_REG18_VREFSEL1V_MASK) << SYS_ANA_REG18_VREFSEL1V_POS);
	value |= ((SYS_ANA_REG18_CAPSWSPI_DEFAULT_VAL & SYS_ANA_REG18_CAPSWSPI_MASK) << SYS_ANA_REG18_CAPSWSPI_POS);
	value |= ((SYS_ANA_REG18_ADREF_SEL_DEFAULT_VAL & SYS_ANA_REG18_ADREF_SEL_MASK) << SYS_ANA_REG18_ADREF_SEL_POS);
	value |= ((SYS_ANA_REG18_NC0_DEFAULT_VAL & SYS_ANA_REG18_NC0_MASK) << SYS_ANA_REG18_NC0_POS);
	value |= ((SYS_ANA_REG18_RESERVED_BIT_26_30_DEFAULT_VAL & SYS_ANA_REG18_RESERVED_BIT_26_30_MASK) << SYS_ANA_REG18_RESERVED_BIT_26_30_POS);
	value |= ((SYS_ANA_REG18_SPI_DACCKPSSEL_DEFAULT_VAL & SYS_ANA_REG18_SPI_DACCKPSSEL_MASK) << SYS_ANA_REG18_SPI_DACCKPSSEL_POS);

	return value;
}

static uint32_t ana_reg19_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG19_ISEL_DEFAULT_VAL & SYS_ANA_REG19_ISEL_MASK) << SYS_ANA_REG19_ISEL_POS);
	value |= ((SYS_ANA_REG19_MICIRSEL1_DEFAULT_VAL & SYS_ANA_REG19_MICIRSEL1_MASK) << SYS_ANA_REG19_MICIRSEL1_POS);
	value |= ((SYS_ANA_REG19_MICDACIT_DEFAULT_VAL & SYS_ANA_REG19_MICDACIT_MASK) << SYS_ANA_REG19_MICDACIT_POS);
	value |= ((SYS_ANA_REG19_MICDACIH_DEFAULT_VAL & SYS_ANA_REG19_MICDACIH_MASK) << SYS_ANA_REG19_MICDACIH_POS);
	value |= ((SYS_ANA_REG19_MICSINGLEEN_DEFAULT_VAL & SYS_ANA_REG19_MICSINGLEEN_MASK) << SYS_ANA_REG19_MICSINGLEEN_POS);
	value |= ((SYS_ANA_REG19_DCCOMPEN_DEFAULT_VAL & SYS_ANA_REG19_DCCOMPEN_MASK) << SYS_ANA_REG19_DCCOMPEN_POS);
	value |= ((SYS_ANA_REG19_MICGAIN_DEFAULT_VAL & SYS_ANA_REG19_MICGAIN_MASK) << SYS_ANA_REG19_MICGAIN_POS);
	value |= ((SYS_ANA_REG19_MICDACEN_DEFAULT_VAL & SYS_ANA_REG19_MICDACEN_MASK) << SYS_ANA_REG19_MICDACEN_POS);
	value |= ((SYS_ANA_REG19_STG2LSEN1V_DEFAULT_VAL & SYS_ANA_REG19_STG2LSEN1V_MASK) << SYS_ANA_REG19_STG2LSEN1V_POS);
	value |= ((SYS_ANA_REG19_OPENLOOPCAL1V_DEFAULT_VAL & SYS_ANA_REG19_OPENLOOPCAL1V_MASK) << SYS_ANA_REG19_OPENLOOPCAL1V_POS);
	value |= ((SYS_ANA_REG19_CALLATCH_DEFAULT_VAL & SYS_ANA_REG19_CALLATCH_MASK) << SYS_ANA_REG19_CALLATCH_POS);
	value |= ((SYS_ANA_REG19_VCMSEL_DEFAULT_VAL & SYS_ANA_REG19_VCMSEL_MASK) << SYS_ANA_REG19_VCMSEL_POS);
	value |= ((SYS_ANA_REG19_DWAMODE_DEFAULT_VAL & SYS_ANA_REG19_DWAMODE_MASK) << SYS_ANA_REG19_DWAMODE_POS);
	value |= ((SYS_ANA_REG19_R2REN_DEFAULT_VAL & SYS_ANA_REG19_R2REN_MASK) << SYS_ANA_REG19_R2REN_POS);
	value |= ((SYS_ANA_REG19_NC_26_27_DEFAULT_VAL & SYS_ANA_REG19_NC_26_27_MASK) << SYS_ANA_REG19_NC_26_27_POS);
	value |= ((SYS_ANA_REG19_MICEN_DEFAULT_VAL & SYS_ANA_REG19_MICEN_MASK) << SYS_ANA_REG19_MICEN_POS);
	value |= ((SYS_ANA_REG19_RST_DEFAULT_VAL & SYS_ANA_REG19_RST_MASK) << SYS_ANA_REG19_RST_POS);
	value |= ((SYS_ANA_REG19_BPDWA1V_DEFAULT_VAL & SYS_ANA_REG19_BPDWA1V_MASK) << SYS_ANA_REG19_BPDWA1V_POS);
	value |= ((SYS_ANA_REG19_HCEN1STG_DEFAULT_VAL & SYS_ANA_REG19_HCEN1STG_MASK) << SYS_ANA_REG19_HCEN1STG_POS);

	return value;
}

static uint32_t ana_reg20_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG20_HPDAC_DEFAULT_VAL & SYS_ANA_REG20_HPDAC_MASK) << SYS_ANA_REG20_HPDAC_POS);
	value |= ((SYS_ANA_REG20_CALCON_SEL_DEFAULT_VAL & SYS_ANA_REG20_CALCON_SEL_MASK) << SYS_ANA_REG20_CALCON_SEL_POS);
	value |= ((SYS_ANA_REG20_OSCDAC_DEFAULT_VAL & SYS_ANA_REG20_OSCDAC_MASK) << SYS_ANA_REG20_OSCDAC_POS);
	value |= ((SYS_ANA_REG20_OCENDAC_DEFAULT_VAL & SYS_ANA_REG20_OCENDAC_MASK) << SYS_ANA_REG20_OCENDAC_POS);
	value |= ((SYS_ANA_REG20_VCMSEL_DEFAULT_VAL & SYS_ANA_REG20_VCMSEL_MASK) << SYS_ANA_REG20_VCMSEL_POS);
	value |= ((SYS_ANA_REG20_ADJDACREF_DEFAULT_VAL & SYS_ANA_REG20_ADJDACREF_MASK) << SYS_ANA_REG20_ADJDACREF_POS);
	value |= ((SYS_ANA_REG20_DCOCHG_DEFAULT_VAL & SYS_ANA_REG20_DCOCHG_MASK) << SYS_ANA_REG20_DCOCHG_POS);
	value |= ((SYS_ANA_REG20_DIFFEN_DEFAULT_VAL & SYS_ANA_REG20_DIFFEN_MASK) << SYS_ANA_REG20_DIFFEN_POS);
	value |= ((SYS_ANA_REG20_ENDACCAL_DEFAULT_VAL & SYS_ANA_REG20_ENDACCAL_MASK) << SYS_ANA_REG20_ENDACCAL_POS);
	value |= ((SYS_ANA_REG20_NC2_DEFAULT_VAL & SYS_ANA_REG20_NC2_MASK) << SYS_ANA_REG20_NC2_POS);
	value |= ((SYS_ANA_REG20_LENDCOC_DEFAULT_VAL & SYS_ANA_REG20_LENDCOC_MASK) << SYS_ANA_REG20_LENDCOC_POS);
	value |= ((SYS_ANA_REG20_NC1_DEFAULT_VAL & SYS_ANA_REG20_NC1_MASK) << SYS_ANA_REG20_NC1_POS);
	value |= ((SYS_ANA_REG20_LENVCMD_DEFAULT_VAL & SYS_ANA_REG20_LENVCMD_MASK) << SYS_ANA_REG20_LENVCMD_POS);
	value |= ((SYS_ANA_REG20_DACDRVEN_DEFAULT_VAL & SYS_ANA_REG20_DACDRVEN_MASK) << SYS_ANA_REG20_DACDRVEN_POS);
	value |= ((SYS_ANA_REG20_NC0_DEFAULT_VAL & SYS_ANA_REG20_NC0_MASK) << SYS_ANA_REG20_NC0_POS);
	value |= ((SYS_ANA_REG20_DACLEN_DEFAULT_VAL & SYS_ANA_REG20_DACLEN_MASK) << SYS_ANA_REG20_DACLEN_POS);
	value |= ((SYS_ANA_REG20_DACG_DEFAULT_VAL & SYS_ANA_REG20_DACG_MASK) << SYS_ANA_REG20_DACG_POS);
	value |= ((SYS_ANA_REG20_DACMUTE_DEFAULT_VAL & SYS_ANA_REG20_DACMUTE_MASK) << SYS_ANA_REG20_DACMUTE_POS);
	value |= ((SYS_ANA_REG20_DACDWAMODE_SEL_DEFAULT_VAL & SYS_ANA_REG20_DACDWAMODE_SEL_MASK) << SYS_ANA_REG20_DACDWAMODE_SEL_POS);
	value |= ((SYS_ANA_REG20_DACSEL_DEFAULT_VAL & SYS_ANA_REG20_DACSEL_MASK) << SYS_ANA_REG20_DACSEL_POS);

	return value;
}

static uint32_t ana_reg21_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG21_LMDCIN_DEFAULT_VAL & SYS_ANA_REG21_LMDCIN_MASK) << SYS_ANA_REG21_LMDCIN_POS);
	value |= ((SYS_ANA_REG21_NC1_DEFAULT_VAL & SYS_ANA_REG21_NC1_MASK) << SYS_ANA_REG21_NC1_POS);
	value |= ((SYS_ANA_REG21_SPIRST_OVC_DEFAULT_VAL & SYS_ANA_REG21_SPIRST_OVC_MASK) << SYS_ANA_REG21_SPIRST_OVC_POS);
	value |= ((SYS_ANA_REG21_NC0_DEFAULT_VAL & SYS_ANA_REG21_NC0_MASK) << SYS_ANA_REG21_NC0_POS);
	value |= ((SYS_ANA_REG21_ENIDACL_DEFAULT_VAL & SYS_ANA_REG21_ENIDACL_MASK) << SYS_ANA_REG21_ENIDACL_POS);
	value |= ((SYS_ANA_REG21_DAC3RDHC0V9_DEFAULT_VAL & SYS_ANA_REG21_DAC3RDHC0V9_MASK) << SYS_ANA_REG21_DAC3RDHC0V9_POS);
	value |= ((SYS_ANA_REG21_HC2S_DEFAULT_VAL & SYS_ANA_REG21_HC2S_MASK) << SYS_ANA_REG21_HC2S_POS);
	value |= ((SYS_ANA_REG21_RFB_CTRL_DEFAULT_VAL & SYS_ANA_REG21_RFB_CTRL_MASK) << SYS_ANA_REG21_RFB_CTRL_POS);
	value |= ((SYS_ANA_REG21_VCMSEL_DEFAULT_VAL & SYS_ANA_REG21_VCMSEL_MASK) << SYS_ANA_REG21_VCMSEL_POS);
	value |= ((SYS_ANA_REG21_ENBS_DEFAULT_VAL & SYS_ANA_REG21_ENBS_MASK) << SYS_ANA_REG21_ENBS_POS);
	value |= ((SYS_ANA_REG21_CALCK_SEL0V9_DEFAULT_VAL & SYS_ANA_REG21_CALCK_SEL0V9_MASK) << SYS_ANA_REG21_CALCK_SEL0V9_POS);
	value |= ((SYS_ANA_REG21_BPDWA0V9_DEFAULT_VAL & SYS_ANA_REG21_BPDWA0V9_MASK) << SYS_ANA_REG21_BPDWA0V9_POS);
	value |= ((SYS_ANA_REG21_LOOPRST0V9_DEFAULT_VAL & SYS_ANA_REG21_LOOPRST0V9_MASK) << SYS_ANA_REG21_LOOPRST0V9_POS);
	value |= ((SYS_ANA_REG21_OCT0V9_DEFAULT_VAL & SYS_ANA_REG21_OCT0V9_MASK) << SYS_ANA_REG21_OCT0V9_POS);
	value |= ((SYS_ANA_REG21_SOUT0V9_DEFAULT_VAL & SYS_ANA_REG21_SOUT0V9_MASK) << SYS_ANA_REG21_SOUT0V9_POS);
	value |= ((SYS_ANA_REG21_HC0V9_DEFAULT_VAL & SYS_ANA_REG21_HC0V9_MASK) << SYS_ANA_REG21_HC0V9_POS);

	return value;
}
#endif //#if CONFIG_SOC_BK7236XX

bk_err_t bk_aud_clk_config(aud_clk_t clk)
{
	if (clk == AUD_CLK_APLL) {
#if defined(CONFIG_SOC_BK7236XX)
		sys_drv_aud_select_clock(1);
		//set apll clock config
		sys_drv_apll_en(1);
		sys_drv_apll_cal_val_set(0x8973CA6F);
		sys_drv_apll_config_set(0xC2A0AE86);
		sys_drv_apll_spi_trigger_set(1);
		delay(10);
		sys_drv_apll_spi_trigger_set(0);
		/* selet apll */
		aud_hal_set_audio_config_apll_sel(1);
#else
		sys_drv_aud_select_clock(0);
		/* selet xtal */
		aud_hal_set_audio_config_apll_sel(0);
#endif //#if CONFIG_SOC_BK7236XX
	} else {
		sys_drv_aud_select_clock(0);
		/* selet xtal */
		aud_hal_set_audio_config_apll_sel(0);
	}

	return BK_OK;
}

bk_err_t bk_aud_clk_deconfig(void)
{
	sys_drv_aud_select_clock(0);
	//set apll clock config
	sys_drv_apll_en(0);
	aud_hal_set_audio_config_apll_sel(0);

	return BK_OK;
}

bk_err_t bk_aud_driver_init(void)
{
	if (s_aud_driver_is_init)
		return BK_OK;

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_AUDIO, PM_POWER_MODULE_STATE_ON);
	sys_drv_aud_select_clock(0);
	bk_pm_clock_ctrl(PM_CLK_ID_AUDIO, CLK_PWR_CTRL_PWR_UP);

	bk_int_isr_register(INT_SRC_AUDIO, aud_isr, NULL);

	/*init audio paramters*/
	sys_drv_aud_int_en(1);
#if defined(CONFIG_SOC_BK7236XX)
/*
	sys_hal_set_ana_reg18_value(0x00BF8085);
	sys_hal_set_ana_reg19_value(0x81800006);
	sys_hal_set_ana_reg20_value(0xFBC02423);
	sys_hal_set_ana_reg21_value(0x00500000);
*/
	//sys_hal_aud_aud_en(1);
	aud_hal_set_clk_control_soft_reset(1);

	sys_hal_set_ana_reg18_value(ana_reg18_value_cal());
	sys_hal_set_ana_reg19_value(ana_reg19_value_cal());
	sys_hal_set_ana_reg20_value(ana_reg20_value_cal());
	sys_hal_set_ana_reg21_value(ana_reg21_value_cal());		//mic1
	sys_hal_set_ana_reg27_value(0x91800006);				//mic2

	/* enable audio bias */
	sys_drv_aud_audbias_en(1);
#endif

#if defined(CONFIG_SOC_BK7256XX)
	//enable audpll en
	sys_drv_aud_audpll_en(1);

	// config analog register
	sys_drv_analog_reg12_set(0x8610E0E0);
	sys_drv_analog_reg13_set(0x0F808400);
	sys_drv_analog_reg14_set(0x40038002);    //gain :15
	sys_drv_analog_reg15_set(0x40038002);
	//sys_drv_analog_reg16_set(0x89C02401);
	sys_drv_analog_reg16_set(0x89C62401);
	sys_drv_analog_reg17_set(0x80100000);

	//enable audvdd 1.0v and 1.5v
	sys_drv_aud_vdd1v_en(1);
	sys_drv_aud_vdd1v5_en(1);
#endif

	s_aud_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_aud_driver_deinit(void)
{
	if (!s_aud_driver_is_init) {
		return BK_OK;
	}

	/* check module init status */
	if (s_aud_module_init_sta.adc_is_init || s_aud_module_init_sta.dmic_is_init || s_aud_module_init_sta.dtmf_is_init || s_aud_module_init_sta.dac_is_init) {
		return BK_OK;
	}

	//reset audo configure
#if defined(CONFIG_AUDIO_ADC)
	bk_aud_adc_deinit();
#endif
#if defined(CONFIG_AUDIO_DAC)
	bk_aud_dac_deinit();
#endif
#if defined(CONFIG_AUDIO_DTMF)
	bk_aud_dtmf_deinit();
#endif
#if defined(CONFIG_AUDIO_DMIC)
	bk_aud_dmic_deinit();
#endif

	//disable audio interrupt
	sys_drv_aud_int_en(0);
	//ungister isr
	bk_int_isr_unregister(INT_SRC_AUDIO);

	// config analog register
	sys_hal_aud_clock_en(0);

#if defined(CONFIG_SOC_BK7236XX)
	sys_hal_set_ana_reg18_value(0);
	sys_hal_set_ana_reg19_value(0);
	sys_hal_set_ana_reg20_value(0);
	sys_hal_set_ana_reg21_value(0);

	sys_drv_aud_audbias_en(0);
	aud_hal_set_clk_control_soft_reset(0);
#endif

#if defined(CONFIG_SOC_BK7256XX)
	//disable audvdd 1.0v and 1.5v
	sys_drv_aud_vdd1v_en(0);
	sys_drv_aud_vdd1v5_en(0);

	// config analog register
	sys_drv_analog_reg12_set(0x0);
	sys_drv_analog_reg13_set(0x0);
	sys_drv_analog_reg14_set(0x0);
	sys_drv_analog_reg15_set(0x0);
	sys_drv_analog_reg16_set(0x0);
	sys_drv_analog_reg17_set(0x0);

	//disable audpll en
	sys_drv_aud_audpll_en(0);
#endif

	bk_pm_clock_ctrl(PM_CLK_ID_AUDIO, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_AUDIO, PM_POWER_MODULE_STATE_OFF);

	s_aud_driver_is_init = false;

	return BK_OK;
}

/* set module init status */
bk_err_t bk_aud_set_module_init_sta(aud_module_id_t id, bool val)
{
	switch (id) {
		case AUD_MODULE_ADC:
			s_aud_module_init_sta.adc_is_init = val;
			break;
		case AUD_MODULE_DMIC:
			s_aud_module_init_sta.dmic_is_init = val;
			break;
		case AUD_MODULE_DTMF:
			s_aud_module_init_sta.dtmf_is_init = val;
			break;
		case AUD_MODULE_DAC:
			s_aud_module_init_sta.dac_is_init = val;
			break;

		default:
			return BK_FAIL;
			break;
	}

	return BK_OK;
}

/* set module init status */
bool bk_aud_get_module_init_sta(aud_module_id_t id)
{
	bool status = false;

	switch (id) {
		case AUD_MODULE_ADC:
			status = s_aud_module_init_sta.adc_is_init;
			break;
		case AUD_MODULE_DMIC:
			status = s_aud_module_init_sta.dmic_is_init;
			break;
		case AUD_MODULE_DTMF:
			status = s_aud_module_init_sta.dtmf_is_init;
			break;
		case AUD_MODULE_DAC:
			status = s_aud_module_init_sta.dac_is_init;
			break;

		default:
			status = false;
			break;
	}

	return status;
}


/* register audio interrupt */
bk_err_t bk_aud_register_aud_isr(aud_isr_id_t isr_id, aud_isr_t isr)
{
//	AUD_RETURN_ON_INVALID_ISR_ID(isr_id);
	bk_err_t ret = BK_OK;
	uint32_t int_level = rtos_enter_critical();

	switch (isr_id) {
#if defined(CONFIG_AUDIO_ADC)
		case AUD_ISR_ADCL: /**< adcl_int_en */
			s_aud_isr.aud_adcl_fifo_handler = isr;
			break;
#endif
#if defined(CONFIG_AUDIO_DMIC)
		case AUD_ISR_DMIC:
			s_aud_isr.aud_dmic_fifo_handler = isr;
			break;
#endif
#if defined(CONFIG_AUDIO_DTMF)
		case AUD_ISR_DTMF:	  /**< dtmf_int_en */
			s_aud_isr.aud_dtmf_fifo_handler = isr;
			break;
#endif
#if defined(CONFIG_AUDIO_DAC)
		case AUD_ISR_DACL:	  /**< dacl_int_en */
			s_aud_isr.aud_dacl_fifo_handler = isr;
			break;
		case AUD_ISR_DACR:	  /**< dacr_int_en */
			s_aud_isr.aud_dacr_fifo_handler = isr;
			break;
#endif

		default:
			ret = BK_FAIL;
			break;
	}

	rtos_exit_critical(int_level);

	return ret;
}

/* audio check interrupt flag and excute correponding isr function when enter interrupt */
static void aud_isr_common(void)
{
#if defined(CONFIG_AUDIO_ADC)
	uint32_t adcl_int_status = aud_hal_get_fifo_status_adcl_int_flag();
	if (adcl_int_status) {
		if (s_aud_isr.aud_adcl_fifo_handler) {
			s_aud_isr.aud_adcl_fifo_handler();
		}
	}
#endif

#if defined(CONFIG_AUDIO_DMIC)
	uint32_t dmic_int_status = aud_hal_get_fifo_status_dmic_int_flag();
	if (dmic_int_status) {
		if (s_aud_isr.aud_dmic_fifo_handler) {
			s_aud_isr.aud_dmic_fifo_handler();
		}
	}
#endif

#if defined(CONFIG_AUDIO_DTMF)
	uint32_t dtmf_int_status = aud_hal_get_fifo_status_dtmf_int_flag();
	if (dtmf_int_status) {
		if (s_aud_isr.aud_dtmf_fifo_handler) {
			s_aud_isr.aud_dtmf_fifo_handler();
		}
	}
#endif

#if defined(CONFIG_AUDIO_DAC)
	uint32_t dacl_int_status = aud_hal_get_fifo_status_dacl_int_flag();
	uint32_t dacr_int_status = aud_hal_get_fifo_status_dacr_int_flag();
	if (dacl_int_status) {
		if (s_aud_isr.aud_dacl_fifo_handler) {
			s_aud_isr.aud_dacl_fifo_handler();
		}
	}

	if (dacr_int_status) {
		if (s_aud_isr.aud_dacr_fifo_handler) {
			s_aud_isr.aud_dacr_fifo_handler();
		}
	}
#endif

}

/* audio interrupt enter*/
static void aud_isr(void)
{
	aud_isr_common();
}

