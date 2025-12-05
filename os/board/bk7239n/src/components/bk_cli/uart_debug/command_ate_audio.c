#include "command_ate.h"

#if defined(CONFIG_ATE_TEST)
#include "sys_driver.h"
//#include "aud_driver.h"
//#include <driver/aud.h>
//#include <driver/aud_types.h>
#include <driver/aud_adc_types.h>
#include <driver/aud_adc.h>
#include <driver/aud_dac_types.h>
#include <driver/aud_dac.h>
#include <driver/i2s.h>
#include <driver/i2s_types.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/dma.h>
#include "sys_driver.h"


#define ADC_FRAME_SIZE    (320)
static dma_id_t adc_dma_id = 0;
static dma_id_t dac_dma_id = 0;
static uint8_t *adc_ringbuff_addr = NULL;
static uint8_t *dac_ringbuff_addr = NULL;
static RingBufferContext adc_rb;
static RingBufferContext dac_rb;
static uint16_t *adc_temp = NULL;
static uint8_t test_mic_id = 0;


/* 搬运audio adc 采集到的一帧mic和ref信号后，触发中断通知AEC处理数据 */
static void audio_adc_dma_finish_isr(void)
{
	/* read adc data from adc ringbuffer */
	uint32_t size = ring_buffer_read(&adc_rb, (uint8_t*)adc_temp, ADC_FRAME_SIZE);
	if (size != ADC_FRAME_SIZE) {
		return;
	}

	/* select r channel data */
	for (uint32_t i = 0; i < ADC_FRAME_SIZE/2; i++) {
		adc_temp[i] = adc_temp[2*i+1];
	}

	size = ring_buffer_get_free_size(&dac_rb);
	if (size >= ADC_FRAME_SIZE/2) {
		ring_buffer_write(&dac_rb, (uint8_t *)adc_temp, ADC_FRAME_SIZE/2);
	}
}

void audio_adc_mic2_to_dac_test(uint32_t state)
{
	aud_adc_config_t adc_config = DEFAULT_AUD_ADC_CONFIG();
	adc_config.adc_chl = AUD_ADC_CHL_LR;
	adc_config.clk_src = AUD_CLK_XTAL;
	adc_config.samp_rate = 16000;
	aud_dac_config_t dac_config = DEFAULT_AUD_DAC_CONFIG();
	dac_config.clk_src = AUD_CLK_XTAL;
	dac_config.samp_rate = 16000;
	dma_config_t dma_config = {0};
	dma_config_t dac_dma_config = {0};
	uint32_t aud_adc_data_addr;
	uint32_t aud_dac_data_addr;
	bk_err_t ret = BK_OK;

	if (state == 2) {

		bk_aud_adc_init(&adc_config);

		//sys_drv_aud_mic1_gain_set(0);
		sys_drv_aud_mic2_gain_set(0);

		//disable audio interrupt when loop test
		sys_drv_aud_int_en(0);
		bk_aud_adc_disable_int();

		//start adc
	//	bk_aud_adc_start();

		/* init dma driver */
		ret = bk_dma_driver_init();
		if (ret != BK_OK) {
			return;
		}

		/* allocate free DMA channel */
		adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((adc_dma_id < DMA_ID_0) || (adc_dma_id >= DMA_ID_MAX)) {
			return;
		}

		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 1;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.trans_type = DMA_TRANS_DEFAULT;

		/* get audio adc and dac fifo address */
		bk_aud_adc_get_fifo_addr(&aud_adc_data_addr);

		/* init ringbuff */
		adc_ringbuff_addr = (uint8_t *)os_malloc(ADC_FRAME_SIZE*2);
		ring_buffer_init(&adc_rb, adc_ringbuff_addr, ADC_FRAME_SIZE*2, adc_dma_id, RB_DMA_TYPE_WRITE);

		adc_temp = (uint16_t *)os_malloc(ADC_FRAME_SIZE);

		/* audio adc to dtcm by dma */
		dma_config.src.dev = DMA_DEV_AUDIO_RX;
		dma_config.dst.dev = DMA_DEV_DTCM;
		dma_config.dst.start_addr = (uint32_t)adc_ringbuff_addr;
		dma_config.dst.end_addr = (uint32_t)adc_ringbuff_addr + ADC_FRAME_SIZE*2;
		dma_config.src.start_addr = aud_adc_data_addr;
		dma_config.src.end_addr = aud_adc_data_addr + 4;

		/* init dma channel */
		ret = bk_dma_init(adc_dma_id, &dma_config);
		if (ret != BK_OK) {
			return;
		}

		/* set dma transfer length */
		bk_dma_set_transfer_len(adc_dma_id, ADC_FRAME_SIZE);

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
		bk_dma_set_dest_sec_attr(adc_dma_id, DMA_ATTR_SEC);
		bk_dma_set_src_sec_attr(adc_dma_id, DMA_ATTR_SEC);
#endif

		//register isr
		bk_dma_register_isr(adc_dma_id, NULL, (void *)audio_adc_dma_finish_isr);
		bk_dma_enable_finish_interrupt(adc_dma_id);

		/* dac config */
		bk_aud_dac_init(&dac_config);

		sys_drv_aud_dacg_set(0);

		//disable audio interrupt when loop test
	//	bk_aud_dac_disable_int();

		//start dac
	//	bk_aud_dac_start();

		/* allocate free DMA channel */
		dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((dac_dma_id < DMA_ID_0) || (dac_dma_id >= DMA_ID_MAX)) {
			return;
		}

		dac_dma_config.mode = DMA_WORK_MODE_REPEAT;
		dac_dma_config.chan_prio = 1;
		dac_dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dac_dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
		dac_dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dac_dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dac_dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dac_dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dac_dma_config.trans_type = DMA_TRANS_DEFAULT;

		/* get audio adc and dac fifo address */
		bk_aud_dac_get_fifo_addr(&aud_dac_data_addr);

		/* init ringbuff */
		dac_ringbuff_addr = (uint8_t *)os_malloc(ADC_FRAME_SIZE);
		ring_buffer_init(&dac_rb, dac_ringbuff_addr, ADC_FRAME_SIZE, dac_dma_id, RB_DMA_TYPE_READ);

		/* audio adc to dtcm by dma */
		dac_dma_config.src.dev = DMA_DEV_DTCM;
		dac_dma_config.dst.dev = DMA_DEV_AUDIO;
		dac_dma_config.dst.start_addr = aud_dac_data_addr;
		dac_dma_config.dst.end_addr = aud_dac_data_addr + 2;
		dac_dma_config.src.start_addr = (uint32_t)dac_ringbuff_addr;
		dac_dma_config.src.end_addr = (uint32_t)dac_ringbuff_addr + ADC_FRAME_SIZE;

		/* init dma channel */
		ret = bk_dma_init(dac_dma_id, &dac_dma_config);
		if (ret != BK_OK) {
			return;
		}

		/* set dma transfer length */
		bk_dma_set_transfer_len(dac_dma_id, ADC_FRAME_SIZE/2);

		os_memset(adc_temp, 0, ADC_FRAME_SIZE/2);
		ring_buffer_write(&dac_rb, (uint8_t *)adc_temp, ADC_FRAME_SIZE/2);

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
		bk_dma_set_dest_sec_attr(dac_dma_id, DMA_ATTR_SEC);
		bk_dma_set_src_sec_attr(dac_dma_id, DMA_ATTR_SEC);
#endif

		/* start dac and adc */
		bk_aud_dac_start();
		bk_aud_dac_start();
		bk_aud_adc_start();

		/* start dma */
		bk_dma_start(dac_dma_id);
		bk_dma_start(adc_dma_id);

		test_mic_id = 2;
	} else if (state == 0) {
		/* check test mic id */
		if (test_mic_id != 2) {
			return;
		}

		for (uint8_t i = 0; i < DMA_ID_MAX; i++) {
			if (bk_dma_user(i) == DMA_DEV_AUDIO) {
				bk_dma_stop(i);
				bk_dma_deinit(i);
				bk_dma_free(DMA_DEV_AUDIO, i);
			}
		}

		bk_dma_driver_deinit();

		if (adc_ringbuff_addr) {
			ring_buffer_clear(&adc_rb);
			os_free(adc_ringbuff_addr);
			adc_ringbuff_addr = NULL;
		}

		if (dac_ringbuff_addr) {
			ring_buffer_clear(&dac_rb);
			os_free(dac_ringbuff_addr);
			dac_ringbuff_addr = NULL;
		}

		if (adc_temp) {
			os_free(adc_temp);
			adc_temp =NULL;
		}

		bk_aud_adc_stop();
		bk_aud_adc_deinit();

		bk_aud_dac_stop();
		bk_aud_dac_deinit();

		test_mic_id = 0;
	}else {
		//not need todo
		return;
	}
}

/**
 * @brief     audio loop test
 *
 * @param mic_id test mic number
 *
 * @return none
 */
void audio_loop_test(uint32_t state)
{
	aud_adc_config_t adc_config = DEFAULT_AUD_ADC_CONFIG();
	aud_dac_config_t dac_config = DEFAULT_AUD_DAC_CONFIG();

	if (state == 1) {

		adc_config.adc_chl = AUD_ADC_CHL_LR;
		adc_config.samp_rate = 16000;
		adc_config.clk_src = AUD_CLK_XTAL;

		dac_config.dac_chl = AUD_DAC_CHL_LR;
		dac_config.samp_rate = 16000;
		dac_config.clk_src = AUD_CLK_XTAL;

		bk_aud_adc_init(&adc_config);
		bk_aud_dac_init(&dac_config);
		/* audio process test */
		sys_drv_aud_dacg_set(0);
		sys_drv_aud_mic1_gain_set(0);
		sys_drv_aud_mic2_gain_set(0);

		//disable audio interrupt when loop test
		sys_drv_aud_int_en(0);
		bk_aud_adc_disable_int();

		//start adc
		bk_aud_adc_start();
		//start adc
		bk_aud_dac_start();

		//enable adc to dac loop test
		bk_aud_adc_start_loop_test();

		test_mic_id = 1;
	}else if (state == 0) {
		if (test_mic_id != 1) {
			return;
		}

		//disable adc and dac
		bk_aud_adc_stop();
		bk_aud_dac_stop();

		//stop loop test
		bk_aud_adc_stop_loop_test();
		bk_aud_adc_deinit();
		bk_aud_dac_deinit();

		test_mic_id = 0;
	} else {
		//not need todo
		return;
	}
}

/* audio auto test */
static void audio_auto_loop_test(uint32_t state, uint32_t sample_rate, uint32_t dac_mode)
{
	aud_adc_config_t adc_config = DEFAULT_AUD_ADC_CONFIG();
	aud_dac_config_t dac_config = DEFAULT_AUD_DAC_CONFIG();

	if (state == 1) {
		adc_config.adc_chl = AUD_ADC_CHL_LR;
		adc_config.clk_src = AUD_CLK_XTAL;
		dac_config.dac_chl = AUD_DAC_CHL_LR;
		dac_config.clk_src = AUD_CLK_XTAL;
		switch (sample_rate) {
			case 1:
				adc_config.samp_rate = 8000;
				dac_config.samp_rate = 8000;
				break;

			case 2:
				adc_config.samp_rate = 11025;
				dac_config.samp_rate = 11025;
				break;

			case 3:
				adc_config.samp_rate = 12000;
				dac_config.samp_rate = 12000;
				break;

			case 4:
				adc_config.samp_rate = 16000;
				dac_config.samp_rate = 16000;
				break;

			case 5:
				adc_config.samp_rate = 22050;
				dac_config.samp_rate = 22050;
				break;

			case 6:
				adc_config.samp_rate = 24000;
				dac_config.samp_rate = 24000;
				break;

			case 7:
				adc_config.samp_rate = 32000;
				dac_config.samp_rate = 32000;
				break;

			case 8:
				adc_config.samp_rate = 44100;
				dac_config.samp_rate = 44100;
				break;

			case 9:
				adc_config.samp_rate = 48000;
				dac_config.samp_rate = 48000;
				break;

			default:
				break;
		}

		if (dac_mode == 0) {
			dac_config.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
		} else if (dac_mode == 1) {
			dac_config.work_mode = AUD_DAC_WORK_MODE_SIGNAL_END;
		} else {
			dac_config.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
		}

		/* audio process test */
		sys_drv_aud_dacg_set(0);
		sys_drv_aud_mic1_gain_set(0);
		sys_drv_aud_mic2_gain_set(0);

		bk_aud_adc_init(&adc_config);
		bk_aud_dac_init(&dac_config);

		//disable audio interrupt when loop test
		sys_drv_aud_int_en(0);
		bk_aud_adc_disable_int();

		//start adc
		bk_aud_adc_start();
		//start adc
		bk_aud_dac_start();

		//enable adc to dac loop test
		bk_aud_adc_start_loop_test();
	}else {
		//disable adc and dac
		bk_aud_adc_stop();
		bk_aud_dac_stop();

		//stop loop test
		bk_aud_adc_stop_loop_test();
		bk_aud_adc_deinit();
		bk_aud_dac_deinit();
	}
}

static void audio_adc_config(uint32_t sample_rate, uint32_t adc_mode)
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

static void audio_adc_deconfig(void)
{
	//disable adc
	bk_aud_adc_stop();
	bk_aud_adc_deinit();
	//bk_aud_driver_deinit();
}

static void audio_dac_config(uint32_t sample_rate, uint8_t dac_gain, uint32_t dac_mode)
{
	aud_dac_config_t dac_config = DEFAULT_AUD_DAC_CONFIG();
	dac_config.dac_chl = AUD_DAC_CHL_LR;
	dac_config.clk_src = AUD_CLK_XTAL;
	dac_config.work_mode = dac_mode;
	dac_config.dac_gain = dac_gain;
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

	bk_aud_dac_init(&dac_config);

	/* audio process test */
	sys_drv_aud_dacg_set(0);

	//disable audio interrupt when loop test
	sys_drv_aud_int_en(0);
	bk_aud_adc_disable_int();

	//start adc
	bk_aud_dac_start();
}

static void audio_dac_deconfig(void)
{
	//disable dac
	bk_aud_dac_stop();
	bk_aud_dac_deinit();
	//bk_aud_driver_deinit();
}

static void i2s_config(uint32_t sample_rate)
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
#if defined(CONFIG_SOC_BK7256XX)
	bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
#endif
#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
	bk_i2s_init(I2S_GPIO_GROUP_2, &i2s_config);
#endif

	bk_i2s_enable(I2S_ENABLE);
}

static void i2s_deconfig(void)
{
	bk_i2s_enable(I2S_DISABLE);

	bk_i2s_deinit();

	bk_i2s_driver_deinit();
}


static dma_id_t aud_i2s_dma_config(uint32_t state)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
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
#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
		dma_config.src.dev = DMA_DEV_AUDIO_RX;
#else
		dma_config.src.dev = DMA_DEV_AUDIO;
#endif
#if defined(CONFIG_SOC_BK7256XX)
		dma_config.dst.dev = DMA_DEV_I2S;
#endif
#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
		dma_config.dst.dev = DMA_DEV_I2S2;
#endif
		dma_config.dst.start_addr = i2s_data_addr;
		dma_config.dst.end_addr = i2s_data_addr + 4;
		dma_config.src.start_addr = aud_adc_data_addr;
		dma_config.src.end_addr = aud_adc_data_addr + 4;
	} else {
		/* audio i2s to dac by dma */
#if defined(CONFIG_SOC_BK7256XX)
		dma_config.src.dev = DMA_DEV_I2S_RX;
#endif
#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
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

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC);
	bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC);
#endif

	return dma_id;
}

static void aud_i2s_dma_deconfig(void)
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

static void aud_i2s_dma_ctrl(uint32_t state, dma_id_t adc_dma_id, dma_id_t dac_dma_id)
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

static const unsigned long PCM_8000[] = {
	0x00010000, 0x5A825A81, 0x7FFF7FFF, 0x5A825A83, 0x00000000, 0xA57FA57E, 0x80018002, 0xA57EA57E,
};

static const unsigned long PCM_16000[] = {
	0x00000001, 0x30FC30FA, 0x5A815A82, 0x76417641, 0x7FFF7FFF, 0x76417641, 0x5A815A82, 0x30FB30FB,
	0x00000000, 0xCF05CF04, 0xA57EA57E, 0x89BF89BF, 0x80018000, 0x89C089BF, 0xA57EA57F, 0xCF05CF04,
};

static const unsigned long PCM_44100[] = {
	0x00000000, 0x122D122E, 0x23FC23FB, 0x350F350F, 0x450F4510, 0x53AA53AA, 0x60926092, 0x6B866B85,
	0x744A744A, 0x7AB57AB5, 0x7EA37EA2, 0x7FFF7FFF, 0x7EC37EC4, 0x7AF77AF6, 0x74AB74AC, 0x6C036C04,
	0x612B612A, 0x5459545A, 0x45D445D3, 0x35E335E3, 0x24DB24DA, 0x13141313, 0x00EA00EA, 0xEEBAEEBB,
	0xDCE5DCE4, 0xCBC5CBC6, 0xBBB6BBB5, 0xAD08AD08, 0xA008A008, 0x94FA94FA, 0x8C188C17, 0x858E858E,
	0x81818181, 0x80038003, 0x811D811D, 0x84C984CA, 0x8AF58AF5, 0x93809381, 0x9E3E9E3E, 0xAAF7AAF7,
	0xB969B96A, 0xC94AC94A, 0xDA46DA46, 0xEC05EC06, 0xFE2DFE2E, 0x105E105E, 0x223B223B, 0x33653365,
	0x43854384, 0x52465246, 0x5F5D5F5D, 0x6A856A85, 0x73847384, 0x7A2C7A2C, 0x7E5B7E5B, 0x7FFA7FFA,
	0x7F007F01, 0x7B747B75, 0x75697569, 0x6CFB6CFB, 0x62586258, 0x55B755B7, 0x47594759, 0x37893789,
	0x26992699, 0x14E114E1, 0x02BC02BC, 0xF08AF08A, 0xDEA6DEA6, 0xCD72CD72, 0xBD42BD43, 0xAE6DAE6D,
	0xA13FA13F, 0x95FD95FD, 0x8CE18CE2, 0x86198619, 0x81CA81CB, 0x800B800C, 0x80E380E3, 0x844E844E,
	0x8A3C8A3C, 0x928B928B, 0x9D149D13, 0xA99CA99C, 0xB7E6B7E6, 0xC7A4C7A5, 0xD889D888, 0xEA39EA39,
	0xFC5AFC5A, 0x0E8F0E8F, 0x20782077, 0x31B831B8, 0x41F641F6, 0x50DF50DF, 0x5E235E23, 0x697F697F,
	0x72B972B8, 0x799F799E, 0x7E0D7E0D, 0x7FEE7FEE, 0x7F387F38, 0x7BED7BED, 0x761F761F, 0x6DEC6DED,
	0x63806380, 0x570F5710, 0x48DB48DA, 0x392C392C, 0x28552855, 0x16AC16AD, 0x048F048F, 0xF25AF259,
	0xE06BE06B, 0xCF1FCF1F, 0xBED2BED3, 0xAFD8AFD7, 0xA27CA27B, 0x97059706, 0x8DB08DB1, 0x86AB86AB,
	0x821C821C, 0x801A801A, 0x80B080AF, 0x83DA83DA, 0x89888988, 0x919C919C, 0x9BED9BEE, 0xA847A846,
	0xB666B666, 0xC603C603, 0xD6CED6CD, 0xE86DE86D, 0xFA89FA87, 0x0CBE0CBF, 0x1EB31EB3, 0x30083007,
	0x40644064, 0x4F734F73, 0x5CE55CE4, 0x68756874, 0x71E771E7, 0x790A790A, 0x7DB87DB9, 0x7FDC7FDD,
	0x7F677F68, 0x7C5F7C5E, 0x76CF76D0, 0x6ED96EDA, 0x64A364A3, 0x58635863, 0x4A594A58, 0x3ACD3ACC,
	0x2A102A0F, 0x18781878, 0x06610661, 0xF429F42A, 0xE230E22F, 0xD0D0D0D0, 0xC066C066, 0xB145B145,
	0xA3BCA3BD, 0x98149813, 0x8E848E85, 0x87438743, 0x82738273, 0x802F8030, 0x80848083, 0x836B836B,
	0x88DA88DB, 0x90B390B3, 0x9ACE9ACD, 0xA6F5A6F5, 0xB4EAB4EA, 0xC464C465, 0xD515D515, 0xE6A3E6A3,
	0xF8B6F8B6, 0x0AED0AEE, 0x1CEC1CED, 0x2E562E57, 0x3ED03ED0, 0x4E024E03, 0x5BA15BA0, 0x67646764,
	0x710E710F, 0x786F786E, 0x7D5E7D5E, 0x7FC37FC3, 0x7F917F91, 0x7CCA7CC9, 0x777A777A, 0x6FBF6FC0,
	0x65C165C2, 0x59B259B3, 0x4BD34BD3, 0x3C693C69, 0x2BC62BC7, 0x1A411A40, 0x08330834, 0xF5FBF5FB,
	0xE3F6E3F7, 0xD283D284, 0xC1FCC1FD, 0xB2B7B2B8, 0xA503A503, 0x99279926, 0x8F608F60, 0x87E187E2,
	0x82D282D2, 0x804D804C, 0x805D805C, 0x83038304, 0x88338833, 0x8FCF8FCF, 0x99B299B1, 0xA5A8A5A8,
	0xB371B372, 0xC2C8C2CA, 0xD35ED35E, 0xE4DAE4DB, 0xF6E4F6E4, 0x091C091C, 0x1B261B25, 0x2CA22CA2,
	0x3D373D37, 0x4C8E4C8E, 0x5A595A59, 0x664E664E, 0x70307030, 0x77CD77CE, 0x7CFD7CFD, 0x7FA37FA3,
	0x7FB37FB4, 0x7D2E7D2E, 0x781F781F, 0x70A170A0, 0x66DA66DA, 0x5AFE5AFD, 0x4D494D49, 0x3E033E04,
	0x2D7D2D7D, 0x1C091C0A, 0x0A050A05, 0xF7CCF7CD, 0xE5BEE5BE, 0xD439D439, 0xC396C396, 0xB42DB42D,
	0xA64DA64E, 0x9A3E9A3E, 0x90409040, 0x88868886, 0x83378337, 0x806E806E, 0x803D803E, 0x82A382A1,
	0x87928791, 0x8EF18EF2, 0x989C989C, 0xA45FA45F, 0xB1FEB1FD, 0xC131C131, 0xD1AAD1AA, 0xE313E313,
	0xF512F512, 0x074A074B, 0x195D195C, 0x2AEC2AEC, 0x3B9C3B9C, 0x4B164B16, 0x590C590B, 0x65336532,
	0x6F4D6F4C, 0x77267726, 0x7C957C95, 0x7F7D7F7D, 0x7FD17FD1, 0x7D8C7D8C, 0x78BD78BC, 0x717B717A,
	0x67ED67ED, 0x5C445C43, 0x4EBB4EBB, 0x3F9B3F9A, 0x2F302F30, 0x1DD01DD0, 0x0BD60BD7, 0xF99FF99E,
	0xE788E789, 0xD5F1D5F1, 0xC533C533, 0xB5A8B5A7, 0xA79DA79D, 0x9B5D9B5D, 0x91289127, 0x89308930,
	0x83A283A2, 0x80988098, 0x80248024, 0x82468247, 0x86F786F6, 0x8E1A8E19, 0x978C978C, 0xA31CA31C,
	0xB08EB08D, 0xBF9CBF9B, 0xCFF7CFF8, 0xE14DE14C, 0xF342F342, 0x05780578, 0x17931792, 0x29332933,
	0x39FE39FD, 0x499B499A, 0x57B957BA, 0x64126413, 0x6E636E64, 0x76787678, 0x7C277C27, 0x7F517F50,
	0x7FE67FE5, 0x7DE47DE3, 0x79557955, 0x72517250, 0x68FA68FB, 0x5D855D84, 0x50295029, 0x412E412E,
	0x30E030E1, 0x1F961F95, 0x0DA70DA6, 0xFB71FB70, 0xE954E953, 0xD7ABD7AB, 0xC6D4C6D3, 0xB726B725,
	0xA8F1A8F1, 0x9C809C80, 0x92139213, 0x89E189E0, 0x84138413, 0x80C880C9, 0x80128011, 0x81F381F3,
	0x86618663, 0x8D478D48, 0x96819680, 0xA1DDA1DD, 0xAF22AF22, 0xBE0ABE0A, 0xCE48CE48, 0xDF88DF89,
	0xF172F171, 0x03A603A5, 0x15C715C8, 0x27782777, 0x385B385B, 0x481B481A, 0x56635664, 0x62ED62EC,
	0x6D746D75, 0x75C475C5, 0x7BB17BB2, 0x7F1C7F1C, 0x7FF57FF4, 0x7E347E35, 0x79E679E6, 0x731F731F,
	0x6A046A03, 0x5EC15EC1, 0x51935194, 0x42BE42BE, 0x328E328F, 0x21592159, 0x0F770F77, 0xFD43FD44,
	0xEB20EB1F, 0xD967D966, 0xC877C877, 0xB8A8B8A7, 0xAA49AA49, 0x9DA89DA9, 0x93059306, 0x8A988A98,
	0x848B848B, 0x80FF80FF, 0x80068006, 0x81A581A4, 0x85D485D3, 0x8C7B8C7C, 0x957B957A, 0xA0A4A0A2,
	0xADBAADBA, 0xBC7BBC7B, 0xCC9CCC9C, 0xDDC6DDC6, 0xEFA2EFA2, 0x01D201D3, 0x13FB13FB, 0x25BA25BA,
	0x36B636B7, 0x46964697, 0x550A5509, 0x61C261C2, 0x6C806C7F, 0x750B750A, 0x7B377B36, 0x7EE37EE2,
	0x7FFD7FFE, 0x7E7F7E80, 0x7A717A71, 0x73E973E8, 0x6B066B06, 0x5FF85FF8, 0x52F952F8, 0x444A444B,
	0x343B343A, 0x231B231A, 0x11461146, 0xFF17FF17, 0xECEDECEC, 0xDB26DB25, 0xCA1DCA1E, 0xBA2CBA2D,
	0xABA6ABA6, 0x9ED59ED5, 0x93FD93FD, 0x8B558B55, 0x85098509, 0x813C813D, 0x80018001, 0x815E815E,
	0x854B854C, 0x8BB58BB5, 0x947B947B, 0x9F6D9F6F, 0xAC56AC57, 0xBAF1BAF0, 0xCAF1CAF1, 0xDC05DC04,
	0xEDD2EDD4,
};

static const unsigned long PCM_48000[] = {
	0x0000FFFF, 0x10B510B5, 0x21202121, 0x30FB30FC, 0x40003FFF, 0x4DEC4DEC, 0x5A825A82, 0x658C658C,
	0x6EDA6ED9, 0x76417641, 0x7BA37BA2, 0x7EE77EE7, 0x7FFF7FFF, 0x7EE67EE6, 0x7BA27BA3, 0x76417641,
	0x6ED86EDA, 0x658C658B, 0x5A825A82, 0x4DEB4DEB, 0x3FFF3FFF, 0x30FC30FB, 0x21202120, 0x10B510B5,
	0x00010000, 0xEF4AEF4B, 0xDEDFDEDF, 0xCF05CF05, 0xC000C000, 0xB214B215, 0xA57EA57F, 0x9A759A74,
	0x91269127, 0x89C089BF, 0x845D845D, 0x811A8119, 0x80018001, 0x811A811A, 0x845E845D, 0x89BF89C0,
	0x91279127, 0x9A749A74, 0xA57EA57E, 0xB214B216, 0xC001C001, 0xCF05CF04, 0xDEDFDEDF, 0xEF4BEF4B,
};

static dma_id_t aud_dac_dma_config(uint32_t samp_rate)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t aud_dac_data_addr;
	uint32_t *aud_ptr = NULL;
	uint32_t aud_len = 0;

	switch (samp_rate) {
		case 1:
			//dac_config.samp_rate = 8000;
			aud_ptr = (uint32_t *)PCM_8000;
			aud_len = sizeof(PCM_8000) / sizeof(PCM_8000[0]);
			break;

		case 2:
			//dac_config.samp_rate = 11025;
			break;

		case 3:
			//dac_config.samp_rate = 12000;
			break;

		case 4:
			//dac_config.samp_rate = 16000;
			aud_ptr = (uint32_t *)PCM_16000;
			aud_len = sizeof(PCM_16000) / sizeof(PCM_16000[0]);
			break;

		case 5:
			//dac_config.samp_rate = 22050;
			break;

		case 6:
			//dac_config.samp_rate = 24000;
			break;

		case 7:
			//dac_config.samp_rate = 32000;
			break;

		case 8:
			//dac_config.samp_rate = 44100;
			aud_ptr = (uint32_t *)PCM_44100;
			aud_len = sizeof(PCM_44100) / sizeof(PCM_44100[0]);
			break;

		case 9:
			//dac_config.samp_rate = 48000;
			aud_ptr = (uint32_t *)PCM_48000;
			aud_len = sizeof(PCM_48000) / sizeof(PCM_48000[0]);
			break;

		default:
			break;
	}
	
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
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;

	bk_aud_dac_get_fifo_addr(&aud_dac_data_addr);
	dma_config.dst.dev = DMA_DEV_AUDIO;
	dma_config.src.start_addr = (uint32_t)aud_ptr;
	dma_config.src.end_addr = (uint32_t)aud_ptr + aud_len*4;
	dma_config.dst.start_addr = aud_dac_data_addr;
	dma_config.dst.end_addr = aud_dac_data_addr + 4;

	dma_config.trans_type = DMA_TRANS_DEFAULT;
	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK) {
		return DMA_ID_MAX;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, aud_len*4);

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC);
	bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC);
#endif

	ret = bk_dma_start(dma_id);
	if (ret != BK_OK)
		return ret;
	
	//start adc
	ret = bk_aud_dac_start();

	return dma_id;
}

/*
01 e0 fc 07 37 X1 X2 X3 X4 X5 X6(hex)

X1: test control
00: stop test
01: start test

X2: test mode
00: loop
01: adc (IIS OUT,master)
02: dac (IIS IN,master)
03: dac & adc (IIS IN,master)
04: dac test mode (1KHz fixed signal)

X3:sample rate setting
1-9: 8K-48K

X4:dac gain setting
0-0x3f: -45db ~18db  0x2d = 0db

X5：adc and dac mode
00: differential
01: single-ended

X6: APLL or XTAL
00: APLL
01: XTAL
*/
void audio_ap_test_for_ate(UINT8 enable, UINT8 test_mode, UINT8 sample_rate, UINT8 dac_gain, UINT8 adc_dac_mode, UINT8 clk_mode)
{
	dma_id_t adc_dma_id = DMA_ID_MAX;
	dma_id_t dac_dma_id = DMA_ID_MAX;
	#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
	uint32_t mic1_config = 0;
	#endif
	UINT8 ret = 0;

	/* parameter check */
	if ((enable < 0 || enable > 1) ||
		(test_mode < 0 || test_mode > 4) ||
		(sample_rate < 1 || sample_rate > 9) ||
		(dac_gain < 0 || dac_gain > 0x3f) ||
		(adc_dac_mode < 0 || adc_dac_mode > 1) ||
		(clk_mode < 0 || clk_mode > 1))
	{

		ret = 1;
		goto audio_exit;
	}

#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
	if (clk_mode == 0)
	{
		bk_aud_clk_config(AUD_CLK_APLL);
	}
	else
	{
		bk_aud_clk_config(AUD_CLK_XTAL);
	}
#endif


	if (enable == 1) {
		/* start test */
		switch (test_mode)
		{
			/* loop mode */
			case 0:
				audio_auto_loop_test(1, sample_rate, adc_dac_mode);
				break;

			/* audio adc: dual mic test*/
			case 1:
				audio_adc_config(sample_rate, adc_dac_mode);
				i2s_config(sample_rate);
				#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
				/*copy mic1 config to mic2*/
				mic1_config = sys_ll_get_ana_reg19_value();
				sys_hal_set_ana_reg27_value(mic1_config);
				#endif
				adc_dma_id = aud_i2s_dma_config(1);
				aud_i2s_dma_ctrl(1, adc_dma_id, dac_dma_id);
				break;

			/* audio dac */
			case 2:
				audio_dac_config(sample_rate, dac_gain, adc_dac_mode);
				i2s_config(sample_rate);
				dac_dma_id = aud_i2s_dma_config(0);
				aud_i2s_dma_ctrl(1, adc_dma_id, dac_dma_id);
				break;

			/* audio adc and dac */
			case 3:
				audio_adc_config(sample_rate, adc_dac_mode);
				#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
				/*copy mic1 config to mic2*/
				mic1_config = sys_ll_get_ana_reg19_value();
				sys_hal_set_ana_reg27_value(mic1_config);
				#endif
				audio_dac_config(sample_rate, dac_gain, adc_dac_mode);
				i2s_config(sample_rate);
				dac_dma_id = aud_i2s_dma_config(0);
				adc_dma_id = aud_i2s_dma_config(1);
				aud_i2s_dma_ctrl(1, adc_dma_id, dac_dma_id);
				break;
				/*only dac test*/
			case 4:
				audio_dac_config(sample_rate, dac_gain, adc_dac_mode);
				aud_dac_dma_config(sample_rate);
				break;
			default:
				break;
		}
	} else if (enable == 0) {
		/* stop test */
		switch (test_mode)
		{
			/* loop mode */
			case 0:
				audio_auto_loop_test(0, sample_rate, adc_dac_mode);
				break;

			/* audio adc */
			case 1:
				aud_i2s_dma_ctrl(0, adc_dma_id, dac_dma_id);
				audio_adc_deconfig();
				bk_aud_driver_deinit();
				i2s_deconfig();
				aud_i2s_dma_deconfig();
				break;

			/* audio dac */
			case 2:
				aud_i2s_dma_ctrl(0, adc_dma_id, dac_dma_id);
				audio_dac_deconfig();
				bk_aud_driver_deinit();
				i2s_deconfig();
				aud_i2s_dma_deconfig();
				break;

			/* audio dac */
			case 3:
				aud_i2s_dma_ctrl(0, adc_dma_id, dac_dma_id);
				audio_adc_deconfig();
				audio_dac_deconfig();
				bk_aud_driver_deinit();
				i2s_deconfig();
				aud_i2s_dma_deconfig();
				break;
			case 4:
				audio_dac_deconfig();
				bk_aud_driver_deinit();
				aud_i2s_dma_deconfig();
				break;
			default:
				ret = 2;
				goto audio_exit;
				break;
		}
	} else {
		ret = 3;
		goto audio_exit;
	}

audio_exit:
	uart_send_bytes_for_ate((UINT8 *)&ret, 1);
}

#endif
