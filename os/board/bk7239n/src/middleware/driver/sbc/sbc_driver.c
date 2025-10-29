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
#include "sys_driver.h"
#include "sbc_hal.h"
#include "sbc_driver.h"
#include <driver/int.h>
#include <driver/sbc.h>
#include <driver/sbc_types.h>
#include "sbc_ll_macro_def.h"
#include <os/mem.h>


typedef struct {
	sbc_decoder_isr_t callback;
	void *param;
} sbc_decoder_callback_t;

static sbc_decoder_callback_t s_sbc_decoder_isr = {NULL};

static void sbc_decoder_isr(void);


/**
 * SBC constant table OFFSET4 for calculate bit allocation
 */
static const int32_t SBC_COMMON_OFFSET4[4][4] =
{
	{ -1, 0, 0, 0 },
	{ -2, 0, 0, 1 },
	{ -2, 0, 0, 1 },
	{ -2, 0, 0, 1 }
};

/**
 * SBC constant table OFFSET8 for calculate bit allocation
 */
static const int32_t SBC_COMMON_OFFSET8[4][8] =
{
	{ -2, 0, 0, 0, 0, 0, 0, 1 },
	{ -3, 0, 0, 0, 0, 0, 1, 2 },
	{ -4, 0, 0, 0, 0, 0, 1, 2 },
	{ -4, 0, 0, 0, 0, 0, 1, 2 }
};

static const uint16_t SBC_SAMPLE_RATES[4] = { 16000, 32000, 44100, 48000 };

uint16_t sbc_common_sample_rate_get(uint32_t idx)
{
    return SBC_SAMPLE_RATES[idx];
}

static uint32_t bk_sbc_decoder_get_mem0_addr(uint32_t **mem0_addr)
{
	*mem0_addr = (uint32_t *)SBC_SBC_MEM0_ADDR;

	return BK_OK;
}

static uint32_t sbc_decoder_frame_length_calc(sbcdecodercontext_t *sbc)
{
	uint32_t frame_length = 0;
	uint32_t blocks       = (uint32_t)sbc->frame.blocks;
	uint32_t subbands     = (uint32_t)sbc->frame.subbands;
	uint32_t num_channels = (uint32_t)sbc->channel_number;

	frame_length = 4 + ((4 * subbands * num_channels) >> 3);

	//for MONO and DUAL_CHANNEL
	if(sbc->frame.channel_mode < 2) {
	    frame_length += (blocks * num_channels * (uint32_t)(sbc->frame.bitpool) + 7) >> 3;
	}
	//for STEREO and JOINT_STEREO
	else {
	    frame_length += ((sbc->frame.channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO) * subbands + blocks * ((uint32_t)sbc->frame.bitpool) + 7) >> 3;
	}

	return frame_length;

}

bk_err_t bk_sbc_decoder_frame_decode(sbcdecodercontext_t *sbc, const uint8_t *data, uint32_t length)
{
	int32_t channel, subband, block, consumed;
	uint32_t *mem0_addr = NULL;
	uint32_t pcm_data = 0;
	sbc_config_t sbc_config;

	sbccommoncontext_t *frame = &sbc->frame;
	
	bk_sbc_decoder_get_mem0_addr(&mem0_addr);

	/* frame_header */
	switch(data[0])
	{
		case SBC_SYNCWORD:
			frame->sample_rate_index = (data[1] >> 6) & 0x03;
			frame->blocks			 = (((data[1] >> 4) & 0x03) + 1) << 2;
			frame->channel_mode      = (data[1] >> 2) & 0x03;
			frame->allocation_method = (data[1] >> 1) & 0x01;
			frame->subbands		     = ((data[1] & 0x01) + 1) << 2;
			frame->bitpool 		     = data[2];
			break;

		case MSBC_SYNCWORD:
			frame->blocks = 15;
			if(data[1] || data[2]) {
				frame->sample_rate_index = (data[1] >> 6) & 0x03;
				frame->channel_mode	     = (data[1] >> 2) & 0x03;
				frame->allocation_method = (data[1] >> 1) & 0x01;
				frame->subbands		     = ((data[1] & 0x01) + 1) << 2;
				frame->bitpool 		     = data[2];
			} else {
				frame->sample_rate_index = 0;
				frame->channel_mode	     = 0;
				frame->allocation_method = 0;
				frame->subbands		     = 8;
				frame->bitpool 		     = 26;
			}
			break;

		default:
			return SBC_DECODER_ERROR_SYNC_INCORRECT;
	}

	sbc->channel_number = (frame->channel_mode == SBC_CHANNEL_MODE_MONO) ? 1 : 2;
	sbc->pcm_length     = frame->blocks * frame->subbands;
	sbc->sample_rate    = sbc_common_sample_rate_get(frame->sample_rate_index);

	if (sbc_decoder_frame_length_calc(sbc) > length) {
		return SBC_DECODER_ERROR_STREAM_EMPTY;
	}

	if (((frame->channel_mode == SBC_CHANNEL_MODE_MONO || frame->channel_mode == SBC_CHANNEL_MODE_DUAL) && (frame->bitpool > (frame->subbands << 4))) ||
		((frame->channel_mode == SBC_CHANNEL_MODE_STEREO || frame->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO) && (frame->bitpool > (frame->subbands << 5)))) {

		return SBC_DECODER_ERROR_BITPOOL_OUT_BOUNDS;
	}

	consumed = 32;

    if(frame->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO)
    {
        int32_t subband;
        uint32_t join = 0;

        for(subband = 0; subband < frame->subbands - 1; subband++)
        {
            join |= ((data[4] >> (7 - subband)) & 0x01) << subband;
        }

        frame->join = (uint8_t)join;

        consumed += frame->subbands;
    }

	/* scale_factor */
	for(channel = 0; channel < sbc->channel_number; channel++)
	{
		int8_t *scale_factor = frame->scale_factor[channel];

		for(subband = 0; subband < frame->subbands; subband++)
		{
			scale_factor[subband] = (data[consumed >> 3] >> (4 - (consumed & 0x7))) & 0x0F;
			consumed += 4;
		}
	}
	
	/* bit_allocation */
	bk_sbc_decoder_bit_allocation(frame);	

	/* subband Level */ 
	for(channel = 0; channel < sbc->channel_number; channel++)
	{
		int8_t *bits	= frame->bits[channel];
		int32_t *levels = frame->mem[channel];

		for(subband = 0; subband < frame->subbands; subband++)
		{
			levels[subband] = (0x1 << bits[subband]) - 1;
		}
	}

	/* audio_samples & reconstruction */
	for(block = 0; block < frame->blocks; block++)
	{
		for(channel = 0; channel < sbc->channel_number; channel++)
		{
			int8_t *bits = frame->bits[channel];
			uint8_t *scale_factor = (uint8_t *)frame->scale_factor[channel];
			uint32_t *levels = (uint32_t *)frame->mem[channel];
			int32_t *pcm;

			bk_sbc_decoder_get_mem0_addr(&mem0_addr);
			pcm = (int32_t *)(mem0_addr + channel * sbc->pcm_length + block * frame->subbands);
			for(subband = 0; subband < frame->subbands; subband++)
			{
				if (levels[subband] > 0) {
					int32_t consumed_byte = consumed >> 3;
					bk_sbc_decoder_set_sbc_bit_num(bits[subband]);
					bk_sbc_decoder_set_scale_factor(scale_factor[subband]);
					bk_sbc_decoder_set_sbc_level(levels[subband]);
					bk_sbc_decoder_set_sbc_res_bit(consumed & 0x7);
					sbc_decoder_hal_set_res_bytel_value(data[consumed_byte]);
					sbc_decoder_hal_set_res_bytem_value(data[consumed_byte + 1]);
					sbc_decoder_hal_set_res_byteh_value(data[consumed_byte + 2]);
					bk_sbc_decoder_start_decode();
					while(bk_sbc_decoder_get_decode_enable_value());
					pcm_data = bk_sbc_decoder_get_pcm_data();
					*pcm++ = pcm_data;
					consumed = consumed + bits[subband];
				} else {
					*pcm++ = 0;
				}
			}
		}
	}

	/* joint_stereo */
	if (frame->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO) {
		int32_t idx, t0, t1;
		int32_t *pcm0;
		int32_t *pcm1;

		pcm0 = (int32_t *)(mem0_addr);
		pcm1 = (int32_t *)(mem0_addr + sbc->pcm_length);

		for(block = 0; block < frame->blocks; block++)
		{
			idx = block * frame->subbands;

			for(subband = 0; subband < frame->subbands; subband++)
			{
				if (frame->join & (0x01 << subband)) {
					t0 = pcm0[idx];
					t1 = pcm1[idx];
					pcm0[idx] = t0 + t1;
					pcm1[idx] = t0 - t1;
				}
				idx++;
			}
		}
	}

	/* padding */
	consumed = ((consumed + 7) & 0xFFFFFFF8) >> 3;

	sbc_config.channel_num = sbc->channel_number == 2 ? SBC_CHANNEL_NUM_TWO : SBC_CHANNEL_NUM_ONE;
	sbc_config.subbands = frame->subbands == 8 ? SBC_SUBBAND_NUMBER_8 : SBC_SUBBAND_NUMBER_4;
	sbc_config.chn_comb = (sbc->channel_number == 2 ? SBC_DECODE_OUTPUT_DOUBLE : SBC_DECODE_OUTPUT_SINGLE);
	sbc_config.blocks = frame->blocks / 4 - 1;

	sbc_decoder_hal_sbc_config(&sbc_config);

	if (frame->blocks == 15) {
		bk_sbc_decoder_support_msbc(1);
	} else {
		bk_sbc_decoder_support_msbc(0);
	}

	bk_sbc_decoder_sbc_enable();
	while(bk_sbc_decoder_get_sbc_enable_status());

	if (sbc->channel_number == 2) {
		uint32_t i;
		int32_t *src ;
		src = (int32_t *)mem0_addr;
		int32_t *dst = (int32_t *)sbc->pcm_sample;

		for(i = 0; i < sbc->pcm_length; i++)
		{
		    *dst++ = *src++;
		}
	} else {
		uint32_t i;
		int32_t *src;
		src = (int32_t *)mem0_addr;
		int16_t *dst = (int16_t*)sbc->pcm_sample;

		for(i = 0; i < sbc->pcm_length; i++)
		{
		    *dst++ = *src++;
		}
	}
//	SBC_LOGI("consumed = %d\r\n", consumed);

	return consumed;

}

bk_err_t bk_sbc_decoder_bit_allocation(sbccommoncontext_t *sbc)
{
	int32_t channel, subband, slicecount, bitcount, bitslice, loudness, max_bitneed;

	int8_t *scale_factor = NULL;
	int8_t *bits = NULL;
	int32_t *bitneed = NULL;

	if ((sbc->channel_mode == SBC_CHANNEL_MODE_MONO) || (sbc->channel_mode == SBC_CHANNEL_MODE_DUAL)) {
		for(channel = 0; channel < sbc->channel_mode + 1; channel++)
		{
			scale_factor = sbc->scale_factor[channel];
			bits         = sbc->bits[channel];
			bitneed 	 = sbc->mem[channel];

			max_bitneed = 0;

			if (sbc->allocation_method == SBC_ALLOCATION_METHOD_SNR) {
				for(subband = 0; subband < sbc->subbands; subband++)
				{
					bitneed[subband] = scale_factor[subband];
					if(bitneed[subband] > max_bitneed) {
						max_bitneed = bitneed[subband];
					}	
				}

			} else {
				uint8_t sample_rate_index = sbc->sample_rate_index;

				for(subband = 0; subband < sbc->subbands; subband++)
				{
					if (scale_factor[subband] == 0) {
						bitneed[subband] = -5;
					} else {
						if (sbc->subbands == 4) {
							loudness = scale_factor[subband] - SBC_COMMON_OFFSET4[sample_rate_index][subband];
	                    } else {
							loudness = scale_factor[subband] - SBC_COMMON_OFFSET8[sample_rate_index][subband];
						}

						if (loudness > 0) {
							bitneed[subband] = loudness / 2;
						} else {
							bitneed[subband] = loudness;
						}
					}

					if (bitneed[subband] > max_bitneed) {
						max_bitneed = bitneed[subband];
					}
				}
			}

			bitcount   = 0;
			slicecount = 0;
			bitslice   = max_bitneed + 1;

			do
			{
				bitslice--;
				bitcount += slicecount;
				slicecount = 0;

				for(subband = 0; subband < sbc->subbands; subband++)
				{
					if ((bitneed[subband] > bitslice + 1) && (bitneed[subband] < bitslice + 16)) {
						slicecount++;
					} else if(bitneed[subband] == bitslice + 1) {
						slicecount += 2;
					}
				}
			} while(bitcount + slicecount < sbc->bitpool);
			
			if (bitcount + slicecount == sbc->bitpool) {
				bitcount += slicecount;
				bitslice--;
			}

			for(subband = 0; subband < sbc->subbands; subband++)
			{
				if (bitneed[subband] < bitslice + 2) {
					bits[subband] = 0;
				} else {
					bits[subband] = bitneed[subband] - bitslice;
					if (bits[subband] > 16) {
						bits[subband] = 16;
					}
				}
			}

			for(subband = 0; bitcount < sbc->bitpool && subband < sbc->subbands; subband++)
			{
				if ((bits[subband] >= 2) && (bits[subband] < 16)) {
					bits[subband]++;
					bitcount++;
				} else if ((bitneed[subband] == bitslice + 1) && (sbc->bitpool > bitcount + 1)) {
					bits[subband] = 2;
					bitcount += 2;
				}
			}

			for(subband = 0; bitcount < sbc->bitpool && subband < sbc->subbands; subband++)
			{
				if (bits[subband] < 16) {
					bits[subband]++;
					bitcount++;
				}
			} 
		}
	}else if ((sbc->channel_mode == SBC_CHANNEL_MODE_STEREO) || (sbc->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO)) {
		max_bitneed = 0;
		if (sbc->allocation_method == SBC_ALLOCATION_METHOD_SNR) {
			for(channel = 0; channel < 2; channel++)
			{
				scale_factor = sbc->scale_factor[channel];
				bitneed = sbc->mem[channel];

				for(subband = 0; subband < sbc->subbands; subband++)
				{
					bitneed[subband] = scale_factor[subband];

					if (bitneed[subband] > max_bitneed) {
						max_bitneed = bitneed[subband];
					}
				}
			}
		} else {
			uint8_t sample_rate_index = sbc->sample_rate_index;
			for(channel = 0; channel < 2; channel++)
			{
				scale_factor = sbc->scale_factor[channel];
				bitneed = sbc->mem[channel];

				for(subband = 0; subband < sbc->subbands; subband++)
				{
					if (scale_factor[subband] == 0) {
						bitneed[subband] = -5;
					} else {
						if (sbc->subbands == 4) {
							loudness = scale_factor[subband] - SBC_COMMON_OFFSET4[sample_rate_index][subband];
						} else {
							loudness = scale_factor[subband] - SBC_COMMON_OFFSET8[sample_rate_index][subband];
						}

						if (loudness > 0) {
							bitneed[subband] = loudness / 2;
						} else {
							bitneed[subband] = loudness;
						}
					}

					if (bitneed[subband] > max_bitneed) {
						max_bitneed = bitneed[subband];
					}
				}
			}
		}

		bitcount = 0;
		slicecount = 0;
		bitslice = max_bitneed + 1;

		do
		{
			bitslice--;
			bitcount += slicecount;
			slicecount = 0;

			for(channel = 0; channel < 2; channel++)
			{
				bitneed = sbc->mem[channel];

				for(subband = 0; subband < sbc->subbands; subband++)
				{
					if ((bitneed[subband] > bitslice + 1) && (bitneed[subband] < bitslice + 16)) {
						slicecount++;
					} else if (bitneed[subband] == bitslice + 1) {
						slicecount += 2;
					}
				}
			}
		} while(bitcount + slicecount < sbc->bitpool);

		if (bitcount + slicecount == sbc->bitpool) {
			bitcount += slicecount;
			bitslice--;
		}

		for(channel = 0; channel < 2; channel++)
		{
			bits = sbc->bits[channel];
			bitneed = sbc->mem[channel];

			for(subband = 0; subband < sbc->subbands; subband++)
			{
				if (bitneed[subband] < bitslice + 2) {
					bits[subband] = 0;
				} else {
					bits[subband] = bitneed[subband] - bitslice;
					if (bits[subband] > 16) {
						bits[subband] = 16;
					}
				}
			}
		}

		subband = 0;

		while(bitcount < sbc->bitpool)
		{
			bits    = sbc->bits[0];
			bitneed = sbc->mem[0];

			if ((bits[subband] >= 2) && (bits[subband] < 16)) {
				bits[subband]++;
				bitcount++;
			} else if ((bitneed[subband] == bitslice + 1) && (sbc->bitpool > bitcount + 1)) {
				bits[subband]  = 2;
				bitcount += 2;
			}

			if (bitcount >= sbc->bitpool) {
				break;
			}

			bits    = sbc->bits[1];
			bitneed = sbc->mem[1];

			if ((bits[subband] >= 2) && (bits[subband] < 16)) {
				bits[subband]++;
				bitcount++;
			} else if ((bitneed[subband] == bitslice + 1) && (sbc->bitpool > bitcount + 1)) {
				bits[subband]  = 2;
				bitcount += 2;
			}

			if (++subband >= sbc->subbands) {
			    break;
			}
        }

		subband = 0;

		while(bitcount < sbc->bitpool)
		{
			bits = sbc->bits[0];

			if (bits[subband] < 16) {
				bits[subband]++;

				if (++bitcount >= sbc->bitpool) {
			    	break;
				}
			}

			bits = sbc->bits[1];

			if (bits[subband] < 16) {
				bits[subband]++;
				bitcount++;
			}

			if (++subband >= sbc->subbands) {
				break;
			}
		}
	}

	return BK_OK;
}

bk_err_t bk_sbc_decoder_init(sbcdecodercontext_t *sbc)
{
	uint32_t channel, i;

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_SBC, PM_POWER_MODULE_STATE_ON);

	sbc_decoder_hal_soft_reset();

	bk_sbc_decoder_mem_init();
	os_memset((void *)sbc, 0, sizeof(sbcdecodercontext_t));

	for(channel = 0; channel < 2; channel++)
	{
		for(i = 0; i < 8 * 2; i++)
		{
			sbc->offset[channel][i] = 10 * i + 10;
		}
	}

	return BK_OK;
}

bk_err_t bk_sbc_decoder_deinit(void)
{
	sbc_config_t sbc_config;
	
	sbc_config.blocks = SBC_BLOCK_NUMBER_4;
	sbc_config.channel_num = SBC_CHANNEL_NUM_ONE;
	sbc_config.chn_comb = SBC_DECODE_OUTPUT_SINGLE;
	sbc_config.subbands = SBC_SUBBAND_NUMBER_4;

	sbc_decoder_hal_sbc_config(&sbc_config);
	
	bk_sbc_decoder_set_sbc_bit_num(0);
	bk_sbc_decoder_set_scale_factor(0);
	bk_sbc_decoder_set_sbc_level(0);
	bk_sbc_decoder_set_sbc_res_bit(0);
	bk_sbc_decoder_set_res_bytel_value(0);
	bk_sbc_decoder_set_res_bytem_value(0);
	bk_sbc_decoder_set_res_byteh_value(0);

	bk_sbc_decoder_mem_init();

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_SBC, PM_POWER_MODULE_STATE_OFF);
	return BK_OK;
}

bk_err_t bk_sbc_decoder_mem_init(void)
{
	sbc_decoder_hal_mem_init();

	return BK_OK;
}

bk_err_t bk_sbc_decoder_sbc_enable(void)
{
	sbc_decoder_hal_sbc_enable();

	return BK_OK;
}

bk_err_t bk_sbc_decoder_get_sbc_enable_status(void)
{
	return sbc_decoder_hal_get_sbc_status_value();
}

bk_err_t bk_sbc_decoder_interrupt_enable(bool enable)
{
	if(enable) {
		bk_int_isr_register(INT_SRC_SBC, sbc_decoder_isr, NULL);
		sys_drv_sbc_int_en(1);

	} else {
		bk_int_isr_unregister(INT_SRC_SBC);
		sys_drv_sbc_int_en(0);
	}

	return BK_OK;
}

bk_err_t bk_sbc_decoder_support_msbc(bool enable)
{
	sbc_decoder_hal_sbc_support_msbc(enable);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_get_sbc_ctrl_value(void)
{
	return sbc_decoder_hal_get_sbc_ctrl_value(); 
}

bk_err_t bk_sbc_decoder_clear_interrupt_status(void)
{
	sbc_decoder_hal_sbc_clear_interrupt_status();
	return BK_OK;
}

bk_err_t bk_sbc_decoder_get_interrupt_status(void)
{
	return sbc_decoder_hal_get_sbc_interrupt_status();
}

bk_err_t bk_sbc_decoder_get_idle_status(void)
{
	return sbc_decoder_hal_get_sbc_idle_status();
}

bk_err_t bk_sbc_decoder_set_res_bytel_value(uint32_t resl_value)
{
	sbc_decoder_hal_set_res_bytel_value(resl_value);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_set_res_bytem_value(uint32_t resm_value)
{
	sbc_decoder_hal_set_res_bytem_value(resm_value);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_set_res_byteh_value(uint32_t resh_value)
{
	sbc_decoder_hal_set_res_byteh_value(resh_value);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_set_sbc_bit_num(uint32_t bit_num)
{
	sbc_decoder_hal_set_sbc_bit_num(bit_num);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_set_scale_factor(uint32_t sf)
{
	sbc_decoder_hal_set_scale_factor(sf);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_set_sbc_level(uint32_t level)
{
	sbc_decoder_hal_set_sbc_level(level);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_set_sbc_res_bit(uint32_t res_bit)
{
	sbc_decoder_hal_set_sbc_res_bit(res_bit);

	return BK_OK;
}

bk_err_t bk_sbc_decoder_start_decode(void)
{
	sbc_decoder_hal_decode_enable();

	return BK_OK;
}

uint32_t bk_sbc_decoder_get_decode_enable_value(void)
{
	return sbc_decoder_hal_get_decode_enable_value();
}

bk_err_t bk_sbc_decoder_get_pcm_data(void)
{
	return sbc_decoder_hal_get_pcm_data();
}

bk_err_t bk_sbc_decoder_register_sbc_isr(sbc_decoder_isr_t isr, void *param)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_sbc_decoder_isr.callback = isr;
	s_sbc_decoder_isr.param = param;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

static void sbc_decoder_isr(void)
{
	uint32_t int_status = 0;
	int_status = bk_sbc_decoder_get_interrupt_status();

	if (int_status) {
		bk_sbc_decoder_clear_interrupt_status();
		if (s_sbc_decoder_isr.callback) {
			s_sbc_decoder_isr.callback(s_sbc_decoder_isr.param);
		}
	}
}


