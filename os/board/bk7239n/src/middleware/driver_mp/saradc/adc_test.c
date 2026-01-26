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

#include "cli.h"
#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"
#include <os/os.h>
#include <os/str.h>
#include "sys_driver.h"
#include <driver/flash_partition.h>
#include "adc_driver.h"
#include "cli_section.h"

static UINT8 s_adc_reading_flag = 0;

static void cli_adc_help(void)
{
    CLI_LOGI("adc_driver init/deinit\n");
    CLI_LOGI("adc_test [channel] [init/deinit/start/stop/dump_statis/get_value/single_step_mode_read\n");
    CLI_LOGI("adc_test [channel] [config] [mode] [src_clk] [adc_clk] [sature_mode] [sampl_rate]\n");
    CLI_LOGI("adc_test [channel] [read/rag_cb] [size] [time_out]\n");
    CLI_LOGI("adc_api_test [set_mode/get_mode/set_clk/set_sample_rate/set_channel/set_filter/\
		set_steady_time/set_sample_cnt/set_saturate\n");
}

static void cli_adc_register_cb(uint32_t param)
{
    CLI_LOGI("param:%d , adc isr cb \r\n", param);
}

static void cli_adc_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc < 2) {
        cli_adc_help();
        return;
    }

    if (os_strcmp(argv[1], "init") == 0) {
        BK_LOG_ON_ERR(bk_adc_driver_init());
        CLI_LOGI("adc driver init success\n");
    } else if (os_strcmp(argv[1], "deinit") == 0) {
        BK_LOG_ON_ERR(bk_adc_driver_deinit());
        CLI_LOGI("adc driver deinit success\n");
    } else {
        cli_adc_help();
        return;
    }
}

static float cli_adc_read_multi_chan(void)
{
    uint16_t value   = 0;
    float cali_value = 0;
    UINT8 adc_chan_buff[12] = {0,1,2,3,4,5,6,10,12,13,14,15};
    UINT32 loop_num = 10;
    adc_config_t config = {0};

    if(s_adc_reading_flag == 1)
    {
        CLI_LOGI("adc_read is running\r\n");
        return 0;
    }

    config.chan = 0;
    config.adc_mode = 3;
    config.src_clk = 1;
    config.clk = 0x30e035;
    config.saturate_mode = 4;
    config.steady_ctrl= 7;
    config.adc_filter = 0;
    if(config.adc_mode == ADC_CONTINUOUS_MODE) {
        config.sample_rate = 0;
    }

    s_adc_reading_flag = 1;

    sys_drv_set_ana_pwd_gadc_buf(1);
    for(UINT8 i = 0; i < (sizeof(adc_chan_buff)/sizeof(UINT8)); i++)
    {
        config.chan = adc_chan_buff[i];

        BK_LOG_ON_ERR(bk_adc_channel_init(&config));

        for(UINT8 j = 0; j < loop_num; j++)
        {
            BK_LOG_ON_ERR(bk_adc_channel_read(config.chan, &value, ADC_READ_SEMAPHORE_WAIT_TIME));

            cali_value = bk_adc_data_calculate(value, config.chan);
            CLI_LOGI("volt:%d mv,chan=%d\n",(uint32_t)(cali_value*1000),config.chan);
        }

        bk_adc_channel_deinit(config.chan);
    }
    sys_drv_set_ana_pwd_gadc_buf(0);
    s_adc_reading_flag = 0;
    return cali_value;
}

static float cli_adc_read_single_chan(UINT8 adc_chan, uint32_t clk)
{
    uint16_t value   = 0;
    float cali_value = 0;

    if(s_adc_reading_flag == 1)
    {
        CLI_LOGI("adc_read is running\r\n");
        return 0;
    }

    s_adc_reading_flag = 1;
    sys_drv_set_ana_pwd_gadc_buf(1);
    adc_config_t config = {0};

    config.chan = adc_chan;
    config.adc_mode = 3;
    config.src_clk = 1;
    config.clk = clk;
    config.saturate_mode = 4;
    config.steady_ctrl= 7;
    config.adc_filter = 0;
    if(config.adc_mode == ADC_CONTINUOUS_MODE) {
        config.sample_rate = 0;
    }

    BK_LOG_ON_ERR(bk_adc_channel_init(&config));
    BK_LOG_ON_ERR(bk_adc_channel_read(adc_chan, &value, ADC_READ_SEMAPHORE_WAIT_TIME));

    os_printf("adc_read_val:%x\r\n", value);

    cali_value = bk_adc_data_calculate(value, adc_chan);

    sys_drv_set_ana_pwd_gadc_buf(0);
    bk_adc_channel_deinit(adc_chan);

    CLI_LOGI("adc:%d, volt value:%d mv\n", value, (uint32_t)(cali_value*1000));
    s_adc_reading_flag = 0;

    return cali_value;
}

#define SARADC_UART_MODE            1
#define SARADC_PSRAM_MODE           2
#define SARADC_SDCARD_MODE          3
#define SARADC_SRAM_MODE            4
#define SARADC_CHANNEL              3
#define SARADC_SAMPLE_RATE          0x0
#define SARADC_READ_TIMEOUT         1000
#define SARADC_CONTINUOUS_MODE      3
#define SARADC_SCLKV_XTAL           1
#define SARADC_STEADY_TIME	    7
#define SARADC_SATURATE_MODE_3      4
#define SARADC_PSRAM_ADDRESS        0x60000000
#define DATA_HANDLE_MODE_PRINT      0x55
#define DATA_HANDLE_MODE_SAVE_FLASH 0xaa
#define SADC_SAMPLE_UNIT_BYTES      2

bk_err_t _cont_mode_cont_rd_data_handler(uint32_t mode, uint16_t *buf, uint32_t data_cnt)
{
    bk_partition_t dump_partition_id = BK_PARTITION_OTA;

    if(NULL == buf) {
        return BK_FAIL;
    }

    if(DATA_HANDLE_MODE_PRINT == mode) {
        uint32_t i;
        uint16_t *tmp_ptr = buf;
        uint8_t log_mode = bk_get_printf_sync();

        bk_set_printf_sync(1);
        for(i = 0; i < data_cnt; i ++) {
            bk_printf("%hd\r\n", tmp_ptr[i]);
        }
        bk_printf("\r\n");
        bk_set_printf_sync(log_mode);
    } else if(DATA_HANDLE_MODE_SAVE_FLASH == mode) {
        bk_err_t ret;

        bk_flash_partition_erase(dump_partition_id, 0, data_cnt + 2047);

        ret = bk_flash_partition_write(dump_partition_id, (uint8_t *)buf, 0, data_cnt);
        if(BK_OK != ret) {
            return ret;
        }
    }

    return BK_OK;
}

static uint16_t *s_rd_data_buf = NULL, *s_sram_buf = NULL;
static uint32_t s_user_rd_cnt  = 1000;
static uint8_t  s_adc_channel_id  = 3;
static uint32_t s_stdout_mode  = 0;

bk_err_t bk_adc_cont_start(adc_config_t *config, uint8_t chan_id, uint16_t *data, uint32_t count);
bk_err_t bk_adc_cont_get_raw(uint8_t chan_id, uint16_t *data, uint32_t count);

static bk_err_t _adc_cont_read(uint8_t chan_id, uint16_t *data, uint32_t count)
{
    bk_adc_cont_get_raw(chan_id, data, count);
    _cont_mode_cont_rd_data_handler(s_stdout_mode, data, count);

    return BK_OK;
}

static void cli_sadc_multi_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t channel_id = 0;
    uint32_t adc_clk_div  = 32;

    if(4 != argc) {
        CLI_LOGI("sadc parameters count are exceptional\r\n");
        return ;
    }

    if(os_strcmp(argv[2], "chan") == 0) {
        channel_id = os_strtoul(argv[3], NULL, 10);
        CLI_LOGI("adc_channel = %d\r\n",channel_id);
    } else {
        CLI_LOGI("get sadc channel exceptionally\r\n");
        return;
    }

    if(os_strcmp(argv[1], "config") == 0) {
        sys_drv_set_ana_pwd_gadc_buf(1);
        adc_config_t config = {0};

        config.chan = channel_id;
        config.adc_mode = ADC_CONTINUOUS_MODE;
        config.src_clk = SARADC_SCLKV_XTAL;
        config.clk = SOC_ADC_XTAL_CLK/2/(adc_clk_div + 1);;
        config.saturate_mode = SARADC_SATURATE_MODE_3;
        config.steady_ctrl= SARADC_STEADY_TIME;
        config.adc_filter = 0;
        config.sample_rate = 0;

        BK_LOG_ON_ERR(bk_adc_channel_init(&config));

        return;
    } else if(os_strcmp(argv[1], "read") == 0) {
        uint32_t i;
        bk_err_t ret = BK_OK;
        uint32_t got_cnt;
        uint16_t *recv_data;
        uint32_t buf_size = 16;

        CLI_LOGI("[1]rxing_data cnt expected_cnt:%d\r\n", buf_size/DEFAULT_ADC_SAMPLE_UNIT_BYTES);
        recv_data = (uint16_t *)os_zalloc(buf_size);
        if(NULL == recv_data) {
            CLI_LOGI("malloc failed buf_size:%d\r\n", buf_size);
            return;
        }

        for(i = 0; i < (buf_size / DEFAULT_ADC_SAMPLE_UNIT_BYTES); i ++) {
            ret = bk_adc_channel_read(channel_id, &recv_data[i], BEKEN_WAIT_FOREVER);
        }

        got_cnt  = buf_size/DEFAULT_ADC_SAMPLE_UNIT_BYTES;
        if(BK_OK == ret) {
            CLI_LOGI("dump_hex rx_data[0x%x] cnt:%d\r\n", recv_data, got_cnt);
            bk_print_dump_hex((uint8_t *)recv_data, got_cnt);
        } else {
            CLI_LOGI("get_raw failed ret:0x%x got_cnt:%d\r\n", ret, got_cnt);
        }
        if(recv_data) {
            os_free(recv_data);
            recv_data = NULL;
        }

        buf_size = 32;
        CLI_LOGI("[2]rxing_data cnt expected_cnt:%d\r\n", buf_size/DEFAULT_ADC_SAMPLE_UNIT_BYTES);
        recv_data = (uint16_t *)os_zalloc(buf_size);
        if(NULL == recv_data) {
            CLI_LOGI("malloc failed buf_size:%d\r\n", buf_size);
            return;
        }

        for(i = 0; i < (buf_size / DEFAULT_ADC_SAMPLE_UNIT_BYTES); i ++) {
            ret = bk_adc_channel_read(channel_id, &recv_data[i], BEKEN_WAIT_FOREVER);
        }

        got_cnt  = buf_size/DEFAULT_ADC_SAMPLE_UNIT_BYTES;
        if(BK_OK == ret) {
            CLI_LOGI("dump_hex rx_data[0x%x] cnt:%d\r\n", recv_data, got_cnt);
            bk_print_dump_hex((uint8_t *)recv_data, got_cnt);
        } else {
            CLI_LOGI("get_raw failed ret:0x%x got_cnt:%d\r\n", ret, got_cnt);
        }
        if(recv_data) {
            os_free(recv_data);
            recv_data = NULL;
        }

        buf_size = 128;
        CLI_LOGI("[3]rxing_data cnt expected_cnt:%d\r\n", buf_size/DEFAULT_ADC_SAMPLE_UNIT_BYTES);
        recv_data = (uint16_t *)os_zalloc(buf_size);
        if(NULL == recv_data) {
            CLI_LOGI("malloc failed buf_size:%d\r\n", buf_size);
            return;
        }

        CLI_LOGI("malloc recv_data:0x%x\r\n", recv_data);
        for(i = 0; i < (buf_size / DEFAULT_ADC_SAMPLE_UNIT_BYTES); i ++) {
            CLI_LOGI("[%d] recv_ptr:0x%x\r\n", i, &recv_data[i]);
            ret = bk_adc_channel_read(channel_id, &recv_data[i], BEKEN_WAIT_FOREVER);
        }

        got_cnt  = buf_size/DEFAULT_ADC_SAMPLE_UNIT_BYTES;
        if(BK_OK == ret) {
            CLI_LOGI("dump_hex rx_data[0x%x] cnt:%d\r\n", recv_data, got_cnt);
            bk_print_dump_hex((uint8_t *)recv_data, got_cnt);
        } else {
            CLI_LOGI("get_raw failed ret:0x%x got_cnt:%d\r\n", ret, got_cnt);
        }
        if(recv_data) {
            os_free(recv_data);
            recv_data = NULL;
        }

        return;
    } else {
        CLI_LOGI("sadc parameters are exceptional\r\n");
        return ;
    }

    return;
}

static void cli_adc_rate_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t clk;
    uint8_t adc_chan;

    if(3 != argc) {
        CLI_LOGI("saradc_rate parameter exceptioanl\r\n");
        return;
    }

    adc_chan = os_strtoul(argv[1], NULL, 10);
    clk = os_strtoul(argv[2], NULL, 10);

    CLI_LOGI("saradc_rate channel:%d clk:0x%x\r\n", adc_chan, clk);

    cli_adc_read_single_chan(adc_chan, clk);
}

static void cli_adc_cont_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t arg_id       = 1;
    uint32_t arg_cnt      = argc;
    uint32_t param_in_cnt = 0;
    uint32_t sample_rate  = 0x0;
    uint8_t  output_mode  = 0;  // ToDo: uart mode and sram mode
    uint32_t adc_clk_div  = 32;

    if((arg_cnt > 1) && (os_strcmp(argv[0 + 1], "read") == 0)) {
        if(0 == s_adc_reading_flag) {
            CLI_LOGI("please saradc start\r\n");
            return;
        }

        _adc_cont_read(s_adc_channel_id, s_rd_data_buf, s_user_rd_cnt);
        return;
    } else if((arg_cnt > 1) && (os_strcmp(argv[0 + 1], "stop") == 0)) {
        bk_adc_channel_deinit(s_adc_channel_id);
        if(NULL != s_sram_buf) {
            os_free(s_sram_buf);
            s_sram_buf = NULL;
        }

        s_adc_reading_flag = 0;
        return;
    } else {
        if(NULL != s_sram_buf) {
            os_free(s_sram_buf);
            s_sram_buf = NULL;
        }

        s_rd_data_buf = NULL;
        s_sram_buf = NULL;
        s_user_rd_cnt   = 1000;
        s_adc_channel_id  = 3;
        output_mode  = 0;  // ToDo: uart mode and sram mode
        sample_rate  = 0x0;
        adc_clk_div  = 32;
        s_stdout_mode  = 0;
    }

    arg_cnt -= 2;
    while (arg_cnt > 1) {
        if (os_strcmp(argv[arg_id+1], "num") == 0) {
            s_user_rd_cnt = os_strtoul(argv[arg_id+2], NULL, 10);
            CLI_LOGI("saradc_num = %d\r\n",s_user_rd_cnt);
            param_in_cnt ++;
        } else if(os_strcmp(argv[arg_id+1], "chan") == 0) {
            s_adc_channel_id = os_strtoul(argv[arg_id+2], NULL, 10);
            CLI_LOGI("adc_channel = %d\r\n",s_adc_channel_id);
            param_in_cnt ++;
        } else if(os_strcmp(argv[arg_id+1], "div") == 0) {
            adc_clk_div = os_strtoul(argv[arg_id+2], NULL, 10);
            CLI_LOGI("div = %d\r\n",adc_clk_div);
            param_in_cnt ++;
        } else if(os_strcmp(argv[arg_id+1], "sam_rate") == 0) {
            sample_rate = os_strtoul(argv[arg_id+2], NULL, 10);
            CLI_LOGI("sample_rate = %d\r\n",sample_rate);
            param_in_cnt ++;
        } else if(os_strcmp(argv[arg_id+1], "mode") == 0) {
            output_mode = os_strtoul(argv[arg_id+2], NULL, 10);
            CLI_LOGI("output_mode = %d\r\n",output_mode);
            param_in_cnt ++;
        } else if(os_strcmp(argv[arg_id+1], "out") == 0) {
            s_stdout_mode = os_strtoul(argv[arg_id+2], NULL, 10);
            CLI_LOGI("output_mode = %d\r\n",s_stdout_mode);
            param_in_cnt ++;
        }

        arg_cnt -= 2;
        arg_id  += 2;
    }

    if(0 == param_in_cnt)
    {
        CLI_LOGI("input partam is exceptional\r\n");
        return;
    }

    if(s_adc_reading_flag == 1)
    {
        CLI_LOGI("adc_cont_read is running\r\n");
        return;
    }

    if(output_mode == SARADC_PSRAM_MODE) {
        #if (defined(CONFIG_PSRAM) && defined(CONFIG_XIP_KERNEL))
        bk_err_t bk_psram_init(void);
        bk_psram_init();
        s_rd_data_buf = (uint16_t *)SARADC_PSRAM_ADDRESS;
        #endif
    } else if(output_mode == SARADC_SDCARD_MODE) {
        //need to do
        ;
    } else if(output_mode == SARADC_UART_MODE) {
        BK_ASSERT(NULL == s_sram_buf);
        s_sram_buf = (uint16_t *)os_zalloc(s_user_rd_cnt * sizeof(s_rd_data_buf[0]));
        if(NULL == s_sram_buf) {
            CLI_LOGI("malloc_buf failed\r\n");
            return;
        }
        s_rd_data_buf = s_sram_buf;
    } else {
        CLI_LOGI("Please configure mode, which decides the malloc space\r\n");
        return;
    }

    if(s_stdout_mode) {
        s_stdout_mode = DATA_HANDLE_MODE_SAVE_FLASH;
    } else {
        s_stdout_mode = DATA_HANDLE_MODE_PRINT;
    }

    s_adc_reading_flag = 1;
    sys_drv_set_ana_pwd_gadc_buf(1);
    adc_config_t config = {0};

    config.chan = s_adc_channel_id;
    config.adc_mode = ADC_CONTINUOUS_MODE;
    config.src_clk = SARADC_SCLKV_XTAL;
    config.clk = SOC_ADC_XTAL_CLK/2/(adc_clk_div + 1);;
    config.saturate_mode = SARADC_SATURATE_MODE_3;
    config.steady_ctrl= SARADC_STEADY_TIME;
    config.adc_filter = 0;
    config.sample_rate = sample_rate;

    BK_LOG_ON_ERR(bk_adc_cont_start(&config, s_adc_channel_id, s_rd_data_buf, s_user_rd_cnt));

    return;
}

static void cli_adc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t adc_chan;

    if (argc < 2) {
        cli_adc_help();
        return;
    }

    adc_chan = os_strtoul(argv[1], NULL, 10);

    if (os_strcmp(argv[2], "config") == 0) {
        adc_config_t config = {0};
        os_memset(&config, 0, sizeof(adc_config_t));

        config.chan = adc_chan;
        config.adc_mode = os_strtoul(argv[3], NULL, 10);
        config.src_clk = os_strtoul(argv[4], NULL, 10);
        config.clk = os_strtoul(argv[5], NULL, 10);
        config.saturate_mode = os_strtoul(argv[6], NULL, 10);
        config.steady_ctrl= 7;
        config.adc_filter = 0;
        if(config.adc_mode == ADC_CONTINUOUS_MODE) {
            config.sample_rate = os_strtoul(argv[7], NULL, 10);
        }

        BK_LOG_ON_ERR(bk_adc_channel_init(&config));
        CLI_LOGI("adc init test:adc_channel:%x, adc_mode:%x,src_clk:%x,adc_clk:%x, saturate_mode:%x, sample_rate:%x",
                 adc_chan, config.adc_mode, config.src_clk, config.clk, config.saturate_mode, config.sample_rate);
    } else if (os_strcmp(argv[2], "deinit") == 0) {
        BK_LOG_ON_ERR(bk_adc_channel_deinit(adc_chan));
        CLI_LOGI("adc deinit test");
    } else if (os_strcmp(argv[2], "start") == 0) {
        CLI_LOGI("start adc test\n");
    } else if (os_strcmp(argv[2], "stop") == 0) {
        CLI_LOGI("adc_stop test: %d\n", adc_chan);
    } else if (os_strcmp(argv[2], "get_value") == 0) {
        uint16_t value = 0;
        float cali_value = 0;

        BK_LOG_ON_ERR(bk_adc_channel_read(adc_chan, &value, ADC_READ_SEMAPHORE_WAIT_TIME));
        cali_value = saradc_calculate(value);
        CLI_LOGI("adc value:%d mv\n", (uint32_t)(cali_value * 1000));
    } else if (os_strcmp(argv[2], "read") == 0) {
        uint32_t sum = 0;
        uint32_t sample_cnt = os_strtoul(argv[3], NULL, 10);
        uint16_t *recv_data = (uint16_t *)os_malloc(sample_cnt * SADC_SAMPLE_UNIT_BYTES);
        if (recv_data == NULL) {
            CLI_LOGE("recv buffer malloc failed\r\n");
            return;
        }

        int time_out = os_strtoul(argv[4], NULL, 10);
        if (time_out < 0) {
            time_out = BEKEN_WAIT_FOREVER;
        }

        BK_LOG_ON_ERR(bk_adc_channel_raw_read(adc_chan, recv_data, sample_cnt, time_out));

        if (!recv_data) {
            CLI_LOGE("adc read failed, recv_data is null \r\n");
        }
        CLI_LOGI("adc: read length :time_out:%d read_size:%d\n",time_out, sample_cnt);

        for (int i = 0; i < sample_cnt; i++) {
            sum = sum + recv_data[i];
            CLI_LOGI("recv_buffer[%d]=%02x, sum =%d\n", i, recv_data[i], sum);
        }
        sum = sum / sample_cnt;
        CLI_LOGI("adc read size:%d, adc_result_from _data_reg:%d\n", sample_cnt, sum);
        os_free(recv_data);
    } else if (os_strcmp(argv[2], "dump_statis") == 0) {
        adc_statis_dump();
        CLI_LOGI("adc dump statis ok\r\n");
    } else if (os_strcmp(argv[2], "single_step_mode_read") == 0) {
        uint16_t data = 0;
        bk_err_t bk_adc_single_read(uint16_t* data);

        bk_adc_single_read(&data);
        CLI_LOGI("adc single step mode: adc value is 0x%x\n", data);
    } else if (os_strcmp(argv[2], "reg_cb") == 0) {
        uint32_t size = os_strtoul(argv[3], NULL, 10);
        BK_LOG_ON_ERR(bk_adc_register_isr_callback(cli_adc_register_cb, size));
        CLI_LOGI("adc isr cb register\r\n");
    }
    else if (0 == os_strcmp(argv[2], "saradc_val_read"))
    {
        uint16_t saradc_val_low = 0;
        uint16_t saradc_val_high = 0;

        saradc_get_calibrate_val(&saradc_val_low, SARADC_CALIBRATE_LOW);
        os_printf("calibrate low value:[%x]\r\n", saradc_val_low);

        saradc_get_calibrate_val(&saradc_val_high, SARADC_CALIBRATE_HIGH);
        os_printf("calibrate high value:[%x]\r\n", saradc_val_high);

    }
    else if(0 == os_strcmp(argv[2], "use_sample_set_saradc_val"))
    {
        uint16_t sample_value= 0;
        uint8_t ret;
        SARADC_MODE saradc_cal_mode;

        adc_config_t config = {0};
        os_memset(&config, 0, sizeof(adc_config_t));

        config.chan = adc_chan;

        config.adc_mode = os_strtoul(argv[3], NULL, 10);
        config.src_clk = os_strtoul(argv[4], NULL, 10);
        config.clk = os_strtoul(argv[5], NULL, 10);
        config.saturate_mode = os_strtoul(argv[6], NULL, 10);
        config.steady_ctrl= 7;
        config.adc_filter = 0;
        if(config.adc_mode == ADC_CONTINUOUS_MODE)
        {
            config.sample_rate = os_strtoul(argv[7], NULL, 10);
        }
        BK_LOG_ON_ERR(bk_adc_channel_init(&config));
        CLI_LOGI("adc init test:adc_channel:%x, adc_mode:%x,src_clk:%x,adc_clk:%x, saturate_mode:%x, sample_rate:%x",
                 adc_chan, config.adc_mode, config.src_clk, config.clk, config.saturate_mode, config.sample_rate);

        if (0 == os_strcmp(argv[8], "low"))
        {
            saradc_cal_mode = SARADC_CALIBRATE_LOW;
        }
        else if (0 == os_strcmp(argv[3], "high"))
        {
            saradc_cal_mode = SARADC_CALIBRATE_HIGH;
        }
        else
        {
            os_printf("invalid parameter\r\n");
            return;
        }

        CLI_LOGI("start adc test\n");

        BK_LOG_ON_ERR(bk_adc_channel_read(adc_chan, &sample_value, ADC_READ_SEMAPHORE_WAIT_TIME));

        ret = saradc_set_calibrate_val(&sample_value, saradc_cal_mode);
        if(ret == SARADC_FAILURE)
        {
            os_printf("saradc_set_calibrate_val fail\r\n");
        }
        os_printf("saradc_set_calibrate_val success\r\n");
        os_printf("mode:[%s] value:[%d]\r\n", (saradc_cal_mode ? "high" : "low"), sample_value);
        //BK_LOG_ON_ERR(bk_adc_stop());
    }
    else if(os_strcmp(argv[2], "single_adc_example") == 0)
    {
        cli_adc_read_single_chan(adc_chan, 0x30e035);
    }
    else if(os_strcmp(argv[2], "multi_adc_example") == 0)
    {
        cli_adc_read_multi_chan();
    } else if (os_strcmp(argv[2], "calib_mode") == 0) {
        #if defined(CONFIG_SARADC_V1P2)
        bk_adc_enter_calib_mode();
        #endif // CONFIG_SARADC_V1P2
    } else
    {
        cli_adc_help();
        return;
    }
}

#define ADC_CMD_CNT (sizeof(s_adc_commands) / sizeof(struct cli_command))

DRV_CLI_CMD_EXPORT
static const struct cli_command s_adc_commands[] = {
    {"sadc_driver", "sadc_driver [init/deinit]", cli_adc_driver_cmd},
    {"sadc", "sadc  [channel] [config/start/stop/dump_statis/single_adc_example/multi_adc_example]", cli_adc_cmd},
    {"sadc_cont", "sadc_cont start/read/stop  [num val] [chan val] [div val] [sam_rate val] [mode val]", cli_adc_cont_cmd},
    {"sadc_multi", "sadc_multi config/read  [chan id]", cli_sadc_multi_cmd},
    {"sadc_rate", "sadc_rate [chan val] [clk val]", cli_adc_rate_cmd},
};

int bk_sadc_register_cli_test_feature(void)
{
    return cli_register_commands(s_adc_commands, ADC_CMD_CNT);
}
// eof

