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
#include "soc/soc.h"
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


//#define  cli_i2s_mem_malloc(size)  kmm_malloc(size)
//#define  cli_i2s_mem_free(ptr)  kmm_free(ptr)
#define  cli_i2s_mem_malloc(size)  os_malloc(size)
#define  cli_i2s_mem_free(ptr)  os_free(ptr)

static RingBufferContext *ch1_tx_rb;
static RingBufferContext *ch1_rx_rb;
static RingBufferContext *ch2_tx_rb;
static RingBufferContext *ch2_rx_rb;
static RingBufferContext *ch3_tx_rb;
static RingBufferContext *ch3_rx_rb;

static uint8_t *ch1_temp = NULL;
static uint8_t *ch2_temp = NULL;
static uint8_t *ch3_temp = NULL;


static void cli_i2s_help(void)
{
	os_printf("i2s_master_test {start|stop group_id} \r\n");
	os_printf("i2s_slave_test {start|stop group_id} \r\n");
	os_printf("pcm_master_test {start|stop group_id} \r\n");
	os_printf("pcm_slave_test {start|stop group_id} \r\n");
	os_printf("dtm_master_test {start|stop group_id} \r\n");
	os_printf("dtm_slave_test {start|stop group_id} \r\n");
	os_printf("2bd_master_test {start|stop group_id} \r\n");
	os_printf("2bd_slave_test {start|stop group_id} \r\n");
#ifdef CPU_TEST_I2S_DEBUG
	os_printf("i2s_cpu_master_test {group_id}\r\n");
#endif
}

static int ch1_tx_data_handle_cb(uint32_t size)
{
	//os_printf("%s, size: %d \n", __func__, size);
	ring_buffer_write(ch1_tx_rb, ch1_temp, size);

	return size;
}

static int ch1_rx_data_handle_cb(uint32_t size)
{
	//os_printf("%s, size: %d \n", __func__, size);
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
	os_printf("rx ch3_temp[0]: 0x%2x \n", ch3_temp[0]);

	return size;
}

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

	//disable audio interrupt when loop test
	sys_drv_aud_int_en(0);
	bk_aud_adc_disable_int();

	//start adc
	bk_aud_adc_start();
}

static void cli_audio_adc_deconfig(void)
{
	//disable adc
	bk_aud_adc_stop();
	bk_aud_adc_deinit();
	//bk_aud_driver_deinit();
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

	//disable audio interrupt when loop test
	sys_drv_aud_int_en(0);
	bk_aud_adc_disable_int();

	//start adc
	bk_aud_dac_start();
}

static void cli_audio_dac_deconfig(void)
{
	//disable dac
	bk_aud_dac_stop();
	bk_aud_dac_deinit();
	//bk_aud_driver_deinit();
}

static void cli_i2s_config(uint32_t sample_rate, uint8_t aud_mode)
{
	i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
	i2s_samp_rate_t samp_rate = I2S_SAMP_RATE_8000;

	switch (sample_rate) {
		case 1:		//8k
			samp_rate = I2S_SAMP_RATE_8000;
			break;

		case 2:		//11.025k
			samp_rate = I2S_SAMP_RATE_11025;
			break;

		case 3:		//12k
			samp_rate = I2S_SAMP_RATE_12000;
			break;

		case 4:		//16k
			samp_rate = I2S_SAMP_RATE_16000;
			break;

		case 5:		//22.05k
			samp_rate = I2S_SAMP_RATE_22050;
			break;

		case 6:		//24k
			samp_rate = I2S_SAMP_RATE_24000;
			break;

		case 7:		//32k
			samp_rate = I2S_SAMP_RATE_32000;
			break;

		case 8:		//44.1k
			samp_rate = I2S_SAMP_RATE_44100;
			break;

		case 9:		//48k
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
#if CONFIG_SOC_BK7256XX
	bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
#endif
#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
	if (aud_mode == 0) // for adc
	{
		i2s_config.role = I2S_ROLE_SLAVE; //I2S_ROLE_MASTER;
		bk_i2s_init(I2S_GPIO_GROUP_1, &i2s_config);
	}
	else // for dac
	{
		i2s_config.role = I2S_ROLE_MASTER; // I2S_ROLE_SLAVE;
		bk_i2s_init(I2S_GPIO_GROUP_2, &i2s_config);
	}

	// bk_i2s_init(I2S_GPIO_GROUP_2, &i2s_config);
#endif

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

    os_memset(&dma_config, 0, sizeof(dma_config));

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
#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
		dma_config.src.dev = DMA_DEV_AUDIO_RX;
#else
		dma_config.src.dev = DMA_DEV_AUDIO;
#endif
#if CONFIG_SOC_BK7256XX
		dma_config.dst.dev = DMA_DEV_I2S;
#endif
#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
		dma_config.dst.dev = DMA_DEV_I2S1;
#endif
		dma_config.dst.start_addr = i2s_data_addr;
		dma_config.dst.end_addr = i2s_data_addr + 4;
		dma_config.src.start_addr = aud_adc_data_addr;
		dma_config.src.end_addr = aud_adc_data_addr + 4;
	} else {
		/* audio i2s to dac by dma */
#if CONFIG_SOC_BK7256XX
		dma_config.src.dev = DMA_DEV_I2S_RX;
#endif
#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
		dma_config.src.dev = DMA_DEV_I2S2_RX;
#endif
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
		// bk_i2s_write_data(3, &temp_data, 1);
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

void cli_i2s_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		printf("i2s master test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(group_id, &i2s_config);
		printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 320 * 2, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)cli_i2s_mem_malloc(640);
		os_memset(temp_data, 0x00, 640);
		size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_printf("ring_buffer_write, size: %d \n", size);
		cli_i2s_mem_free(temp_data);
		bk_i2s_start();

		os_printf("i2s master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		cli_i2s_mem_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("i2s master test stop successful \r\n");
	}
	#if CLI_FOR_AUDIO_ATE_TEST
	/*
	  Hardware connect:
	  P44 -> P40
	  P45 -> P41
	  P46 -> P43
	  P47 -> P42
	*/
	else if(os_strcmp(argv[1], "adc") == 0){ // mic -> I2S1
		dma_id_t cli_adc_dma_id = DMA_ID_MAX;
		cli_audio_adc_config(1, 0);
		uint32_t mic1_config = 0;
		mic1_config = sys_ll_get_ana_reg19_value();
		os_printf("mic1_config: %x\n\r", mic1_config);
		sys_hal_set_ana_reg27_value(mic1_config);
		cli_i2s_config(1, 0);
		cli_adc_dma_id = cli_aud_i2s_dma_config(1);
		bk_i2s_enable(I2S_ENABLE);
		cli_aud_i2s_dma_ctrl(1, cli_adc_dma_id, cli_adc_dma_id);
	}
	else if(os_strcmp(argv[1], "dac") == 0) // I2S2 -> DAC
	{
		dma_id_t cli_dac_dma_id = DMA_ID_MAX;
		dma_id_t cli_adc_dma_id = DMA_ID_MAX;
		cli_audio_dac_config(1, 0);
		cli_i2s_config(1, 1);
		bk_i2s_enable(I2S_ENABLE);
		cli_dac_dma_id = cli_aud_i2s_dma_config(0);
		cli_aud_i2s_dma_ctrl(1, cli_adc_dma_id, cli_dac_dma_id);

		os_printf("init i2s driver and config successful \r\n");
	}
	else if (os_strcmp(argv[1], "mic_conf") == 0)
	{
		uint32_t mic1_config = 0;
		mic1_config = sys_ll_get_ana_reg27_value();
		os_printf("mic1_config: %x\n\r", mic1_config);
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

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("i2s slave test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.role = I2S_ROLE_SLAVE;
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		bk_i2s_start();

		os_printf("i2s slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("i2s slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		cli_i2s_mem_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("i2s slave test stop successful \r\n");
	} else {
		//bk_i2s_start_test();
		cli_i2s_help();
		return;
	}
}


void cli_pcm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_PCM_CONFIG();
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("pcm master test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
        bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)cli_i2s_mem_malloc(640);
		os_memset(temp_data, 0xF1, 640);
		size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_printf("ring_buffer_write, size: %d \n", size);
		cli_i2s_mem_free(temp_data);
		bk_i2s_start();

		os_printf("pcm master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("pcm master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		cli_i2s_mem_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("pcm master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_pcm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_PCM_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("pcm slave test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		i2s_config.role = I2S_ROLE_SLAVE;
        bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}
		bk_i2s_start();
		os_printf("pcm slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("pcm slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		cli_i2s_mem_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("pcm slave test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}


void cli_dtm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_TDM_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("pcm master test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);
		ch2_temp = cli_i2s_mem_malloc(320);
		os_memset(ch2_temp, 0xF2, 320);
		ch3_temp = cli_i2s_mem_malloc(320);
		os_memset(ch3_temp, 0xF3, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		//i2s_config.samp_rate = I2S_SAMP_RATE_16000;
        bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX, 640, ch2_tx_data_handle_cb, &ch2_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX, 640, ch3_tx_data_handle_cb, &ch3_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)cli_i2s_mem_malloc(640);
		os_memset(temp_data, 0xF1, 640);
		ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_memset(temp_data, 0xF1, 640);
		ring_buffer_write(ch2_tx_rb, temp_data, 640);
		os_memset(temp_data, 0xF1, 640);
		ring_buffer_write(ch3_tx_rb, temp_data, 640);
		cli_i2s_mem_free(temp_data);
		os_printf("write data ok \r\n");
		bk_i2s_start();

		os_printf("pcm master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("pcm master test stop \r\n");
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
		os_printf("pcm master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_dtm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_TDM_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("dtm slave test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);
		ch2_temp = cli_i2s_mem_malloc(320);
		ch3_temp = cli_i2s_mem_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		//i2s_config.samp_rate = I2S_SAMP_RATE_48000;
		i2s_config.role = I2S_ROLE_SLAVE;
        bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX, 640, ch2_rx_data_handle_cb, &ch2_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		ret = bk_i2s_chl_init(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX, 640, ch3_rx_data_handle_cb, &ch3_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		bk_i2s_start();
		os_printf("dtm slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("dtm slave test stop \r\n");
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
		os_printf("pcm slave test stop successful \r\n");
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

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("2B+D master test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);
		os_memset(ch1_temp, 0xF1, 320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
        bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		uint8_t *temp_data = (uint8_t *)cli_i2s_mem_malloc(640);
		os_memset(temp_data, 0xF1, 640);
		size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
		os_printf("ring_buffer_write, size: %d \n", size);
		cli_i2s_mem_free(temp_data);
		bk_i2s_start();

		os_printf("2B+D master test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("2B+D master test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		cli_i2s_mem_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("2B+D master test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

void cli_2bd_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_2BD_CONFIG();
	bk_err_t ret = BK_OK;

	if (argc != 3) {
		cli_i2s_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("2B+D slave test start \r\n");

        int group_id = os_strtoul(argv[2], NULL, 10);
        if (group_id >= SOC_I2S_UNIT_NUM)
        {
            os_printf("i2s group id is invalid\n");
            return;
        }

		ch1_temp = cli_i2s_mem_malloc(320);

		//init i2s driver
		bk_i2s_driver_init();

		//init i2s configure
		i2s_config.role = I2S_ROLE_SLAVE;
		i2s_config.samp_rate = I2S_SAMP_RATE_48000;
        bk_i2s_init(group_id, &i2s_config);
		os_printf("init i2s driver and config successful \r\n");

		ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &ch1_rx_rb);
		if (ret != BK_OK) {
			os_printf("bk_i2s_chl_init fail \n");
			return;
		}

		bk_i2s_start();

		os_printf("2B+D slave test complete \r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("2B+D slave test stop \r\n");
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		bk_i2s_deinit();
		bk_i2s_driver_deinit();

		cli_i2s_mem_free(ch1_temp);
		ch1_temp = NULL;
		os_printf("2B+D slave test stop successful \r\n");
	} else {
		cli_i2s_help();
		return;
	}
}

#ifdef CPU_TEST_I2S_DEBUG
void cli_i2s_cpu_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
	uint32_t temp_data = 0xF0F0F0F0;
	uint32_t data = 0xF1F1F1F1;

	if (argc != 2) {
		cli_i2s_help();
		return;
	}

	os_printf("i2s cpu master test start \r\n");

    int group_id = os_strtoul(argv[2], NULL, 10);
    if (group_id >= SOC_I2S_UNIT_NUM)
    {
        os_printf("i2s group id is invalid\n");
        return;
    }

	//init i2s driver
	bk_i2s_driver_init();

	//init i2s configure
	i2s_config.samp_rate = I2S_SAMP_RATE_48000;
    bk_i2s_init(group_id, &i2s_config);
	os_printf("init i2s driver and config successful \r\n");

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

	os_printf("i2s cpu master test complete \r\n");

}
#endif


#define I2S_CMD_CNT (sizeof(s_i2s_commands) / sizeof(struct cli_command))
DRV_CLI_CMD_EXPORT static const struct cli_command s_i2s_commands[] = {
	{"i2s_master_test", "i2s_master_test {start|stop group_id}", cli_i2s_master_test_cmd},
	{"i2s_slave_test", "i2s_slave_test {start|stop group_id}", cli_i2s_slave_test_cmd},
	{"pcm_master_test", "pcm_master_test {start|stop group_id}", cli_pcm_master_test_cmd},
	{"pcm_slave_test", "pcm_slave_test {start|stop group_id}", cli_pcm_slave_test_cmd},
	{"dtm_master_test", "dtm_master_test {start|stop group_id}", cli_dtm_master_test_cmd},
	{"dtm_slave_test", "dtm_slave_test {start|stop group_id}", cli_dtm_slave_test_cmd},
	{"2bd_master_test", "2bd_master_test {start|stop group_id}", cli_2bd_master_test_cmd},
	{"2bd_slave_test", "2bd_slave_test {start|stop group_id}", cli_2bd_slave_test_cmd},
#ifdef CPU_TEST_I2S_DEBUG
	{"i2s_cpu_master_test", "i2s_cpu_master_test {group_id}", cli_i2s_cpu_master_test_cmd},
#endif
};

int cli_i2s_init(void)
{
	return cli_register_commands(s_i2s_commands, I2S_CMD_CNT);
}

