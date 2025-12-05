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

#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/dma2d.h>
#include <driver/qspi.h>
#include <driver/qspi_types.h>
#include <driver/lcd_types.h>
#include <driver/lcd_qspi.h>
#include <driver/media_types.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "bk_misc.h"
#include "lcd_qspi_driver.h"
#include <driver/dma.h>
#include "bk_general_dma.h"


#if defined(CONFIG_SOC_BK7256XX)
#define LCD_QSPI_DATA_ADDR      0x68000000
#define LCD_QSPI_RESET_PIN      GPIO_47
#endif

#if defined(CONFIG_SOC_BK7236XX)
#define LCD_QSPI0_DATA_ADDR     0x64000000
#define LCD_QSPI1_DATA_ADDR     0x68000000
#define LCD_QSPI_RESET_PIN      GPIO_40
#endif

static qspi_driver_t s_lcd_qspi[SOC_QSPI_UNIT_NUM] = {
	{
		.hal.hw = (qspi_hw_t *)(SOC_QSPI0_REG_BASE),
	},
#if (SOC_QSPI_UNIT_NUM > 1)
	{
		.hal.hw = (qspi_hw_t *)(SOC_QSPI1_REG_BASE),
	}
#endif
};

static beken_semaphore_t lcd_qspi_semaphore = NULL;

#if (defined(CONFIG_SOC_BK7256XX))

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
static void dma2d_transfer_complete_isr(void)
{
	bk_err_t ret = BK_OK;
	bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
	ret = rtos_set_semaphore(&lcd_qspi_semaphore);
	if (ret != BK_OK) {
		LCD_QSPI_LOGE("lcd qspi semaphore set failed\r\n");
		return;
	}
}
#endif
#elif defined(CONFIG_SOC_BK7236XX)
static dma_id_t lcd_qspi_dma_id = DMA_ID_MAX;
static uint32_t dma_repeat_once_len = 0;
extern media_debug_t *media_debug;

static void lcd_qspi_dma_finish_isr(void)
{
    bk_err_t ret = BK_OK;
    uint32_t value = 0;

    value = bk_dma_get_repeat_wr_pause(lcd_qspi_dma_id);
    if (value) {
        media_debug->isr_lcd++;
        bk_dma_stop(lcd_qspi_dma_id);
        //bk_lcd_qspi_quad_write_stop();

        ret = rtos_set_semaphore(&lcd_qspi_semaphore);
        if (ret != BK_OK) {
            LCD_QSPI_LOGE("lcd qspi semaphore set failed\r\n");
            return;
        }
    }
}
#endif

static bk_err_t lcd_qspi_driver_init(qspi_id_t qspi_id, lcd_qspi_clk_t clk)
{
    qspi_config_t lcd_qspi_config;
    os_memset(&lcd_qspi_config, 0, sizeof(lcd_qspi_config));

    os_memset(&s_lcd_qspi, 0, sizeof(s_lcd_qspi));
    s_lcd_qspi[qspi_id].hal.id = qspi_id;
    qspi_hal_init(&s_lcd_qspi[qspi_id].hal);

    switch (clk) {
        case LCD_QSPI_80M:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 5;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_64M:
            lcd_qspi_config.src_clk = QSPI_SCLK_320M;
            lcd_qspi_config.src_clk_div = 4;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_60M:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 7;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_53M:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 8;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_48M:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 9;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_40M:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 11;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_32M:
            lcd_qspi_config.src_clk = QSPI_SCLK_320M;
            lcd_qspi_config.src_clk_div = 9;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        case LCD_QSPI_30M:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 15;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;

        default:
            lcd_qspi_config.src_clk = QSPI_SCLK_480M;
            lcd_qspi_config.src_clk_div = 11;
            BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &lcd_qspi_config));
            break;
    }

    return BK_OK;
}

static bk_err_t lcd_qspi_hardware_reset(void)
{
    gpio_dev_unmap(LCD_QSPI_RESET_PIN);
    gpio_dev_map(LCD_QSPI_RESET_PIN, 0);
    bk_gpio_enable_pull(LCD_QSPI_RESET_PIN);
    bk_gpio_pull_up(LCD_QSPI_RESET_PIN);
    rtos_delay_milliseconds(10);
    bk_gpio_pull_down(LCD_QSPI_RESET_PIN);
    rtos_delay_milliseconds(10);
    bk_gpio_pull_up(LCD_QSPI_RESET_PIN);
    rtos_delay_milliseconds(120);

    return BK_OK;
}

static bk_err_t lcd_qspi_quad_write_enable(qspi_id_t qspi_id)
{
    qspi_hal_set_cmd_a_l(&s_lcd_qspi[qspi_id].hal, 0x00002C00);
    qspi_hal_set_cmd_a_h(&s_lcd_qspi[qspi_id].hal, 0x10000100);
    qspi_hal_set_cmd_a_cfg1(&s_lcd_qspi[qspi_id].hal, 0xEAAA);
    qspi_hal_set_cmd_a_cfg2(&s_lcd_qspi[qspi_id].hal, 0x80008000);

    return BK_OK;
}

#if defined(CONFIG_SH8601A_PARTIAL)
/*设置部分显示区域坐标函数*/
void sh8601a_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye)
{
    uint16 xs_l, xs_h, xe_l, xe_h;
    uint16 ys_l, ys_h, ye_l, ye_h; 

    xs_h = xs >> 8;
    xs_l = xs & 0xff;

    xe_h = xe >> 8;
    xe_l = xe & 0xff;

    ys_h = ys >> 8;
    ys_l = ys & 0xff;

    ye_h = ye >> 8;
    ye_l = ye & 0xff;

    uint8_t param_clumn[4] = {xs_h, xs_l, xe_h, xe_l};
    uint8_t param_row[4]= {ys_h, ys_l, ye_h, ye_l};

//    LCD_QSPI_LOGE("xs_h:0x%x, xs_l:0x%x, xe_h:0x%x, xe_l:0x%x\r\n",xs_h, xs_l, xe_h, xe_l);
//    LCD_QSPI_LOGE("ys_h:0x%x, ys_l:0x%x, ye_h:0x%x, ye_l:0x%x\r\n",ys_h, ys_l, ye_h, ye_l);
    bk_lcd_qspi_send_cmd(0x2, 0x30, param_clumn,4);
    bk_lcd_qspi_send_cmd(0x2, 0x31, param_row,4);
}
#endif

static bk_err_t lcd_qspi_common_init(qspi_id_t qspi_id)
{
    bk_err_t ret = BK_OK;

    lcd_qspi_hardware_reset();

    ret = rtos_init_semaphore(&lcd_qspi_semaphore, 1);
    if (ret != kNoErr) {
        LCD_QSPI_LOGE("lcd qspi semaphore init failed.\r\n");
        return BK_FAIL;
    }

#if (defined(CONFIG_SOC_BK7256XX))
    ret = bk_dma2d_driver_init();
    if (ret != BK_OK) {
        LCD_QSPI_LOGE("dma2d driver init failed!\r\n");
        return BK_FAIL;
    }
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
    bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete_isr);
    if (ret != BK_OK) {
        LCD_QSPI_LOGE("dma2d interrupt register failed!\r\n");
        return BK_FAIL;
    }
    bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 1);
#endif

#elif defined(CONFIG_SOC_BK7236XX)
    ret = bk_dma_driver_init();
    if (ret != BK_OK) {
        LCD_QSPI_LOGE("dma driver init failed!\r\n");
        return BK_FAIL;
    }

    lcd_qspi_dma_id = bk_dma_alloc(DMA_DEV_DTCM);
    if ((lcd_qspi_dma_id < DMA_ID_0) || (lcd_qspi_dma_id >= DMA_ID_MAX)) {
        LCD_QSPI_LOGE("lcd qspi dma malloc failed!\r\n");
        return BK_FAIL;
    }

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
    bk_dma_set_src_sec_attr(lcd_qspi_dma_id, DMA_ATTR_SEC);
    bk_dma_set_dest_sec_attr(lcd_qspi_dma_id, DMA_ATTR_SEC);
    bk_dma_set_dest_burst_len(lcd_qspi_dma_id, BURST_LEN_INC16);
    bk_dma_set_src_burst_len(lcd_qspi_dma_id, BURST_LEN_INC16);
#endif
#endif

    lcd_qspi_quad_write_enable(qspi_id);

    return BK_OK;
}

static bk_err_t lcd_qspi_common_deinit(void)
{
    bk_err_t ret = BK_OK;

    ret = rtos_deinit_semaphore(&lcd_qspi_semaphore);
    if (ret != kNoErr) {
        LCD_QSPI_LOGE("lcd qspi semaphore deinit failed.\r\n");
        return BK_FAIL;
    }

#if (defined(CONFIG_SOC_BK7256XX))
    bk_dma2d_driver_deinit();
#elif defined(CONFIG_SOC_BK7236XX)
    bk_dma_free(DMA_DEV_DTCM, lcd_qspi_dma_id);
    BK_LOG_ON_ERR(bk_dma_driver_deinit());
#endif

    return BK_OK;
}

#if (defined(CONFIG_SOC_BK7256XX))
static void bk_lcd_qspi_dma2d_fill(uint32_t width, uint32_t height, uint32_t color, bool data_reverse)
{
    dma2d_config_t dma2d_config = {0};

    dma2d_config.init.mode           = DMA2D_R2M;              /**< Mode Register to Memory */
    dma2d_config.init.color_mode     = DMA2D_OUTPUT_RGB888;    /**< DMA2D Output color mode is RGB888 */
    dma2d_config.init.output_offset  = 0;                      /**< offset in output */
    dma2d_config.init.red_blue_swap  = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
    dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;    /**< No alpha inversion for the output image */
    dma2d_config.init.out_byte_by_byte_reverse   = data_reverse;
    bk_dma2d_init(&dma2d_config);

    bk_dma2d_transfer_config(&dma2d_config, color, LCD_QSPI_DATA_ADDR, width, height);
    bk_dma2d_start_transfer();
}

static void bk_lcd_qspi_dma2d_memcpy(uint32_t *Psrc, uint32_t xsize, uint32_t ysize, bool data_reverse)
{
    dma2d_config_t dma2d_config = {0};

#if defined(CONFIG_LVGL)
#if (CONFIG_LV_COLOR_DEPTH == 32)
    dma2d_config.init.mode         = DMA2D_M2M_PFC;
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888;
    dma2d_config.init.out_byte_by_byte_reverse   = data_reverse;
    dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB888; /**< Output color mode is RGB888 */
#else
    dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;	 /**< Input color is RGB565 */
    dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565; /**< Output color mode is RGB565 */
#endif
#else
#if (CONFIG_LCD_QSPI_COLOR_DEPTH_BYTE == 2)
    dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;    /**< Input color is RGB565 */
    dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565; /**< Output color mode is RGB565 */
#else
    dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB888;	 /**< Input color is RGB888 */
    dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB888; /**< Output color mode is RGB888 */
#endif
#endif

    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;		/**< No R&B swap for the input image */
    dma2d_config.init.output_offset = 0;                   /**< No offset on output */
    dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
    dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from RGB888 input */
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0;                     /**< No offset in input */
    dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

    bk_dma2d_init(&dma2d_config);
    bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

    bk_dma2d_transfer_config(&dma2d_config, (uint32_t)Psrc, LCD_QSPI_DATA_ADDR, xsize, ysize);
    bk_dma2d_start_transfer();
}
#endif

static bk_err_t lcd_qspi_reg_data_convert(qspi_id_t qspi_id, uint8_t *data, uint32_t data_len)
{
    uint32_t data_buffer[data_len];
    uint8_t i, j, data_tmp1;
    uint8_t data_tmp2[4];

    bk_qspi_read(qspi_id, data_buffer, data_len * 4);
    for (i = 0; i < data_len; i++) {
        for (j = 0; j < 4; j++) {
            data_tmp1 = (data_buffer[i] >> ((j * 8) + 4)) & 0x1;
            data_tmp2[j] = (data_tmp1 << 1) | ((data_buffer[i] >> (j * 8)) & 0x1);
        }

        data[i] = (data_tmp2[0] << 6) | (data_tmp2[1] << 4) | (data_tmp2[2] << 2) | (data_tmp2[3]);
    }

    return BK_OK;
}

bk_err_t bk_lcd_qspi_read_data(qspi_id_t qspi_id, uint8_t *data, const lcd_device_t *device, uint8_t regist_addr, uint8_t data_len)
{
    qspi_hal_set_cmd_d_h(&s_lcd_qspi[qspi_id].hal, 0);
    qspi_hal_set_cmd_d_cfg1(&s_lcd_qspi[qspi_id].hal, 0);
    qspi_hal_set_cmd_d_cfg2(&s_lcd_qspi[qspi_id].hal, 0);

    qspi_hal_set_cmd_d_h(&s_lcd_qspi[qspi_id].hal, (regist_addr << 16 | device->qspi->reg_read_cmd) & 0xFF00FF);
    qspi_hal_set_cmd_d_cfg1(&s_lcd_qspi[qspi_id].hal, 0x300);

    qspi_hal_set_cmd_d_data_line(&s_lcd_qspi[qspi_id].hal, 2);
    qspi_hal_set_cmd_d_data_length(&s_lcd_qspi[qspi_id].hal, data_len * 4);
    qspi_hal_set_cmd_d_dummy_clock(&s_lcd_qspi[qspi_id].hal, device->qspi->reg_read_config.dummy_clk);
    qspi_hal_set_cmd_d_dummy_mode(&s_lcd_qspi[qspi_id].hal, device->qspi->reg_read_config.dummy_mode);

    qspi_hal_cmd_d_start(&s_lcd_qspi[qspi_id].hal);
    qspi_hal_wait_cmd_done(&s_lcd_qspi[qspi_id].hal);

    lcd_qspi_reg_data_convert(qspi_id, data, data_len);

    return BK_OK;
}

bk_err_t bk_lcd_qspi_send_cmd(qspi_id_t qspi_id, uint8_t write_cmd, uint8_t cmd, const uint8_t *data, uint8_t data_len)
{
    qspi_hal_set_cmd_c_l(&s_lcd_qspi[qspi_id].hal, 0);
    qspi_hal_set_cmd_c_h(&s_lcd_qspi[qspi_id].hal, 0);
    qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi[qspi_id].hal, 0);
    qspi_hal_set_cmd_c_cfg2(&s_lcd_qspi[qspi_id].hal, 0);

    if (data_len == 0) {
        qspi_hal_set_cmd_c_h(&s_lcd_qspi[qspi_id].hal, (cmd << 16 | write_cmd) & 0xFF00FF);
    } else if (data_len > 0 && data_len <= 4) {
        uint32_t value = 0;
        for (uint8_t i = 0; i < data_len; i++) {
            value = value | (data[i] << (i * 8));
        }
        qspi_hal_set_cmd_c_l(&s_lcd_qspi[qspi_id].hal, value);
        qspi_hal_set_cmd_c_h(&s_lcd_qspi[qspi_id].hal, (cmd << 16 | write_cmd) & 0xFF00FF);
    } else if (data_len > 4 && data_len != 0xFF) {
        qspi_hal_set_cmd_c_h(&s_lcd_qspi[qspi_id].hal, (cmd << 16 | write_cmd) & 0xFF00FF);
        qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi[qspi_id].hal, 0x300);
        qspi_hal_set_cmd_c_cfg2(&s_lcd_qspi[qspi_id].hal, data_len << 2);
        bk_qspi_write(qspi_id, data, data_len);
        qspi_hal_cmd_c_start(&s_lcd_qspi[qspi_id].hal);
        qspi_hal_wait_cmd_done(&s_lcd_qspi[qspi_id].hal);
        qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi[qspi_id].hal, 0);
        qspi_hal_set_cmd_c_cfg2(&s_lcd_qspi[qspi_id].hal, 0);
        return BK_OK;
    }

    qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi[qspi_id].hal, 0x3 << ((data_len + 4) * 2));
    qspi_hal_cmd_c_start(&s_lcd_qspi[qspi_id].hal);
    qspi_hal_wait_cmd_done(&s_lcd_qspi[qspi_id].hal);

    return BK_OK;
}

bk_err_t bk_lcd_qspi_quad_write_start(qspi_id_t qspi_id, lcd_qspi_write_config_t reg_config, bool addr_is_4wire)
{
    uint32_t cmd_c_h = 0;

    qspi_hal_force_spi_cs_low_enable(&s_lcd_qspi[qspi_id].hal);

    qspi_hal_set_cmd_c_h(&s_lcd_qspi[qspi_id].hal, 0);

    for (uint8_t i = 0; i < reg_config.cmd_len; i++) {
        cmd_c_h = qspi_hal_get_cmd_c_h(&s_lcd_qspi[qspi_id].hal);
        cmd_c_h |= ((reg_config.cmd[i]) << i * 8);
        qspi_hal_set_cmd_c_h(&s_lcd_qspi[qspi_id].hal, cmd_c_h);
    }

    if (addr_is_4wire) {
        qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi[qspi_id].hal, 0x3A8);
    } else {
        qspi_hal_set_cmd_c_cfg1(&s_lcd_qspi[qspi_id].hal, 0x300);
    }
    qspi_hal_cmd_c_start(&s_lcd_qspi[qspi_id].hal);
    qspi_hal_wait_cmd_done(&s_lcd_qspi[qspi_id].hal);

#if (defined(CONFIG_SOC_BK7236XX))
    qspi_hal_io_cpu_mem_select(&s_lcd_qspi[qspi_id].hal, 1);
#endif

    qspi_hal_disable_cmd_sck_enable(&s_lcd_qspi[qspi_id].hal);

    return BK_OK;
}

bk_err_t bk_lcd_qspi_quad_write_stop(qspi_id_t qspi_id)
{
    qspi_hal_disable_cmd_sck_disable(&s_lcd_qspi[qspi_id].hal);
    qspi_hal_force_spi_cs_low_disable(&s_lcd_qspi[qspi_id].hal);

#if (defined(CONFIG_SOC_BK7236XX))
    qspi_hal_io_cpu_mem_select(&s_lcd_qspi[qspi_id].hal, 0);
#endif

    return BK_OK;
}

#if defined(CONFIG_SOC_BK7236XX)
static bk_err_t lcd_qspi_refresh_by_line_lcd_head_config(qspi_id_t qspi_id, const lcd_device_t *device)
{
    uint8_t *cmd = NULL;
    uint32_t head_cmd[4];
    uint8_t i;

    cmd = device->qspi->pixel_write_config.cmd;
    for (i = 0; i < device->qspi->pixel_write_config.cmd_len; i++) {
        if (0 == cmd[i]) {
            head_cmd[i] = 0x0;
            continue;
        } else {
        cmd[i] = ((cmd[i] >> 4) & 0x0F) | ((cmd[i] << 4) & 0xF0);
        cmd[i] = ((cmd[i] >> 2) & 0x33) | ((cmd[i] << 2) & 0xCC);
        cmd[i] = ((cmd[i] >> 1) & 0x55) | ((cmd[i] << 1) & 0xAA);

        head_cmd[i] = ((cmd[i] << 21) & 0x10000000) | ((cmd[i] << 18) & 0x01000000) |
                      ((cmd[i] << 15) & 0x00100000) | ((cmd[i] << 12) & 0x00010000) |
                      ((cmd[i] << 9) & 0x00001000) | ((cmd[i] << 6) & 0x00000100) |
                      ((cmd[i] << 3) & 0x00000010) | (cmd[i] & 0x00000001);
        }
    }

    qspi_hal_enable_soft_reset(&s_lcd_qspi[qspi_id].hal);
    qspi_hal_set_lcd_head_cmd0(&s_lcd_qspi[qspi_id].hal, head_cmd[0]);
    qspi_hal_set_lcd_head_cmd1(&s_lcd_qspi[qspi_id].hal, head_cmd[1]);
    qspi_hal_set_lcd_head_cmd2(&s_lcd_qspi[qspi_id].hal, head_cmd[2]);
    qspi_hal_set_lcd_head_cmd3(&s_lcd_qspi[qspi_id].hal, head_cmd[3]);
    qspi_hal_set_lcd_head_resolution(&s_lcd_qspi[qspi_id].hal, device->qspi->refresh_config.line_len * 2, device->ppi & 0xFFFF);

    qspi_hal_enable_lcd_head_selection_without_ram(&s_lcd_qspi[qspi_id].hal);
    qspi_hal_set_lcd_head_len(&s_lcd_qspi[qspi_id].hal, 0x20);
    qspi_hal_set_lcd_head_dly(&s_lcd_qspi[qspi_id].hal, (device->qspi->clk >> 1) + 6);

    return BK_OK;
}

bk_err_t lcd_qspi_get_dma_repeat_once_len(const lcd_device_t *device)
{
    uint32_t len = 0;
    uint32_t value = 0;
    value = 0x10000 / device->qspi->refresh_config.line_len;
    while ((device->ppi & 0xFFFF) % value) {
        value = value - 1;
    }

    len = value * device->qspi->refresh_config.line_len;

    return len;
}

#endif

bk_err_t bk_lcd_qspi_init(qspi_id_t qspi_id, const lcd_device_t *device)
{
    bk_err_t ret = BK_OK;

    if (device == NULL) {
        LCD_QSPI_LOGE("lcd qspi device not found\r\n");
        return BK_FAIL;
    }

    ret = lcd_qspi_driver_init(qspi_id, device->qspi->clk);
    if (ret != BK_OK) {
        LCD_QSPI_LOGE("lcd qspi driver init failed!\r\n");
        return ret;
    }

    ret = lcd_qspi_common_init(qspi_id);
    if (ret == BK_FAIL) {
        LCD_QSPI_LOGE("lcd qspi common init failed!\r\n");
        return ret;
    }

#if defined(CONFIG_SOC_BK7236XX)
    dma_repeat_once_len = lcd_qspi_get_dma_repeat_once_len(device);
    LCD_QSPI_LOGI("dma_repeat_once_len = %d\r\n", dma_repeat_once_len);
    bk_dma_set_transfer_len(lcd_qspi_dma_id, dma_repeat_once_len);
    if (qspi_id == QSPI_ID_0) {
        dma_set_dst_pause_addr(lcd_qspi_dma_id, LCD_QSPI0_DATA_ADDR + device->qspi->frame_len);
    } else if (qspi_id == QSPI_ID_1) {
        dma_set_dst_pause_addr(lcd_qspi_dma_id, LCD_QSPI1_DATA_ADDR + device->qspi->frame_len);
    } else {
        LCD_QSPI_LOGE("unsupported lcd qspi id\r\n");
        return BK_FAIL;
    }

    qspi_hal_enable_soft_reset(&s_lcd_qspi[qspi_id].hal);
    if (device->qspi->refresh_method == LCD_QSPI_REFRESH_BY_LINE) {
        lcd_qspi_refresh_by_line_lcd_head_config(qspi_id, device);
    }
#endif

    if (device->qspi->init_cmd != NULL) {
        const lcd_qspi_init_cmd_t *init = device->qspi->init_cmd;
        for (uint32_t i = 0; i < device->qspi->device_init_cmd_len; i++) {
            if (init->data_len == 0xff) {
                rtos_delay_milliseconds(init->data[0]);
            } else {
                bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, init->cmd, init->data, init->data_len);
            }
            init++;
        }
    } else {
        LCD_QSPI_LOGE("lcd qspi device don't init\r\n");
        return BK_FAIL;
    }

    return BK_OK;
}

bk_err_t bk_lcd_qspi_deinit(qspi_id_t qspi_id)
{
    bk_err_t ret = BK_OK;

    ret = lcd_qspi_common_deinit();
    if (ret == BK_FAIL) {
        LCD_QSPI_LOGE("lcd qspi common deinit failed!\r\n");
        return ret;
    }

    BK_LOG_ON_ERR(bk_qspi_deinit(qspi_id));

    return BK_OK;
}

bk_err_t bk_lcd_qspi_send_data(qspi_id_t qspi_id, const lcd_device_t *device, uint32_t *data, uint32_t data_len)
{
    bk_err_t ret = BK_OK;

#if (defined(CONFIG_SOC_BK7256XX))
    if (device->qspi->refresh_method == LCD_QSPI_REFRESH_BY_LINE) {
        for (uint16_t i = 0; i < device->qspi->refresh_config.vsw; i++) {
            bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, device->qspi->refresh_config.vsync_cmd, NULL, 0);
            delay_us(40);
        }

        for (uint16_t i = 0; i < device->qspi->refresh_config.hfp; i++) {
            bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, device->qspi->refresh_config.hsync_cmd, NULL, 0);
            delay_us(40);
        }

        for (uint16_t i = 0; i < (device->ppi & 0xFFFF); i++) {
            bk_lcd_qspi_quad_write_start(qspi_id, device->qspi->pixel_write_config, 0);
            if (data_len == 1) {
                bk_lcd_qspi_dma2d_fill(device->ppi >> 16, 1, *data, 0);
            } else {
#if defined(CONFIG_LVGL)
#if (CONFIG_LV_COLOR_DEPTH == 32)
                bk_lcd_qspi_dma2d_memcpy(data + i * (device->ppi >> 16), device->ppi >> 16, 1, 0);
#else
                bk_lcd_qspi_dma2d_memcpy(data + i * (device->qspi->refresh_config.line_len >> 2), device->ppi >> 16, 1, 0);
#endif
#else
                bk_lcd_qspi_dma2d_memcpy(data + i * (device->qspi->refresh_config.line_len >> 2), device->ppi >> 16, 1, 0);
#endif
            }
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
            ret = rtos_get_semaphore(&lcd_qspi_semaphore, 5);
            if (ret != kNoErr) {
                LCD_QSPI_LOGE("ret = %d, lcd qspi get semaphore failed!\r\n", ret);
                return BK_FAIL;
            }
#endif
            bk_lcd_qspi_quad_write_stop(qspi_id);
        }

        for (uint16_t i = 0; i < device->qspi->refresh_config.hbp; i++) {
            bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, device->qspi->refresh_config.hsync_cmd, NULL, 0);
            delay_us(40);
        }
    } else if (device->qspi->refresh_method == LCD_QSPI_REFRESH_BY_FRAME) {
        bk_lcd_qspi_quad_write_start(qspi_id, device->qspi->pixel_write_config, 1);

        if (data_len == 1) {
            bk_lcd_qspi_dma2d_fill(device->ppi >> 16, device->ppi & 0xFFFF, *data, 1);
        } else {
            bk_lcd_qspi_dma2d_memcpy(data, device->ppi >> 16, device->ppi & 0xFFFF, 1);
        }
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
        ret = rtos_get_semaphore(&lcd_qspi_semaphore, 30);
        if (ret != kNoErr) {
            LCD_QSPI_LOGE("ret = %d, lcd qspi get semaphore failed!\r\n", ret);
            return BK_FAIL;
        }
#endif
        bk_lcd_qspi_quad_write_stop(qspi_id);
    } else {
        LCD_QSPI_LOGE("invalid lcd qspi refresh method\r\n");
        return BK_FAIL;
    }
#elif (defined(CONFIG_SOC_BK7236XX))
    if (qspi_id == QSPI_ID_0) {
        bk_dma_stateless_judgment_configuration((void *)LCD_QSPI0_DATA_ADDR, (void *)data, data_len, lcd_qspi_dma_id, (void *)lcd_qspi_dma_finish_isr);
    } else if (qspi_id == QSPI_ID_1) {
        bk_dma_stateless_judgment_configuration((void *)LCD_QSPI1_DATA_ADDR, (void *)data, data_len, lcd_qspi_dma_id, (void *)lcd_qspi_dma_finish_isr);
    } else {
        LCD_QSPI_LOGE("unsupported lcd qspi id\r\n");
        return BK_FAIL;
    }

    dma_set_src_pause_addr(lcd_qspi_dma_id, (uint32_t)data + data_len);

    if (device->qspi->refresh_method == LCD_QSPI_REFRESH_BY_LINE) {
        for (uint16_t i = 0; i < device->qspi->refresh_config.vsw; i++) {
            bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, device->qspi->refresh_config.vsync_cmd, NULL, 0);
            delay_us(40);
        }

        for (uint16_t i = 0; i < device->qspi->refresh_config.hfp; i++) {
            bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, device->qspi->refresh_config.hsync_cmd, NULL, 0);
            delay_us(40);
        }

        qspi_hal_clear_lcd_head(&s_lcd_qspi[qspi_id].hal, 1);
        qspi_hal_clear_lcd_head(&s_lcd_qspi[qspi_id].hal, 0);
        bk_lcd_qspi_quad_write_start(qspi_id, device->qspi->pixel_write_config, 0);
        bk_dma_start(lcd_qspi_dma_id);

        ret = rtos_get_semaphore(&lcd_qspi_semaphore, 3000);
        if (ret != kNoErr) {
            LCD_QSPI_LOGE("ret = %d, lcd qspi get semaphore failed!\r\n", ret);
            return BK_FAIL;
        }
        delay_us(5);
        bk_lcd_qspi_quad_write_stop(qspi_id);

        for (uint16_t i = 0; i < device->qspi->refresh_config.hbp; i++) {
            bk_lcd_qspi_send_cmd(qspi_id, device->qspi->reg_write_cmd, device->qspi->refresh_config.hsync_cmd, NULL, 0);
            delay_us(40);
        }
    } else if (device->qspi->refresh_method == LCD_QSPI_REFRESH_BY_FRAME) {
        bk_lcd_qspi_quad_write_start(qspi_id, device->qspi->pixel_write_config, 0);
        bk_dma_start(lcd_qspi_dma_id);

        ret = rtos_get_semaphore(&lcd_qspi_semaphore, 3000);
        if (ret != kNoErr) {
            LCD_QSPI_LOGE("ret = %d, lcd qspi get semaphore failed!\r\n", ret);
            return BK_FAIL;
        }
        delay_us(5);
        bk_lcd_qspi_quad_write_stop(qspi_id);
    } else {
        LCD_QSPI_LOGE("invalid lcd qspi refresh method\r\n");
        return BK_FAIL;
    }
#endif

    return BK_OK;
}

uint8_t g_lcd_qspi_open_flag = 0;

void bk_lcd_qspi_disp_open(qspi_id_t qspi_id, const lcd_device_t *device)
{
    if(g_lcd_qspi_open_flag) {
        LCD_QSPI_LOGI("[%s] have opened\r\n", __FUNCTION__);
        return;
    }

    bk_lcd_qspi_init(qspi_id, device);
    g_lcd_qspi_open_flag = 1;
    LCD_QSPI_LOGI("[%s] open success, frame_len:%d\r\n", __FUNCTION__, device->qspi->frame_len);
}

void bk_lcd_qspi_disp_close(qspi_id_t qspi_id)
{
    g_lcd_qspi_open_flag = 0;

    bk_lcd_qspi_deinit(qspi_id);
    LCD_QSPI_LOGI("[%s] close success\r\n", __FUNCTION__);
}

