#ifndef _AMEBAD2_AUDIO_CLOCK_H_
#define _AMEBAD2_AUDIO_CLOCK_H_

typedef struct {
	u32 Clock;
	u32 PLL_DIV;
	u32 MCLK_DIV;
} AUDIO_ClockParams;

typedef struct {
	u32 sr;
	u32 chn_len;
	u32 chn_cnt;
	u32 codec_multiplier_with_rate;
	u32 sport_mclk_fixed_max;
} AUDIO_InitParams;

#define PLL_CLK    0
#define XTAL_CLOCK 1

#define PLL_CLOCK_24P576M	24576000
#define PLL_CLOCK_45P1584M	45158400
#define PLL_CLOCK_98P304M	98304000
#define I2S_CLOCK_XTAL40M	40000000

#define SPORT_MCLK_DIV_MAX_NUM	3
#define PLL_CLOCK_MAX_NUM	    3
#define MAX_PLL_DIV	            8


/** @defgroup AUDIO_CLOCK_Exported_Functions
  * @{
  */

bool is_sport_ni_mi_supported(u32 clock, u32 sr, u32 chn_len, u32 chn_cnt);
void Audio_Clock_Choose(u32 clock_sel, AUDIO_InitParams *initparams, AUDIO_ClockParams *params);


/**
* @}
*/


#endif


