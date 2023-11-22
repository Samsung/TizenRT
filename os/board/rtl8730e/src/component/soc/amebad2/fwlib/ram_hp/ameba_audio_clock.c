#include "ameba_soc.h"
#include "ameba_audio_clock.h"

static const char *TAG = "AUDIO";
const u32 i2s_clock_source[4] = {PLL_CLOCK_98P304M, PLL_CLOCK_24P576M, PLL_CLOCK_45P1584M, I2S_CLOCK_XTAL40M};
const u32 mclk_div[3] = {1, 2, 4};

/**
  * @brief  Determine whether the clock can be divided normally.
  * @param	clock: audio clock.
  * @param  sr: sport sample rate.
  * @param  chn_len: sport channel length.
  * @param  chn_cnt: sport channel number.
  * @retval None
  */
bool is_sport_ni_mi_supported(u32 clock, u32 sr, u32 chn_len, u32 chn_cnt)
{
	int ni = 1;
	int mi = 1;
	int max_ni = 32767;
	int max_mi = 65535;
	bool ni_mi_found = 0;

	for (; ni <= max_ni ; ni++) {
		if (clock * ni % (chn_cnt * chn_len * sr) == 0) {
			mi = clock * ni / (chn_cnt * chn_len * sr);
			//printf("check founded: ni : %d, mi: %d \n", ni, mi);
			if (mi < 2 * ni) {
				//printf("mi <= ni, check fail, try another pll divider or sport mclk divider \n");
				break;
			}
			if (mi <= max_mi) {
				//printf("check ni : %d, mi: %d success\n", ni, mi);
				ni_mi_found = 1;
				break;
			}
		}
	}


	return ni_mi_found;
}

/**
  * @brief  Choose the appropriate clock according to the corresponding audio parameters.
  * @param  clock_sel: select pll clock or xtal clock.
  * @param  initparams: i2s init parameters: sr,channel length, channel number, codec_multiplier_with_rate and sport_mclk_fixed_max.
  * @param  params: audio clock parameters: clock, pll divider and mclk divider.
  * @retval None
  */
void Audio_Clock_Choose(u32 clock_sel, AUDIO_InitParams *initparams, AUDIO_ClockParams *params)
{
	u32 sport_mclk_div_index = 0;
	u32 clock_index = 0;
	u32 pll_div = 1;
	bool choose_done = 0;

	if (clock_sel == PLL_CLK) {
		if (initparams->codec_multiplier_with_rate && initparams->sport_mclk_fixed_max) {
			printf("not supported multi\n");
		}

		// external codec needs mclk to be codec_multiplier_with_rate * fs.
		if (initparams->codec_multiplier_with_rate) {
			for (; sport_mclk_div_index < SPORT_MCLK_DIV_MAX_NUM; sport_mclk_div_index++) {
				for (; clock_index < PLL_CLOCK_MAX_NUM; clock_index++) {
					if ((i2s_clock_source[clock_index] / mclk_div[sport_mclk_div_index]) % (initparams->codec_multiplier_with_rate * initparams->sr) != 0) {
						continue;
					} else {
						pll_div = (i2s_clock_source[clock_index] / mclk_div[sport_mclk_div_index]) / (initparams->codec_multiplier_with_rate * initparams->sr);
						if (pll_div <= 8 && is_sport_ni_mi_supported((i2s_clock_source[clock_index] / pll_div), initparams->sr, initparams->chn_len, initparams->chn_cnt)) {
							choose_done = 1;
							break;
						} else {
							continue;
						}
					}
				}
				if (choose_done) {
					break;
				}

				clock_index = 0;
			}
		}

		// external codec needs mclk to be less than sport_mclk_fixed_max.
		if (initparams->sport_mclk_fixed_max) {
			if (!(initparams->sr % 4000)) {
				for (; clock_index < PLL_CLOCK_MAX_NUM; clock_index++) {
					for (pll_div = 1; pll_div < MAX_PLL_DIV + 1; pll_div++) {
						for (; sport_mclk_div_index < SPORT_MCLK_DIV_MAX_NUM; sport_mclk_div_index++) {
							if (i2s_clock_source[clock_index] / pll_div / mclk_div[sport_mclk_div_index] <= initparams->sport_mclk_fixed_max) {
								if (is_sport_ni_mi_supported((i2s_clock_source[clock_index] / pll_div), initparams->sr, initparams->chn_len, initparams->chn_cnt)) {
									choose_done = 1;
									goto end;
								} else {
									continue;
								}
							} else {
								continue;
							}
						}
						sport_mclk_div_index = 0;
					}
				}
			} else if (!(initparams->sr % 11025)) {
				for (pll_div = 1; pll_div < MAX_PLL_DIV + 1; pll_div++) {
					for (; sport_mclk_div_index < SPORT_MCLK_DIV_MAX_NUM; sport_mclk_div_index++) {
						if (PLL_CLOCK_45P1584M / pll_div / mclk_div[sport_mclk_div_index] <= initparams->sport_mclk_fixed_max) {
							if (is_sport_ni_mi_supported((PLL_CLOCK_45P1584M), initparams->sr, initparams->chn_len, initparams->chn_cnt)) {
								choose_done = 1;
								clock_index = 2;
								goto end;
							} else {
								continue;
							}
						} else {
							continue;
						}
					}

					sport_mclk_div_index = 0;
				}
			}
		}

end:
		if (!choose_done) {
			RTK_LOGE(TAG, "can't find proper clock for the current rate:%d \n", initparams->sr);
			return;
		}

	} else {
		clock_index = 3;
		pll_div = 1;
		sport_mclk_div_index = 2;
	}

	params->Clock = i2s_clock_source[clock_index];
	params->PLL_DIV = pll_div;
	params->MCLK_DIV = mclk_div[sport_mclk_div_index];

}


