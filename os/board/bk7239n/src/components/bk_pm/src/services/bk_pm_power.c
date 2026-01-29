// Copyright 2021-2025 Beken
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
#include <modules/pm.h>
#include <sys_types.h>
#include "sys_driver.h"
#include <driver/pwr_clk.h>

#if CONFIG_INT_WDT
#include <driver/wdt.h>
#include <bk_wdt.h>
#endif
#if CONFIG_AON_WDT
#include <driver/aon_wdt.h>
#endif

#include "pm_power.h"
#include "pm_psram.h"
#include "pm_debug.h"


static bool s_pm_phy_calibration_state    = false;
static bool s_pm_is_phy_reinit_flag       = false;

static uint32_t s_pm_encp_pm_state                      = 0;
static uint32_t s_pm_bakp_pm_state                      = 0;
static uint32_t s_pm_ahpb_pm_state                      = 0;
static uint32_t s_pm_audio_pm_state                     = 0;
static uint32_t s_pm_video_pm_state                     = 0;
static uint32_t s_pm_btsp_pm_state                      = 0;
static uint32_t s_pm_thread_pm_state                    = 0;
static uint32_t s_pm_phy_pm_state                       = 0;


static void pm_module_check_power_on(uint32_t *pm_off_modules, uint32_t *pm_on_modules, pm_power_module_name_e module);
static void pm_module_check_power_off(uint32_t *pm_off_modules, uint32_t *pm_on_modules, pm_power_module_name_e module);


/*=========================MODULES POWER CTRL START========================*/
bk_err_t bk_pm_module_power_on(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules, pm_power_module_name_e module)
{
	bk_err_t ret = BK_OK;
	uint32_t domain, submodule;
	GLOBAL_INT_DECLARATION();

	if (module < PM_POWER_MODULE_NAME_NONE) {
		if (module == PM_POWER_MODULE_NAME_BTSP) {
			if (bk_pm_mem_auto_power_down_state_get()) {
				pm_module_check_power_on(pm_off_modules, pm_on_modules, PM_POWER_MODULE_NAME_MEM3);
			}
			sys_drv_module_power_ctrl(module, PM_POWER_MODULE_STATE_ON);
			GLOBAL_INT_DISABLE();
			*pm_off_modules &= ~(0x1 << PM_POWER_MODULE_NAME_BTSP);
			*pm_on_modules |= 0x1 << PM_POWER_MODULE_NAME_BTSP;
			*pm_sleeped_modules &= ~(0x1ULL << PM_POWER_MODULE_NAME_BTSP);
			GLOBAL_INT_RESTORE();
		} else {
			sys_drv_module_power_ctrl(module, PM_POWER_MODULE_STATE_ON);
			if (module == PM_POWER_MODULE_NAME_PHY) {
				s_pm_phy_calibration_state = 0x1;
			}
			GLOBAL_INT_DISABLE();
			*pm_off_modules &= ~(0x1 << module);
			*pm_on_modules |= 0x1 << module;
			GLOBAL_INT_RESTORE();
		}
	} else {
		domain = module/PM_MODULE_SUB_POWER_DOMAIN_MAX;
		submodule = module%PM_MODULE_SUB_POWER_DOMAIN_MAX;
		switch (domain) {
			case POWER_MODULE_NAME_ENCP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_encp_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_BAKP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_bakp_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_AHBP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_ahpb_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_AUDP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_audio_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				*pm_sleeped_modules &= ~(0x1ULL << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_VIDP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_video_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				*pm_sleeped_modules &= ~(0x1ULL << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_BTSP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_btsp_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				*pm_sleeped_modules &= ~(0x1ULL << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					if (bk_pm_mem_auto_power_down_state_get()) {
						pm_module_check_power_on(pm_off_modules, pm_on_modules, PM_POWER_MODULE_NAME_MEM3);
					}
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_THREAD:
			{
				GLOBAL_INT_DISABLE();
				s_pm_thread_pm_state |= (0x1 << submodule);
				*pm_off_modules &= ~(0x1 << domain);
				*pm_on_modules |= (0x1 << domain);
				GLOBAL_INT_RESTORE();

				if (sys_drv_module_power_state_get(domain)) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
				}
			}
				break;

			case PM_POWER_MODULE_NAME_PHY:
			{
				GLOBAL_INT_DISABLE();
				s_pm_phy_pm_state |= (0x1 << submodule);
				GLOBAL_INT_RESTORE();
				if ((0x0 == sys_drv_module_power_state_get(domain))
					&& (s_pm_phy_calibration_state == 0x1)) {
					if (pm_debug_mode() & 0x2)
						BK_LOGD(NULL, "phy power on already\r\n");
				} else {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_ON);
					if (0x0 == sys_drv_module_power_state_get(domain)) {
						if (PM_POWER_SUB_MODULE_NAME_PHY_RF != module) {
							#if CONFIG_WIFI_ENABLE
							extern void phy_wakeup_reinit(uint8 is_wifi);
							phy_wakeup_reinit(module == PM_POWER_SUB_MODULE_NAME_PHY_WIFI);
							#elif CONFIG_BLUETOOTH
							extern void phy_wakeup_for_bluetooth();
							phy_wakeup_for_bluetooth();
							#else
							#endif
							s_pm_is_phy_reinit_flag = true;
							GLOBAL_INT_DISABLE();
							s_pm_phy_calibration_state = 0x1;
							*pm_off_modules &= ~(0x1 << domain);
							*pm_on_modules |= (0x1 << domain);
							GLOBAL_INT_RESTORE();
						} else {
							GLOBAL_INT_DISABLE();
							*pm_off_modules &= ~(0x1 << domain);
							*pm_on_modules |= (0x1 << domain);
							GLOBAL_INT_RESTORE();
						}
					} else {
						if (pm_debug_mode() & 0x2) {
							BK_LOGD(NULL, "phy power on fail 0x%x\r\n", sys_drv_module_power_state_get(domain));
						}
						ret = BK_FAIL;
					}
				}
			}
				break;

			default:
				ret = BK_FAIL;
				break;
		}
	}

	return ret;
}

bk_err_t bk_pm_module_power_off(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules, pm_power_module_name_e module)
{
	uint32_t domain, submodule;
	GLOBAL_INT_DECLARATION();

	if (module < PM_POWER_MODULE_NAME_NONE) {
		if ((module == PM_POWER_MODULE_NAME_VIDP)
			|| (module == PM_POWER_MODULE_NAME_AUDP)
			|| (module == PM_POWER_MODULE_NAME_PHY)) {
			if (pm_debug_mode() & 0x2)
				BK_LOGD(NULL, "module[%d] can not directly power off\r\n", module);
			return BK_FAIL;
		}

		if (module == PM_POWER_MODULE_NAME_CPU1) {
			sys_drv_module_power_ctrl(module, PM_POWER_MODULE_STATE_OFF);
			GLOBAL_INT_DISABLE();
			*pm_off_modules |= (0x1 << module);
			*pm_on_modules &= ~(0x1 << module);
			GLOBAL_INT_RESTORE();
			#if (CONFIG_CPU_CNT > 1)
			bk_pm_cp1_work_state_set(PM_MAILBOX_COMMUNICATION_INIT);
			#endif
		} else {
			sys_drv_module_power_ctrl(module, PM_POWER_MODULE_STATE_OFF);
			GLOBAL_INT_DISABLE();
			*pm_off_modules |= 0x1 << module;
			*pm_on_modules &= ~(0x1 << module);
			GLOBAL_INT_RESTORE();
		}
	} else {
		domain = module/PM_MODULE_SUB_POWER_DOMAIN_MAX;
		submodule = module%PM_MODULE_SUB_POWER_DOMAIN_MAX;
		switch (domain) {
			case POWER_MODULE_NAME_ENCP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_encp_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_encp_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_BAKP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_bakp_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_bakp_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_AHBP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_ahpb_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_ahpb_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_AUDP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_audio_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_audio_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
					*pm_sleeped_modules |= (0x1ULL << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_VIDP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_video_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_video_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
					*pm_sleeped_modules |= (0x1ULL << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_BTSP:
			{
				GLOBAL_INT_DISABLE();
				s_pm_btsp_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_btsp_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
					*pm_sleeped_modules |= (0x1ULL << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_THREAD:
			{
				GLOBAL_INT_DISABLE();
				s_pm_thread_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_thread_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			case PM_POWER_MODULE_NAME_PHY:
			{
				GLOBAL_INT_DISABLE();
				s_pm_phy_pm_state &= ~(0x1 << submodule);

				if (0x0 == s_pm_phy_pm_state) {
					sys_drv_module_power_ctrl(domain, PM_POWER_MODULE_STATE_OFF);
					s_pm_phy_calibration_state = 0x0;
					*pm_off_modules |= (0x1 << domain);
					*pm_on_modules &= ~(0x1 << domain);
				}
				GLOBAL_INT_RESTORE();
			}
				break;

			default:
				break;
		}
	}

	return BK_OK;
}

int32_t bk_pm_module_power_state_get(pm_power_module_name_e module)
{
	uint32_t domain;
	int32_t state;

	if (module < PM_POWER_MODULE_NAME_NONE) {
		return sys_drv_module_power_state_get(module);
	}

	domain = module/PM_MODULE_SUB_POWER_DOMAIN_MAX;
	switch (domain) {
		case PM_POWER_MODULE_NAME_ENCP:
		case PM_POWER_MODULE_NAME_BAKP:
		case PM_POWER_MODULE_NAME_AHBP:
		case PM_POWER_MODULE_NAME_AUDP:
		case PM_POWER_MODULE_NAME_VIDP:
		case PM_POWER_MODULE_NAME_BTSP:
		case PM_POWER_MODULE_NAME_THREAD:
		case PM_POWER_MODULE_NAME_PHY:
			state = sys_drv_module_power_state_get(domain);
			break;

		default:
			BK_LOGD(NULL, "pm module[%d] not support ,get power state fail.\r\n", module);
			state = BK_ERR_NOT_SUPPORT;
			break;
	}

	return state;
}

void pm_check_power_on_module(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules)
{
	uint32_t off_modules = 0;

	if (!(*pm_on_modules & ((0x1 << PM_POWER_MODULE_NAME_BTSP)))) // when the module not power on , set the module sleep state
	{
		*pm_sleeped_modules |= 0x1ULL << PM_POWER_MODULE_NAME_BTSP;
		*pm_off_modules |= 0x1 << PM_POWER_MODULE_NAME_BTSP;
		// BK_LOGD(NULL, "bt not power on \r\n");
	}

	if (!(*pm_on_modules & (0x1 << PM_POWER_MODULE_NAME_WIFIP_MAC))) // when the module not power on , set the module sleep state
	{
		*pm_sleeped_modules |= 0x1ULL << PM_POWER_MODULE_NAME_WIFIP_MAC;
		*pm_off_modules |= 0x1 << PM_POWER_MODULE_NAME_WIFIP_MAC;
		// BK_LOGD(NULL, "wifi not power on \r\n");
	}

	if (!(*pm_on_modules & (0x1 << PM_POWER_MODULE_NAME_AUDP))) // when the module not power on , set the module sleep state
	{
		*pm_sleeped_modules |= 0x1ULL << PM_POWER_MODULE_NAME_AUDP;
		*pm_off_modules |= 0x1 << PM_POWER_MODULE_NAME_AUDP;
		// BK_LOGD(NULL, "audio not power on \r\n");
	}

	if (!(*pm_on_modules & (0x1 << PM_POWER_MODULE_NAME_VIDP))) // when the module not power on , set the module sleep state
	{
		*pm_sleeped_modules |= 0x1ULL << PM_POWER_MODULE_NAME_VIDP;
		*pm_off_modules |= 0x1 << PM_POWER_MODULE_NAME_VIDP;
		// BK_LOGD(NULL, "video not power on \r\n");
	}

}

void pm_check_power_off_module(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules)
{
}

static void pm_module_check_power_on(uint32_t *pm_off_modules, uint32_t *pm_on_modules, pm_power_module_name_e module)
{
	if (PM_POWER_MODULE_STATE_OFF == sys_drv_module_power_state_get(module))
	{
		sys_drv_module_power_ctrl(module, PM_POWER_MODULE_STATE_ON);

		*pm_off_modules &= ~(0x1 << module);
		*pm_on_modules |= 0x1 << module;
	}
}

static void pm_module_check_power_off(uint32_t *pm_off_modules, uint32_t *pm_on_modules, pm_power_module_name_e module)
{
	if (PM_POWER_MODULE_STATE_ON == sys_drv_module_power_state_get(module))
	{
		if (pm_debug_mode() & 0x2)
		{
			BK_LOGD(NULL, "pm_module_check_power_off module[%d][%d]\r\n", module, sys_drv_module_power_state_get(module));
		}
		sys_drv_module_power_ctrl(module, PM_POWER_MODULE_STATE_OFF);

		*pm_off_modules |= 0x1 << module;
		*pm_on_modules &= ~(0x1 << module);
	}
}

uint32_t bk_pm_get_audio_vote_pwr_state(void)
{
	return s_pm_audio_pm_state;
}
uint32_t bk_pm_get_video_vote_pwr_state(void)
{
	return s_pm_video_pm_state ;
}

uint32_t bk_pm_phy_pm_state_get()
{
	return s_pm_phy_pm_state;
}
/*=========================MODULES POWER CTRL END========================*/

/*=========================RF POWER CTRL START========================*/
// TODO: is still in use ?
static uint32_t s_pm_rf_on_modules;
static uint32_t s_pm_rf_off_modules;
void pm_rf_power_ctrl(pm_power_module_name_e module, pm_power_module_state_e power_state)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	if (power_state == PM_POWER_MODULE_STATE_ON)
	{
		s_pm_rf_on_modules |= 0x1 << module;
	}
	else
	{
		s_pm_rf_off_modules |= 0x1 << module;
	}
	sys_drv_module_RF_power_ctrl(module, power_state);
	GLOBAL_INT_RESTORE();
}
void pm_rf_switch(pm_power_module_name_e name)
{
}
/*=========================RF POWER CTRL END========================*/

/*=========================SPECIFIC API START========================*/
bool bk_pm_phy_cali_state_get()
{
	return s_pm_phy_calibration_state;
}

bool bk_pm_phy_reinit_flag_get()
{
	return s_pm_is_phy_reinit_flag;
}

void bk_pm_phy_reinit_flag_clear()
{
	s_pm_is_phy_reinit_flag = false;
}
/*=========================SPECIFIC API END========================*/

/*=========================DEBUG/TEST CTRL START========================*/
void pm_power_dump(void)
{
	LOGD("pm video,audio:0x%x 0x%x \r\n",s_pm_video_pm_state,s_pm_audio_pm_state);
	LOGD("pm ahpb,bakp:0x%x 0x%x\r\n",s_pm_ahpb_pm_state,s_pm_bakp_pm_state);
}

void pm_power_modules_dump_with_sleep_mode(pm_sleep_mode_e sleep_mode)
{
	LOGI("%s2 0x%X 0x%X 0x%X 0x%X\r\n", pm_sleep_mode_to_string(sleep_mode),
		s_pm_ahpb_pm_state, s_pm_video_pm_state, s_pm_audio_pm_state, s_pm_bakp_pm_state);
}

// TODO: rename or remove
bk_err_t pm_debug_module_state(void)
{
	#if CONFIG_PSRAM && CONFIG_PSRAM_AS_SYS_MEMORY
	pm_cp1_psram_malloc_state_get();
	#endif

	if(s_pm_ahpb_pm_state > 0)
	{
		LOGD("Ahbp not PD[module:0x%x]\r\n",s_pm_ahpb_pm_state);
	}
	if(s_pm_bakp_pm_state > 0)
	{
		LOGI("Bakp not PD[module:0x%x]\r\n",s_pm_bakp_pm_state);
	}
	if(s_pm_video_pm_state > 0)
	{
		LOGD("Video not PD[modulue:0x%x]\r\n",s_pm_video_pm_state);
	}
	if(s_pm_audio_pm_state > 0)
	{
		LOGD("Audio not PD[modulue:0x%x]\r\n",s_pm_audio_pm_state);
	}

	if(!bk_pm_module_power_state_get(PM_POWER_MODULE_NAME_CPU1))
	{
		LOGD("Cp1 not PD[state:0x%x]\r\n",bk_pm_module_power_state_get(PM_POWER_MODULE_NAME_CPU1));
	}

	// TODO: why not using bk_pm_module_power_state_get(PM_POWER_MODULE_NAME_CPU2) ?
	// if(!(REG_READ(PM_DEBUG_SYS_REG_BASE+0x6*4)&0x2))
	// {
	// 	BK_LOGD(NULL, "Cp2 not PD[state:0x%x]\r\n",REG_READ(PM_DEBUG_SYS_REG_BASE+0x6*4));
	// }

	#if CONFIG_PSRAM && CONFIG_PSRAM_AS_SYS_MEMORY
	pm_debug_psram();
	#endif

	return BK_OK;
}
/*=========================DEBUG/TEST CTRL END========================*/
