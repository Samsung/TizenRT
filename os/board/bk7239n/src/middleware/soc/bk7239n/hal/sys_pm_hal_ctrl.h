// Copyright 2020-2025 Beken
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

#include <soc/soc.h>

typedef struct {
	uint32_t aon_ldo_self_lp      : 1; // bit[0]
	uint32_t io_ldo_self_lp       : 1; // bit[1]
	uint32_t ana_ldo_self_lp      : 1; // bit[2]
	uint32_t digital_ldo_self_lp  : 1; // bit[3]
	uint32_t core_ldo_self_lp     : 1; // bit[4]

	uint32_t aon_ldo_volt          : 3; // bit[5:7]
	uint32_t io_ldo_volt           : 3; // bit[8:10]
	uint32_t ana_trsw_en           : 1; // bit[11]
	uint32_t ana_ldo_rx_volt       : 3; // bit[12:14]
	uint32_t ana_ldo_tx_volt       : 3; // bit[15:17]
	uint32_t digital_ldo_lp        : 1; // bit[18]
	uint32_t digital_ldo_low_volt  : 2; // bit[19:20]
	uint32_t digital_ldo_high_volt : 3; // bit[21:23]
	uint32_t core_ldo_lp           : 1; // bit[24]
	uint32_t core_ldo_low_volt     : 3; // bit[25:27]
	uint32_t core_ldo_high_volt    : 4; // bit[28:31]

	uint32_t io_ldo_current_limit  : 1; // bit[0]
	uint32_t ana_ldo_current_limit : 1; // bit[1]
	uint32_t aon_ldo_power         : 1; // bit[2]
	uint32_t lv_ctrl_pd            : 1; // bit[3]
	uint32_t lv_ctrl_hf            : 1; // bit[4]
	uint32_t lv_ctrl_flash         : 1; // bit[5]
	uint32_t lv_ctrl_core          : 1; // bit[6]
} sys_pm_ctrl_t;

typedef struct {
	uint16_t volt_mv;
	uint8_t reg_value;
} sys_pm_volt_reg_map_t;
 
#define SYS_PM_AON_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{600, 0},\
	{650, 1},\
	{700, 2},\
	{750, 3},\
	{800, 4},\
	{850, 5},\
	{900, 6},\
	{950, 7}\
}

#define SYS_PM_IO_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{2900, 0},\
	{3000, 1},\
	{3100, 2},\
	{3200, 3},\
	{3300, 4},\
	{3400, 5},\
	{3500, 6},\
	{3600, 7}\
}

#define SYS_PM_ANA_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{1250, 0},\
	{1300, 1},\
	{1350, 2},\
	{1400, 3},\
	{1450, 4},\
	{1500, 5},\
	{1550, 6},\
	{1600, 7}\
}

#define SYS_PM_DIGITAL_LOW_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{450, 0},\
	{500, 1},\
	{550, 2},\
	{600, 3}\
}

#define SYS_PM_DIGITAL_HIGH_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{700, 0},\
	{750, 1},\
	{800, 2},\
	{850, 3},\
	{900, 4},\
	{950, 5},\
	{1000, 6},\
	{1050, 7}\
}

#define SYS_PM_CORE_LOW_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{450, 0},\
	{475, 1},\
	{500, 2},\
	{525, 3},\
	{550, 4},\
	{575, 5},\
	{600, 6},\
	{625, 7}\
}

#define SYS_PM_CORE_HIGH_VOLT_MAP(map) \
	sys_pm_volt_reg_map_t map[] = {\
	{600, 0},\
	{625, 1},\
	{650, 2},\
	{675, 3},\
	{700, 4},\
	{725, 5},\
	{750, 6},\
	{775, 7},\
	{800, 8},\
	{825, 9},\
	{850, 10},\
	{875, 11},\
	{900, 12},\
	{925, 13},\
	{950, 14},\
	{975, 15}\
}

#if defined(CONFIG_PM_HAL_CTRL)
extern sys_pm_ctrl_t s_sys_pm;

#define IF_LV_CTRL_PD() if (s_sys_pm.lv_ctrl_pd)
#define IF_LV_CTRL_HF() if (s_sys_pm.lv_ctrl_hf)
#define IF_LV_CTRL_FLASH() if (s_sys_pm.lv_ctrl_flash)
#define IF_LV_CTRL_CORE() if (s_sys_pm.lv_ctrl_core)

#else

#define IF_LV_CTRL_PD()
#define IF_LV_CTRL_HF()
#define IF_LV_CTRL_FLASH() 
#define IF_LV_CTRL_CORE()
#endif
