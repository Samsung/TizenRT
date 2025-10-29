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
//#include <common/bk_include.h>
//#include <soc/soc.h>
//#include <soc/soc_port.h>
//#include "hal_config.h"
//#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DISP_INT_EN            1  /**< enable lcd cpu level int*/
#define DSIP_DISCLK_ALWAYS_ON  0  /**< 0: bus clock open when module is select,1:bus clock always open*/

#define BK_ERR_LCD_NOT_INIT    (BK_ERR_LCD_BASE - 1) /**< LCD driver not init */

/**< 8080 cfg*/
#define VERIFY_1MS_COUNT   0x64
#define TIK_CNT            0x2

//#ifdef CONFIG_SOC_BK7258
#define DATA_FIFO_WR_THRD  896  //  1000 //
#define DATA_FIFO_RD_THRD   16  //  10   //

//#ifdef CONFIG_SOC_BK7256
//#define DATA_FIFO_WR_THRD 0x80
//#define DATA_FIFO_RD_THRD 0x180

#define CMD_FIFO_WR_THRD   0x60
#define CMD_FIFO_RD_THRD   0

typedef enum {
	CONTINUE_MODE = 0,
	DISCONTINUE_MODE
} discontinue_mode;



typedef enum {
	DISP_DIV_H_0 = 0,
	DISP_DIV_H_1,
	DISP_DIV_H_2,
	DISP_DIV_H_3,	
	DISP_DIV_H_4,
	DISP_DIV_H_5,
	DISP_DIV_H_6,
	DISP_DIV_H_7,
} disp_div_h_t;

typedef enum {
	DISP_DIV_L_0 = 0,
	DISP_DIV_L_1,
} disp_div_l_t;

typedef enum {
	DISP_CLK_320M = 0,
	DISP_CLK_120M,

} disp_core_clk_sel_t;

#ifdef __cplusplus
}
#endif

