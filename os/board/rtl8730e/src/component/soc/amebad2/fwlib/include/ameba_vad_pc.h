#ifndef _AMEBAD2_VAD_PC_H_
#define _AMEBAD2_VAD_PC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup VAD
  * @brief VAD driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup VAD_Register_Definitions VAD Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup VAD_CODEC_CTRLx
 * @brief
 * @{
 **/
#define VAD_BIT_PC_ADC_x_DMIC_MIX_MUTE            ((u32)0x00000001 << 27)          /*!<R/W 0x1  DMIC input path mute control 0: Un-Mute 1: Mute */
#define VAD_MASK_PC_ADC_x_DMIC_LPF2ND_FC_SEL      ((u32)0x00000003 << 25)          /*!<R/W 0x1  DMIC SRC 2nd LPF fc (-3dB) */
#define VAD_PC_ADC_x_DMIC_LPF2ND_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 25))
#define VAD_GET_PC_ADC_x_DMIC_LPF2ND_FC_SEL(x)    ((u32)(((x >> 25) & 0x00000003)))
#define VAD_MASK_PC_ADC_x_DMIC_LPF1ST_FC_SEL      ((u32)0x00000007 << 22)          /*!<R/W 0x5  DMIC SRC 1st LPF fc (-3dB) */
#define VAD_PC_ADC_x_DMIC_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000007) << 22))
#define VAD_GET_PC_ADC_x_DMIC_LPF1ST_FC_SEL(x)    ((u32)(((x >> 22) & 0x00000007)))
#define VAD_BIT_PC_ADC_x_DMIC_LPF1ST_EN           ((u32)0x00000001 << 21)          /*!<R/W 0x1  DMIC SRC in AD filter 1st LPF control 0: disable 1: enable */
#define VAD_MASK_PC_ADC_x_DMIC_BOOST_GAIN         ((u32)0x00000003 << 19)          /*!<R/W 0x0  ADC digital boost gain 2'b00: 0dB 2'b01: 10dB 2'b10: 20dB 2'b11: 30dB */
#define VAD_PC_ADC_x_DMIC_BOOST_GAIN(x)           ((u32)(((x) & 0x00000003) << 19))
#define VAD_GET_PC_ADC_x_DMIC_BOOST_GAIN(x)       ((u32)(((x >> 19) & 0x00000003)))
#define VAD_MASK_PC_ADC_x_AD_ZDET_TOUT            ((u32)0x00000003 << 17)          /*!<R/W 0x0  ADC zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define VAD_PC_ADC_x_AD_ZDET_TOUT(x)              ((u32)(((x) & 0x00000003) << 17))
#define VAD_GET_PC_ADC_x_AD_ZDET_TOUT(x)          ((u32)(((x >> 17) & 0x00000003)))
#define VAD_MASK_PC_ADC_x_AD_ZDET_FUNC            ((u32)0x00000003 << 15)          /*!<R/W 0x2  ADC zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zero detection & increase/decrease change 2'b11: unchange */
#define VAD_PC_ADC_x_AD_ZDET_FUNC(x)              ((u32)(((x) & 0x00000003) << 15))
#define VAD_GET_PC_ADC_x_AD_ZDET_FUNC(x)          ((u32)(((x >> 15) & 0x00000003)))
#define VAD_BIT_PC_ADC_x_AD_MUTE                  ((u32)0x00000001 << 14)          /*!<R/W 0x1  Digital Mute at Digital Volme 0: Un-Mute 1: Mute */
#define VAD_BIT_PC_ADC_x_AD_MIX_MUTE              ((u32)0x00000001 << 13)          /*!<R/W 0x1  analog ADC input path mute control 0: Un-Mute 1: Mute */
#define VAD_MASK_PC_ADC_x_AD_HPF_COEF             ((u32)0x00000007 << 10)          /*!<R/W 0x0  High pass filter coefficient selection (3dB point/sampling rate) 3'b000: 2^-13 (0.3Hz/16k) 3'b001: 2^-12 (0.6Hz/16k) 3'b010: 2^-11 (1.2Hz/16k) 3'b011: 2^-10 (2.5Hz/16k) 3'b100: 2^-9 (5Hz/16k) 3'b101: 2^-8 (10Hz/16k) 3'b110: 2^-7 (20Hz/16k) 3'b111: 2^-6 (40Hz/16k) */
#define VAD_PC_ADC_x_AD_HPF_COEF(x)               ((u32)(((x) & 0x00000007) << 10))
#define VAD_GET_PC_ADC_x_AD_HPF_COEF(x)           ((u32)(((x >> 10) & 0x00000007)))
#define VAD_MASK_PC_ADC_x_AD_GAIN                 ((u32)0x0000007F << 3)          /*!<R/W 0x2F  ADC digital volume in 0.375dB step, -17.625dB ~ 30dB 7'h00: -17.625dB ... 7'h2f: 0dB 7'h30: 0.375dB ... 7'h7f: 30dB */
#define VAD_PC_ADC_x_AD_GAIN(x)                   ((u32)(((x) & 0x0000007F) << 3))
#define VAD_GET_PC_ADC_x_AD_GAIN(x)               ((u32)(((x >> 3) & 0x0000007F)))
#define VAD_BIT_PC_ADC_x_AD_DCHPF_EN              ((u32)0x00000001 << 2)          /*!<R/W 0x1  High pass filter enable control 1'b0: disable 1'b1: enable */
#define VAD_MASK_PC_ADC_x_AD_COMP_GAIN            ((u32)0x00000003 << 0)          /*!<R/W 0x0  ADC compensate gain 2'b00: 0dB 2'b01: 1dB 2'b10: 2dB 2'b11: 3dB */
#define VAD_PC_ADC_x_AD_COMP_GAIN(x)              ((u32)(((x) & 0x00000003) << 0))
#define VAD_GET_PC_ADC_x_AD_COMP_GAIN(x)          ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL0
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_LT_UPDATE_COUNTER_THR  ((u32)0x0000000F << 28)          /*!<R/W 0x7  Counter threshold of long_term energy to update no speech segment */
#define VAD_PITCH_DET_LT_UPDATE_COUNTER_THR(x)    ((u32)(((x) & 0x0000000F) << 28))
#define VAD_GET_PITCH_DET_LT_UPDATE_COUNTER_THR(x) ((u32)(((x >> 28) & 0x0000000F)))
#define VAD_MASK_PITCH_DET_LT_PARAM_A_SEL         ((u32)0x00000003 << 26)          /*!<R/W 0x1  The coefficient of long_term energy calculation 0: 2^-4 1: 2^-5 2: 2^-6 3: 2^-4 */
#define VAD_PITCH_DET_LT_PARAM_A_SEL(x)           ((u32)(((x) & 0x00000003) << 26))
#define VAD_GET_PITCH_DET_LT_PARAM_A_SEL(x)       ((u32)(((x >> 26) & 0x00000003)))
#define VAD_MASK_PITCH_DET_INITIAL_TIME           ((u32)0x0000003F << 20)          /*!<R/W 0x20  Initial time after rst_n check time = REG * 4ms */
#define VAD_PITCH_DET_INITIAL_TIME(x)             ((u32)(((x) & 0x0000003F) << 20))
#define VAD_GET_PITCH_DET_INITIAL_TIME(x)         ((u32)(((x >> 20) & 0x0000003F)))
#define VAD_MASK_PITCH_DET_BG_CHK_THR             ((u32)0x00000007 << 17)          /*!<R/W 0x3  The threshold for checking the potential energy variation in background. */
#define VAD_PITCH_DET_BG_CHK_THR(x)               ((u32)(((x) & 0x00000007) << 17))
#define VAD_GET_PITCH_DET_BG_CHK_THR(x)           ((u32)(((x >> 17) & 0x00000007)))
#define VAD_MASK_PITCH_DET_BG_CHK_INTERVAL        ((u32)0x0000FFFF << 1)          /*!<R/W 0xC8  The time for checking the energy variation in background when potential energy variation detected check time = REG * 4ms */
#define VAD_PITCH_DET_BG_CHK_INTERVAL(x)          ((u32)(((x) & 0x0000FFFF) << 1))
#define VAD_GET_PITCH_DET_BG_CHK_INTERVAL(x)      ((u32)(((x >> 1) & 0x0000FFFF)))
#define VAD_BIT_PITCH_DET_6_9_EN                  ((u32)0x00000001 << 0)          /*!<R/W 0x0  Enable signale for selection of the harmonics pattern 6&9 0: disable 1: enbale */
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL1
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_BG_VAR_TIME            ((u32)0x0000FFFF << 16)          /*!<R/W 0x4E2  The accumulated time when pitch_det_bg_chk_interval keeps checking check time = REG * 4ms */
#define VAD_PITCH_DET_BG_VAR_TIME(x)              ((u32)(((x) & 0x0000FFFF) << 16))
#define VAD_GET_PITCH_DET_BG_VAR_TIME(x)          ((u32)(((x >> 16) & 0x0000FFFF)))
#define VAD_MASK_PITCH_DET_BG_STABLE_TIME         ((u32)0x0000FFFF << 0)          /*!<R/W 0x3E8  The check time when there is no potential energy variation in background. check time = REG * 4ms */
#define VAD_PITCH_DET_BG_STABLE_TIME(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define VAD_GET_PITCH_DET_BG_STABLE_TIME(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL2
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_NO_SPEECH_COUNTER_THR  ((u32)0x000001FF << 23)          /*!<R/W 0x1F3  Counter threshold to monitor how long there is no speech occurrence */
#define VAD_PITCH_DET_NO_SPEECH_COUNTER_THR(x)    ((u32)(((x) & 0x000001FF) << 23))
#define VAD_GET_PITCH_DET_NO_SPEECH_COUNTER_THR(x) ((u32)(((x >> 23) & 0x000001FF)))
#define VAD_MASK_PITCH_DET_MV_THR                 ((u32)0x0000001F << 18)          /*!<R/W 0x8  Threshold of majority vote to determine whether the speech features appear range: 4'h0 ~ 4'h13 */
#define VAD_PITCH_DET_MV_THR(x)                   ((u32)(((x) & 0x0000001F) << 18))
#define VAD_GET_PITCH_DET_MV_THR(x)               ((u32)(((x >> 18) & 0x0000001F)))
#define VAD_MASK_PITCH_DET_LT_UPDATE_MODE         ((u32)0x00000003 << 16)          /*!<R/W 0x2  3 modes for lt_update 0: always update 1: for RTL simulation 2: follow initial procedural 3: follow initial procedural */
#define VAD_PITCH_DET_LT_UPDATE_MODE(x)           ((u32)(((x) & 0x00000003) << 16))
#define VAD_GET_PITCH_DET_LT_UPDATE_MODE(x)       ((u32)(((x >> 16) & 0x00000003)))
#define VAD_BIT_PITCH_DET_ENERGY_MODE_MANUAL_EN   ((u32)0x00000001 << 15)          /*!<R/W 0x0  Enable signal of manual selection energy mode 0: disable 1: enable */
#define VAD_BIT_PITCH_DET_ENERGY_MODE_AUTO_EN     ((u32)0x00000001 << 14)          /*!<R/W 0x0  Enable signal of auto selection energy mode 0: disable 1: enable */
#define VAD_MASK_PITCH_DET_ENERGY_KEEP_THR        ((u32)0x000007FF << 3)          /*!<R/W 0x1C  The accumulated time when pitch_det_energy_chk_thr keeps checking check time = REG * 4ms */
#define VAD_PITCH_DET_ENERGY_KEEP_THR(x)          ((u32)(((x) & 0x000007FF) << 3))
#define VAD_GET_PITCH_DET_ENERGY_KEEP_THR(x)      ((u32)(((x >> 3) & 0x000007FF)))
#define VAD_MASK_PITCH_DET_ENERGY_CHK_THR         ((u32)0x00000007 << 0)          /*!<R/W 0x1  The threshold for checking the potential energy variation in voice */
#define VAD_PITCH_DET_ENERGY_CHK_THR(x)           ((u32)(((x) & 0x00000007) << 0))
#define VAD_GET_PITCH_DET_ENERGY_CHK_THR(x)       ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL3
 * @brief
 * @{
 **/
#define VAD_BIT_PITCH_DET_ONSET_HIGH_SEL_EN       ((u32)0x00000001 << 30)          /*!<R/W 0x1  Enable signal for onset_high function 0: disable 1: enable */
#define VAD_MASK_PITCH_DET_OD_THR_UPPERBOUND      ((u32)0x0000001F << 25)          /*!<R/W 0x14  Upperbound threshold of onset detection */
#define VAD_PITCH_DET_OD_THR_UPPERBOUND(x)        ((u32)(((x) & 0x0000001F) << 25))
#define VAD_GET_PITCH_DET_OD_THR_UPPERBOUND(x)    ((u32)(((x >> 25) & 0x0000001F)))
#define VAD_MASK_PITCH_DET_OD_THR_STEP            ((u32)0x0000001F << 20)          /*!<R/W 0x1  Threshold step for onset detection threshold update */
#define VAD_PITCH_DET_OD_THR_STEP(x)              ((u32)(((x) & 0x0000001F) << 20))
#define VAD_GET_PITCH_DET_OD_THR_STEP(x)          ((u32)(((x >> 20) & 0x0000001F)))
#define VAD_MASK_PITCH_DET_OD_THR_LOWERBOUND      ((u32)0x0000001F << 15)          /*!<R/W 0x2  Lowerbound threshold of onset detection */
#define VAD_PITCH_DET_OD_THR_LOWERBOUND(x)        ((u32)(((x) & 0x0000001F) << 15))
#define VAD_GET_PITCH_DET_OD_THR_LOWERBOUND(x)    ((u32)(((x >> 15) & 0x0000001F)))
#define VAD_MASK_PITCH_DET_OD_THR_ENERGY          ((u32)0x0000001F << 10)          /*!<R/W 0x0  Energy mode threshold of onset detection */
#define VAD_PITCH_DET_OD_THR_ENERGY(x)            ((u32)(((x) & 0x0000001F) << 10))
#define VAD_GET_PITCH_DET_OD_THR_ENERGY(x)        ((u32)(((x >> 10) & 0x0000001F)))
#define VAD_MASK_PITCH_DET_OD_THR_DEFAULT         ((u32)0x0000001F << 5)          /*!<R/W 0x6  Default threshold of onset detection */
#define VAD_PITCH_DET_OD_THR_DEFAULT(x)           ((u32)(((x) & 0x0000001F) << 5))
#define VAD_GET_PITCH_DET_OD_THR_DEFAULT(x)       ((u32)(((x >> 5) & 0x0000001F)))
#define VAD_MASK_PITCH_DET_OD_THR_BG_NOISE        ((u32)0x0000001F << 0)          /*!<R/W 0x4  bg_noise threshold of onset detection */
#define VAD_PITCH_DET_OD_THR_BG_NOISE(x)          ((u32)(((x) & 0x0000001F) << 0))
#define VAD_GET_PITCH_DET_OD_THR_BG_NOISE(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL4
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_QUASI_SPEECH_THR       ((u32)0x0000000F << 26)          /*!<R/W 0x5  Threshold to determine quasi speech occurrence range: 4'h0 ~ 4'h7 */
#define VAD_PITCH_DET_QUASI_SPEECH_THR(x)         ((u32)(((x) & 0x0000000F) << 26))
#define VAD_GET_PITCH_DET_QUASI_SPEECH_THR(x)     ((u32)(((x >> 26) & 0x0000000F)))
#define VAD_MASK_PITCH_DET_PEAK_SEARCH_THR2       ((u32)0x000003FF << 16)          /*!<R/W 0x80  Higher threshold for peak search */
#define VAD_PITCH_DET_PEAK_SEARCH_THR2(x)         ((u32)(((x) & 0x000003FF) << 16))
#define VAD_GET_PITCH_DET_PEAK_SEARCH_THR2(x)     ((u32)(((x >> 16) & 0x000003FF)))
#define VAD_MASK_PITCH_DET_PEAK_SEARCH_THR        ((u32)0x000003FF << 6)          /*!<R/W 0x40  Threshold for peak search */
#define VAD_PITCH_DET_PEAK_SEARCH_THR(x)          ((u32)(((x) & 0x000003FF) << 6))
#define VAD_GET_PITCH_DET_PEAK_SEARCH_THR(x)      ((u32)(((x >> 6) & 0x000003FF)))
#define VAD_MASK_PITCH_DET_ONSET_MASK             ((u32)0x0000003F << 0)          /*!<R/W 0x0  Force the onset output of each subband to 1 6'h00_0001: force the onset output of subband 1 to 1 6'h00_0010: force the onset output of subband 2 to 1 6'h00_0100: force the onset output of subband 3 to 1 6'h00_1000: force the onset output of subband 4 to 1 6'h01_0000: force the onset output of subband 5 to 1 6'h10_0000: force the onset output of subband 6 to 1 */
#define VAD_PITCH_DET_ONSET_MASK(x)               ((u32)(((x) & 0x0000003F) << 0))
#define VAD_GET_PITCH_DET_ONSET_MASK(x)           ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL5
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_ST_ABS_KEEP_THR        ((u32)0x000007FF << 21)          /*!<R/W 0x1C  The accumulated time when pitch_det_st_abs_chk_num keeps checking check time = REG * 4ms */
#define VAD_PITCH_DET_ST_ABS_KEEP_THR(x)          ((u32)(((x) & 0x000007FF) << 21))
#define VAD_GET_PITCH_DET_ST_ABS_KEEP_THR(x)      ((u32)(((x >> 21) & 0x000007FF)))
#define VAD_MASK_PITCH_DET_ST_ABS_CHK_NUM         ((u32)0x00000007 << 18)          /*!<R/W 0x3  The threshold for checking the absolute energy variation */
#define VAD_PITCH_DET_ST_ABS_CHK_NUM(x)           ((u32)(((x) & 0x00000007) << 18))
#define VAD_GET_PITCH_DET_ST_ABS_CHK_NUM(x)       ((u32)(((x >> 18) & 0x00000007)))
#define VAD_BIT_PITCH_DET_ST_7_C_6_EN             ((u32)0x00000001 << 17)          /*!<R/W 0x1  Enable control of comparison between st_6 & st_7 for high pitch case */
#define VAD_MASK_PITCH_DET_ST_4_7_VAR_THR         ((u32)0x0000007F << 10)          /*!<R/W 0x36  The threshold for checking the energy variation in subband 4~7 */
#define VAD_PITCH_DET_ST_4_7_VAR_THR(x)           ((u32)(((x) & 0x0000007F) << 10))
#define VAD_GET_PITCH_DET_ST_4_7_VAR_THR(x)       ((u32)(((x >> 10) & 0x0000007F)))
#define VAD_MASK_PITCH_DET_SPEECH_INTERVAL_THR    ((u32)0x000003FF << 0)          /*!<R/W 0x7D  The time interval for pitch_det_flag to decide the non-speech data after pitch_det_flag_hold is high check time = REG * 4ms */
#define VAD_PITCH_DET_SPEECH_INTERVAL_THR(x)      ((u32)(((x) & 0x000003FF) << 0))
#define VAD_GET_PITCH_DET_SPEECH_INTERVAL_THR(x)  ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL6
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_THR_ST_VALID           ((u32)0x00001FFF << 18)          /*!<R/W 0x3A  Threshold to determine the validation of short_term energy range: 2^-9 ~ 2^-21 */
#define VAD_PITCH_DET_THR_ST_VALID(x)             ((u32)(((x) & 0x00001FFF) << 18))
#define VAD_GET_PITCH_DET_THR_ST_VALID(x)         ((u32)(((x >> 18) & 0x00001FFF)))
#define VAD_BIT_PITCH_DET_THR_ONSET_ADAPT_EN      ((u32)0x00000001 << 17)          /*!<R/W 0x0  Enable signal of adaptive onset threshold update 0: disable, the value of onset threshold is fixed at pitch_det_od_thr_default 1: enable */
#define VAD_MASK_PITCH_DET_THR_LT_UPDATE          ((u32)0x0000000F << 13)          /*!<R/W 0x1  Threshold for long_term energy update */
#define VAD_PITCH_DET_THR_LT_UPDATE(x)            ((u32)(((x) & 0x0000000F) << 13))
#define VAD_GET_PITCH_DET_THR_LT_UPDATE(x)        ((u32)(((x >> 13) & 0x0000000F)))
#define VAD_MASK_PITCH_DET_ST_ABS_THR             ((u32)0x00001FFF << 0)          /*!<R/W 0x6A  The threshold of absolute energy range: 2^-9 ~ 2^-21 */
#define VAD_PITCH_DET_ST_ABS_THR(x)               ((u32)(((x) & 0x00001FFF) << 0))
#define VAD_GET_PITCH_DET_ST_ABS_THR(x)           ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL7
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_THR_ST_VALID_LB        ((u32)0x00001FFF << 17)          /*!<R/W 0x3A  The lower bound for auto thr_st_valid range: 2^-9 ~ 2^-21 */
#define VAD_PITCH_DET_THR_ST_VALID_LB(x)          ((u32)(((x) & 0x00001FFF) << 17))
#define VAD_GET_PITCH_DET_THR_ST_VALID_LB(x)      ((u32)(((x >> 17) & 0x00001FFF)))
#define VAD_MASK_PITCH_DET_THR_ST_VALID_HB        ((u32)0x00001FFF << 4)          /*!<R/W 0x1E00  The higher bound for auto thr_st_valid range: 2^-9 ~ 2^-21 */
#define VAD_PITCH_DET_THR_ST_VALID_HB(x)          ((u32)(((x) & 0x00001FFF) << 4))
#define VAD_GET_PITCH_DET_THR_ST_VALID_HB(x)      ((u32)(((x >> 4) & 0x00001FFF)))
#define VAD_BIT_PITCH_DET_THR_ST_VALID_AUTO_EN    ((u32)0x00000001 << 3)          /*!<R/W 0x1  Auto thr_st_valid enable control 0: disable 1: enable */
#define VAD_MASK_PITCH_DET_THR_ST_VALID_ADP_LV    ((u32)0x00000007 << 0)          /*!<R/W 0x4  The level control for auto thr_st_valid 0: *2 1: /1 2: /2 3: /4 4: /8 5: /16 6: /32 7: /64 */
#define VAD_PITCH_DET_THR_ST_VALID_ADP_LV(x)      ((u32)(((x) & 0x00000007) << 0))
#define VAD_GET_PITCH_DET_THR_ST_VALID_ADP_LV(x)  ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL8
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_XZERO_VALID_THR        ((u32)0x0000001F << 16)          /*!<R/W 0xC  The thr for the number of valid xzero frame range: 5'h0 ~ 5'h13 */
#define VAD_PITCH_DET_XZERO_VALID_THR(x)          ((u32)(((x) & 0x0000001F) << 16))
#define VAD_GET_PITCH_DET_XZERO_VALID_THR(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define VAD_BIT_PITCH_DET_XZERO_TONE_EN           ((u32)0x00000001 << 15)          /*!<R/W 0x1  Tone detection enable control 0: disable 1: enable */
#define VAD_BIT_PITCH_DET_XZERO_DET_EN            ((u32)0x00000001 << 14)          /*!<R/W 0x1  xzero detection enable control 0: disable 1: ebable */
#define VAD_MASK_PITCH_DET_XZERO_CYCLE_EQ_NUM     ((u32)0x00000007 << 11)          /*!<R/W 0x5  The number of equivalent cycles range: 3'h0 ~ 3'h5 */
#define VAD_PITCH_DET_XZERO_CYCLE_EQ_NUM(x)       ((u32)(((x) & 0x00000007) << 11))
#define VAD_GET_PITCH_DET_XZERO_CYCLE_EQ_NUM(x)   ((u32)(((x >> 11) & 0x00000007)))
#define VAD_MASK_PITCH_DET_XZERO_CYCLE_DIFF_THR   ((u32)0x00000007 << 8)          /*!<R/W 0x2  the tolerance of the difference among cycles */
#define VAD_PITCH_DET_XZERO_CYCLE_DIFF_THR(x)     ((u32)(((x) & 0x00000007) << 8))
#define VAD_GET_PITCH_DET_XZERO_CYCLE_DIFF_THR(x) ((u32)(((x >> 8) & 0x00000007)))
#define VAD_MASK_PITCH_DET_XZERO_CYCLE_CNT_LB     ((u32)0x000000FF << 0)          /*!<R/W 0x8  the lower bound for the cycle among xzeros */
#define VAD_PITCH_DET_XZERO_CYCLE_CNT_LB(x)       ((u32)(((x) & 0x000000FF) << 0))
#define VAD_GET_PITCH_DET_XZERO_CYCLE_CNT_LB(x)   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup VAD_PITCH_DET_CTRL9
 * @brief
 * @{
 **/
#define VAD_MASK_PITCH_DET_XZERO_LB_THR           ((u32)0x000001FF << 13)          /*!<R/W 0x3  the lower bound of valid number of xzero */
#define VAD_PITCH_DET_XZERO_LB_THR(x)             ((u32)(((x) & 0x000001FF) << 13))
#define VAD_GET_PITCH_DET_XZERO_LB_THR(x)         ((u32)(((x >> 13) & 0x000001FF)))
#define VAD_MASK_PITCH_DET_XZERO_HB_THR           ((u32)0x000001FF << 4)          /*!<R/W 0x8C  the higher bound of valid number of xzero */
#define VAD_PITCH_DET_XZERO_HB_THR(x)             ((u32)(((x) & 0x000001FF) << 4))
#define VAD_GET_PITCH_DET_XZERO_HB_THR(x)         ((u32)(((x >> 4) & 0x000001FF)))
#define VAD_BIT_PITCH_DET_CLR                     ((u32)0x00000001 << 1)          /*!<WA0 0x0  Clear vad_flag 0: Bypass vad flag 1: Clear vad flag (force output = 0) */
#define VAD_BIT_FLAG                              ((u32)0x00000001 << 0)          /*!<R 0x0  vad flag 0: detection on-going 1: hit */
/** @} */

/** @defgroup VAD_CLK_CTRL
 * @brief
 * @{
 **/
#define VAD_BIT_PC_TCON_DMIC_SRC_3_EN             ((u32)0x00000001 << 19)          /*!<R/W 0x0  ADC filter 3 dmic LPF control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_DMIC_SRC_2_EN             ((u32)0x00000001 << 18)          /*!<R/W 0x0  ADC filter 2 dmic LPF control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_DMIC_SRC_1_EN             ((u32)0x00000001 << 17)          /*!<R/W 0x0  ADC filter 1 dmic LPF control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_DMIC_SRC_0_EN             ((u32)0x00000001 << 16)          /*!<R/W 0x0  ADC filter 0 dmic LPF control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_ADC_3_CLK_GATE_EN         ((u32)0x00000001 << 15)          /*!<R/W 0x0  ADC filter 3 clock control 1'b0: normal 1'b1: gated clock */
#define VAD_BIT_PC_TCON_ADC_2_CLK_GATE_EN         ((u32)0x00000001 << 14)          /*!<R/W 0x0  ADC filter 2 clock control 1'b0: normal 1'b1: gated clock */
#define VAD_BIT_PC_TCON_ADC_1_CLK_GATE_EN         ((u32)0x00000001 << 13)          /*!<R/W 0x0  ADC filter 1 clock control 1'b0: normal 1'b1: gated clock */
#define VAD_BIT_PC_TCON_ADC_0_CLK_GATE_EN         ((u32)0x00000001 << 12)          /*!<R/W 0x0  ADC filter 0 clock control 1'b0: normal 1'b1: gated clock */
#define VAD_BIT_PC_TCON_ADC_3_EN                  ((u32)0x00000001 << 11)          /*!<R/W 0x0  ADC filter 3 control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_ADC_2_EN                  ((u32)0x00000001 << 10)          /*!<R/W 0x0  ADC filter 2 control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_ADC_1_EN                  ((u32)0x00000001 << 9)          /*!<R/W 0x0  ADC filter 1 control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_ADC_0_EN                  ((u32)0x00000001 << 8)          /*!<R/W 0x0  ADC filter 0 control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_VAD_CLK_GATE_EN           ((u32)0x00000001 << 7)          /*!<R/W 0x0  PC vad clock control 1'b0: normal 1'b1: gated clock */
#define VAD_BIT_PC_TCON_VAD_EN                    ((u32)0x00000001 << 6)          /*!<R/W 0x0  PC vad control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_DMIC_EN                   ((u32)0x00000001 << 5)          /*!<R/W 0x0  dmic clock control 1'b0: disable 1'b1: enable */
#define VAD_BIT_PC_TCON_AD_ANA_EN                 ((u32)0x00000001 << 4)          /*!<R/W 0x0  Analog ADC clock control 1'b0: disable 1'b1: enable */
#define VAD_MASK_PC_TCON_DMIC_CLK_SEL             ((u32)0x00000007 << 1)          /*!<R/W 0x0  Select dmic clock 3'b000: 2MHz 3'b001: 1MHz 3'b010: 500KHz 3'b011: 250KHz 3'b100: 125KHz Others: reserved */
#define VAD_PC_TCON_DMIC_CLK_SEL(x)               ((u32)(((x) & 0x00000007) << 1))
#define VAD_GET_PC_TCON_DMIC_CLK_SEL(x)           ((u32)(((x >> 1) & 0x00000007)))
#define VAD_BIT_PC_TCON_AD_ANA_CLK_SEL            ((u32)0x00000001 << 0)          /*!<R/W 0x0  Select analog ADC clock 1'b0: 4MHz 1'b1: 2MHz */
/** @} */

/** @defgroup VAD_FILTER_CTRL
 * @brief
 * @{
 **/
#define VAD_MASK_PC_DMIC_DATA_ADC_3_SEL           ((u32)0x00000007 << 28)          /*!<R/W 0x0  ADC filter 3 dmic data selection 3'b000: DMIC0 3'b001: DMIC1 3'b010: DMIC2 3'b011: DMIC3 3'b100: DMIC4 3'b101: DMIC5 3'b110: DMIC6 3'b111: DMIC7 */
#define VAD_PC_DMIC_DATA_ADC_3_SEL(x)             ((u32)(((x) & 0x00000007) << 28))
#define VAD_GET_PC_DMIC_DATA_ADC_3_SEL(x)         ((u32)(((x >> 28) & 0x00000007)))
#define VAD_MASK_PC_DMIC_DATA_ADC_2_SEL           ((u32)0x00000007 << 24)          /*!<R/W 0x0  ADC filter 2 dmic data selection 3'b000: DMIC0 3'b001: DMIC1 3'b010: DMIC2 3'b011: DMIC3 3'b100: DMIC4 3'b101: DMIC5 3'b110: DMIC6 3'b111: DMIC7 */
#define VAD_PC_DMIC_DATA_ADC_2_SEL(x)             ((u32)(((x) & 0x00000007) << 24))
#define VAD_GET_PC_DMIC_DATA_ADC_2_SEL(x)         ((u32)(((x >> 24) & 0x00000007)))
#define VAD_MASK_PC_DMIC_DATA_ADC_1_SEL           ((u32)0x00000007 << 20)          /*!<R/W 0x0  ADC filter 1 dmic data selection 3'b000: DMIC0 3'b001: DMIC1 3'b010: DMIC2 3'b011: DMIC3 3'b100: DMIC4 3'b101: DMIC5 3'b110: DMIC6 3'b111: DMIC7 */
#define VAD_PC_DMIC_DATA_ADC_1_SEL(x)             ((u32)(((x) & 0x00000007) << 20))
#define VAD_GET_PC_DMIC_DATA_ADC_1_SEL(x)         ((u32)(((x >> 20) & 0x00000007)))
#define VAD_MASK_PC_DMIC_DATA_ADC_0_SEL           ((u32)0x00000007 << 16)          /*!<R/W 0x0  ADC filter 0 dmic data selection 3'b000: DMIC0 3'b001: DMIC1 3'b010: DMIC2 3'b011: DMIC3 3'b100: DMIC4 3'b101: DMIC5 3'b110: DMIC6 3'b111: DMIC7 */
#define VAD_PC_DMIC_DATA_ADC_0_SEL(x)             ((u32)(((x) & 0x00000007) << 16))
#define VAD_GET_PC_DMIC_DATA_ADC_0_SEL(x)         ((u32)(((x >> 16) & 0x00000007)))
#define VAD_MASK_PC_ADC_SDM_DATA_ADC_3_SEL        ((u32)0x00000007 << 12)          /*!<R/W 0x0  ADC filter 3 sdm data selection 3'b000: ADC0 3'b001: ADC1 3'b010: ADC2 3'b011: ADC3 3'b100: ADC4 Others: reserved */
#define VAD_PC_ADC_SDM_DATA_ADC_3_SEL(x)          ((u32)(((x) & 0x00000007) << 12))
#define VAD_GET_PC_ADC_SDM_DATA_ADC_3_SEL(x)      ((u32)(((x >> 12) & 0x00000007)))
#define VAD_MASK_PC_ADC_SDM_DATA_ADC_2_SEL        ((u32)0x00000007 << 8)          /*!<R/W 0x0  ADC filter 2 sdm data selection 3'b000: ADC0 3'b001: ADC1 3'b010: ADC2 3'b011: ADC3 3'b100: ADC4 others: reserved */
#define VAD_PC_ADC_SDM_DATA_ADC_2_SEL(x)          ((u32)(((x) & 0x00000007) << 8))
#define VAD_GET_PC_ADC_SDM_DATA_ADC_2_SEL(x)      ((u32)(((x >> 8) & 0x00000007)))
#define VAD_MASK_PC_ADC_SDM_DATA_ADC_1_SEL        ((u32)0x00000007 << 4)          /*!<R/W 0x0  ADC filter 1 sdm data selection 3'b000: ADC0 3'b001: ADC1 3'b010: ADC2 3'b011: ADC3 3'b100: ADC4 others: reserved */
#define VAD_PC_ADC_SDM_DATA_ADC_1_SEL(x)          ((u32)(((x) & 0x00000007) << 4))
#define VAD_GET_PC_ADC_SDM_DATA_ADC_1_SEL(x)      ((u32)(((x >> 4) & 0x00000007)))
#define VAD_MASK_PC_ADC_SDM_DATA_ADC_0_SEL        ((u32)0x00000007 << 0)          /*!<R/W 0x0  ADC filter 0 sdm data selection 3'b000: ADC0 3'b001: ADC1 3'b010: ADC2 3'b011: ADC3 3'b100: ADC4 others: reserved */
#define VAD_PC_ADC_SDM_DATA_ADC_0_SEL(x)          ((u32)(((x) & 0x00000007) << 0))
#define VAD_GET_PC_ADC_SDM_DATA_ADC_0_SEL(x)      ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup VAD_SEL_CTRL
 * @brief
 * @{
 **/
#define VAD_MASK_PC_VAD_IN_SEL                    ((u32)0x00000003 << 4)          /*!<R/W 0x0  Select the input source of vad 2'b00: adc_0_out 2'b01: adc_1_out 2'b10: adc_2_out 2'b11: adc_3_out */
#define VAD_PC_VAD_IN_SEL(x)                      ((u32)(((x) & 0x00000003) << 4))
#define VAD_GET_PC_VAD_IN_SEL(x)                  ((u32)(((x >> 4) & 0x00000003)))
#define VAD_MASK_PC_SEL_VAD_FLAG                  ((u32)0x00000007 << 0)          /*!<R/W 0x0  Select vad flag Source 3'b000: pitch_det_flag_hold 3'b001: pitch_det_flag 3'b010: vad_energy 3'b011: sel_vad_energy 3'b100: vad_abs_flag 3'b101: vad_abs_flag_hold Others: reserved */
#define VAD_PC_SEL_VAD_FLAG(x)                    ((u32)(((x) & 0x00000007) << 0))
#define VAD_GET_PC_SEL_VAD_FLAG(x)                ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup VAD_BUF_CTRL0
 * @brief
 * @{
 **/
#define VAD_BIT_BIT_SEL                           ((u32)0x00000001 << 28)          /*!<R/W 0x0  0: 16 bits 1: 24 bits By default, 16 bits are used, data performance meets KWS requirements, and 24 bits are reserved. */
#define VAD_MASK_CODEC3_IN_BUF_ADD_BLOCK          ((u32)0x00000003 << 26)          /*!<R/W 0x3  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VAD_CODEC3_IN_BUF_ADD_BLOCK(x)            ((u32)(((x) & 0x00000003) << 26))
#define VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(x)        ((u32)(((x >> 26) & 0x00000003)))
#define VAD_MASK_CODEC2_IN_BUF_ADD_BLOCK          ((u32)0x00000003 << 24)          /*!<R/W 0x2  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VAD_CODEC2_IN_BUF_ADD_BLOCK(x)            ((u32)(((x) & 0x00000003) << 24))
#define VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(x)        ((u32)(((x >> 24) & 0x00000003)))
#define VAD_MASK_CODEC1_IN_BUF_ADD_BLOCK          ((u32)0x00000003 << 22)          /*!<R/W 0x1  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VAD_CODEC1_IN_BUF_ADD_BLOCK(x)            ((u32)(((x) & 0x00000003) << 22))
#define VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(x)        ((u32)(((x >> 22) & 0x00000003)))
#define VAD_MASK_CODEC0_IN_BUF_ADD_BLOCK          ((u32)0x00000003 << 20)          /*!<R/W 0x0  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VAD_CODEC0_IN_BUF_ADD_BLOCK(x)            ((u32)(((x) & 0x00000003) << 20))
#define VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(x)        ((u32)(((x >> 20) & 0x00000003)))
#define VAD_BIT_ADC_ANALOG_CLK_SEL                ((u32)0x00000001 << 19)          /*!<R/W 0x0  0: AUDIO CODEC 1: PC_VAD CODEC */
#define VAD_BIT_PC_DBGCH_IN_VADBUF_EN             ((u32)0x00000001 << 18)          /*!<R/W 0x0  Vad_pc Debug Channel CODEC data sent to VAD_BUF 2channel, one is data, one is flag, debug mode multiplexes CH0/CH1 write SRAM entry address 0: Disable 1: Enable */
#define VAD_BIT_CH3_IN_VADBUF_EN                  ((u32)0x00000001 << 13)          /*!<R/W 0x0  Enable VAD CODEC CH3 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VAD_BIT_CH2_IN_VADBUF_EN                  ((u32)0x00000001 << 12)          /*!<R/W 0x0  Enable VAD CODEC CH2 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VAD_BIT_CH1_IN_VADBUF_EN                  ((u32)0x00000001 << 11)          /*!<R/W 0x0  Enable VAD CODEC C1 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VAD_BIT_CH0_IN_VADBUF_EN                  ((u32)0x00000001 << 10)          /*!<R/W 0x0  Enable VAD CODEC CH0 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VAD_BIT_BUF_CTRL_INPUT_SEL                ((u32)0x00000001 << 9)          /*!<R/W 0x0  The data source used to select VAD_BUF_CTRL is used to switch CLK of writing SRAM, 4M at VAD_PC and 40M at VAD_BT through this bit. 0: VAD_PC 1: VAD_BT */
#define VAD_MASK_CH_W_VAD_ASSERT                  ((u32)0x00000003 << 5)          /*!<R 0x0  The writing channel number when VAD asserts, this status bit is used to record the channel number being written by VAD_BUF_CTRL while VAD is valid. 0: CH0 1: CH1 2: CH2 3: CH3 */
#define VAD_CH_W_VAD_ASSERT(x)                    ((u32)(((x) & 0x00000003) << 5))
#define VAD_GET_CH_W_VAD_ASSERT(x)                ((u32)(((x >> 5) & 0x00000003)))
#define VAD_BIT_W_MODE_SEL                        ((u32)0x00000001 << 4)          /*!<R/W 0x0  VAD BUF CTRL the way to write VAD BUF 0: Block 1: Interleaving */
#define VAD_BIT_BUF_SHARE_SRAM_ENABLE             ((u32)0x00000001 << 3)          /*!<R/WE 0x0  Whether to occupy the space control bit of SRAM 0: Disable 1: Enable */
#define VAD_BIT_BUF_LOCK                          ((u32)0x00000001 << 2)          /*!<R/WE 0x0  0: Not locked 1: Locked Note: When the bit writes 0, the bit[1:0] can be modified during the working process of the system;When the BIT writes 1, the bit[1:0] cannot be modified during the working process of the system. Only when the whole system is powered on can it be modified again. */
#define VAD_MASK_W_SRAM_ADDRESS_SEL               ((u32)0x00000003 << 0)          /*!<R/WE 0x0  00: 32KB 01: 64KB 10: 96KB 11: 128KB */
#define VAD_W_SRAM_ADDRESS_SEL(x)                 ((u32)(((x) & 0x00000003) << 0))
#define VAD_GET_W_SRAM_ADDRESS_SEL(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup VAD_ASSERT_W_SRAM_ADDRSS
 * @brief
 * @{
 **/
#define VAD_MASK_ASSERT_W_SRAM_ADDRSS             ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  This address is the current address for writing SRAM when VAD asserts */
#define VAD_ASSERT_W_SRAM_ADDRSS(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define VAD_GET_ASSERT_W_SRAM_ADDRSS(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup VAD_BUF_CTRL_W_ADDRESS
 * @brief
 * @{
 **/
#define VAD_MASK_BUF_CTRL_W_ADDRESS               ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Record VAD_BUF_CTRL write SRAM address. When KM0/KM4 reads VAD_BUF data, refer to this address to avoid reading the wrong data */
#define VAD_BUF_CTRL_W_ADDRESS(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define VAD_GET_BUF_CTRL_W_ADDRESS(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup VAD_AUDIO_TEST
 * @brief
 * @{
 **/
#define VAD_MASK_AUDIO_TEST_SUBMOD                ((u32)0x00000003 << 0)          /*!<R/W 0x0  00: audio ADC output test 01: audio DAC input test 1x: audio codec test */
#define VAD_AUDIO_TEST_SUBMOD(x)                  ((u32)(((x) & 0x00000003) << 0))
#define VAD_GET_AUDIO_TEST_SUBMOD(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup VAD_Exported_Types VAD Exported Types
  * @{
*****************************************************************************/


/**
 * @brief VAD Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t VAD_CODEC_CTRLx[4];                     /*!<  Register,  Address offset: 0x00-0x0C */
	__IO uint32_t VAD_PITCH_DET_CTRL0;                    /*!<  Register,  Address offset: 0x10 */
	__IO uint32_t VAD_PITCH_DET_CTRL1;                    /*!<  Register,  Address offset: 0x14 */
	__IO uint32_t VAD_PITCH_DET_CTRL2;                    /*!<  Register,  Address offset: 0x18 */
	__IO uint32_t VAD_PITCH_DET_CTRL3;                    /*!<  Register,  Address offset: 0x1C */
	__IO uint32_t VAD_PITCH_DET_CTRL4;                    /*!<  Register,  Address offset: 0x20 */
	__IO uint32_t VAD_PITCH_DET_CTRL5;                    /*!<  Register,  Address offset: 0x24 */
	__IO uint32_t VAD_PITCH_DET_CTRL6;                    /*!<  Register,  Address offset: 0x28 */
	__IO uint32_t VAD_PITCH_DET_CTRL7;                    /*!<  Register,  Address offset: 0x2C */
	__IO uint32_t VAD_PITCH_DET_CTRL8;                    /*!<  Register,  Address offset: 0x30 */
	__IO uint32_t VAD_PITCH_DET_CTRL9;                    /*!<  Register,  Address offset: 0x34 */
	__IO uint32_t VAD_CLK_CTRL;                           /*!<  Register,  Address offset: 0x38 */
	__IO uint32_t VAD_FILTER_CTRL;                        /*!<  Register,  Address offset: 0x3C */
	__IO uint32_t VAD_SEL_CTRL;                           /*!<  Register,  Address offset: 0x40 */
	__IO uint32_t RSVD0[3];                               /*!<  Reserved,  Address offset:0x44-0x4C */
	__IO uint32_t VAD_BUF_CTRL0;                          /*!<  Register,  Address offset: 0x50 */
	__I  uint32_t VAD_ASSERT_W_SRAM_ADDRSS;               /*!<  Register,  Address offset: 0x54 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x58 */
	__I  uint32_t VAD_BUF_CTRL_W_ADDRESS;                 /*!<  Register,  Address offset: 0x5C */
	__IO uint32_t VAD_AUDIO_TEST;                         /*!<  Register,  Address offset: 0x60 */
} VAD_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
