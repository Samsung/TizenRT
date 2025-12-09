/* Copyright 2020-2025 Beken
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cli.h"
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/i2s.h>
#include <driver/i2s_types.h>
#include <driver/dma.h>
#include "i2s_hal.h"
#include "sys_driver.h"
//#include "i2s_driver.h"
#include <driver/dma.h>

#if CLI_FOR_AUDIO_ATE_TEST
#include <driver/aud_adc_types.h>
#include <driver/aud_adc.h>
#include <driver/aud_dac_types.h>
#include <driver/aud_dac.h>
#endif
#include <tinyara/mm/mm.h>


/* ============================================================================
 * Constants and Macros
 * ============================================================================ */
#define CPU_TEST_I2S_DEBUG          0  /* Test i2s not depend on DMA */
#define I2S_BUF_CNT                 (320)
#define DTM_BUF_CNT                 (640)
#define TWO_BD_BUF_CNT              (640)
#define PCM_BUF_CNT                 (640)


//#define  cli_i2s_mem_malloc(size)  kmm_malloc(size)
//#define  cli_i2s_mem_free(ptr)  kmm_free(ptr)
#define  cli_i2s_mem_zalloc(size)  os_zalloc(size)
#define  cli_i2s_mem_free(ptr)  os_free(ptr)

/* ============================================================================
 * Global Variables
 * ============================================================================ */
static RingBufferContext *ch1_tx_rb;
static RingBufferContext *ch1_rx_rb;
static RingBufferContext *ch2_tx_rb;
static RingBufferContext *ch2_rx_rb;
static RingBufferContext *ch3_tx_rb;
static RingBufferContext *ch3_rx_rb;

static uint8_t *ch1_temp = NULL;
static uint8_t *ch2_temp = NULL;
static uint8_t *ch3_temp = NULL;

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

/* Function declarations */
static void cli_i2s_help(void);

/**
 * @brief Validate basic command arguments
 * @param argc Argument count
 * @param min_args Minimum required arguments
 * @return true if valid, false otherwise
 */
static bool cli_validate_args(int argc, int min_args)
{
        if (argc < min_args) {
                cli_i2s_help();
                return false;
        }
        return true;
}

/**
 * @brief Parse and validate group_id parameter
 * @param argv Command arguments
 * @param group_id Output parameter for group_id
 * @return true if valid, false otherwise
 */
static bool cli_parse_group_id(char **argv, int *group_id)
{
        *group_id = os_strtoul(argv[2], NULL, 10);
        if (*group_id >= SOC_I2S_UNIT_NUM) {
                BK_LOGD(NULL, "i2s group id is invalid\n");
                return false;
        }
        return true;
}

/**
 * @brief Parse and validate sample rate parameter
 * @param argv Command arguments
 * @param samp_rate Output parameter for sample rate
 * @return true if valid, false otherwise
 */
static bool cli_parse_sample_rate(char **argv, int *samp_rate)
{
        *samp_rate = os_strtoul(argv[3], NULL, 10);
        if (*samp_rate < 0 || *samp_rate > 11) {
                BK_LOGD(NULL, "sample rate is invalid (0-11)\n");
                return false;
        }
        return true;
}

/**
 * @brief Validate start command arguments
 * @param argc Argument count
 * @param argv Command arguments
 * @param group_id Output parameter for group_id
 * @param samp_rate Output parameter for sample rate
 * @return true if valid, false otherwise
 */
static bool cli_validate_start_cmd(int argc, char **argv, int *group_id, int *samp_rate)
{
        if (!cli_validate_args(argc, 4)) {
                return false;
        }
        if (!cli_parse_group_id(argv, group_id)) {
                return false;
        }
        if (!cli_parse_sample_rate(argv, samp_rate)) {
                return false;
        }
        return true;
}

/**
 * @brief Validate stop command arguments
 * @param argc Argument count
 * @param argv Command arguments
 * @param group_id Output parameter for group_id
 * @return true if valid, false otherwise
 */
static bool cli_validate_stop_cmd(int argc, char **argv, int *group_id)
{
        if (!cli_validate_args(argc, 3)) {
                return false;
        }
        if (!cli_parse_group_id(argv, group_id)) {
                return false;
        }
        return true;
}

/**
 * @brief Convert sample rate parameter to I2S sample rate enum
 * @param samp_rate Sample rate parameter (0-11)
 * @return I2S sample rate enum value
 */
static i2s_samp_rate_t cli_i2s_get_samp_rate(int samp_rate)
{
        switch (samp_rate) {
        case 0:         /* 8k */
                return I2S_SAMP_RATE_8000;
        case 1:         /* 12k */
                return I2S_SAMP_RATE_12000;
        case 2:         /* 16k */
                return I2S_SAMP_RATE_16000;
        case 3:         /* 24k */
                return I2S_SAMP_RATE_24000;
        case 4:         /* 32k */
                return I2S_SAMP_RATE_32000;
        case 5:         /* 48k */
                return I2S_SAMP_RATE_48000;
        case 6:         /* 96k */
                return I2S_SAMP_RATE_96000;
        case 7:         /* 8.0182k */
                return I2S_SAMP_RATE_8018;
        case 8:         /* 11.025k */
                return I2S_SAMP_RATE_11025;
        case 9:         /* 22.05k */
                return I2S_SAMP_RATE_22050;
        case 10:        /* 44.1k */
                return I2S_SAMP_RATE_44100;
        case 11:        /* 88.2k */
                return I2S_SAMP_RATE_88200;
        default:
                return I2S_SAMP_RATE_8000;
        }
}

static void cli_i2s_help(void)
{
        /* I2S Test Commands */
        os_printf("i2s_master_test start group_id samp_rate \r\n");
        os_printf("i2s_master_test stop group_id \r\n");
        os_printf("i2s_slave_test start group_id samp_rate \r\n");
        os_printf("i2s_slave_test stop group_id \r\n");
        
        /* PCM Test Commands */
        os_printf("pcm_master_test start group_id samp_rate \r\n");
        os_printf("pcm_master_test stop group_id \r\n");
        os_printf("pcm_slave_test start group_id samp_rate \r\n");
        os_printf("pcm_slave_test stop group_id \r\n");
        
        /* TDM Test Commands */
        os_printf("tdm_master_test start group_id samp_rate \r\n");
        os_printf("tdm_master_test stop group_id \r\n");
        os_printf("tdm_slave_test start group_id samp_rate \r\n");
        os_printf("tdm_slave_test stop group_id \r\n");
        
        /* 2B+D Test Commands */
        os_printf("2bd_master_test start group_id samp_rate \r\n");
        os_printf("2bd_master_test stop group_id \r\n");
        os_printf("2bd_slave_test start group_id samp_rate \r\n");
        os_printf("2bd_slave_test stop group_id \r\n");
        
        /* Sample Rate Options */
        os_printf("Sample rate options: \r\n");
        os_printf("  0: 8K, 1: 12K, 2: 16K, 3: 24K, 4: 32K, 5: 48K, 6: 96K \r\n");
        os_printf("  7: 8.0182K, 8: 11.025K, 9: 22.050K, 10: 44.1K, 11: 88.2K \r\n");
#if CPU_TEST_I2S_DEBUG
        os_printf("i2s_cpu_master_test {group_id}\r\n");
#endif
}

static void cli_iis_init_buf(void *buf, uint32_t len)
{
        uint8_t *tmp = (uint8_t *)buf;
        uint32_t i;

        for(i = 0; i < len; i ++) {
                tmp[i] = (i & 0xff);
        }
}

static int ch1_tx_data_handle_cb(uint32_t size)
{
        ring_buffer_write(ch1_tx_rb, ch1_temp, size);

        return size;
}

static int ch1_rx_data_handle_cb(uint32_t size)
{
        ring_buffer_read(ch1_rx_rb, ch1_temp, size);
        os_printf("rx ch1_temp[0]: 0x%2x, ch1_temp[1]: 0x%2x\n", ch1_temp[0], ch1_temp[1]);

        return size;
}

static int ch2_tx_data_handle_cb(uint32_t size)
{
        ring_buffer_write(ch2_tx_rb, ch2_temp, size);

        return size;
}

static int ch2_rx_data_handle_cb(uint32_t size)
{
        ring_buffer_read(ch2_rx_rb, ch2_temp, size);
        os_printf("rx ch2_temp[0]: 0x%2x \n", ch2_temp[0]);

        return size;
}

static int ch3_tx_data_handle_cb(uint32_t size)
{
        ring_buffer_write(ch3_tx_rb, ch3_temp, size);

        return size;
}

static int ch3_rx_data_handle_cb(uint32_t size)
{
        ring_buffer_read(ch3_rx_rb, ch3_temp, size);
        BK_LOGD(NULL, "rx ch3_temp[0]: 0x%2x \n", ch3_temp[0]);

        return size;
}

/* ============================================================================
 * Audio Configuration Functions (for ATE Test)
 * ============================================================================ */
#if CLI_FOR_AUDIO_ATE_TEST
static void cli_audio_adc_config(uint32_t sample_rate, uint32_t adc_mode)
{
        aud_adc_config_t adc_config = DEFAULT_AUD_ADC_CONFIG();

        adc_config.adc_chl = AUD_ADC_CHL_LR;
        adc_config.clk_src = AUD_CLK_XTAL;
        adc_config.adc_mode = adc_mode;
        switch (sample_rate) {
        case 1:
                adc_config.samp_rate = 8000;
                break;

        case 2:
                adc_config.samp_rate = 11025;
                break;

        case 3:
                adc_config.samp_rate = 12000;
                break;

        case 4:
                adc_config.samp_rate = 16000;
                break;

        case 5:
                adc_config.samp_rate = 22050;
                break;

        case 6:
                adc_config.samp_rate = 24000;
                break;

        case 7:
                adc_config.samp_rate = 32000;
                break;

        case 8:
                adc_config.samp_rate = 44100;
                break;

        case 9:
                adc_config.samp_rate = 48000;
                break;

        default:
                break;
        }

        /* audio process test */
        sys_drv_aud_mic1_gain_set(0);
        sys_drv_aud_mic2_gain_set(0);

        bk_aud_adc_init(&adc_config);

        /* disable audio interrupt when loop test */
        sys_drv_aud_int_en(0);
        bk_aud_adc_disable_int();

        /* start adc */
        bk_aud_adc_start();
}

static void cli_audio_adc_deconfig(void)
{
        bk_aud_adc_stop();
        bk_aud_adc_deinit();
}

static void cli_audio_dac_config(uint32_t sample_rate, uint32_t dac_mode)
{
        aud_dac_config_t dac_config = DEFAULT_AUD_DAC_CONFIG();

        dac_config.dac_chl = AUD_DAC_CHL_LR;
        dac_config.clk_src = AUD_CLK_XTAL;
        dac_config.work_mode = dac_mode;
        switch (sample_rate) {
        case 1:
                dac_config.samp_rate = 8000;
                break;

        case 2:
                dac_config.samp_rate = 11025;
                break;

        case 3:
                dac_config.samp_rate = 12000;
                break;

        case 4:
                dac_config.samp_rate = 16000;
                break;

        case 5:
                dac_config.samp_rate = 22050;
                break;

        case 6:
                dac_config.samp_rate = 24000;
                break;

        case 7:
                dac_config.samp_rate = 32000;
                break;

        case 8:
                dac_config.samp_rate = 44100;
                break;

        case 9:
                dac_config.samp_rate = 48000;
                break;

        default:
                break;
        }

        /* audio process test */
        sys_drv_aud_dacg_set(0);

        bk_aud_dac_init(&dac_config);

        /* disable audio interrupt when loop test */
        sys_drv_aud_int_en(0);
        bk_aud_adc_disable_int();

        /* start dac */
        bk_aud_dac_start();
}

static void cli_audio_dac_deconfig(void)
{
        bk_aud_dac_stop();
        bk_aud_dac_deinit();
}

static void cli_i2s_config(uint32_t sample_rate, uint8_t aud_mode)
{
        i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
        i2s_samp_rate_t samp_rate = I2S_SAMP_RATE_8000;

        switch (sample_rate) {
        case 1:         /* 8k */
                samp_rate = I2S_SAMP_RATE_8000;
                break;

        case 2:         /* 11.025k */
                samp_rate = I2S_SAMP_RATE_11025;
                break;

        case 3:         /* 12k */
                samp_rate = I2S_SAMP_RATE_12000;
                break;

        case 4:         /* 16k */
                samp_rate = I2S_SAMP_RATE_16000;
                break;

        case 5:         /* 22.05k */
                samp_rate = I2S_SAMP_RATE_22050;
                break;

        case 6:         /* 24k */
                samp_rate = I2S_SAMP_RATE_24000;
                break;

        case 7:         /* 32k */
                samp_rate = I2S_SAMP_RATE_32000;
                break;

        case 8:         /* 44.1k */
                samp_rate = I2S_SAMP_RATE_44100;
                break;

        case 9:         /* 48k */
                samp_rate = I2S_SAMP_RATE_48000;
                break;

        default:
                break;
        }

        i2s_config.samp_rate = I2S_SAMP_RATE_8000;
        i2s_config.samp_rate = samp_rate;
        i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;

        /* init i2s */
        bk_i2s_driver_init();

        /* config i2s */
        if (aud_mode == 0) { /* for adc */
                i2s_config.role = I2S_ROLE_SLAVE; /* I2S_ROLE_MASTER; */
                bk_i2s_init(I2S_GPIO_GROUP_1, &i2s_config);
        } else { /* for dac */
                i2s_config.role = I2S_ROLE_MASTER; /* I2S_ROLE_SLAVE; */
                bk_i2s_init(I2S_GPIO_GROUP_2, &i2s_config);
        }

        bk_i2s_enable(I2S_ENABLE);
}

static void cli_i2s_deconfig(void)
{
        bk_i2s_enable(I2S_DISABLE);
        bk_i2s_deinit();
        bk_i2s_driver_deinit();
}

static dma_id_t cli_aud_i2s_dma_config(uint32_t state)
{
        bk_err_t ret = BK_OK;
        dma_config_t dma_config = {0};
        uint32_t i2s_data_addr;
        uint32_t aud_adc_data_addr;
        uint32_t aud_dac_data_addr;

        /* init dma driver */
        ret = bk_dma_driver_init();
        if (ret != BK_OK) {
                return DMA_ID_MAX;
        }

        /* allocate free DMA channel */
        dma_id_t dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
        if ((dma_id < DMA_ID_0) || (dma_id >= DMA_ID_MAX)) {
                return DMA_ID_MAX;
        }

        dma_config.mode = DMA_WORK_MODE_REPEAT;
        dma_config.chan_prio = 1;
        dma_config.src.width = DMA_DATA_WIDTH_32BITS;
        dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
        dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
        dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
        dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
        dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;

        /* get audio adc and dac fifo address */
        bk_aud_adc_get_fifo_addr(&aud_adc_data_addr);
        bk_aud_dac_get_fifo_addr(&aud_dac_data_addr);

        /* get i2s address */
        bk_i2s_get_data_addr(I2S_CHANNEL_1, &i2s_data_addr);

        if (state) {
                /* audio adc to i2s by dma */
                dma_config.src.dev = DMA_DEV_AUDIO_RX;
                dma_config.dst.dev = DMA_DEV_I2S1;
                dma_config.dst.start_addr = i2s_data_addr;
                dma_config.dst.end_addr = i2s_data_addr + 4;
                dma_config.src.start_addr = aud_adc_data_addr;
                dma_config.src.end_addr = aud_adc_data_addr + 4;
        } else {
                /* audio i2s to dac by dma */
                dma_config.src.dev = DMA_DEV_I2S2_RX;
                dma_config.dst.dev = DMA_DEV_AUDIO;
                dma_config.src.start_addr = i2s_data_addr;
                dma_config.src.end_addr = i2s_data_addr + 4;
                dma_config.dst.start_addr = aud_dac_data_addr;
                dma_config.dst.end_addr = aud_dac_data_addr + 4;
        }
        dma_config.trans_type = DMA_TRANS_DEFAULT;
        /* init dma channel */
        ret = bk_dma_init(dma_id, &dma_config);
        if (ret != BK_OK) {
                return DMA_ID_MAX;
        }

        /* set dma transfer length */
        bk_dma_set_transfer_len(dma_id, 4);

#if (CONFIG_SPE)
        bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC);
        bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC);
#endif

        return dma_id;
}

static void cli_aud_i2s_dma_deconfig(void)
{
        for (uint8_t i = 0; i < DMA_ID_MAX; i++) {
                if (bk_dma_user(i) == DMA_DEV_AUDIO) {
                        bk_dma_stop(i);
                        bk_dma_deinit(i);
                        bk_dma_free(DMA_DEV_AUDIO, i);
                }
        }

        bk_dma_driver_deinit();
}

static void cli_aud_i2s_dma_ctrl(uint32_t state, dma_id_t adc_dma_id, dma_id_t dac_dma_id)
{
        uint32_t temp_data = 0xF0F0F0F0;

        if (state) {
                /* start dma */
                if (adc_dma_id != DMA_ID_MAX) {
                        bk_dma_start(adc_dma_id);
                }

                if (dac_dma_id != DMA_ID_MAX) {
                        bk_dma_start(dac_dma_id);
                }

                bk_i2s_write_data(0, &temp_data, 1);
                bk_i2s_write_data(1, &temp_data, 1);
                bk_i2s_write_data(2, &temp_data, 1);
                /* bk_i2s_write_data(3, &temp_data, 1); */
        } else {
                /* stop dma */
                if (adc_dma_id != DMA_ID_MAX) {
                        bk_dma_stop(adc_dma_id);
                }

                if (dac_dma_id != DMA_ID_MAX) {
                        bk_dma_stop(dac_dma_id);
                }
        }
}
#endif

/* ============================================================================
 * I2S Test Command Functions
 * ============================================================================ */
void cli_i2s_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
        bk_err_t ret = BK_OK;
        uint32_t size = 0;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "i2s master test start \r\n");

                /* Memory allocation */
                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                if (ch1_temp == NULL) {
                        BK_LOGD(NULL, "Failed to allocate memory for ch1_temp\n");
                        return;
                }
                cli_iis_init_buf(ch1_temp, I2S_BUF_CNT);

                /* I2S driver initialization */
                bk_i2s_driver_init();
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                /* Channel initialization */
                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, I2S_BUF_CNT * 2, ch1_tx_data_handle_cb, &ch1_tx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                /* Data preparation and transmission */
                uint8_t *temp_data = (uint8_t *)cli_i2s_mem_zalloc(I2S_BUF_CNT * 2);
                if (temp_data == NULL) {
                        BK_LOGD(NULL, "Failed to allocate memory for temp_data\n");
                        cli_i2s_mem_free(ch1_temp);
                        ch1_temp = NULL;
                        return;
                }
                size = ring_buffer_write(ch1_tx_rb, temp_data, I2S_BUF_CNT * 2);
                BK_LOGD(NULL, "ring_buffer_write, size: %d \n", size);
                cli_i2s_mem_free(temp_data);
                temp_data = NULL;
                
                /* Start I2S transmission */
                bk_i2s_start();
                BK_LOGD(NULL, "i2s master test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "i2s master test stop \r\n");
                
                /* Stop I2S operations */
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                /* Cleanup memory */
                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                
                BK_LOGD(NULL, "i2s master test stop successful \r\n");
        }
#if CLI_FOR_AUDIO_ATE_TEST
        /*
          Hardware connect:
          P44 -> P40
          P45 -> P41
          P46 -> P43
          P47 -> P42
        */
        else if(os_strcmp(argv[1], "adc") == 0) { /* mic -> I2S1 */
                dma_id_t cli_adc_dma_id = DMA_ID_MAX;
                uint32_t mic1_config;

                cli_audio_adc_config(1, 0);
                mic1_config = sys_ll_get_ana_reg19_value();
                BK_LOGD(NULL, "mic1_config: %x\n\r", mic1_config);
                sys_hal_set_ana_reg27_value(mic1_config);
                cli_i2s_config(1, 0);
                cli_adc_dma_id = cli_aud_i2s_dma_config(1);
                bk_i2s_enable(I2S_ENABLE);
                cli_aud_i2s_dma_ctrl(1, cli_adc_dma_id, cli_adc_dma_id);
        }
        else if(os_strcmp(argv[1], "dac") == 0) { /* I2S2 -> DAC */
                dma_id_t cli_dac_dma_id = DMA_ID_MAX;
                dma_id_t cli_adc_dma_id = DMA_ID_MAX;
                cli_audio_dac_config(1, 0);
                cli_i2s_config(1, 1);
                bk_i2s_enable(I2S_ENABLE);
                cli_dac_dma_id = cli_aud_i2s_dma_config(0);
                cli_aud_i2s_dma_ctrl(1, cli_adc_dma_id, cli_dac_dma_id);

                BK_LOGD(NULL, "init i2s driver and config successful \r\n");
        }
        else if (os_strcmp(argv[1], "mic_conf") == 0) {
                uint32_t mic1_config = 0;
                mic1_config = sys_ll_get_ana_reg27_value();
                BK_LOGD(NULL, "mic1_config: %x\n\r", mic1_config);
        }
#endif
        else {
                cli_i2s_help();
                return;
        }
}

void cli_i2s_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
        bk_err_t ret = BK_OK;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "i2s slave test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                if (ch1_temp == NULL) {
                        BK_LOGD(NULL, "Failed to allocate memory for ch1_temp\n");
                        return;
                }

                bk_i2s_driver_init();

                /* init i2s configure */
                i2s_config.role = I2S_ROLE_SLAVE;
                /* Set sample rate based on input parameter */
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                bk_i2s_start();

                BK_LOGD(NULL, "i2s slave test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "i2s slave test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                BK_LOGD(NULL, "i2s slave test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

void cli_pcm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_PCM_CONFIG();
        bk_err_t ret = BK_OK;
        uint32_t size = 0;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "pcm master test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                cli_iis_init_buf(ch1_temp, I2S_BUF_CNT);

                /* init i2s driver */
                bk_i2s_driver_init();

                /* init i2s configure */
                /* Set sample rate based on input parameter */
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, PCM_BUF_CNT, ch1_tx_data_handle_cb, &ch1_tx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                uint8_t *temp_data = (uint8_t *)cli_i2s_mem_zalloc(PCM_BUF_CNT);
                cli_iis_init_buf(temp_data, PCM_BUF_CNT);

                size = ring_buffer_write(ch1_tx_rb, temp_data, PCM_BUF_CNT);
                BK_LOGD(NULL, "ring_buffer_write, size: %d \n", size);
                cli_i2s_mem_free(temp_data);
                bk_i2s_start();

                BK_LOGD(NULL, "pcm master test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "pcm master test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                BK_LOGD(NULL, "pcm master test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

void cli_pcm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_PCM_CONFIG();
        bk_err_t ret = BK_OK;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "pcm slave test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);

                /* init i2s driver */
                bk_i2s_driver_init();

                /* init i2s configure */
                /* Set sample rate based on input parameter */
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                i2s_config.role = I2S_ROLE_SLAVE;
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }
                bk_i2s_start();
                BK_LOGD(NULL, "pcm slave test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "pcm slave test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                BK_LOGD(NULL, "pcm slave test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

void cli_tdm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_TDM_CONFIG();
        bk_err_t ret = BK_OK;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "tdm master test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                cli_iis_init_buf(ch1_temp, I2S_BUF_CNT);
                ch2_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                cli_iis_init_buf(ch2_temp, I2S_BUF_CNT);
                ch3_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                cli_iis_init_buf(ch3_temp, I2S_BUF_CNT);

                /* init i2s driver */
                bk_i2s_driver_init();

                /* init i2s configure */
                /* Set sample rate based on input parameter */
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, DTM_BUF_CNT, ch1_tx_data_handle_cb, &ch1_tx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                ret = bk_i2s_chl_init(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX, DTM_BUF_CNT, ch2_tx_data_handle_cb, &ch2_tx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                ret = bk_i2s_chl_init(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX, DTM_BUF_CNT, ch3_tx_data_handle_cb, &ch3_tx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                uint8_t *temp_data = (uint8_t *)cli_i2s_mem_zalloc(DTM_BUF_CNT);
                cli_iis_init_buf(temp_data, DTM_BUF_CNT);
                ring_buffer_write(ch1_tx_rb, temp_data, DTM_BUF_CNT);
                cli_iis_init_buf(temp_data, DTM_BUF_CNT);
                ring_buffer_write(ch2_tx_rb, temp_data, DTM_BUF_CNT);
                cli_iis_init_buf(temp_data, DTM_BUF_CNT);
                ring_buffer_write(ch3_tx_rb, temp_data, DTM_BUF_CNT);
                cli_i2s_mem_free(temp_data);
                BK_LOGD(NULL, "write data ok \r\n");
                bk_i2s_start();

                BK_LOGD(NULL, "pcm master test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                BK_LOGD(NULL, "pcm master test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
                bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX);
                bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                cli_i2s_mem_free(ch2_temp);
                ch2_temp = NULL;
                cli_i2s_mem_free(ch3_temp);
                ch3_temp = NULL;
                BK_LOGD(NULL, "pcm master test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

void cli_tdm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_TDM_CONFIG();
        bk_err_t ret = BK_OK;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "tdm slave test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                ch2_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                ch3_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);

                bk_i2s_driver_init();

                i2s_config.role = I2S_ROLE_SLAVE;
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                ret = bk_i2s_chl_init(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX, 640, ch2_rx_data_handle_cb, &ch2_rx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                ret = bk_i2s_chl_init(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX, 640, ch3_rx_data_handle_cb, &ch3_rx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                bk_i2s_start();
                BK_LOGD(NULL, "tdm slave test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "tdm slave test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
                bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX);
                bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                cli_i2s_mem_free(ch2_temp);
                ch2_temp = NULL;
                cli_i2s_mem_free(ch3_temp);
                ch3_temp = NULL;
                BK_LOGD(NULL, "pcm slave test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

void cli_2bd_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_2BD_CONFIG();
        bk_err_t ret = BK_OK;
        uint32_t size = 0;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "2B+D master test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);
                cli_iis_init_buf(ch1_temp, I2S_BUF_CNT);

                /* init i2s driver */
                bk_i2s_driver_init();

                /* init i2s configure */
                /* Set sample rate based on input parameter */
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, TWO_BD_BUF_CNT, ch1_tx_data_handle_cb, &ch1_tx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                uint8_t *temp_data = (uint8_t *)cli_i2s_mem_zalloc(TWO_BD_BUF_CNT);
                cli_iis_init_buf(temp_data, TWO_BD_BUF_CNT);
                size = ring_buffer_write(ch1_tx_rb, temp_data, TWO_BD_BUF_CNT);
                BK_LOGD(NULL, "ring_buffer_write, size: %d \n", size);
                cli_i2s_mem_free(temp_data);
                bk_i2s_start();

                BK_LOGD(NULL, "2B+D master test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "2B+D master test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                BK_LOGD(NULL, "2B+D master test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

void cli_2bd_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_2BD_CONFIG();
        bk_err_t ret = BK_OK;
        int group_id, samp_rate;

        if (argc < 3) {
                cli_i2s_help();
                return;
        }

        if (os_strcmp(argv[1], "start") == 0) {
                /* Parameter validation */
                if (!cli_validate_start_cmd(argc, argv, &group_id, &samp_rate)) {
                        return;
                }
                
                BK_LOGD(NULL, "2B+D slave test start \r\n");

                ch1_temp = cli_i2s_mem_zalloc(I2S_BUF_CNT);

                /* init i2s driver */
                bk_i2s_driver_init();

                /* init i2s configure */
                i2s_config.role = I2S_ROLE_SLAVE;
                i2s_config.samp_rate = cli_i2s_get_samp_rate(samp_rate);
                bk_i2s_init(group_id, &i2s_config);
                BK_LOGD(NULL, "init i2s driver and config successful \r\n");

                ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
                if (ret != BK_OK) {
                        BK_LOGD(NULL, "bk_i2s_chl_init fail \n");
                        return;
                }

                bk_i2s_start();

                BK_LOGD(NULL, "2B+D slave test complete \r\n");
        } else if (os_strcmp(argv[1], "stop") == 0) {
                /* Parameter validation */
                if (!cli_validate_stop_cmd(argc, argv, &group_id)) {
                        return;
                }
                
                BK_LOGD(NULL, "2B+D slave test stop \r\n");
                bk_i2s_stop();
                bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
                bk_i2s_deinit();
                bk_i2s_driver_deinit();

                cli_i2s_mem_free(ch1_temp);
                ch1_temp = NULL;
                BK_LOGD(NULL, "2B+D slave test stop successful \r\n");
        } else {
                cli_i2s_help();
                return;
        }
}

#if CPU_TEST_I2S_DEBUG
void cli_i2s_cpu_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
        i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
        uint32_t temp_data = 0xF0F0F0F0;
        uint32_t data = 0xF1F1F1F1;

        if (argc != 2) {
                cli_i2s_help();
                return;
        }

        BK_LOGD(NULL, "i2s cpu master test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM) {
                BK_LOGD(NULL, "i2s group id is invalid\n");
                return;
        }

        /* init i2s driver */
        bk_i2s_driver_init();

        /* init i2s configure */
        i2s_config.samp_rate = I2S_SAMP_RATE_88200;
        bk_i2s_init(group_id, &i2s_config);
        BK_LOGD(NULL, "init i2s driver and config successful \r\n");

        bk_i2s_enable(I2S_ENABLE);
        bk_i2s_write_data(I2S_CHANNEL_1, &temp_data, 1);
        bk_i2s_write_data(I2S_CHANNEL_2, &temp_data, 1);
        bk_i2s_write_data(I2S_CHANNEL_3, &temp_data, 1);

        uint32_t write_flag = 0;
        while (1) {
                bk_i2s_get_write_ready(&write_flag);
                if (write_flag && (1<<5)) {
                        break;
                }
        }

        while (1) {
                bk_i2s_write_data(I2S_CHANNEL_1, &data, 1);
        }

        BK_LOGD(NULL, "i2s cpu master test complete \r\n");
}
#endif

/* ============================================================================
 * Command Registration
 * ============================================================================ */
#define I2S_CMD_CNT (sizeof(s_i2s_commands) / sizeof(struct cli_command))
DRV_CLI_CMD_EXPORT static const struct cli_command s_i2s_commands[] = {
        {"i2s_master_test", "i2s_master_test start group_id samp_rate / i2s_master_test stop group_id", cli_i2s_master_test_cmd},
        {"i2s_slave_test", "i2s_slave_test start group_id samp_rate / i2s_slave_test stop group_id", cli_i2s_slave_test_cmd},
        {"pcm_master_test", "pcm_master_test start group_id samp_rate / pcm_master_test stop group_id", cli_pcm_master_test_cmd},
        {"pcm_slave_test", "pcm_slave_test start group_id samp_rate / pcm_slave_test stop group_id", cli_pcm_slave_test_cmd},
        {"tdm_master_test", "tdm_master_test start group_id samp_rate / tdm_master_test stop group_id", cli_tdm_master_test_cmd},
        {"tdm_slave_test", "tdm_slave_test start group_id samp_rate / tdm_slave_test stop group_id", cli_tdm_slave_test_cmd},
        {"2bd_master_test", "2bd_master_test start group_id samp_rate / 2bd_master_test stop group_id", cli_2bd_master_test_cmd},
        {"2bd_slave_test", "2bd_slave_test start group_id samp_rate / 2bd_slave_test stop group_id", cli_2bd_slave_test_cmd},
#if CPU_TEST_I2S_DEBUG
        {"i2s_cpu_master_test", "i2s_cpu_master_test {group_id}", cli_i2s_cpu_master_test_cmd},
#endif
};

int cli_i2s_init(void)
{
        return cli_register_commands(s_i2s_commands, I2S_CMD_CNT);
}

