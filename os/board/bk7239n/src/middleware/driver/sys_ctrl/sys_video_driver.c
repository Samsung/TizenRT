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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"

/**  Video Start **/
uint32_t sys_drv_lcd_set(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h,uint8_t clk_always_on)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_lcd_disp_clk_en(clk_src_sel, clk_div_l,clk_div_h, clk_always_on);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_lcd_close(void)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_lcd_disp_close();
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_dma2d_set(uint8_t clk_always_on)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_dma2d_clk_en(clk_always_on);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_jpeg_dec_disckg(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_jpeg_dec_disckg(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}


/**  Video End **/

/**  Jpeg Start **/
uint32_t sys_drv_mclk_mux_set(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_mclk_mux_set(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_jpeg_clk_sel(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_jpeg_clk_sel(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_clk_div_mode1_clkdiv_jpeg(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_clk_div_mode1_clkdiv_jpeg(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_jpeg_disckg(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_jpeg_disckg(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_video_power_en(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_video_power_en(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_auxs_cis(uint32_t cksel, uint32_t ckdiv)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_auxs_cis_clk_sel(cksel);
	sys_hal_set_auxs_cis_clk_div(ckdiv);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_jpeg_clk_en(uint32_t enable)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();

	if (enable)
		sys_hal_set_jpeg_clk_en(1);
	else
		sys_hal_set_jpeg_clk_en(0);

	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_cis_auxs_clk_en(uint32_t enable)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level;
	int_level = sys_drv_enter_critical();

	if (enable)
		sys_hal_set_cis_auxs_clk_en(1);
	else
		sys_hal_set_cis_auxs_clk_en(0);

	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}


/**  Jpeg End **/

/** H264 Start **/

uint32_t sys_drv_set_h264_clk_sel(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_jpeg_clk_sel(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_clk_div_mode1_clkdiv_h264(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level = sys_drv_enter_critical();
	sys_hal_set_clk_div_mode1_clkdiv_jpeg(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_set_h264_clk_en()
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level = sys_drv_enter_critical();
	sys_hal_set_h264_clk_en(1);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}

uint32_t sys_drv_h264_set_mclk_div(uint32_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	uint32_t int_level = sys_drv_enter_critical();
	sys_hal_mclk_div_set(value);
	sys_drv_exit_critical(int_level);

	if(!ret)
		ret = sys_amp_res_release();

	return ret;
}



/** H264 End **/
