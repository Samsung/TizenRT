#ifndef _AMEBA_AUDIO_H_
#define _AMEBA_AUDIO_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup AUDIO
  * @brief AUDIO driver modules
  * @{
  */

/** @defgroup AUDIO_CODEC
  * @brief AUDIO_CODEC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup AUDIO_CODEC_Register_Definitions AUDIO_CODEC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup CODEC_audio_control_0
 * @brief
 * @{
 **/
#define AUD_BIT_AUDIO_IP_EN                  ((u32)0x00000001 << 0)          /*!<R/W 0   */
#define AUD_BIT_DAAD_LPBK_EN                 ((u32)0x00000001 << 1)          /*!<R/W 0  Digital DAC --> ADC loop back control 1'b0: Disable 1'b1: Enable */
#define AUD_BIT_ADDA_LPBK_EN                 ((u32)0x00000001 << 2)          /*!<R/W 0  Digital ADC --> DAC loop back control 1'b0: Disable 1'b1: Enable */
#define AUD_BIT_AUDIO_CONTROL_0_DUMMY        ((u32)0x00000001 << 3)          /*!<R/W 0  DUMMY */
#define AUD_MASK_SYS_CLK_RATE_SEL            ((u32)0x00000003 << 4)          /*!<R/W 0x2  audio_sys_clock_sel */
#define AUD_SYS_CLK_RATE_SEL(x)              ((u32)(((x) & 0x00000003) << 4))
#define AUD_GET_SYS_CLK_RATE_SEL(x)          ((u32)(((x >> 4) & 0x00000003)))
#define AUD_BIT_RWS_SEQ_EN                   ((u32)0x00000001 << 6)          /*!<R/W 0  rws_seq_en[0]: 0:disable, 1:enable */
#define AUD_MASK_AUDIO_DBG_SEL               ((u32)0x0000001F << 13)          /*!<R/W 0  Debug probe selcetion */
#define AUD_AUDIO_DBG_SEL(x)                 ((u32)(((x) & 0x0000001F) << 13))
#define AUD_GET_AUDIO_DBG_SEL(x)             ((u32)(((x >> 13) & 0x0000001F)))
/** @} */

/** @defgroup CODEC_audio_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_SEL_BB_CK_DEPOP             ((u32)0x00000003 << 0)          /*!<R/W 1  sel_bb_ck_depop */
#define AUD_SEL_BB_CK_DEPOP(x)               ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_SEL_BB_CK_DEPOP(x)           ((u32)(((x >> 0) & 0x00000003)))
#define AUD_BIT_BB_CK_DEPOP_EN               ((u32)0x00000001 << 2)          /*!<R/W 0  bb_ck_depop_en */
#define AUD_BIT_CKX_MICBIAS_EN               ((u32)0x00000001 << 3)          /*!<R/W 0  ckx_micbias_en */
#define AUD_BIT_PDM_CLK_DOUBLE               ((u32)0x00000001 << 4)          /*!<R/W 0  pdm_clk_double */
#define AUD_MASK_PDM_CLK_SEL                 ((u32)0x00000003 << 5)          /*!<R/W 0  PDM clock selection 2'b0: 5MHz 2'b1: 2.5MHz 2'b2: 6.67MHz */
#define AUD_PDM_CLK_SEL(x)                   ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_PDM_CLK_SEL(x)               ((u32)(((x >> 5) & 0x00000003)))
#define AUD_MASK_PDM_CH_SWAP                 ((u32)0x00000003 << 7)          /*!<R/W 0x2  pdm_ch_swap */
#define AUD_PDM_CH_SWAP(x)                   ((u32)(((x) & 0x00000003) << 7))
#define AUD_GET_PDM_CH_SWAP(x)               ((u32)(((x >> 7) & 0x00000003)))
#define AUD_BIT_PDM_DATA_PHASE_SEL           ((u32)0x00000001 << 9)          /*!<R/W 1  pdm_data_phase_sel */
#define AUD_BIT_PDM_GAIN_SHIFT_EN            ((u32)0x00000001 << 10)          /*!<R/W 1  pdm_gain_shift_en */
#define AUD_BIT_PDM_CLK_INV_SEL              ((u32)0x00000001 << 11)          /*!<R/W 0  pdm_clk_inv_sel */
#define AUD_BIT_I2S_DATA_RND_EN              ((u32)0x00000001 << 12)          /*!<R/W 1  i2s_data_rnd_en */
#define AUD_MASK_AUDIO_CONTROL_1_DUMMY       ((u32)0x0000000F << 13)          /*!<R/W 0  DUMMY */
#define AUD_AUDIO_CONTROL_1_DUMMY(x)         ((u32)(((x) & 0x0000000F) << 13))
#define AUD_GET_AUDIO_CONTROL_1_DUMMY(x)     ((u32)(((x >> 13) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_clock_control_1
 * @brief
 * @{
 **/
#define AUD_BIT_DA_L_EN                      ((u32)0x00000001 << 0)          /*!<R/W 0  DAC l channel clock enable */
#define AUD_BIT_CLOCK_CONTROL_1_DUMMY0       ((u32)0x00000001 << 1)          /*!<R/W 0  DUMMY */
#define AUD_BIT_MOD_L_EN                     ((u32)0x00000001 << 2)          /*!<R/W 0  SDM l channel clock enable */
#define AUD_BIT_CLOCK_CONTROL_1_DUMMY1       ((u32)0x00000001 << 3)          /*!<R/W 0  DUMMY */
#define AUD_BIT_DA_ANA_CLK_EN                ((u32)0x00000001 << 4)          /*!<R/W 0  DAC/ADC analog clock enable */
#define AUD_BIT_DA_FIFO_EN                   ((u32)0x00000001 << 5)          /*!<R/W 0  DAC FIFO clock enable */
#define AUD_BIT_AD_0_EN                      ((u32)0x00000001 << 6)          /*!<R/W 0  ADC channel 0 clock enable */
#define AUD_BIT_AD_1_EN                      ((u32)0x00000001 << 7)          /*!<R/W 0  ADC channel 1 clock enable */
#define AUD_BIT_AD_2_EN                      ((u32)0x00000001 << 8)          /*!<R/W 0  ADC channel 2 clock enable */
#define AUD_BIT_AD_3_EN                      ((u32)0x00000001 << 9)          /*!<R/W 0  ADC channel 3 clock enable */
#define AUD_BIT_AD_0_FIFO_EN                 ((u32)0x00000001 << 14)          /*!<R/W 0  ADC channel 0 FIFO clock enable */
#define AUD_BIT_AD_1_FIFO_EN                 ((u32)0x00000001 << 15)          /*!<R/W 0  ADC channel 1 FIFO clock enable */
#define AUD_BIT_AD_2_FIFO_EN                 ((u32)0x00000001 << 16)          /*!<R/W 0  ADC channel 2 FIFO clock enable */
#define AUD_BIT_AD_3_FIFO_EN                 ((u32)0x00000001 << 17)          /*!<R/W 0  ADC channel 3 FIFO clock enable */
#define AUD_BIT_AD_ANA_CLK_EN                ((u32)0x00000001 << 22)          /*!<R/W 0  ADC analog clock enable */
/** @} */

/** @defgroup CODEC_clock_control_2
 * @brief
 * @{
 **/
#define AUD_BIT_AD_ANA_0_EN                  ((u32)0x00000001 << 0)          /*!<R/W 0  ADC filter channel 0 clock enable: analog ADC path */
#define AUD_BIT_AD_ANA_1_EN                  ((u32)0x00000001 << 1)          /*!<R/W 0  ADC filter channel 1 clock enable: analog ADC path */
#define AUD_BIT_AD_ANA_2_EN                  ((u32)0x00000001 << 2)          /*!<R/W 0  ADC filter channel 2 clock enable: analog ADC path */
#define AUD_BIT_AD_ANA_3_EN                  ((u32)0x00000001 << 3)          /*!<R/W 0  ADC filter channel 3 clock enable: analog ADC path */
#define AUD_BIT_DMIC_0_EN                    ((u32)0x00000001 << 8)          /*!<R/W 0  ADC filter channel 0 clock enable: dmic path */
#define AUD_BIT_DMIC_1_EN                    ((u32)0x00000001 << 9)          /*!<R/W 0  ADC filter channel 1 clock enable: dmic path */
#define AUD_BIT_DMIC_2_EN                    ((u32)0x00000001 << 10)          /*!<R/W 0  ADC filter channel 2 clock enable: dmic path */
#define AUD_BIT_DMIC_3_EN                    ((u32)0x00000001 << 11)          /*!<R/W 0  ADC filter channel 3 clock enable: dmic path */
/** @} */

/** @defgroup CODEC_clock_control_3
 * @brief
 * @{
 **/
#define AUD_MASK_DMIC1_CLK_SEL               ((u32)0x00000007 << 0)          /*!<R/W 1  Set clock of digital microphone 3'b000: 5MHz 3'b001: 2.5MHz 3'b010: 1.25MHz 3'b011: 625kHz 3'b100: 312.5kHz 3'b101: Reseved 3'b110: Reserved 3'b111: 769.2kHz */
#define AUD_DMIC1_CLK_SEL(x)                 ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_DMIC1_CLK_SEL(x)             ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_DMIC1_CLK_EN                 ((u32)0x00000001 << 3)          /*!<R/W 0  Digital microphone clock enable */
/** @} */

/** @defgroup CODEC_clock_control_4
 * @brief
 * @{
 **/
#define AUD_MASK_SAMPLE_RATE_0               ((u32)0x0000000F << 0)          /*!<R/W 0  Set sample rate source 0 4'h0: 48K 4'h1: 96K 4'h2: 192K 4'h3: 32K 4'h4: 176.4K 4'h5: 16K 4'h6: Reserved 4'h7: 8K 4'h8: 44.1K 4'h9: 88.2K 4'ha:24K 4'hb: 12K 4'hc: 22.05K 4'hd: 11.025K 4'he~4'hf: Reserved */
#define AUD_SAMPLE_RATE_0(x)                 ((u32)(((x) & 0x0000000F) << 0))
#define AUD_GET_SAMPLE_RATE_0(x)             ((u32)(((x >> 0) & 0x0000000F)))
#define AUD_MASK_SAMPLE_RATE_1               ((u32)0x0000000F << 4)          /*!<R/W 0  Set sample rate source 1 4'h0: 48K 4'h1: 96K 4'h2: 192K 4'h3: 32K 4'h4: 176.4K 4'h5: 16K 4'h6: Reserved 4'h7: 8K 4'h8: 44.1K 4'h9: 88.2K 4'ha:24K 4'hb: 12K 4'hc: 22.05K 4'hd: 11.025K 4'he~4'hf: Reserved */
#define AUD_SAMPLE_RATE_1(x)                 ((u32)(((x) & 0x0000000F) << 4))
#define AUD_GET_SAMPLE_RATE_1(x)             ((u32)(((x >> 4) & 0x0000000F)))
#define AUD_MASK_SAMPLE_RATE_2               ((u32)0x0000000F << 8)          /*!<R/W 0  Set sample rate source 2 4'h0: 48K 4'h1: 96K 4'h2: 192K 4'h3: 32K 4'h4: 176.4K 4'h5: 16K 4'h6: Reserved 4'h7: 8K 4'h8: 44.1K 4'h9: 88.2K 4'ha:24K 4'hb: 12K 4'hc: 22.05K 4'hd: 11.025K 4'he~4'hf: Reserved */
#define AUD_SAMPLE_RATE_2(x)                 ((u32)(((x) & 0x0000000F) << 8))
#define AUD_GET_SAMPLE_RATE_2(x)             ((u32)(((x >> 8) & 0x0000000F)))
#define AUD_MASK_SAMPLE_RATE_3               ((u32)0x0000000F << 12)          /*!<R/W 0  Set sample rate source 3 4'h0: 48K 4'h1: 96K 4'h2: 192K 4'h3: 32K 4'h4: 176.4K 4'h5: 16K 4'h6: Reserved 4'h7: 8K 4'h8: 44.1K 4'h9: 88.2K 4'ha:24K 4'hb: 12K 4'hc: 22.05K 4'hd: 11.025K 4'he~4'hf: Reserved */
#define AUD_SAMPLE_RATE_3(x)                 ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_SAMPLE_RATE_3(x)             ((u32)(((x >> 12) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_clock_control_5
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_FS_SRC_SEL            ((u32)0x00000003 << 0)          /*!<R/W 0  Channel 0 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_0_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_ADC_0_FS_SRC_SEL(x)          ((u32)(((x >> 0) & 0x00000003)))
#define AUD_MASK_ADC_1_FS_SRC_SEL            ((u32)0x00000003 << 2)          /*!<R/W 0  Channel 1 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_1_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 2))
#define AUD_GET_ADC_1_FS_SRC_SEL(x)          ((u32)(((x >> 2) & 0x00000003)))
#define AUD_MASK_ADC_2_FS_SRC_SEL            ((u32)0x00000003 << 4)          /*!<R/W 0  Channel 2 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_2_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 4))
#define AUD_GET_ADC_2_FS_SRC_SEL(x)          ((u32)(((x >> 4) & 0x00000003)))
#define AUD_MASK_ADC_3_FS_SRC_SEL            ((u32)0x00000003 << 6)          /*!<R/W 0  Channel 3 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_3_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 6))
#define AUD_GET_ADC_3_FS_SRC_SEL(x)          ((u32)(((x >> 6) & 0x00000003)))
#define AUD_MASK_DAC_L_FS_SRC_SEL            ((u32)0x00000003 << 16)          /*!<R/W 0  DAC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_DAC_L_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_L_FS_SRC_SEL(x)          ((u32)(((x >> 16) & 0x00000003)))
/** @} */

/** @defgroup CODEC_clock_control_6
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_0_ASRC_EN                ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 0 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_1_ASRC_EN                ((u32)0x00000001 << 1)          /*!<R/W 0  Channel 1 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_2_ASRC_EN                ((u32)0x00000001 << 2)          /*!<R/W 0  Channel 2 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_3_ASRC_EN                ((u32)0x00000001 << 3)          /*!<R/W 0  Channel 3 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_DAC_L_ASRC_EN                ((u32)0x00000001 << 8)          /*!<R/W 0  DAC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
/** @} */

/** @defgroup CODEC_clock_control_7
 * @brief
 * @{
 **/
#define AUD_BIT_DA_ANA_CLK_SEL               ((u32)0x00000001 << 0)          /*!<R/W 0  Set clk_da_ana phase 1'b0: Inphase 1'b1: Inverse phase */
#define AUD_BIT_AD_ANA_CLK_SEL               ((u32)0x00000001 << 1)          /*!<R/W 0  Set clk_ad_ana phase 1'b0: Inphase 1'b1: Inverse phase */
#define AUD_BIT_AD_LATCH_PHASE               ((u32)0x00000001 << 2)          /*!<R/W 0  Set latch adc data phase 1'b0: Inphase 1'b1: Inverse phase */
#define AUD_MASK_ANA_CLK_RATE_SEL            ((u32)0x00000003 << 3)          /*!<R/W 0  Set DA/AD analog clock 0: 10MHz 1: 5MHz 2: 2.5MHz 3: Reseved */
#define AUD_ANA_CLK_RATE_SEL(x)              ((u32)(((x) & 0x00000003) << 3))
#define AUD_GET_ANA_CLK_RATE_SEL(x)          ((u32)(((x >> 3) & 0x00000003)))
#define AUD_MASK_AD_LPF_CLK_RATE_SEL         ((u32)0x00000003 << 5)          /*!<R/W 0  Set AD LPF clock 0: 10MHz 1: 5MHz 2: 2.5MHz 3: Reseved */
#define AUD_AD_LPF_CLK_RATE_SEL(x)           ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_AD_LPF_CLK_RATE_SEL(x)       ((u32)(((x >> 5) & 0x00000003)))
#define AUD_MASK_ADC_0_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 7)          /*!<R/W 0  Channel 0 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_0_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 7))
#define AUD_GET_ADC_0_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 7) & 0x00000003)))
#define AUD_MASK_ADC_1_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 9)          /*!<R/W 0  Channel 1 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_1_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 9))
#define AUD_GET_ADC_1_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 9) & 0x00000003)))
#define AUD_MASK_ADC_2_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 11)          /*!<R/W 0  Channel 2 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_2_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 11))
#define AUD_GET_ADC_2_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 11) & 0x00000003)))
#define AUD_MASK_ADC_3_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 3 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_3_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_3_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ANA_CLK_PHASE_SEL            ((u32)0x00000001 << 23)          /*!<R/W 0  1:analog clock shift 80MHz phase, 0:disable */
#define AUD_BIT_AD_ANA_OSR_DOUBLE            ((u32)0x00000001 << 24)          /*!<R/W 0  1:latch adc data with 10MHz clock */
/** @} */

/** @defgroup CODEC_asrc_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ASRC_RATE_SEL_TX            ((u32)0x00000003 << 0)          /*!<R/W 0  0: fs supports 4kHz ~ 60kHz 1: fs supports 60kHz ~ 120kHz 2: fs supports 120kHz ~ 200kHz 3: Reserved */
#define AUD_ASRC_RATE_SEL_TX(x)              ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_ASRC_RATE_SEL_TX(x)          ((u32)(((x >> 0) & 0x00000003)))
#define AUD_MASK_ASRC_GAIN_SEL_TX            ((u32)0x00000003 << 2)          /*!<R/W 0x3  ASRC convergence rate: larger is faster but more noisy */
#define AUD_ASRC_GAIN_SEL_TX(x)              ((u32)(((x) & 0x00000003) << 2))
#define AUD_GET_ASRC_GAIN_SEL_TX(x)          ((u32)(((x >> 2) & 0x00000003)))
#define AUD_BIT_ASRC_AUTO_ADJUST_TX          ((u32)0x00000001 << 4)          /*!<R/W 1  HW auto adjust convergence rate 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_asrc_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ASRC_SDM_INTI_TX            ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Set initial value of tracked frequency */
#define AUD_ASRC_SDM_INTI_TX(x)              ((u32)(((x) & 0x00FFFFFF) << 0))
#define AUD_GET_ASRC_SDM_INTI_TX(x)          ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup CODEC_asrc_control_2
 * @brief
 * @{
 **/
#define AUD_MASK_ASRC_RATE_SEL_RX_0          ((u32)0x00000003 << 0)          /*!<R/W 0  0: fs supports 4kHz ~ 60kHz 1: fs supports 60kHz ~ 120kHz 2/3: Reserved */
#define AUD_ASRC_RATE_SEL_RX_0(x)            ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_ASRC_RATE_SEL_RX_0(x)        ((u32)(((x >> 0) & 0x00000003)))
#define AUD_MASK_ASRC_GAIN_SEL_RX_0          ((u32)0x00000003 << 2)          /*!<R/W 0x3  ASRC convergence rate: larger is faster but more noisy */
#define AUD_ASRC_GAIN_SEL_RX_0(x)            ((u32)(((x) & 0x00000003) << 2))
#define AUD_GET_ASRC_GAIN_SEL_RX_0(x)        ((u32)(((x >> 2) & 0x00000003)))
#define AUD_BIT_ASRC_AUTO_ADJUST_RX_0        ((u32)0x00000001 << 4)          /*!<R/W 1  HW auto adjust convergence rate 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_asrc_control_3
 * @brief
 * @{
 **/
#define AUD_MASK_ASRC_SDM_INTI_RX_0          ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Set initial value of tracked frequency */
#define AUD_ASRC_SDM_INTI_RX_0(x)            ((u32)(((x) & 0x00FFFFFF) << 0))
#define AUD_GET_ASRC_SDM_INTI_RX_0(x)        ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup CODEC_i2s_0_control
 * @brief
 * @{
 **/
#define AUD_BIT_I2S_0_RST_N_REG              ((u32)0x00000001 << 0)          /*!<R/W 0  I2S 0 1'b1: rst_n to audio digital IP is de-asserted 1'b0: rst_n to audio digital IP is asserted */
#define AUD_BIT_I2S_0_INV_SCLK               ((u32)0x00000001 << 1)          /*!<R/W 0  I2S 0 1'b1: I2S/PCM bit clock is inverted */
#define AUD_BIT_I2S_0_SELF_LPBK_EN           ((u32)0x00000001 << 2)          /*!<R/W 0  I2S 0 1'b1: internal loopback mode is enabled */
#define AUD_BIT_I2S_0_SAME_LRC_EN            ((u32)0x00000001 << 3)          /*!<R/W 0  I2S 0 1'b1: LRC_TX share to LRC_RX */
#define AUD_MASK_I2S_0_TDM_MODE_RX           ((u32)0x00000003 << 4)          /*!<R/W 0  I2S 0 Rx channel TDM mode 0: 2 channel 1: 4 channel 2/3: reserved */
#define AUD_I2S_0_TDM_MODE_RX(x)             ((u32)(((x) & 0x00000003) << 4))
#define AUD_GET_I2S_0_TDM_MODE_RX(x)         ((u32)(((x >> 4) & 0x00000003)))
#define AUD_MASK_I2S_0_DATA_FORMAT_SEL_TX    ((u32)0x00000003 << 6)          /*!<R/W 0  I2S 0 Tx channel data format 2'b00: I2S 2'b01: left-justified 2'b10: PCM mode A 2'b11: PCM mode B */
#define AUD_I2S_0_DATA_FORMAT_SEL_TX(x)      ((u32)(((x) & 0x00000003) << 6))
#define AUD_GET_I2S_0_DATA_FORMAT_SEL_TX(x)  ((u32)(((x >> 6) & 0x00000003)))
#define AUD_MASK_I2S_0_DATA_FORMAT_SEL_RX    ((u32)0x00000003 << 8)          /*!<R/W 0  I2S 0 Rx channel data format 2'b00: I2S 2'b01: left-justified 2'b10: PCM mode A 2'b11: PCM mode B */
#define AUD_I2S_0_DATA_FORMAT_SEL_RX(x)      ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_I2S_0_DATA_FORMAT_SEL_RX(x)  ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_I2S_0_DATA_LEN_SEL_TX       ((u32)0x00000003 << 10)          /*!<R/W 0  I2S 0 Tx channel data length 2'b00: 16 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_0_DATA_LEN_SEL_TX(x)         ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_I2S_0_DATA_LEN_SEL_TX(x)     ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_I2S_0_DATA_LEN_SEL_RX       ((u32)0x00000003 << 12)          /*!<R/W 0  I2S 0 Rx channel data length 2'b00: 16 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_0_DATA_LEN_SEL_RX(x)         ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_I2S_0_DATA_LEN_SEL_RX(x)     ((u32)(((x >> 12) & 0x00000003)))
#define AUD_MASK_I2S_0_CH_LEN_SEL_TX         ((u32)0x00000003 << 14)          /*!<R/W 0  I2S 0 Tx channel channel length 2'b00: 16 bits 2'b01: 32 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_0_CH_LEN_SEL_TX(x)           ((u32)(((x) & 0x00000003) << 14))
#define AUD_GET_I2S_0_CH_LEN_SEL_TX(x)       ((u32)(((x >> 14) & 0x00000003)))
#define AUD_MASK_I2S_0_CH_LEN_SEL_RX         ((u32)0x00000003 << 16)          /*!<R/W 0  I2S 0 Rx channel chennel length 2'b00: 16 bits 2'b01: 32 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_0_CH_LEN_SEL_RX(x)           ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_I2S_0_CH_LEN_SEL_RX(x)       ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_I2S_0_DATA_CH_SEL_TX        ((u32)0x00000003 << 18)          /*!<R/W 0  I2S 0 Tx channel data channel 2'b00: L/R 2'b01: R/L 2'b10: L/L 2'b11: R/R */
#define AUD_I2S_0_DATA_CH_SEL_TX(x)          ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_I2S_0_DATA_CH_SEL_TX(x)      ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_I2S_0_MASTER_SEL             ((u32)0x00000001 << 20)          /*!<R/W 0  I2S 0 master source sel 1'b0: sport internal 1'b1: external I2S */
/** @} */

/** @defgroup CODEC_i2s_0_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_I2S_0_DATA_CH0_SEL_RX       ((u32)0x00000007 << 0)          /*!<R/W 0  I2S 0 Rx channel data channel 0 0: CH0 1: CH1 2: CH2 3: CH3 4/5/6/7: reserved */
#define AUD_I2S_0_DATA_CH0_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_I2S_0_DATA_CH0_SEL_RX(x)     ((u32)(((x >> 0) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH1_SEL_RX       ((u32)0x00000007 << 3)          /*!<R/W 0  I2S 0 Rx channel data channel 1 0: CH0 1: CH1 2: CH2 3: CH3 4/5/6/7: reserved */
#define AUD_I2S_0_DATA_CH1_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 3))
#define AUD_GET_I2S_0_DATA_CH1_SEL_RX(x)     ((u32)(((x >> 3) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH2_SEL_RX       ((u32)0x00000007 << 6)          /*!<R/W 0  I2S 0 Rx channel data channel 2 0: CH0 1: CH1 2: CH2 3: CH3 4/5/6/7: reserved */
#define AUD_I2S_0_DATA_CH2_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 6))
#define AUD_GET_I2S_0_DATA_CH2_SEL_RX(x)     ((u32)(((x >> 6) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH3_SEL_RX       ((u32)0x00000007 << 9)          /*!<R/W 0  I2S 0 Rx channel data channel 3 0: CH0 1: CH1 2: CH2 3: CH3 4/5/6/7: reserved */
#define AUD_I2S_0_DATA_CH3_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 9))
#define AUD_GET_I2S_0_DATA_CH3_SEL_RX(x)     ((u32)(((x >> 9) & 0x00000007)))
#define AUD_BIT_I2S_0_DATA_CH0_RX_DISABLE    ((u32)0x00000001 << 24)          /*!<R/W 0  I2S 0 Rx channel data channel 0 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH1_RX_DISABLE    ((u32)0x00000001 << 25)          /*!<R/W 0  I2S 0 Rx channel data channel 1 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH2_RX_DISABLE    ((u32)0x00000001 << 26)          /*!<R/W 0  I2S 0 Rx channel data channel 2 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH3_RX_DISABLE    ((u32)0x00000001 << 27)          /*!<R/W 0  I2S 0 Rx channel data channel 3 1: disabe 0: enable */
/** @} */

/** @defgroup CODEC_adc_0_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 0 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling other: reserved */
#define AUD_ADC_0_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_0_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_0_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 0 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 0 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 0 DMIC path SRC 1st LPF FC */
#define AUD_ADC_0_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_0_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_0_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 0 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_0_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 0 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 others: reserved */
#define AUD_ADC_0_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_0_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_0_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 0 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_AD_LPF2ND_FC_SEL      ((u32)0x00000003 << 12)          /*!<R/W 0  Channel 0 ADC path SRC 2nd LPF FC */
#define AUD_ADC_0_AD_LPF2ND_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_ADC_0_AD_LPF2ND_FC_SEL(x)    ((u32)(((x >> 12) & 0x00000003)))
#define AUD_BIT_ADC_0_AD_LPF1ST_EN           ((u32)0x00000001 << 14)          /*!<R/W 1  Channel 0 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 15)          /*!<R/W 0  Channel 0 ADC path SRC 1st LPF FC */
#define AUD_ADC_0_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 15))
#define AUD_GET_ADC_0_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 15) & 0x00000003)))
#define AUD_BIT_ADC_0_AD_MIX_MUTE            ((u32)0x00000001 << 17)          /*!<R/W 1  Channel 0 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_0_AD_ZDET_FUNC          ((u32)0x00000003 << 18)          /*!<R/W 0x2  Channel 0 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_0_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_0_AD_ZDET_FUNC(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_MASK_ADC_0_AD_ZDET_TOUT          ((u32)0x00000003 << 20)          /*!<R/W 0  Channel 0 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_0_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 20))
#define AUD_GET_ADC_0_AD_ZDET_TOUT(x)        ((u32)(((x >> 20) & 0x00000003)))
#define AUD_BIT_ADC_0_AD_MUTE                ((u32)0x00000001 << 22)          /*!<R/W 0  Channel 0 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_BIT_ADC_0_HPF_RSVD               ((u32)0x00000001 << 23)          /*!<R/W 0  Channel 0 ADC path reserved */
#define AUD_MASK_ADC_0_DECI_SRC_SEL          ((u32)0x00000003 << 24)          /*!<R/W 0  Channel 0 ADC path decimation source select 0: ADC 1: reserved 2: MUSIC 3: reserved */
#define AUD_ADC_0_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 24))
#define AUD_GET_ADC_0_DECI_SRC_SEL(x)        ((u32)(((x >> 24) & 0x00000003)))
#define AUD_BIT_ADC_0_DCHPF_EN               ((u32)0x00000001 << 26)          /*!<R/W 0  Channel 0 ADC path high pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_DCHPF_FC_SEL          ((u32)0x00000007 << 27)          /*!<R/W 0x5  Channel 0 ADC path high pass filter Fc */
#define AUD_ADC_0_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 27))
#define AUD_GET_ADC_0_DCHPF_FC_SEL(x)        ((u32)(((x >> 27) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_0_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 0 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_0_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_0_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_0_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 0 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_0_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_0_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_0_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 0 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_0_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_0_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
#define AUD_BIT_ADC_0_FIFO_KEEP_ONE          ((u32)0x00000001 << 16)          /*!<R/W 0  Channel 0 i2s sample buffering 0: 4 samples, 1: 1 sample */
/** @} */

/** @defgroup CODEC_adc_1_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 1 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling other: reserved */
#define AUD_ADC_1_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_1_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_1_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 1 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 1 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 1 DMIC path SRC 1st LPF FC */
#define AUD_ADC_1_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_1_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_1_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 1 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_1_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 1 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 others: reserved */
#define AUD_ADC_1_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_1_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_1_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 1 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_AD_LPF2ND_FC_SEL      ((u32)0x00000003 << 12)          /*!<R/W 0  Channel 1 ADC path SRC 2nd LPF FC */
#define AUD_ADC_1_AD_LPF2ND_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_ADC_1_AD_LPF2ND_FC_SEL(x)    ((u32)(((x >> 12) & 0x00000003)))
#define AUD_BIT_ADC_1_AD_LPF1ST_EN           ((u32)0x00000001 << 14)          /*!<R/W 1  Channel 1 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 15)          /*!<R/W 0  Channel 1 ADC path SRC 1st LPF FC */
#define AUD_ADC_1_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 15))
#define AUD_GET_ADC_1_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 15) & 0x00000003)))
#define AUD_BIT_ADC_1_AD_MIX_MUTE            ((u32)0x00000001 << 17)          /*!<R/W 1  Channel 1 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_1_AD_ZDET_FUNC          ((u32)0x00000003 << 18)          /*!<R/W 0x2  Channel 1 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_1_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_1_AD_ZDET_FUNC(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_MASK_ADC_1_AD_ZDET_TOUT          ((u32)0x00000003 << 20)          /*!<R/W 0  Channel 1 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_1_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 20))
#define AUD_GET_ADC_1_AD_ZDET_TOUT(x)        ((u32)(((x >> 20) & 0x00000003)))
#define AUD_BIT_ADC_1_AD_MUTE                ((u32)0x00000001 << 22)          /*!<R/W 0  Channel 1 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_BIT_ADC_1_HPF_RSVD               ((u32)0x00000001 << 23)          /*!<R/W 0  Channel 1 ADC path reserved */
#define AUD_MASK_ADC_1_DECI_SRC_SEL          ((u32)0x00000003 << 24)          /*!<R/W 0  Channel 1 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_1_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 24))
#define AUD_GET_ADC_1_DECI_SRC_SEL(x)        ((u32)(((x >> 24) & 0x00000003)))
#define AUD_BIT_ADC_1_DCHPF_EN               ((u32)0x00000001 << 26)          /*!<R/W 0  Channel 1 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_DCHPF_FC_SEL          ((u32)0x00000007 << 27)          /*!<R/W 0x5  Channel 1 ADC path High pass filter FC */
#define AUD_ADC_1_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 27))
#define AUD_GET_ADC_1_DCHPF_FC_SEL(x)        ((u32)(((x >> 27) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_1_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 1 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_1_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_1_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_1_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 1 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_1_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_1_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_1_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_1_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_1_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_1_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 1 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_1_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_1_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
#define AUD_BIT_ADC_1_FIFO_KEEP_ONE          ((u32)0x00000001 << 16)          /*!<R/W 0  Channel 1 i2s sample buffering 0: 4 samples, 1: 1 sample */
/** @} */

/** @defgroup CODEC_adc_2_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 2 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling other: reserved */
#define AUD_ADC_2_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_2_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_2_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 2 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 2 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 2 DMIC path SRC 1st LPF FC */
#define AUD_ADC_2_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_2_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_2_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 2 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_2_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 2 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 others: reserved */
#define AUD_ADC_2_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_2_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_2_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 2 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_AD_LPF2ND_FC_SEL      ((u32)0x00000003 << 12)          /*!<R/W 0  Channel 2 ADC path SRC 2nd LPF FC */
#define AUD_ADC_2_AD_LPF2ND_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_ADC_2_AD_LPF2ND_FC_SEL(x)    ((u32)(((x >> 12) & 0x00000003)))
#define AUD_BIT_ADC_2_AD_LPF1ST_EN           ((u32)0x00000001 << 14)          /*!<R/W 1  Channel 2 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 15)          /*!<R/W 0  Channel 2 ADC path SRC 1st LPF FC */
#define AUD_ADC_2_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 15))
#define AUD_GET_ADC_2_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 15) & 0x00000003)))
#define AUD_BIT_ADC_2_AD_MIX_MUTE            ((u32)0x00000001 << 17)          /*!<R/W 1  Channel 2 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_2_AD_ZDET_FUNC          ((u32)0x00000003 << 18)          /*!<R/W 0x2  Channel 2 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_2_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_2_AD_ZDET_FUNC(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_MASK_ADC_2_AD_ZDET_TOUT          ((u32)0x00000003 << 20)          /*!<R/W 0  Channel 2 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_2_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 20))
#define AUD_GET_ADC_2_AD_ZDET_TOUT(x)        ((u32)(((x >> 20) & 0x00000003)))
#define AUD_BIT_ADC_2_AD_MUTE                ((u32)0x00000001 << 22)          /*!<R/W 0  Channel 2 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_BIT_ADC_2_HPF_RSVD               ((u32)0x00000001 << 23)          /*!<R/W 0  Channel 2 ADC path reserved */
#define AUD_MASK_ADC_2_DECI_SRC_SEL          ((u32)0x00000003 << 24)          /*!<R/W 0  Channel 2 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_2_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 24))
#define AUD_GET_ADC_2_DECI_SRC_SEL(x)        ((u32)(((x >> 24) & 0x00000003)))
#define AUD_BIT_ADC_2_DCHPF_EN               ((u32)0x00000001 << 26)          /*!<R/W 0  Channel 2 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_DCHPF_FC_SEL          ((u32)0x00000007 << 27)          /*!<R/W 0x5  Channel 2 ADC path High pass filter FC */
#define AUD_ADC_2_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 27))
#define AUD_GET_ADC_2_DCHPF_FC_SEL(x)        ((u32)(((x >> 27) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_2_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 2 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_2_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_2_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_2_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 2 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_2_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_2_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_2_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_2_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_2_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_2_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 2 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_2_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_2_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
#define AUD_BIT_ADC_2_FIFO_KEEP_ONE          ((u32)0x00000001 << 16)          /*!<R/W 0  Channel 2 i2s sample buffering 0: 4 samples, 1: 1 sample */
/** @} */

/** @defgroup CODEC_adc_3_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 3 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling other: reserved */
#define AUD_ADC_3_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_3_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_3_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 3 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 3 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 3 DMIC path SRC 1st LPF FC */
#define AUD_ADC_3_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_3_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_3_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 3 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_3_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 3 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 others: reserved */
#define AUD_ADC_3_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_3_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_3_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 3 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_AD_LPF2ND_FC_SEL      ((u32)0x00000003 << 12)          /*!<R/W 0  Channel 3 ADC path SRC 2nd LPF FC */
#define AUD_ADC_3_AD_LPF2ND_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_ADC_3_AD_LPF2ND_FC_SEL(x)    ((u32)(((x >> 12) & 0x00000003)))
#define AUD_BIT_ADC_3_AD_LPF1ST_EN           ((u32)0x00000001 << 14)          /*!<R/W 1  Channel 3 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 15)          /*!<R/W 0  Channel 3 ADC path SRC 1st LPF FC */
#define AUD_ADC_3_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 15))
#define AUD_GET_ADC_3_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 15) & 0x00000003)))
#define AUD_BIT_ADC_3_AD_MIX_MUTE            ((u32)0x00000001 << 17)          /*!<R/W 1  Channel 3 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_3_AD_ZDET_FUNC          ((u32)0x00000003 << 18)          /*!<R/W 0x2  Channel 3 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_3_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_3_AD_ZDET_FUNC(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_MASK_ADC_3_AD_ZDET_TOUT          ((u32)0x00000003 << 20)          /*!<R/W 0  Channel 3 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_3_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 20))
#define AUD_GET_ADC_3_AD_ZDET_TOUT(x)        ((u32)(((x >> 20) & 0x00000003)))
#define AUD_BIT_ADC_3_AD_MUTE                ((u32)0x00000001 << 22)          /*!<R/W 0  Channel 3 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_BIT_ADC_3_HPF_RSVD               ((u32)0x00000001 << 23)          /*!<R/W 0  Channel 3 ADC path reserved */
#define AUD_MASK_ADC_3_DECI_SRC_SEL          ((u32)0x00000003 << 24)          /*!<R/W 0  Channel 3 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_3_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 24))
#define AUD_GET_ADC_3_DECI_SRC_SEL(x)        ((u32)(((x >> 24) & 0x00000003)))
#define AUD_BIT_ADC_3_DCHPF_EN               ((u32)0x00000001 << 26)          /*!<R/W 0  Channel 3 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_DCHPF_FC_SEL          ((u32)0x00000007 << 27)          /*!<R/W 0x5  Channel 3 ADC path High pass filter FC */
#define AUD_ADC_3_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 27))
#define AUD_GET_ADC_3_DCHPF_FC_SEL(x)        ((u32)(((x >> 27) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_3_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 3 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_3_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_3_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_3_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 3 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_3_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_3_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_3_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_3_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_3_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_3_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 3 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_3_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_3_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
#define AUD_BIT_ADC_3_FIFO_KEEP_ONE          ((u32)0x00000001 << 16)          /*!<R/W 0  Channel 3 i2s sample buffering 0: 4 samples, 1: 1 sample */
/** @} */

/** @defgroup CODEC_dac_l_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_DA_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0xaf  Channel L DAC path dvol gain control (0.375dB/step) 8'hAF: 0dB 8'h00: -65.625dB */
#define AUD_DAC_L_DA_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_DAC_L_DA_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_BIT_DAC_L_DA_SRC_SEL             ((u32)0x00000001 << 16)          /*!<R/W 0  Channel L DAC path input sel 0:I2S L channel 1: test tone */
#define AUD_BIT_DAC_L_CONTROL_0_DUMMY        ((u32)0x00000001 << 17)          /*!<R/W 0  DUMMY */
#define AUD_BIT_DAC_L_TEST_TONE_EN           ((u32)0x00000001 << 20)          /*!<R/W 0  Channel L DAC path test tone enable */
#define AUD_MASK_DAC_L_TEST_FC_SEL           ((u32)0x0000007F << 21)          /*!<R/W 0x0  Channel L DAC path test tone frquency: (fs/192)*(tone_fc_sel+1)Hz */
#define AUD_DAC_L_TEST_FC_SEL(x)             ((u32)(((x) & 0x0000007F) << 21))
#define AUD_GET_DAC_L_TEST_FC_SEL(x)         ((u32)(((x >> 21) & 0x0000007F)))
#define AUD_MASK_DAC_L_TEST_GAIN_SEL         ((u32)0x0000000F << 28)          /*!<R/W 0x0  Channel L DAC path test tone gain control 0 -6.02*(gain_sel)dB */
#define AUD_DAC_L_TEST_GAIN_SEL(x)           ((u32)(((x) & 0x0000000F) << 28))
#define AUD_GET_DAC_L_TEST_GAIN_SEL(x)       ((u32)(((x >> 28) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_dac_l_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_DA_ZDET_FUNC          ((u32)0x00000003 << 0)          /*!<R/W 0x2  Channel L DAC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_DAC_L_DA_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_DAC_L_DA_ZDET_FUNC(x)        ((u32)(((x >> 0) & 0x00000003)))
#define AUD_MASK_DAC_L_DA_ZDET_TOUT          ((u32)0x00000003 << 2)          /*!<R/W 0  Channel L DAC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_DAC_L_DA_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 2))
#define AUD_GET_DAC_L_DA_ZDET_TOUT(x)        ((u32)(((x >> 2) & 0x00000003)))
#define AUD_BIT_DAC_L_DA_MUTE                ((u32)0x00000001 << 4)          /*!<R/W 0  Channel L DAC path dvol mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_DAHPF_EN               ((u32)0x00000001 << 5)          /*!<R/W 0  Channel L DAC path Narrow-band 1st HPF enable control 0: Disable 1: Enable */
#define AUD_MASK_DAC_L_DA_DITHER_SEL         ((u32)0x00000003 << 6)          /*!<R/W 0  Channel L DAC path dither select 2'b00: Disable 2'b01: LSB 2'b10: LSB+1 2'b11: LSB+2 */
#define AUD_DAC_L_DA_DITHER_SEL(x)           ((u32)(((x) & 0x00000003) << 6))
#define AUD_GET_DAC_L_DA_DITHER_SEL(x)       ((u32)(((x >> 6) & 0x00000003)))
#define AUD_BIT_DAC_L_DMIX_MUTE_DA           ((u32)0x00000001 << 8)          /*!<R/W 0  Channel L DAC path 128fs-domain mixer da path mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_CONTROL_1_DUMMY0       ((u32)0x00000001 << 9)          /*!<R/W 0  DUMMY */
#define AUD_BIT_DAC_L_DMIX_MUTE_DC           ((u32)0x00000001 << 10)          /*!<R/W 0  Channel L DAC path 128fs-domain mixer a DC from dac_l_tone_compensation_offset 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_MUSIC_MUTE_EN          ((u32)0x00000001 << 11)          /*!<R/W 0  Channel L DAC path music output mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_PDM_EN                 ((u32)0x00000001 << 13)          /*!<R/W 1  Channel L DAC path PDM 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_SDM_EXTEND_FB_EN       ((u32)0x00000001 << 14)          /*!<R/W 1  Channel L DAC path feedback extend (for debug purpose; default need turn on) 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_SDM_EF_EN              ((u32)0x00000001 << 15)          /*!<R/W 1  Channel L DAC path 2nd order SDM enable 0: Disable 1: Enable */
#define AUD_MASK_DAC_L_SDM_DITHER_SEL        ((u32)0x00000003 << 16)          /*!<R/W 0  Channel L DAC path SDM dither select 2'b00: Disable 2'b01: LSB 2'b10: LSB+1 2'b11: LSB+2 */
#define AUD_DAC_L_SDM_DITHER_SEL(x)          ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_L_SDM_DITHER_SEL(x)      ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_DAC_L_DA_FLT_TYPE           ((u32)0x00000003 << 18)          /*!<R/W 0  Channel L DAC path interpolation filter type */
#define AUD_DAC_L_DA_FLT_TYPE(x)             ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_DAC_L_DA_FLT_TYPE(x)         ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_DAC_L_CONTROL_1_DUMMY1       ((u32)0x00000001 << 21)          /*!<R/W 0  DUMMY */
/** @} */

/** @defgroup CODEC_dac_l_control_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_DC_OFFSET             ((u32)0x0000FFFF << 0)          /*!<R/W 0  Channel L DAC path 128fs-domain mixe a DC compensation for spur issue 2's complemet */
#define AUD_DAC_L_DC_OFFSET(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define AUD_GET_DAC_L_DC_OFFSET(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
#define AUD_MASK_DAC_L_CONTROL_2_DUMMY0      ((u32)0x0000000F << 16)          /*!<R/W 0  DUMMY */
#define AUD_DAC_L_CONTROL_2_DUMMY0(x)        ((u32)(((x) & 0x0000000F) << 16))
#define AUD_GET_DAC_L_CONTROL_2_DUMMY0(x)    ((u32)(((x >> 16) & 0x0000000F)))
#define AUD_MASK_DAC_L_CONTROL_2_DUMMY1      ((u32)0x0000000F << 20)          /*!<R/W 0x7  DUMMY */
#define AUD_DAC_L_CONTROL_2_DUMMY1(x)        ((u32)(((x) & 0x0000000F) << 20))
#define AUD_GET_DAC_L_CONTROL_2_DUMMY1(x)    ((u32)(((x >> 20) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_adc_align_control_0
 * @brief
 * @{
 **/
#define AUD_BIT_SP_AD_FIFO_ALIGN_EN          ((u32)0x00000001 << 0)          /*!<R/W 0  1:Uplink fifo will align with I2S, 0:disable */
#define AUD_BIT_SP_AD_ALIGN_EN               ((u32)0x00000001 << 2)          /*!<R/W 0  1:Uplink channel will align with I2S, 0:disable */
/** @} */

/** @defgroup CODEC_adc_align_control
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_ALIGN_CH_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Uplink CH0 align channel selection: 0:CH0, 1:CH1, 2:CH2, 3:CH3, others:reserved */
#define AUD_ADC_0_ALIGN_CH_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_0_ALIGN_CH_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_0_ALIGN_EN               ((u32)0x00000001 << 3)          /*!<R/W 0  Uplink CH0 align enable */
#define AUD_MASK_ADC_1_ALIGN_CH_SEL          ((u32)0x00000007 << 4)          /*!<R/W 1  Uplink CH1 align channel selection: 0:CH0, 1:CH1, 2:CH2, 3:CH3, others:reserved */
#define AUD_ADC_1_ALIGN_CH_SEL(x)            ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_1_ALIGN_CH_SEL(x)        ((u32)(((x >> 4) & 0x00000007)))
#define AUD_BIT_ADC_1_ALIGN_EN               ((u32)0x00000001 << 7)          /*!<R/W 0  Uplink CH1 align enable */
#define AUD_MASK_ADC_2_ALIGN_CH_SEL          ((u32)0x00000007 << 8)          /*!<R/W 0x2  Uplink CH2 align channel selection: 0:CH0, 1:CH1, 2:CH2, 3:CH3, others:reserved */
#define AUD_ADC_2_ALIGN_CH_SEL(x)            ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_2_ALIGN_CH_SEL(x)        ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_2_ALIGN_EN               ((u32)0x00000001 << 11)          /*!<R/W 0  Uplink CH2 align enable */
#define AUD_MASK_ADC_3_ALIGN_CH_SEL          ((u32)0x00000007 << 12)          /*!<R/W 0x2  Uplink CH3 align channel selection: 0:CH0, 1:CH1, 2:CH2, 3:CH3, others:reserved */
#define AUD_ADC_3_ALIGN_CH_SEL(x)            ((u32)(((x) & 0x00000007) << 12))
#define AUD_GET_ADC_3_ALIGN_CH_SEL(x)        ((u32)(((x >> 12) & 0x00000007)))
#define AUD_BIT_ADC_3_ALIGN_EN               ((u32)0x00000001 << 15)          /*!<R/W 0  Uplink CH3 align enable */
/** @} */

/** @defgroup CODEC_dac_mix_control
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_MIX_CONTROL_DUMMY       ((u32)0x0000000F << 0)          /*!<R/W 0  DUMMY */
#define AUD_DAC_MIX_CONTROL_DUMMY(x)         ((u32)(((x) & 0x0000000F) << 0))
#define AUD_GET_DAC_MIX_CONTROL_DUMMY(x)     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_phase_compensation_control_register_0
 * @brief
 * @{
 **/
#define AUD_BIT_PHASE_DELAY_COUNTER_EN_0     ((u32)0x00000001 << 0)          /*!<R/W 0  enable Uplink CH0 phase compensation */
#define AUD_MASK_PHASE_DELAY_COUNTER_0       ((u32)0x000003FF << 1)          /*!<R/W 0  Uplink CH 0 phase compensation delay */
#define AUD_PHASE_DELAY_COUNTER_0(x)         ((u32)(((x) & 0x000003FF) << 1))
#define AUD_GET_PHASE_DELAY_COUNTER_0(x)     ((u32)(((x >> 1) & 0x000003FF)))
/** @} */

/** @defgroup CODEC_phase_compensation_control_register_1
 * @brief
 * @{
 **/
#define AUD_BIT_PHASE_DELAY_COUNTER_EN_1     ((u32)0x00000001 << 0)          /*!<R/W 0  enable Uplink CH1 phase compensation */
#define AUD_MASK_PHASE_DELAY_COUNTER_1       ((u32)0x000003FF << 1)          /*!<R/W 0  Uplink CH 1 phase compensation delay */
#define AUD_PHASE_DELAY_COUNTER_1(x)         ((u32)(((x) & 0x000003FF) << 1))
#define AUD_GET_PHASE_DELAY_COUNTER_1(x)     ((u32)(((x >> 1) & 0x000003FF)))
/** @} */

/** @defgroup CODEC_phase_compensation_control_register_2
 * @brief
 * @{
 **/
#define AUD_BIT_PHASE_DELAY_COUNTER_EN_2     ((u32)0x00000001 << 0)          /*!<R/W 0  enable Uplink CH2 phase compensation */
#define AUD_MASK_PHASE_DELAY_COUNTER_2       ((u32)0x000003FF << 1)          /*!<R/W 0  Uplink CH 2 phase compensation delay */
#define AUD_PHASE_DELAY_COUNTER_2(x)         ((u32)(((x) & 0x000003FF) << 1))
#define AUD_GET_PHASE_DELAY_COUNTER_2(x)     ((u32)(((x >> 1) & 0x000003FF)))
/** @} */

/** @defgroup CODEC_adc_0_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_0_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 0 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 0 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_0_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_0_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_0_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 0 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_0_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_0_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_1_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_1_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 1 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 1 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_1_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_1_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_1_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 1 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_1_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_1_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_2_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_2_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 2 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 2 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_2_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_2_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_2_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 2 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_2_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_2_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_3_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_3_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 3 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 3 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_3_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_3_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_3_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 3 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_3_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_3_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_dac_l_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_L_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel L DAC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_DAC_L_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel L DAC path silence detection threshold 3'b000: -78db 3'b001: -84db 3'b010: -90db 3'b011: -102db 3'b100: -108db 3'b101: -114db 3'b110: -126db 3'b111: -132db */
#define AUD_DAC_L_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_DAC_L_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_DAC_L_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel L DAC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_DAC_L_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_DAC_L_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_dac_l_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_L_EQ_REVD                ((u32)0x00000001 << 0)          /*!<R/W 0  reserved */
/** @} */

/** @defgroup CODEC_dac_r_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_R_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  reserved */
/** @} */

/** @defgroup CODEC_adc_0_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_0_EQ_RSVD                ((u32)0x00000001 << 0)          /*!<R/W 0  reserved */
/** @} */

/** @defgroup CODEC_adc_2_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_2_EQ_RSVD                ((u32)0x00000001 << 0)          /*!<R/W 0  reserved */
/** @} */

/** @defgroup CODEC_adc_4_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_4_EQ_RSVD                ((u32)0x00000001 << 0)          /*!<R/W 0  reserved */
/** @} */

/** @defgroup CODEC_vad_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_VAD_RESET                    ((u32)0x00000001 << 0)          /*!<R/W 1  Reset VAD module */
#define AUD_MASK_VAD_EQ_SRC_SEL              ((u32)0x00000007 << 1)          /*!<R/W 0  VAD source selection 2'b00: adc_0_eq_out 2'b01: adc_1_eq_out 2'b10: adc_2_eq_out 2'b11: adc_3_eq_out */
#define AUD_VAD_EQ_SRC_SEL(x)                ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_VAD_EQ_SRC_SEL(x)            ((u32)(((x >> 1) & 0x00000007)))
/** @} */

/** @defgroup CODEC_vad_voice_level
 * @brief
 * @{
 **/
#define AUD_MASK_VAD_VOICE_LEVEL             ((u32)0x000001FF << 0)          /*!<R/W 0x2c  The minimum value of mean power of PE&D unit0 */
#define AUD_VAD_VOICE_LEVEL(x)               ((u32)(((x) & 0x000001FF) << 0))
#define AUD_GET_VAD_VOICE_LEVEL(x)           ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup CODEC_vad_det_thr
 * @brief
 * @{
 **/
#define AUD_MASK_VAD_DET_THR                 ((u32)0x000000FF << 0)          /*!<R/W 0x19  Setting vad_flag threshold between frame power and mean power of PE&D unit0 */
#define AUD_VAD_DET_THR(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_VAD_DET_THR(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CODEC_vad_intr_en
 * @brief
 * @{
 **/
#define AUD_MASK_VAD_INTR_EN                 ((u32)0x000000FF << 0)          /*!<R/W 0  New enables of vad interrupts */
#define AUD_VAD_INTR_EN(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_VAD_INTR_EN(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CODEC_dac_dre_control_0
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_DRE_RSVD                 ((u32)0x00000001 << 0)          /*!<R/W 0  reserved */
/** @} */

/** @defgroup CODEC_ana_read
 * @brief
 * @{
 **/
#define AUD_BIT_MICBIAS_OC                   ((u32)0x00000001 << 0)          /*!<R   The status flag of MICBIAS over-current protection */
/** @} */

/** @defgroup CODEC_adc_0_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 0 ADC LPF out valus */
#define AUD_ADC_0_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_0_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_adc_1_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 1 ADC LPF out valus */
#define AUD_ADC_1_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_1_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_adc_2_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 2 ADC LPF out valus */
#define AUD_ADC_2_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_2_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_adc_3_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 3 ADC LPF out valus */
#define AUD_ADC_3_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_3_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_silence_inform
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_L_SILENCE_DET_O          ((u32)0x00000001 << 0)          /*!<R   dac_l_silence data status (result of silence detection) 1'b0: not dac_l_silence data 1'b1: dac_l_silence is detected */
#define AUD_BIT_DAC_L_SILENCE_DET_STATUS     ((u32)0x00000001 << 1)          /*!<R   Ongoing status of dac_l_silence detection 1'b0: dac_l_silence detection is resting (clock is gating) 1'b1: dac_l_silence detection is working */
#define AUD_BIT_ADC_0_SILENCE_DET_O          ((u32)0x00000001 << 4)          /*!<R   adc_0_silencedata status (result of silence detection) 1'b0: not adc_0_silencedata 1'b1: adc_0_silenceis detected */
#define AUD_BIT_ADC_0_SILENCE_DET_STATUS     ((u32)0x00000001 << 5)          /*!<R   Ongoing status of adc_0_silencedetection 1'b0: adc_0_silencedetection is resting (clock is gating) 1'b1: adc_0_silencedetection is working */
#define AUD_BIT_ADC_1_SILENCE_DET_O          ((u32)0x00000001 << 6)          /*!<R   adc_1_silencedata status (result of silence detection) 1'b0: not adc_1_silencedata 1'b1: adc_1_silenceis detected */
#define AUD_BIT_ADC_1_SILENCE_DET_STATUS     ((u32)0x00000001 << 7)          /*!<R   Ongoing status of adc_1_silencedetection 1'b0: adc_1_silencedetection is resting (clock is gating) 1'b1: adc_1_silencedetection is working */
#define AUD_BIT_ADC_2_SILENCE_DET_O          ((u32)0x00000001 << 8)          /*!<R   adc_2_silencedata status (result of silence detection) 1'b0: not adc_2_silencedata 1'b1: adc_2_silenceis detected */
#define AUD_BIT_ADC_2_SILENCE_DET_STATUS     ((u32)0x00000001 << 9)          /*!<R   Ongoing status of adc_2_silencedetection 1'b0: adc_2_silencedetection is resting (clock is gating) 1'b1: adc_2_silencedetection is working */
#define AUD_BIT_ADC_3_SILENCE_DET_O          ((u32)0x00000001 << 10)          /*!<R   adc_3_silencedata status (result of silence detection) 1'b0: not adc_3_silencedata 1'b1: adc_3_silenceis detected */
#define AUD_BIT_ADC_3_SILENCE_DET_STATUS     ((u32)0x00000001 << 11)          /*!<R   Ongoing status of adc_3_silencedetection 1'b0: adc_3_silencedetection is resting (clock is gating) 1'b1: adc_3_silencedetection is working */
/** @} */

/** @defgroup CODEC_audio_ro_dummy1
 * @brief
 * @{
 **/
#define AUD_MASK_AUDIO_RO_DUMMY1             ((u32)0xFFFFFFFF << 0)          /*!<R    */
#define AUD_AUDIO_RO_DUMMY1(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define AUD_GET_AUDIO_RO_DUMMY1(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup CODEC_vad_intr_st
 * @brief
 * @{
 **/
#define AUD_MASK_VAD_INTR_ST                 ((u32)0x000000FF << 0)          /*!<R   Status of new vad interrupts and clear them by writing 1 */
#define AUD_VAD_INTR_ST(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_VAD_INTR_ST(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CODEC_vad_det_flag
 * @brief
 * @{
 **/
#define AUD_BIT_VAD_DET_FLAG                 ((u32)0x00000001 << 0)          /*!<R   Vad detection flag 1 */
/** @} */


/*==========AUDIO Register Address Definition==========*/
#define CODEC_AUDIO_CONTROL_0                        0x0000
#define CODEC_AUDIO_CONTROL_1                        0x0004
#define CODEC_CLOCK_CONTROL_1                        0x0008
#define CODEC_CLOCK_CONTROL_2                        0x000C
#define CODEC_CLOCK_CONTROL_3                        0x0010
#define CODEC_CLOCK_CONTROL_4                        0x0014
#define CODEC_CLOCK_CONTROL_5                        0x0018
#define CODEC_CLOCK_CONTROL_6                        0x001C
#define CODEC_CLOCK_CONTROL_7                        0x0020
#define CODEC_ASRC_CONTROL_0                         0x0024
#define CODEC_ASRC_CONTROL_1                         0x0028
#define CODEC_ASRC_CONTROL_2                         0x002C
#define CODEC_ASRC_CONTROL_3                         0x0030
#define CODEC_ASRC_CONTROL_4                         0x0034
#define CODEC_ASRC_CONTROL_5                         0x0038
#define CODEC_I2S_0_CONTROL                          0x003C
#define CODEC_I2S_0_CONTROL_1                        0x0040
#define CODEC_I2S_1_CONTROL                          0x0044
#define CODEC_I2S_1_CONTROL_1                        0x0048
#define CODEC_I2S_AD_SEL_CONTROL                     0x004C
#define CODEC_ADC_0_CONTROL_0                        0x0050
#define CODEC_ADC_0_CONTROL_1                        0x0054
#define CODEC_ADC_1_CONTROL_0                        0x0058
#define CODEC_ADC_1_CONTROL_1                        0x005C
#define CODEC_ADC_2_CONTROL_0                        0x0060
#define CODEC_ADC_2_CONTROL_1                        0x0064
#define CODEC_ADC_3_CONTROL_0                        0x0068
#define CODEC_ADC_3_CONTROL_1                        0x006C
#define CODEC_ADC_4_CONTROL_0                        0x0070
#define CODEC_ADC_4_CONTROL_1                        0x0074
#define CODEC_ADC_5_CONTROL_0                        0x0078
#define CODEC_ADC_5_CONTROL_1                        0x007C
#define CODEC_ADC_6_CONTROL_0                        0x0080
#define CODEC_ADC_6_CONTROL_1                        0x0084
#define CODEC_ADC_7_CONTROL_0                        0x0088
#define CODEC_ADC_7_CONTROL_1                        0x008C
#define CODEC_DAC_L_CONTROL_0                        0x0090
#define CODEC_DAC_L_CONTROL_1                        0x0094
#define CODEC_DAC_L_CONTROL_2                        0x0098
#define CODEC_DAC_R_CONTROL_0                        0x009C
#define CODEC_DAC_R_CONTROL_1                        0x00A0
#define CODEC_DAC_R_CONTROL_2                        0x00A4
#define CODEC_ADC_ALIGN_CONTROL_0                    0x00AC
#define CODEC_ADC_ALIGN_CONTROL                      0x00B0
#define CODEC_DAC_MIX_CONTROL                        0x00BC
#define CODEC_PHASE_COMPENSATION_CONTROL_REGISTER_0  0x00C0
#define CODEC_PHASE_COMPENSATION_CONTROL_REGISTER_1  0x00C4
#define CODEC_PHASE_COMPENSATION_CONTROL_REGISTER_2  0x00C8
#define CODEC_ADC_0_SILENCE_CONTROL                  0x0100
#define CODEC_ADC_1_SILENCE_CONTROL                  0x0104
#define CODEC_ADC_2_SILENCE_CONTROL                  0x0108
#define CODEC_ADC_3_SILENCE_CONTROL                  0x010C
#define CODEC_ADC_4_SILENCE_CONTROL                  0x0110
#define CODEC_ADC_5_SILENCE_CONTROL                  0x0114
#define CODEC_ADC_6_SILENCE_CONTROL                  0x0118
#define CODEC_ADC_7_SILENCE_CONTROL                  0x011C
#define CODEC_DAC_L_SILENCE_CONTROL                  0x0120
#define CODEC_DAC_R_SILENCE_CONTROL                  0x0124
#define CODEC_ST_CONTROL_0                           0x0128
#define CODEC_ST_CONTROL_1                           0x012C
#define CODEC_ST_DS_CONTROL_0                        0x0130
#define CODEC_ST_DS_CONTROL_1                        0x0134
#define CODEC_ST_DS_L_EQ_CTRL                        0x0138
#define CODEC_ST_DS_L_BIQUAD_H0_0                    0x013C
#define CODEC_ST_DS_L_BIQUAD_B1_0                    0x0140
#define CODEC_ST_DS_L_BIQUAD_B2_0                    0x0144
#define CODEC_ST_DS_L_BIQUAD_A1_0                    0x0148
#define CODEC_ST_DS_L_BIQUAD_A2_0                    0x014C
#define CODEC_ST_DS_L_BIQUAD_H0_1                    0x0150
#define CODEC_ST_DS_L_BIQUAD_B1_1                    0x0154
#define CODEC_ST_DS_L_BIQUAD_B2_1                    0x0158
#define CODEC_ST_DS_L_BIQUAD_A1_1                    0x015C
#define CODEC_ST_DS_L_BIQUAD_A2_1                    0x0160
#define CODEC_ST_DS_L_BIQUAD_H0_2                    0x0164
#define CODEC_ST_DS_L_BIQUAD_B1_2                    0x0168
#define CODEC_ST_DS_L_BIQUAD_B2_2                    0x016C
#define CODEC_ST_DS_L_BIQUAD_A1_2                    0x0170
#define CODEC_ST_DS_L_BIQUAD_A2_2                    0x0174
#define CODEC_ST_DS_R_EQ_CTRL                        0x0178
#define CODEC_ST_DS_R_BIQUAD_H0_0                    0x017C
#define CODEC_ST_DS_R_BIQUAD_B1_0                    0x0180
#define CODEC_ST_DS_R_BIQUAD_B2_0                    0x0184
#define CODEC_ST_DS_R_BIQUAD_A1_0                    0x0188
#define CODEC_ST_DS_R_BIQUAD_A2_0                    0x018C
#define CODEC_ST_DS_R_BIQUAD_H0_1                    0x0190
#define CODEC_ST_DS_R_BIQUAD_B1_1                    0x0194
#define CODEC_ST_DS_R_BIQUAD_B2_1                    0x0198
#define CODEC_ST_DS_R_BIQUAD_A1_1                    0x019C
#define CODEC_ST_DS_R_BIQUAD_A2_1                    0x01A0
#define CODEC_ST_DS_R_BIQUAD_H0_2                    0x01A4
#define CODEC_ST_DS_R_BIQUAD_B1_2                    0x01A8
#define CODEC_ST_DS_R_BIQUAD_B2_2                    0x01AC
#define CODEC_ST_DS_R_BIQUAD_A1_2                    0x01B0
#define CODEC_ST_DS_R_BIQUAD_A2_2                    0x01B4
#define CODEC_DAC_L_EQ_CTRL                          0x0200
#define CODEC_DAC_L_BIQUAD_H0_0                      0x0204
#define CODEC_DAC_L_BIQUAD_B1_0                      0x0208
#define CODEC_DAC_L_BIQUAD_B2_0                      0x020C
#define CODEC_DAC_L_BIQUAD_A1_0                      0x0210
#define CODEC_DAC_L_BIQUAD_A2_0                      0x0214
#define CODEC_DAC_L_BIQUAD_H0_1                      0x0218
#define CODEC_DAC_L_BIQUAD_B1_1                      0x021C
#define CODEC_DAC_L_BIQUAD_B2_1                      0x0220
#define CODEC_DAC_L_BIQUAD_A1_1                      0x0224
#define CODEC_DAC_L_BIQUAD_A2_1                      0x0228
#define CODEC_DAC_L_BIQUAD_H0_2                      0x022C
#define CODEC_DAC_L_BIQUAD_B1_2                      0x0230
#define CODEC_DAC_L_BIQUAD_B2_2                      0x0234
#define CODEC_DAC_L_BIQUAD_A1_2                      0x0238
#define CODEC_DAC_L_BIQUAD_A2_2                      0x023C
#define CODEC_DAC_L_BIQUAD_H0_3                      0x0240
#define CODEC_DAC_L_BIQUAD_B1_3                      0x0244
#define CODEC_DAC_L_BIQUAD_B2_3                      0x0248
#define CODEC_DAC_L_BIQUAD_A1_3                      0x024C
#define CODEC_DAC_L_BIQUAD_A2_3                      0x0250
#define CODEC_DAC_L_BIQUAD_H0_4                      0x0254
#define CODEC_DAC_L_BIQUAD_B1_4                      0x0258
#define CODEC_DAC_L_BIQUAD_B2_4                      0x025C
#define CODEC_DAC_L_BIQUAD_A1_4                      0x0260
#define CODEC_DAC_L_BIQUAD_A2_4                      0x0264
#define CODEC_DAC_L_BIQUAD_H0_5                      0x0268
#define CODEC_DAC_L_BIQUAD_B1_5                      0x026C
#define CODEC_DAC_L_BIQUAD_B2_5                      0x0270
#define CODEC_DAC_L_BIQUAD_A1_5                      0x0274
#define CODEC_DAC_L_BIQUAD_A2_5                      0x0278
#define CODEC_DAC_L_BIQUAD_H0_6                      0x027C
#define CODEC_DAC_L_BIQUAD_B1_6                      0x0280
#define CODEC_DAC_L_BIQUAD_B2_6                      0x0284
#define CODEC_DAC_L_BIQUAD_A1_6                      0x0288
#define CODEC_DAC_L_BIQUAD_A2_6                      0x028C
#define CODEC_DAC_L_BIQUAD_H0_7                      0x0290
#define CODEC_DAC_L_BIQUAD_B1_7                      0x0294
#define CODEC_DAC_L_BIQUAD_B2_7                      0x0298
#define CODEC_DAC_L_BIQUAD_A1_7                      0x029C
#define CODEC_DAC_L_BIQUAD_A2_7                      0x02A0
#define CODEC_DAC_L_BIQUAD_H0_8                      0x02A4
#define CODEC_DAC_L_BIQUAD_B1_8                      0x02A8
#define CODEC_DAC_L_BIQUAD_B2_8                      0x02AC
#define CODEC_DAC_L_BIQUAD_A1_8                      0x02B0
#define CODEC_DAC_L_BIQUAD_A2_8                      0x02B4
#define CODEC_DAC_L_BIQUAD_H0_9                      0x02B8
#define CODEC_DAC_L_BIQUAD_B1_9                      0x02BC
#define CODEC_DAC_L_BIQUAD_B2_9                      0x02C0
#define CODEC_DAC_L_BIQUAD_A1_9                      0x02C4
#define CODEC_DAC_L_BIQUAD_A2_9                      0x02C8
#define CODEC_DAC_R_EQ_CTRL                          0x0300
#define CODEC_DAC_R_BIQUAD_H0_0                      0x0304
#define CODEC_DAC_R_BIQUAD_B1_0                      0x0308
#define CODEC_DAC_R_BIQUAD_B2_0                      0x030C
#define CODEC_DAC_R_BIQUAD_A1_0                      0x0310
#define CODEC_DAC_R_BIQUAD_A2_0                      0x0314
#define CODEC_DAC_R_BIQUAD_H0_1                      0x0318
#define CODEC_DAC_R_BIQUAD_B1_1                      0x031C
#define CODEC_DAC_R_BIQUAD_B2_1                      0x0320
#define CODEC_DAC_R_BIQUAD_A1_1                      0x0324
#define CODEC_DAC_R_BIQUAD_A2_1                      0x0328
#define CODEC_DAC_R_BIQUAD_H0_2                      0x032C
#define CODEC_DAC_R_BIQUAD_B1_2                      0x0330
#define CODEC_DAC_R_BIQUAD_B2_2                      0x0334
#define CODEC_DAC_R_BIQUAD_A1_2                      0x0338
#define CODEC_DAC_R_BIQUAD_A2_2                      0x033C
#define CODEC_DAC_R_BIQUAD_H0_3                      0x0340
#define CODEC_DAC_R_BIQUAD_B1_3                      0x0344
#define CODEC_DAC_R_BIQUAD_B2_3                      0x0348
#define CODEC_DAC_R_BIQUAD_A1_3                      0x034C
#define CODEC_DAC_R_BIQUAD_A2_3                      0x0350
#define CODEC_DAC_R_BIQUAD_H0_4                      0x0354
#define CODEC_DAC_R_BIQUAD_B1_4                      0x0358
#define CODEC_DAC_R_BIQUAD_B2_4                      0x035C
#define CODEC_DAC_R_BIQUAD_A1_4                      0x0360
#define CODEC_DAC_R_BIQUAD_A2_4                      0x0364
#define CODEC_DAC_R_BIQUAD_H0_5                      0x0368
#define CODEC_DAC_R_BIQUAD_B1_5                      0x036C
#define CODEC_DAC_R_BIQUAD_B2_5                      0x0370
#define CODEC_DAC_R_BIQUAD_A1_5                      0x0374
#define CODEC_DAC_R_BIQUAD_A2_5                      0x0378
#define CODEC_DAC_R_BIQUAD_H0_6                      0x037C
#define CODEC_DAC_R_BIQUAD_B1_6                      0x0380
#define CODEC_DAC_R_BIQUAD_B2_6                      0x0384
#define CODEC_DAC_R_BIQUAD_A1_6                      0x0388
#define CODEC_DAC_R_BIQUAD_A2_6                      0x038C
#define CODEC_DAC_R_BIQUAD_H0_7                      0x0390
#define CODEC_DAC_R_BIQUAD_B1_7                      0x0394
#define CODEC_DAC_R_BIQUAD_B2_7                      0x0398
#define CODEC_DAC_R_BIQUAD_A1_7                      0x039C
#define CODEC_DAC_R_BIQUAD_A2_7                      0x03A0
#define CODEC_DAC_R_BIQUAD_H0_8                      0x03A4
#define CODEC_DAC_R_BIQUAD_B1_8                      0x03A8
#define CODEC_DAC_R_BIQUAD_B2_8                      0x03AC
#define CODEC_DAC_R_BIQUAD_A1_8                      0x03B0
#define CODEC_DAC_R_BIQUAD_A2_8                      0x03B4
#define CODEC_DAC_R_BIQUAD_H0_9                      0x03B8
#define CODEC_DAC_R_BIQUAD_B1_9                      0x03BC
#define CODEC_DAC_R_BIQUAD_B2_9                      0x03C0
#define CODEC_DAC_R_BIQUAD_A1_9                      0x03C4
#define CODEC_DAC_R_BIQUAD_A2_9                      0x03C8
#define CODEC_ADC_0_EQ_CTRL                          0x0400
#define CODEC_ADC_0_BIQUAD_H0_0                      0x0404
#define CODEC_ADC_0_BIQUAD_B1_0                      0x0408
#define CODEC_ADC_0_BIQUAD_B2_0                      0x040C
#define CODEC_ADC_0_BIQUAD_A1_0                      0x0410
#define CODEC_ADC_0_BIQUAD_A2_0                      0x0414
#define CODEC_ADC_0_BIQUAD_H0_1                      0x0418
#define CODEC_ADC_0_BIQUAD_B1_1                      0x041C
#define CODEC_ADC_0_BIQUAD_B2_1                      0x0420
#define CODEC_ADC_0_BIQUAD_A1_1                      0x0424
#define CODEC_ADC_0_BIQUAD_A2_1                      0x0428
#define CODEC_ADC_0_BIQUAD_H0_2                      0x042C
#define CODEC_ADC_0_BIQUAD_B1_2                      0x0430
#define CODEC_ADC_0_BIQUAD_B2_2                      0x0434
#define CODEC_ADC_0_BIQUAD_A1_2                      0x0438
#define CODEC_ADC_0_BIQUAD_A2_2                      0x043C
#define CODEC_ADC_0_BIQUAD_H0_3                      0x0440
#define CODEC_ADC_0_BIQUAD_B1_3                      0x0444
#define CODEC_ADC_0_BIQUAD_B2_3                      0x0448
#define CODEC_ADC_0_BIQUAD_A1_3                      0x044C
#define CODEC_ADC_0_BIQUAD_A2_3                      0x0450
#define CODEC_ADC_0_BIQUAD_H0_4                      0x0454
#define CODEC_ADC_0_BIQUAD_B1_4                      0x0458
#define CODEC_ADC_0_BIQUAD_B2_4                      0x045C
#define CODEC_ADC_0_BIQUAD_A1_4                      0x0460
#define CODEC_ADC_0_BIQUAD_A2_4                      0x0464
#define CODEC_ADC_1_EQ_CTRL                          0x0468
#define CODEC_ADC_1_BIQUAD_H0_0                      0x046C
#define CODEC_ADC_1_BIQUAD_B1_0                      0x0470
#define CODEC_ADC_1_BIQUAD_B2_0                      0x0474
#define CODEC_ADC_1_BIQUAD_A1_0                      0x0478
#define CODEC_ADC_1_BIQUAD_A2_0                      0x047C
#define CODEC_ADC_1_BIQUAD_H0_1                      0x0480
#define CODEC_ADC_1_BIQUAD_B1_1                      0x0484
#define CODEC_ADC_1_BIQUAD_B2_1                      0x0488
#define CODEC_ADC_1_BIQUAD_A1_1                      0x048C
#define CODEC_ADC_1_BIQUAD_A2_1                      0x0490
#define CODEC_ADC_1_BIQUAD_H0_2                      0x0494
#define CODEC_ADC_1_BIQUAD_B1_2                      0x0498
#define CODEC_ADC_1_BIQUAD_B2_2                      0x049C
#define CODEC_ADC_1_BIQUAD_A1_2                      0x04A0
#define CODEC_ADC_1_BIQUAD_A2_2                      0x04A4
#define CODEC_ADC_1_BIQUAD_H0_3                      0x04A8
#define CODEC_ADC_1_BIQUAD_B1_3                      0x04AC
#define CODEC_ADC_1_BIQUAD_B2_3                      0x04B0
#define CODEC_ADC_1_BIQUAD_A1_3                      0x04B4
#define CODEC_ADC_1_BIQUAD_A2_3                      0x04B8
#define CODEC_ADC_1_BIQUAD_H0_4                      0x04BC
#define CODEC_ADC_1_BIQUAD_B1_4                      0x04C0
#define CODEC_ADC_1_BIQUAD_B2_4                      0x04C4
#define CODEC_ADC_1_BIQUAD_A1_4                      0x04C8
#define CODEC_ADC_1_BIQUAD_A2_4                      0x04CC
#define CODEC_ADC_2_EQ_CTRL                          0x0500
#define CODEC_ADC_2_BIQUAD_H0_0                      0x0504
#define CODEC_ADC_2_BIQUAD_B1_0                      0x0508
#define CODEC_ADC_2_BIQUAD_B2_0                      0x050C
#define CODEC_ADC_2_BIQUAD_A1_0                      0x0510
#define CODEC_ADC_2_BIQUAD_A2_0                      0x0514
#define CODEC_ADC_2_BIQUAD_H0_1                      0x0518
#define CODEC_ADC_2_BIQUAD_B1_1                      0x051C
#define CODEC_ADC_2_BIQUAD_B2_1                      0x0520
#define CODEC_ADC_2_BIQUAD_A1_1                      0x0524
#define CODEC_ADC_2_BIQUAD_A2_1                      0x0528
#define CODEC_ADC_2_BIQUAD_H0_2                      0x052C
#define CODEC_ADC_2_BIQUAD_B1_2                      0x0530
#define CODEC_ADC_2_BIQUAD_B2_2                      0x0534
#define CODEC_ADC_2_BIQUAD_A1_2                      0x0538
#define CODEC_ADC_2_BIQUAD_A2_2                      0x053C
#define CODEC_ADC_2_BIQUAD_H0_3                      0x0540
#define CODEC_ADC_2_BIQUAD_B1_3                      0x0544
#define CODEC_ADC_2_BIQUAD_B2_3                      0x0548
#define CODEC_ADC_2_BIQUAD_A1_3                      0x054C
#define CODEC_ADC_2_BIQUAD_A2_3                      0x0550
#define CODEC_ADC_2_BIQUAD_H0_4                      0x0554
#define CODEC_ADC_2_BIQUAD_B1_4                      0x0558
#define CODEC_ADC_2_BIQUAD_B2_4                      0x055C
#define CODEC_ADC_2_BIQUAD_A1_4                      0x0560
#define CODEC_ADC_2_BIQUAD_A2_4                      0x0564
#define CODEC_ADC_3_EQ_CTRL                          0x0568
#define CODEC_ADC_3_BIQUAD_H0_0                      0x056C
#define CODEC_ADC_3_BIQUAD_B1_0                      0x0570
#define CODEC_ADC_3_BIQUAD_B2_0                      0x0574
#define CODEC_ADC_3_BIQUAD_A1_0                      0x0578
#define CODEC_ADC_3_BIQUAD_A2_0                      0x057C
#define CODEC_ADC_3_BIQUAD_H0_1                      0x0580
#define CODEC_ADC_3_BIQUAD_B1_1                      0x0584
#define CODEC_ADC_3_BIQUAD_B2_1                      0x0588
#define CODEC_ADC_3_BIQUAD_A1_1                      0x058C
#define CODEC_ADC_3_BIQUAD_A2_1                      0x0590
#define CODEC_ADC_3_BIQUAD_H0_2                      0x0594
#define CODEC_ADC_3_BIQUAD_B1_2                      0x0598
#define CODEC_ADC_3_BIQUAD_B2_2                      0x059C
#define CODEC_ADC_3_BIQUAD_A1_2                      0x05A0
#define CODEC_ADC_3_BIQUAD_A2_2                      0x05A4
#define CODEC_ADC_3_BIQUAD_H0_3                      0x05A8
#define CODEC_ADC_3_BIQUAD_B1_3                      0x05AC
#define CODEC_ADC_3_BIQUAD_B2_3                      0x05B0
#define CODEC_ADC_3_BIQUAD_A1_3                      0x05B4
#define CODEC_ADC_3_BIQUAD_A2_3                      0x05B8
#define CODEC_ADC_3_BIQUAD_H0_4                      0x05BC
#define CODEC_ADC_3_BIQUAD_B1_4                      0x05C0
#define CODEC_ADC_3_BIQUAD_B2_4                      0x05C4
#define CODEC_ADC_3_BIQUAD_A1_4                      0x05C8
#define CODEC_ADC_3_BIQUAD_A2_4                      0x05CC
#define CODEC_ADC_4_EQ_CTRL                          0x0600
#define CODEC_ADC_4_BIQUAD_H0_0                      0x0604
#define CODEC_ADC_4_BIQUAD_B1_0                      0x0608
#define CODEC_ADC_4_BIQUAD_B2_0                      0x060C
#define CODEC_ADC_4_BIQUAD_A1_0                      0x0610
#define CODEC_ADC_4_BIQUAD_A2_0                      0x0614
#define CODEC_ADC_4_BIQUAD_H0_1                      0x0618
#define CODEC_ADC_4_BIQUAD_B1_1                      0x061C
#define CODEC_ADC_4_BIQUAD_B2_1                      0x0620
#define CODEC_ADC_4_BIQUAD_A1_1                      0x0624
#define CODEC_ADC_4_BIQUAD_A2_1                      0x0628
#define CODEC_ADC_4_BIQUAD_H0_2                      0x062C
#define CODEC_ADC_4_BIQUAD_B1_2                      0x0630
#define CODEC_ADC_4_BIQUAD_B2_2                      0x0634
#define CODEC_ADC_4_BIQUAD_A1_2                      0x0638
#define CODEC_ADC_4_BIQUAD_A2_2                      0x063C
#define CODEC_ADC_4_BIQUAD_H0_3                      0x0640
#define CODEC_ADC_4_BIQUAD_B1_3                      0x0644
#define CODEC_ADC_4_BIQUAD_B2_3                      0x0648
#define CODEC_ADC_4_BIQUAD_A1_3                      0x064C
#define CODEC_ADC_4_BIQUAD_A2_3                      0x0650
#define CODEC_ADC_4_BIQUAD_H0_4                      0x0654
#define CODEC_ADC_4_BIQUAD_B1_4                      0x0658
#define CODEC_ADC_4_BIQUAD_B2_4                      0x065C
#define CODEC_ADC_4_BIQUAD_A1_4                      0x0660
#define CODEC_ADC_4_BIQUAD_A2_4                      0x0664
#define CODEC_ADC_5_EQ_CTRL                          0x0668
#define CODEC_ADC_5_BIQUAD_H0_0                      0x066C
#define CODEC_ADC_5_BIQUAD_B1_0                      0x0670
#define CODEC_ADC_5_BIQUAD_B2_0                      0x0674
#define CODEC_ADC_5_BIQUAD_A1_0                      0x0678
#define CODEC_ADC_5_BIQUAD_A2_0                      0x067C
#define CODEC_ADC_5_BIQUAD_H0_1                      0x0680
#define CODEC_ADC_5_BIQUAD_B1_1                      0x0684
#define CODEC_ADC_5_BIQUAD_B2_1                      0x0688
#define CODEC_ADC_5_BIQUAD_A1_1                      0x068C
#define CODEC_ADC_5_BIQUAD_A2_1                      0x0690
#define CODEC_ADC_5_BIQUAD_H0_2                      0x0694
#define CODEC_ADC_5_BIQUAD_B1_2                      0x0698
#define CODEC_ADC_5_BIQUAD_B2_2                      0x069C
#define CODEC_ADC_5_BIQUAD_A1_2                      0x06A0
#define CODEC_ADC_5_BIQUAD_A2_2                      0x06A4
#define CODEC_ADC_5_BIQUAD_H0_3                      0x06A8
#define CODEC_ADC_5_BIQUAD_B1_3                      0x06AC
#define CODEC_ADC_5_BIQUAD_B2_3                      0x06B0
#define CODEC_ADC_5_BIQUAD_A1_3                      0x06B4
#define CODEC_ADC_5_BIQUAD_A2_3                      0x06B8
#define CODEC_ADC_5_BIQUAD_H0_4                      0x06BC
#define CODEC_ADC_5_BIQUAD_B1_4                      0x06C0
#define CODEC_ADC_5_BIQUAD_B2_4                      0x06C4
#define CODEC_ADC_5_BIQUAD_A1_4                      0x06C8
#define CODEC_ADC_5_BIQUAD_A2_4                      0x06CC
#define CODEC_VAD_CTRL                               0x0700
#define CODEC_VAD_VOICE_LEVEL                        0x0704
#define CODEC_VAD_DET_THR                            0x0708
#define CODEC_VAD_INTR_EN                            0x070C
#define CODEC_DAC_DRE_CONTROL_0                      0x0710
#define CODEC_ANA_READ                               0x0800
#define CODEC_ADC_0_LPF_RD                           0x0804
#define CODEC_ADC_1_LPF_RD                           0x0808
#define CODEC_ADC_2_LPF_RD                           0x080C
#define CODEC_ADC_3_LPF_RD                           0x0810
#define CODEC_ADC_4_LPF_RD                           0x0814
#define CODEC_ADC_5_LPF_RD                           0x0818
#define CODEC_ADC_6_LPF_RD                           0x081C
#define CODEC_ADC_7_LPF_RD                           0x0820
#define CODEC_SILENCE_INFORM                         0x0824
#define CODEC_AUDIO_RO_DUMMY1                        0x0828
#define CODEC_VAD_INTR_ST                            0x0900
#define CODEC_VAD_DET_FLAG                           0x0904

/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

/* Exported types ------------------------------------------------------------*/
/** @defgroup AUDIO_CODEC_Exported_Types AUDIO_CODEC Exported Types
  * @{
  */

/**
 * @brief AUDIO_CODEC Init Structure Definition
 */
typedef struct {

	u32 CODEC_SelI2STxSR;				/*!< Specifies the AUDIO CODEC TX Sample rate
										This parameter can be a value of @ref AUDIO_CODEC_Sample_Rate */

	u32 CODEC_SelI2STxWordLen; 			/*!< Specifies the AUDIO CODEC TX Word length
										This parameter can be a value of @ref AUDIO_CODEC_Word_Length */

	u32 CODEC_SelI2STxCHLen;			/*!< Specifies the AUDIO SPORT TX Channel length
										This parameter can be a value of @ref AUDIO_CODEC_Channel_Length  */

	u32 CODEC_SelI2STxDataFormat;		/*!< Specifies the AUDIO CODEC I2S TX selection of data fornat
										This parameter can be a value of @ref AUDIO_CODEC_Interface_Format */

	u32 CODEC_SelI2SRxSR;				/*!< Specifies the AUDIO CODEC RX Sample rate
										This parameter can be a value of @ref AUDIO_CODEC_Sample_Rate */

	u32 CODEC_SelI2SRxWordLen; 			/*!< Specifies the AUDIO CODEC RX Word length
										This parameter can be a value of @ref AUDIO_CODEC_Word_Length */

	u32 CODEC_SelI2SRxCHLen;			/*!< Specifies the AUDIO SPORT RX Channel length
										This parameter can be a value of @ref AUDIO_CODEC_Channel_Length  */

	u32 CODEC_SelI2SRxDataFormat;		/*!< Specifies the AUDIO CODEC I2S RX selection of data fornat
										This parameter can be a value of @ref AUDIO_CODEC_Interface_Format */

	u32 CODEC_SelRxI2STdm;				/*!< Specifies the AUDIO CODEC Rx I2S TDM MODE
										This parameter can be a value of @ref AUDIO_CODEC_I2S_TDM_Mode */

} I2S_InitTypeDef;
/**
  * @}
  */

/* Exported constants ------------------------------------------------------------*/
/** @defgroup AUDIO_CODEC_Exported_Constants AUDIO_CODEC Exported Constants
  * @{
  */

/** @defgroup AUDIO_CODEC_Sample_Rate
  * @{
  */
#define SR_48K			((u32)0x00000000)
#define SR_96K			((u32)0x00000001)
#define SR_192K			((u32)0x00000002)
#define SR_32K			((u32)0x00000003)
#define SR_176P4K		((u32)0x00000004)
#define SR_16K			((u32)0x00000005)
#define SR_8K			((u32)0x00000007)
#define SR_44P1K		((u32)0x00000008)
#define SR_88P2K		((u32)0x00000009)
#define SR_24K			((u32)0x0000000a)
#define SR_12K			((u32)0x0000000b)
#define SR_22P05K		((u32)0x0000000c)
#define SR_11P025K		((u32)0x0000000d)

#define IS_CODEC_SR_SEL(SEL) (((SEL) == SR_48K) || \
							((SEL) == SR_96K) || \
							((SEL) == SR_192K) || \
							((SEL) == SR_32K) || \
							((SEL) == SR_176P4K) || \
							((SEL) == SR_16K) || \
							((SEL) == SR_8K) || \
							((SEL) == SR_44P1K) || \
							((SEL) == SR_88P2K) || \
							((SEL) == SR_24K) || \
							((SEL) == SR_12K) || \
							((SEL) == SR_22P05K) || \
							((SEL) == SR_11P025K))
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Word_Length
  * @{
  */
#define WL_16			((u32)0x00000000)
#define WL_24			((u32)0x00000002)
#define WL_8			((u32)0x00000003)

#define IS_CODEC_WL_SEL(SEL) (((SEL) == WL_16) || \
							((SEL) == WL_24) || \
							((SEL) == WL_8))
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Channel_Length
  * @{
  */
#define CL_16			((u32)0x00000000)
#define CL_32			((u32)0x00000001)
#define CL_24			((u32)0x00000002)
#define CL_8			((u32)0x00000003)

#define IS_CODEC_CL_SEL(SEL) (((SEL) == CL_16) || \
							((SEL) == CL_32) || \
							((SEL) == CL_24) || \
							((SEL) == CL_8))
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Interface_Format
  * @{
  */
#define DF_I2S						((u32)0x00000000)
#define DF_LEFT						((u32)0x00000001)
#define DF_PCM_A						((u32)0x00000002)
#define DF_PCM_B						((u32)0x00000003)

#define IS_CODEC_DATA_FMT(FORMAT) (((FORMAT) == DF_I2S) || \
											((FORMAT) == DF_LEFT) || \
											((FORMAT) == DF_PCM_A) || \
											((FORMAT) == DF_PCM_B))
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_I2S_TDM_Mode
  * @{
  */
#define I2S_NOTDM						    ((u32)0x00000000)
#define I2S_TDM4							((u32)0x00000001)

#define IS_CODEC_I2S_TDM_SEL(SEL) (((SEL) == I2S_NOTDM) || \
							((SEL) == I2S_TDM4))
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_AMIC
  * @{
  */
#define AMIC1							((u32)0x00000001)
#define AMIC2							((u32)0x00000002)
#define AMIC3							((u32)0x00000003)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_ADC
  * @{
  */
#define ADC1							((u32)0x00000001)
#define ADC2							((u32)0x00000002)
#define ADC3							((u32)0x00000003)
#define ADC4							((u32)0x00000004)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_ADC_Channel
  * @{
  */
#define ADCHN1							((u32)0x00000001)
#define ADCHN2							((u32)0x00000002)
#define ADCHN3							((u32)0x00000003)
#define ADCHN4							((u32)0x00000004)
#define IS_CODEC_ADCHN_SEL(SEL) (((SEL) == ADCHN1) || \
								((SEL) == ADCHN2) || \
								((SEL) == ADCHN3) || \
								((SEL) == ADCHN4))
/**
* @}
*/

/** @defgroup AUDIO_CODEC_LineIn_Channel
  * @{
  */
#define LINEIN1							((u32)0x00000001)
#define LINEIN2							((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Power
  * @{
  */
#define NORMALPOWER						((u32)0x00000001)
#define SHUTDOWN						((u32)0x00000000)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_DMIC
  * @{
  */
#define DMIC1							((u32)0x00000001)
#define DMIC2							((u32)0x00000002)
#define DMIC3							((u32)0x00000003)
#define DMIC4							((u32)0x00000004)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_DMIC_Clock
  * @{
  */
#define DMIC_5M			((u32)0x00000000)
#define DMIC_2P5M		((u32)0x00000001)
#define DMIC_1P25M		((u32)0x00000002)
#define DMIC_625K		((u32)0x00000003)
#define DMIC_312P5K		((u32)0x00000004)
#define DMIC_769P2K		((u32)0x00000007)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_PDM_Clock
  * @{
  */
#define PDM_5M			((u32)0x00000000)
#define PDM_2P5M		((u32)0x00000001)
#define PDM_6P67M		((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Micbst_Gain
  * @{
  */
#define MICBST_GAIN_0DB			((u32)0x00000000)
#define MICBST_GAIN_5DB			((u32)0x00000001)
#define MICBST_GAIN_10DB		((u32)0x00000002)
#define MICBST_GAIN_15DB		((u32)0x00000003)
#define MICBST_GAIN_20DB		((u32)0x00000004)
#define MICBST_GAIN_25DB		((u32)0x00000005)
#define MICBST_GAIN_30DB		((u32)0x00000006)
#define MICBST_GAIN_35DB		((u32)0x00000007)
#define MICBST_GAIN_40DB		((u32)0x00000008)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Power_Mode
  * @{
  */
#define POWER_ON			((u32)0x00000001)
#define POWER_DOWN			((u32)0x00000000)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Mute
  * @{
  */
#define UNMUTE			((u32)0x00000000)
#define MUTE			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_I2S_Master_Source
  * @{
  */
#define INTERNAL_SPORT			((u32)0x00000000)
#define EXTERNAL_I2S			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_LintOut_Mode
  * @{
  */
#define SINGLE			((u32)0x00000000)
#define DIFF			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Sample_Rate_Source
  * @{
  */
#define SOURCE0			((u32)0x00000000)
#define SOURCE1			((u32)0x00000001)
#define SOURCE2			((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_DAC_Source
  * @{
  */
#define I2SL					((u32)0x00000000)
#define TESTTONE			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_ADC_MUTE_Control
  * @{
  */
#define LINEIN				((u32)0x00000001)
#define MICIN				((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_DAC_MUTE_Control
  * @{
  */
#define ANALOGIN			((u32)0x00000001)
#define DACIN				((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_PATH
  * @{
  */
#define DACPATH			((u32)0x00000001)
#define ADCPATH			((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_ADC_Mix_Select
  * @{
  */
#define DMIC			((u32)0x00000001)
#define ANAAD				((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_DAC_Channel
  * @{
  */
#define DAC_L			((u32)0x00000000)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_ADC_Decimation_Source
  * @{
  */
#define AUDIO_ADC			((u32)0x00000000)
#define ANC				((u32)0x00000001)
#define MUSIC			((u32)0x00000002)
#define ANCANDMUSIC				((u32)0x00000003)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Application
  * @{
  */

#define APP_AMIC_RECORD ((u32)0x00000000)
#define APP_DMIC_RECORD ((u32)0x00000001)
#define APP_LINE_OUT	((u32)0x00000002)
#define APP_PDM_OUT		((u32)0x00000003)
#define APP_TESTTONE	((u32)0x00000004)
#define APP_DAAD_LPBK	((u32)0x00000005)
#define APP_ADDA_LPBK	((u32)0x00000006)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_PAD
  * @{
  */
#define PAD_DACL ((u32)0x00000000)
#define PAD_MIC1 ((u32)0x00000001)
#define PAD_MIC2 ((u32)0x00000002)
#define PAD_MIC3 ((u32)0x00000003)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Zero_Detection_Function_Select
  * @{
  */
#define IME				 ((u32)0x00000000)
#define ZDET_IME		 ((u32)0x00000001)
#define ZDET_STEP		 ((u32)0x00000002)
#define ZDET_TIMEOUT	 ((u32)0x00000003)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions ------------------------------------------------------------*/
/** @defgroup AUDIO_CODEC_Exported_Functions AUDIO_CODEC Exported Functions
  * @{
  */
_LONG_CALL_ void AUDIO_CODEC_SetAudioIP(u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetI2SIP(u32 i2s_sel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetI2SSRC(u32 i2s_sel, u32 src);
_LONG_CALL_ void AUDIO_CODEC_SetANAClk(u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetI2SRXTDM(u32 i2s_sel, u32 tdmmode);
_LONG_CALL_ void AUDIO_CODEC_I2S_StructInit(I2S_InitTypeDef *I2S_Struct);
_LONG_CALL_ void AUDIO_CODEC_SetI2SParameters(u32 i2s_sel, u32 path, I2S_InitTypeDef *I2S_Struct);
_LONG_CALL_ void AUDIO_CODEC_SetADCSRSrc(u32 src, u32 sr);
_LONG_CALL_ void AUDIO_CODEC_SetDACSRSrc(u32 src, u32 sr);
_LONG_CALL_ void AUDIO_CODEC_DisPAD(u32 path);
_LONG_CALL_ void AUDIO_CODEC_SetLDOMode(u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_EnableADC(u32 ad_chn, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCVolume(u32 adc_sel, u32 gain);
_LONG_CALL_ void AUDIO_CODEC_SetADCHPF(u32 adc_sel, u32 fc, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCASRC(u32 adc_sel, u32 sr, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCMute(u32 adc_sel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCMixMute(u32 adc_num, u32 type,				  u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCANASrc(u32 ad_chn, u32 amic_num);
_LONG_CALL_ void AUDIO_CODEC_SetADCANAFilter(u32 adc_num, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetMicBiasPowerMode(u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstPowerMode(u32 amic_num, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstChnMute(u32 amic_sel, u32 type, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstInputMode(u32 amic_num, u32 mode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstGain(u32 amic_sel, u32 gain);
_LONG_CALL_ void AUDIO_CODEC_SetADCDmicFilter(u32 adc_num, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDmicClk(u32 clk, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDmicSrc(u32 ad_chn, u32 dmic_num);
_LONG_CALL_ void AUDIO_CODEC_SetDACSrc(u32 i2s_sel, u32 dac_l_src, u32 dac_r_src);
_LONG_CALL_ void AUDIO_CODEC_EnableDAC(u32 channel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACMute(u32 channel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACVolume(u32 channel, u32 gain);
_LONG_CALL_ void AUDIO_CODEC_SetDACHPF(u32 channel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACASRC(u32 sr, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACPowerMode(u32 channel, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetLineOutPowerMode(u32 channel, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetLineOutMode(u32 channel, u32 mode);
_LONG_CALL_ void AUDIO_CODEC_SetLineOutMute(u32 channel, u32 type, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetPDMClk(u32 channel, u32 clk, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetTesttone(u32 fc, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDAADLPBK(u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADDALPBK(u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCDecimationSrc(u32 adc_sel, u32 type);
_LONG_CALL_ void AUDIO_CODEC_SetDACZDET(u32 channel, u32 type);
_LONG_CALL_ void AUDIO_CODEC_SetDACZDETTimeOut(u32 channel, u32 time_out);
_LONG_CALL_ void AUDIO_CODEC_SetADCZDET(u32 adc_sel, u32 type);
_LONG_CALL_ void AUDIO_CODEC_SetADCZDETTimeOut(u32 adc_sel, u32 time_out);
_LONG_CALL_ void AUDIO_CODEC_Record(u32 i2s_sel, u32 type, I2S_InitTypeDef *I2S_InitStruct);
_LONG_CALL_ void AUDIO_CODEC_Playback(u32 i2s_sel, u32 type, I2S_InitTypeDef *I2S_InitStruct);


/**
  * @}
  */

/* MANUAL_GEN_END */

#endif
/** @} */

/** @} */

/** @} */
