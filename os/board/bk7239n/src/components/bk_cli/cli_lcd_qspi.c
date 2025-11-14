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
#include <os/os.h>
#include <driver/qspi.h>
#include <driver/qspi_types.h>
#include <driver/lcd_qspi.h>
#include <driver/lcd_qspi_types.h>
#include <driver/lcd.h>
#include "cli.h"
#if (defined(CONFIG_FATFS))
#include "ff.h"
#include "diskio.h"
#endif


#define READ_SDCARD_PICTURE_SIZE             1024 * 24
#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

#if (defined(CONFIG_FATFS))
static __attribute__((section(".itcm_sec_code "))) void memcpy_word(uint32_t *dst, uint32_t *src, uint32_t size)
{
    uint32_t i = 0;

    for (i = 0; i < size; i++) {
        write_data((dst+i), get_addr_data(src+i));
    }
}
#endif

void cli_lcd_qspi_display_picture_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    frame_buffer_t *disp_frame = NULL;

    uint32_t total_size = 0;
    const lcd_device_t *lcd_device = NULL;


    if (argc != 4) {
        CLI_LOGI("lcd_qspi_display_picture_test {start|stop} {file name} {device name}\r\n");
        return;
    }

    if (os_strcmp(argv[1], "start") == 0) {

        lcd_device = get_lcd_device_by_name(argv[1]);
        if (lcd_device == NULL) {
            CLI_LOGW("lcd device not found, please input correct device name!\r\n");
            return;
        }

        bk_lcd_qspi_disp_task_start(lcd_device);
        lcd_driver_init(lcd_device);

        disp_frame = frame_buffer_fb_display_malloc_wait(lcd_device->qspi->frame_len);

#if (defined(CONFIG_FATFS))
        char cFileName[50];
        FIL file;
        FRESULT fr;
        FSIZE_t size_64bit = 0;
        uint32 uiTemp = 0;
        uint8_t *sram_addr = NULL;
        uint32_t once_read_len = READ_SDCARD_PICTURE_SIZE;
        uint32_t *paddr  = NULL;
        paddr = (uint32_t *)disp_frame->frame;

        sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, argv[2]);
        sram_addr = os_malloc(once_read_len);
        if (sram_addr == NULL) {
            os_printf("sram mem malloc failed!\r\n");
            return;
        }
        char *ucRdTemp = (char *)sram_addr;

        fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
        if (fr != FR_OK) {
            os_printf("open %s fail.\r\n", cFileName);
            return;
        }
        size_64bit = f_size(&file);
        total_size = (uint32_t)size_64bit;// total byte
        os_printf("read file total_size = %d.\r\n", total_size);

        while(1) {
            fr = f_read(&file, ucRdTemp, once_read_len, &uiTemp);
            if (fr != FR_OK) {
                os_printf("read file fail.\r\n");
                return;
            }

            if (uiTemp == 0)
            {
            	os_printf("read file complete.\r\n");
            	break;
            }

            if(once_read_len != uiTemp) {
                if (uiTemp % 4) {
                    uiTemp = (uiTemp / 4 + 1) * 4;
                }
                memcpy_word(paddr, (uint32_t *)sram_addr, uiTemp/4);
            } else {
                memcpy_word(paddr, (uint32_t *)sram_addr, once_read_len/4);
                paddr += (once_read_len/4);
            }
        }
        os_free(sram_addr);

        fr = f_close(&file);
        if (fr != FR_OK) {
            os_printf("close %s fail!\r\n", cFileName);
            return;
        }
#else
        os_printf("Not support\r\n");
#endif
        lcd_driver_backlight_open();

        lcd_driver_display_frame_sync(disp_frame, BK_FALSE);
    } else if (os_strcmp(argv[1], "stop") == 0) {
        lcd_driver_deinit();
        bk_lcd_qspi_disp_task_start(lcd_device);
    } else {
        CLI_LOGW("lcd_qspi_display_picture_test {start|stop} {file_name} {device name}\r\n");
    }
}

void cli_lcd_qspi_read_reg_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint8_t idx = 0;
    const lcd_device_t *lcd_device = NULL;

    lcd_device = get_lcd_device_by_name(argv[1]);
    if (lcd_device == NULL) {
        CLI_LOGW("lcd device not found, please input correct device name!\r\n");
        return;
    }

    uint8_t register_addr = os_strtoul(argv[2], NULL, 16) & 0xFF;
    uint8_t data_len = os_strtoul(argv[3], NULL, 10) & 0xFF;

    bk_lcd_qspi_disp_task_start(lcd_device);

    bk_lcd_qspi_disp_open(lcd_device);

    uint8_t reg_data[data_len];
    bk_lcd_qspi_read_data(reg_data, lcd_device, register_addr, data_len);
    for (idx = 0; idx < data_len; idx++) {
        CLI_LOGI("reg_data[%d]: 0x%02x\r\n", idx, reg_data[idx]);
    }
}


#define LCD_QSPI_CMD_CNT	(sizeof(s_lcd_qspi_commands) / sizeof(struct cli_command))

static const struct cli_command s_lcd_qspi_commands[] = {
    {"lcd_qspi_display_picture_test", "lcd_qspi_display_picture_test {start|stop} {file name} {device name}", cli_lcd_qspi_display_picture_test_cmd},
    {"lcd_qspi_read_reg_test", "lcd_qspi_read_reg {reg_address} {data_len}", cli_lcd_qspi_read_reg_test_cmd},
};

int cli_lcd_qspi_init(void)
{
    return cli_register_commands(s_lcd_qspi_commands, LCD_QSPI_CMD_CNT);
}

