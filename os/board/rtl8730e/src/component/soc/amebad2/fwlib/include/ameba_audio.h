#ifndef _AMEBAD2_AUDIO_H_
#define _AMEBAD2_AUDIO_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup AUDIO
 *  @brief AUDIO driver modules
 *  @{
 */

/** @defgroup AUDIO_CODEC
 *  @brief AUDIO_CODEC driver modules
 *  @{
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
#define AUD_MASK_ADC_0_EQ_NUM_SEL            ((u32)0x00000003 << 5)          /*!<R/W 0  00: 5 band 01: 3 band 02: 0 band 11: Reserved */
#define AUD_ADC_0_EQ_NUM_SEL(x)              ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_0_EQ_NUM_SEL(x)          ((u32)(((x >> 5) & 0x00000003)))
#define AUD_MASK_ADC_1_EQ_NUM_SEL            ((u32)0x00000003 << 7)          /*!<R/W 0  00: 5 band 01: 3 band 02: 0 band 11: Reserved */
#define AUD_ADC_1_EQ_NUM_SEL(x)              ((u32)(((x) & 0x00000003) << 7))
#define AUD_GET_ADC_1_EQ_NUM_SEL(x)          ((u32)(((x >> 7) & 0x00000003)))
#define AUD_MASK_ADC_2_EQ_NUM_SEL            ((u32)0x00000003 << 9)          /*!<R/W 0  00: 5 band 01: 3 band 02: 0 band 11: Reserved */
#define AUD_ADC_2_EQ_NUM_SEL(x)              ((u32)(((x) & 0x00000003) << 9))
#define AUD_GET_ADC_2_EQ_NUM_SEL(x)          ((u32)(((x >> 9) & 0x00000003)))
#define AUD_MASK_ADC_3_EQ_NUM_SEL            ((u32)0x00000003 << 11)          /*!<R/W 0  00: 5 band 01: 3 band 02: 0 band 11: Reserved */
#define AUD_ADC_3_EQ_NUM_SEL(x)              ((u32)(((x) & 0x00000003) << 11))
#define AUD_GET_ADC_3_EQ_NUM_SEL(x)          ((u32)(((x >> 11) & 0x00000003)))
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
#define AUD_MASK_PDM_CLK_SEL                 ((u32)0x00000003 << 5)          /*!<R/W 0  pdm_clk_sel */
#define AUD_PDM_CLK_SEL(x)                   ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_PDM_CLK_SEL(x)               ((u32)(((x >> 5) & 0x00000003)))
#define AUD_MASK_PDM_CH_SWAP                 ((u32)0x00000003 << 7)          /*!<R/W 0x2  pdm_ch_swap */
#define AUD_PDM_CH_SWAP(x)                   ((u32)(((x) & 0x00000003) << 7))
#define AUD_GET_PDM_CH_SWAP(x)               ((u32)(((x >> 7) & 0x00000003)))
#define AUD_BIT_PDM_DATA_PHASE_SEL           ((u32)0x00000001 << 9)          /*!<R/W 1  pdm_data_phase_sel */
#define AUD_BIT_PDM_GAIN_SHIFT_EN            ((u32)0x00000001 << 10)          /*!<R/W 1  pdm_gain_shift_en */
#define AUD_BIT_PDM_CLK_INV_SEL              ((u32)0x00000001 << 11)          /*!<R/W 1  pdm_clk_inv_sel */
#define AUD_BIT_I2S_DATA_RND_EN              ((u32)0x00000001 << 12)          /*!<R/W 1  i2s_data_rnd_en */
/** @} */

/** @defgroup CODEC_clock_control_1
 * @brief
 * @{
 **/
#define AUD_BIT_DA_L_EN                      ((u32)0x00000001 << 0)          /*!<R/W 0  DAC l channel clock enable */
#define AUD_BIT_DA_R_EN                      ((u32)0x00000001 << 1)          /*!<R/W 0  DAC r channel clock enable */
#define AUD_BIT_MOD_L_EN                     ((u32)0x00000001 << 2)          /*!<R/W 0  SDM l channel clock enable */
#define AUD_BIT_MOD_R_EN                     ((u32)0x00000001 << 3)          /*!<R/W 0  SDM r channel clock enable */
#define AUD_BIT_DA_ANA_CLK_EN                ((u32)0x00000001 << 4)          /*!<R/W 0  DAC/ADC analog clock enable */
#define AUD_BIT_DA_FIFO_EN                   ((u32)0x00000001 << 5)          /*!<R/W 0  DAC FIFO clock enable */
#define AUD_BIT_AD_0_EN                      ((u32)0x00000001 << 6)          /*!<R/W 0  ADC channel 0 clock enable */
#define AUD_BIT_AD_1_EN                      ((u32)0x00000001 << 7)          /*!<R/W 0  ADC channel 1 clock enable */
#define AUD_BIT_AD_2_EN                      ((u32)0x00000001 << 8)          /*!<R/W 0  ADC channel 2 clock enable */
#define AUD_BIT_AD_3_EN                      ((u32)0x00000001 << 9)          /*!<R/W 0  ADC channel 3 clock enable */
#define AUD_BIT_AD_4_EN                      ((u32)0x00000001 << 10)          /*!<R/W 0  ADC channel 4 clock enable */
#define AUD_BIT_AD_5_EN                      ((u32)0x00000001 << 11)          /*!<R/W 0  ADC channel 5 clock enable */
#define AUD_BIT_AD_6_EN                      ((u32)0x00000001 << 12)          /*!<R/W 0  ADC channel 6 clock enable */
#define AUD_BIT_AD_7_EN                      ((u32)0x00000001 << 13)          /*!<R/W 0  ADC channel 7 clock enable */
#define AUD_BIT_AD_0_FIFO_EN                 ((u32)0x00000001 << 14)          /*!<R/W 0  ADC channel 0 FIFO clock enable */
#define AUD_BIT_AD_1_FIFO_EN                 ((u32)0x00000001 << 15)          /*!<R/W 0  ADC channel 1 FIFO clock enable */
#define AUD_BIT_AD_2_FIFO_EN                 ((u32)0x00000001 << 16)          /*!<R/W 0  ADC channel 2 FIFO clock enable */
#define AUD_BIT_AD_3_FIFO_EN                 ((u32)0x00000001 << 17)          /*!<R/W 0  ADC channel 3 FIFO clock enable */
#define AUD_BIT_AD_4_FIFO_EN                 ((u32)0x00000001 << 18)          /*!<R/W 0  ADC channel 4 FIFO clock enable */
#define AUD_BIT_AD_5_FIFO_EN                 ((u32)0x00000001 << 19)          /*!<R/W 0  ADC channel 5 FIFO clock enable */
#define AUD_BIT_AD_6_FIFO_EN                 ((u32)0x00000001 << 20)          /*!<R/W 0  ADC channel 6 FIFO clock enable */
#define AUD_BIT_AD_7_FIFO_EN                 ((u32)0x00000001 << 21)          /*!<R/W 0  ADC channel 7 FIFO clock enable */
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
#define AUD_BIT_AD_ANA_4_EN                  ((u32)0x00000001 << 4)          /*!<R/W 0  ADC filter channel 4 clock enable: analog ADC path */
#define AUD_BIT_AD_ANA_5_EN                  ((u32)0x00000001 << 5)          /*!<R/W 0  ADC filter channel 5 clock enable: analog ADC path */
#define AUD_BIT_AD_ANA_6_EN                  ((u32)0x00000001 << 6)          /*!<R/W 0  ADC filter channel 6 clock enable: analog ADC path */
#define AUD_BIT_AD_ANA_7_EN                  ((u32)0x00000001 << 7)          /*!<R/W 0  ADC filter channel 7 clock enable: analog ADC path */
#define AUD_BIT_DMIC_0_EN                    ((u32)0x00000001 << 8)          /*!<R/W 0  ADC filter channel 0 clock enable: dmic path */
#define AUD_BIT_DMIC_1_EN                    ((u32)0x00000001 << 9)          /*!<R/W 0  ADC filter channel 1 clock enable: dmic path */
#define AUD_BIT_DMIC_2_EN                    ((u32)0x00000001 << 10)          /*!<R/W 0  ADC filter channel 2 clock enable: dmic path */
#define AUD_BIT_DMIC_3_EN                    ((u32)0x00000001 << 11)          /*!<R/W 0  ADC filter channel 3 clock enable: dmic path */
#define AUD_BIT_DMIC_4_EN                    ((u32)0x00000001 << 12)          /*!<R/W 0  ADC filter channel 4 clock enable: dmic path */
#define AUD_BIT_DMIC_5_EN                    ((u32)0x00000001 << 13)          /*!<R/W 0  ADC filter channel 5 clock enable: dmic path */
#define AUD_BIT_DMIC_6_EN                    ((u32)0x00000001 << 14)          /*!<R/W 0  ADC filter channel 6 clock enable: dmic path */
#define AUD_BIT_DMIC_7_EN                    ((u32)0x00000001 << 15)          /*!<R/W 0  ADC filter channel 7 clock enable: dmic path */
#define AUD_BIT_ST_L_EN                      ((u32)0x00000001 << 16)          /*!<R/W 0  Sidetone L channel clock enable */
#define AUD_BIT_ST_R_EN                      ((u32)0x00000001 << 17)          /*!<R/W 0  Sidetone R channel clock enable */
#define AUD_BIT_ST_DS_L_EN                   ((u32)0x00000001 << 18)          /*!<R/W 0  Sidetone downsaple path L channel clock enable */
#define AUD_BIT_ST_DS_R_EN                   ((u32)0x00000001 << 19)          /*!<R/W 0  Sidetone downsaple path R channel clock enable */
#define AUD_BIT_AD_0_EQ_EN                   ((u32)0x00000001 << 20)          /*!<R/W 0  ADC channel 0 EQ clock enable */
#define AUD_BIT_AD_1_EQ_EN                   ((u32)0x00000001 << 21)          /*!<R/W 0  ADC channel 1 EQ clock enable */
#define AUD_BIT_AD_2_EQ_EN                   ((u32)0x00000001 << 22)          /*!<R/W 0  ADC channel 2 EQ clock enable */
#define AUD_BIT_AD_3_EQ_EN                   ((u32)0x00000001 << 23)          /*!<R/W 0  ADC channel 3 EQ clock enable */
#define AUD_BIT_AD_4_EQ_EN                   ((u32)0x00000001 << 24)          /*!<R/W 0  ADC channel 4 EQ clock enable */
#define AUD_BIT_AD_5_EQ_EN                   ((u32)0x00000001 << 25)          /*!<R/W 0  ADC channel 5 EQ clock enable */
#define AUD_BIT_DA_L_EQ_EN                   ((u32)0x00000001 << 26)          /*!<R/W 0  DAC L channel EQ clock enable */
#define AUD_BIT_DA_R_EQ_EN                   ((u32)0x00000001 << 27)          /*!<R/W 0  DAC R channel EQ clock enable */
/** @} */

/** @defgroup CODEC_clock_control_3
 * @brief
 * @{
 **/
#define AUD_MASK_DMIC_CLK_SEL                ((u32)0x00000007 << 0)          /*!<R/W 1  Set clock of digital microphone 3'b000: 5MHz 3'b001: 2.5MHz 3'b010: 1.25MHz 3'b011: 625kHz 3'b100: 312.5kHz 3'b101: Reseved 3'b110: Reserved 3'b111: 769.2kHz */
#define AUD_DMIC_CLK_SEL(x)                  ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_DMIC_CLK_SEL(x)              ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_DMIC_CLK_EN                  ((u32)0x00000001 << 3)          /*!<R/W 0  Digital microphone clock enable */
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
#define AUD_BIT_SAMPLE_RATE_0_EN             ((u32)0x00000001 << 12)          /*!<R/W 1  Sample rate source 0 power enable */
#define AUD_BIT_SAMPLE_RATE_1_EN             ((u32)0x00000001 << 13)          /*!<R/W 1  Sample rate source 1 power enable */
#define AUD_BIT_SAMPLE_RATE_2_EN             ((u32)0x00000001 << 14)          /*!<R/W 1  Sample rate source 2 power enable */
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
#define AUD_MASK_ADC_4_FS_SRC_SEL            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 4 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_4_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_4_FS_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_5_FS_SRC_SEL            ((u32)0x00000003 << 10)          /*!<R/W 0  Channel 5 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_5_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_5_FS_SRC_SEL(x)          ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_6_FS_SRC_SEL            ((u32)0x00000003 << 12)          /*!<R/W 0  Channel 6 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_6_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_ADC_6_FS_SRC_SEL(x)          ((u32)(((x >> 12) & 0x00000003)))
#define AUD_MASK_ADC_7_FS_SRC_SEL            ((u32)0x00000003 << 14)          /*!<R/W 0  Channel 7 ADC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_ADC_7_FS_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 14))
#define AUD_GET_ADC_7_FS_SRC_SEL(x)          ((u32)(((x >> 14) & 0x00000003)))
#define AUD_MASK_DAC_FS_SRC_SEL              ((u32)0x00000003 << 16)          /*!<R/W 0  DAC path sample rate Source selection 2'b00: Source 0 2'b01: Source 1 2'b10: Source 2 */
#define AUD_DAC_FS_SRC_SEL(x)                ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_FS_SRC_SEL(x)            ((u32)(((x >> 16) & 0x00000003)))
/** @} */

/** @defgroup CODEC_clock_control_6
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_0_ASRC_EN                ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 0 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_1_ASRC_EN                ((u32)0x00000001 << 1)          /*!<R/W 0  Channel 1 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_2_ASRC_EN                ((u32)0x00000001 << 2)          /*!<R/W 0  Channel 2 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_3_ASRC_EN                ((u32)0x00000001 << 3)          /*!<R/W 0  Channel 3 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_4_ASRC_EN                ((u32)0x00000001 << 4)          /*!<R/W 0  Channel 4 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_5_ASRC_EN                ((u32)0x00000001 << 5)          /*!<R/W 0  Channel 5 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_6_ASRC_EN                ((u32)0x00000001 << 6)          /*!<R/W 0  Channel 6 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_ADC_7_ASRC_EN                ((u32)0x00000001 << 7)          /*!<R/W 0  Channel 7 ADC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
#define AUD_BIT_DAC_ASRC_EN                  ((u32)0x00000001 << 8)          /*!<R/W 0  DAC path ASRC enable 0: Disable 1: Enable If ASRC is enabled, sample_rate becomes useless. */
/** @} */

/** @defgroup CODEC_clock_control_7
 * @brief
 * @{
 **/
#define AUD_BIT_DA_ANA_CLK_SEL               ((u32)0x00000001 << 0)          /*!<R/W 0  Set clk_da_ana phase 1'b0: Inphase 1'b1: Inverse phase */
#define AUD_BIT_AD_ANA_CLK_SEL               ((u32)0x00000001 << 1)          /*!<R/W 0  Set clk_ad_ana phase 1'b0: Inphase 1'b1: Inverse phase */
#define AUD_BIT_ADC_LATCH_PHASE              ((u32)0x00000001 << 2)          /*!<R/W 0  Set latch adc data phase 1'b0: Inphase 1'b1: Inverse phase */
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
#define AUD_MASK_ADC_4_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 15)          /*!<R/W 0  Channel 4 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_4_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 15))
#define AUD_GET_ADC_4_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 15) & 0x00000003)))
#define AUD_MASK_ADC_5_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 17)          /*!<R/W 0  Channel 5 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_5_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 17))
#define AUD_GET_ADC_5_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 17) & 0x00000003)))
#define AUD_MASK_ADC_6_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 19)          /*!<R/W 0  Channel 6 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_6_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 19))
#define AUD_GET_ADC_6_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 19) & 0x00000003)))
#define AUD_MASK_ADC_7_DMIC_LPF_CLK_SEL      ((u32)0x00000003 << 21)          /*!<R/W 0  Channel 7 ADC path DMIC LPF clock 2'b00: 10M 2'b01: 5M 2'b10: 2.5M 2'b11: 769.2*2K */
#define AUD_ADC_7_DMIC_LPF_CLK_SEL(x)        ((u32)(((x) & 0x00000003) << 21))
#define AUD_GET_ADC_7_DMIC_LPF_CLK_SEL(x)    ((u32)(((x >> 21) & 0x00000003)))
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

/** @defgroup CODEC_asrc_control_4
 * @brief
 * @{
 **/
#define AUD_MASK_ASRC_RATE_SEL_RX_1          ((u32)0x00000003 << 0)          /*!<R/W 0  0: fs supports 4kHz ~ 60kHz 1: fs supports 60kHz ~ 120kHz 2/3: Reserved */
#define AUD_ASRC_RATE_SEL_RX_1(x)            ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_ASRC_RATE_SEL_RX_1(x)        ((u32)(((x >> 0) & 0x00000003)))
#define AUD_MASK_ASRC_GAIN_SEL_RX_1          ((u32)0x00000003 << 2)          /*!<R/W 0x3  ASRC convergence rate: larger is faster but more noisy */
#define AUD_ASRC_GAIN_SEL_RX_1(x)            ((u32)(((x) & 0x00000003) << 2))
#define AUD_GET_ASRC_GAIN_SEL_RX_1(x)        ((u32)(((x >> 2) & 0x00000003)))
#define AUD_BIT_ASRC_AUTO_ADJUST_RX_1        ((u32)0x00000001 << 4)          /*!<R/W 1  HW auto adjust convergence rate 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_asrc_control_5
 * @brief
 * @{
 **/
#define AUD_MASK_ASRC_SDM_INTI_RX_1          ((u32)0x00FFFFFF << 0)          /*!<R/W 0  Set initial value of tracked frequency */
#define AUD_ASRC_SDM_INTI_RX_1(x)            ((u32)(((x) & 0x00FFFFFF) << 0))
#define AUD_GET_ASRC_SDM_INTI_RX_1(x)        ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup CODEC_i2s_0_control
 * @brief
 * @{
 **/
#define AUD_BIT_I2S_0_RST_N_REG              ((u32)0x00000001 << 0)          /*!<R/W 0  I2S 0 1'b1: rst_n to audio digital IP is de-asserted 1'b0: rst_n to audio digital IP is asserted */
#define AUD_BIT_I2S_0_INV_SCLK               ((u32)0x00000001 << 1)          /*!<R/W 0  I2S 0 1'b1: I2S/PCM bit clock is inverted */
#define AUD_BIT_I2S_0_SELF_LPBK_EN           ((u32)0x00000001 << 2)          /*!<R/W 0  I2S 0 1'b1: internal loopback mode is enabled */
#define AUD_BIT_I2S_0_SAME_LRC_EN            ((u32)0x00000001 << 3)          /*!<R/W 0  I2S 0 1'b1: LRC_TX share to LRC_RX */
#define AUD_MASK_I2S_0_TDM_MODE_RX           ((u32)0x00000003 << 4)          /*!<R/W 0  I2S 0 Rx channel TDM mode 0: 2 channel 1: 4 channel 2: 6 channel 3: 8 channel */
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
#define AUD_MASK_I2S_0_DATA_CH0_SEL_RX       ((u32)0x00000007 << 0)          /*!<R/W 0  I2S 0 Rx channel data channel 0 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH0_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_I2S_0_DATA_CH0_SEL_RX(x)     ((u32)(((x >> 0) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH1_SEL_RX       ((u32)0x00000007 << 3)          /*!<R/W 1  I2S 0 Rx channel data channel 1 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH1_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 3))
#define AUD_GET_I2S_0_DATA_CH1_SEL_RX(x)     ((u32)(((x >> 3) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH2_SEL_RX       ((u32)0x00000007 << 6)          /*!<R/W 0  I2S 0 Rx channel data channel 2 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH2_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 6))
#define AUD_GET_I2S_0_DATA_CH2_SEL_RX(x)     ((u32)(((x >> 6) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH3_SEL_RX       ((u32)0x00000007 << 9)          /*!<R/W 0  I2S 0 Rx channel data channel 3 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH3_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 9))
#define AUD_GET_I2S_0_DATA_CH3_SEL_RX(x)     ((u32)(((x >> 9) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH4_SEL_RX       ((u32)0x00000007 << 12)          /*!<R/W 0  I2S 0 Rx channel data channel 4 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH4_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 12))
#define AUD_GET_I2S_0_DATA_CH4_SEL_RX(x)     ((u32)(((x >> 12) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH5_SEL_RX       ((u32)0x00000007 << 15)          /*!<R/W 1  I2S 0 Rx channel data channel 5 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH5_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 15))
#define AUD_GET_I2S_0_DATA_CH5_SEL_RX(x)     ((u32)(((x >> 15) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH6_SEL_RX       ((u32)0x00000007 << 18)          /*!<R/W 0  I2S 0 Rx channel data channel 6 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH6_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 18))
#define AUD_GET_I2S_0_DATA_CH6_SEL_RX(x)     ((u32)(((x >> 18) & 0x00000007)))
#define AUD_MASK_I2S_0_DATA_CH7_SEL_RX       ((u32)0x00000007 << 21)          /*!<R/W 0  I2S 0 Rx channel data channel 7 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_0_DATA_CH7_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 21))
#define AUD_GET_I2S_0_DATA_CH7_SEL_RX(x)     ((u32)(((x >> 21) & 0x00000007)))
#define AUD_BIT_I2S_0_DATA_CH0_RX_DISABLE    ((u32)0x00000001 << 24)          /*!<R/W 0  I2S 0 Rx channel data channel 0 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH1_RX_DISABLE    ((u32)0x00000001 << 25)          /*!<R/W 0  I2S 0 Rx channel data channel 1 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH2_RX_DISABLE    ((u32)0x00000001 << 26)          /*!<R/W 0  I2S 0 Rx channel data channel 2 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH3_RX_DISABLE    ((u32)0x00000001 << 27)          /*!<R/W 0  I2S 0 Rx channel data channel 3 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH4_RX_DISABLE    ((u32)0x00000001 << 28)          /*!<R/W 0  I2S 0 Rx channel data channel 4 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH5_RX_DISABLE    ((u32)0x00000001 << 29)          /*!<R/W 0  I2S 0 Rx channel data channel 5 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH6_RX_DISABLE    ((u32)0x00000001 << 30)          /*!<R/W 0  I2S 0 Rx channel data channel 6 1: disabe 0: enable */
#define AUD_BIT_I2S_0_DATA_CH7_RX_DISABLE    ((u32)0x00000001 << 31)          /*!<R/W 0  I2S 0 Rx channel data channel 7 1: disabe 0: enable */
/** @} */

/** @defgroup CODEC_i2s_1_control
 * @brief
 * @{
 **/
#define AUD_BIT_I2S_1_RST_N_REG              ((u32)0x00000001 << 0)          /*!<R/W 0  I2S 1 1'b1: rst_n to audio digital IP is de-asserted 1'b0: rst_n to audio digital IP is asserted */
#define AUD_BIT_I2S_1_INV_SCLK               ((u32)0x00000001 << 1)          /*!<R/W 0  I2S 1 1'b1: I2S/PCM bit clock is inverted */
#define AUD_BIT_I2S_1_SELF_LPBK_EN           ((u32)0x00000001 << 2)          /*!<R/W 0  I2S 1 1'b1: internal loopback mode is enabled */
#define AUD_BIT_I2S_1_SAME_LRC_EN            ((u32)0x00000001 << 3)          /*!<R/W 0  I2S 1 1'b1: LRC_TX share to LRC_RX */
#define AUD_MASK_I2S_1_TDM_MODE_RX           ((u32)0x00000003 << 4)          /*!<R/W 0  I2S 1 Rx channel TDM mode 0: 2 channel 1: 4 channel 2: 6 channel 3: 8 channel */
#define AUD_I2S_1_TDM_MODE_RX(x)             ((u32)(((x) & 0x00000003) << 4))
#define AUD_GET_I2S_1_TDM_MODE_RX(x)         ((u32)(((x >> 4) & 0x00000003)))
#define AUD_MASK_I2S_1_DATA_FORMAT_SEL_TX    ((u32)0x00000003 << 6)          /*!<R/W 0  I2S 1 Tx channel data format 2'b00: I2S 2'b01: left-justified 2'b10: PCM mode A 2'b11: PCM mode B */
#define AUD_I2S_1_DATA_FORMAT_SEL_TX(x)      ((u32)(((x) & 0x00000003) << 6))
#define AUD_GET_I2S_1_DATA_FORMAT_SEL_TX(x)  ((u32)(((x >> 6) & 0x00000003)))
#define AUD_MASK_I2S_1_DATA_FORMAT_SEL_RX    ((u32)0x00000003 << 8)          /*!<R/W 0  I2S 1 Rx channel data format 2'b00: I2S 2'b01: left-justified 2'b10: PCM mode A 2'b11: PCM mode B */
#define AUD_I2S_1_DATA_FORMAT_SEL_RX(x)      ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_I2S_1_DATA_FORMAT_SEL_RX(x)  ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_I2S_1_DATA_LEN_SEL_TX       ((u32)0x00000003 << 10)          /*!<R/W 0  I2S 1 Tx channel data length 2'b00: 16 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_1_DATA_LEN_SEL_TX(x)         ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_I2S_1_DATA_LEN_SEL_TX(x)     ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_I2S_1_DATA_LEN_SEL_RX       ((u32)0x00000003 << 12)          /*!<R/W 0  I2S 1 Rx channel data length 2'b00: 16 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_1_DATA_LEN_SEL_RX(x)         ((u32)(((x) & 0x00000003) << 12))
#define AUD_GET_I2S_1_DATA_LEN_SEL_RX(x)     ((u32)(((x >> 12) & 0x00000003)))
#define AUD_MASK_I2S_1_CH_LEN_SEL_TX         ((u32)0x00000003 << 14)          /*!<R/W 0  I2S 1 Tx channel channel length 2'b00: 16 bits 2'b01: 32 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_1_CH_LEN_SEL_TX(x)           ((u32)(((x) & 0x00000003) << 14))
#define AUD_GET_I2S_1_CH_LEN_SEL_TX(x)       ((u32)(((x >> 14) & 0x00000003)))
#define AUD_MASK_I2S_1_CH_LEN_SEL_RX         ((u32)0x00000003 << 16)          /*!<R/W 0  I2S 1 Rx channel chennel length 2'b00: 16 bits 2'b01: 32 bits 2'b10: 24 bits 2'b11: 8 bits */
#define AUD_I2S_1_CH_LEN_SEL_RX(x)           ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_I2S_1_CH_LEN_SEL_RX(x)       ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_I2S_1_DATA_CH_SEL_TX        ((u32)0x00000003 << 18)          /*!<R/W 0  I2S 1 Tx channel data channel 2'b00: L/R 2'b01: R/L 2'b10: L/L 2'b11: R/R */
#define AUD_I2S_1_DATA_CH_SEL_TX(x)          ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_I2S_1_DATA_CH_SEL_TX(x)      ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_I2S_1_MASTER_SEL             ((u32)0x00000001 << 20)          /*!<R/W 0  I2S 1 master source sel 1'b0: sport internal 1'b1: external I2S */
/** @} */

/** @defgroup CODEC_i2s_1_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_I2S_1_DATA_CH0_SEL_RX       ((u32)0x00000007 << 0)          /*!<R/W 0x2  I2S 1 Rx channel data channel 0 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH0_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_I2S_1_DATA_CH0_SEL_RX(x)     ((u32)(((x >> 0) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH1_SEL_RX       ((u32)0x00000007 << 3)          /*!<R/W 0x3  I2S 1 Rx channel data channel 1 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH1_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 3))
#define AUD_GET_I2S_1_DATA_CH1_SEL_RX(x)     ((u32)(((x >> 3) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH2_SEL_RX       ((u32)0x00000007 << 6)          /*!<R/W 0  I2S 1 Rx channel data channel 2 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH2_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 6))
#define AUD_GET_I2S_1_DATA_CH2_SEL_RX(x)     ((u32)(((x >> 6) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH3_SEL_RX       ((u32)0x00000007 << 9)          /*!<R/W 0  I2S 1 Rx channel data channel 3 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH3_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 9))
#define AUD_GET_I2S_1_DATA_CH3_SEL_RX(x)     ((u32)(((x >> 9) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH4_SEL_RX       ((u32)0x00000007 << 12)          /*!<R/W 0x2  I2S 1 Rx channel data channel 4 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH4_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 12))
#define AUD_GET_I2S_1_DATA_CH4_SEL_RX(x)     ((u32)(((x >> 12) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH5_SEL_RX       ((u32)0x00000007 << 15)          /*!<R/W 0x3  I2S 1 Rx channel data channel 5 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH5_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 15))
#define AUD_GET_I2S_1_DATA_CH5_SEL_RX(x)     ((u32)(((x >> 15) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH6_SEL_RX       ((u32)0x00000007 << 18)          /*!<R/W 0  I2S 1 Rx channel data channel 6 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH6_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 18))
#define AUD_GET_I2S_1_DATA_CH6_SEL_RX(x)     ((u32)(((x >> 18) & 0x00000007)))
#define AUD_MASK_I2S_1_DATA_CH7_SEL_RX       ((u32)0x00000007 << 21)          /*!<R/W 0  I2S 1 Rx channel data channel 7 0: CH0 1: CH1 2: CH2 3: CH3 4: CH4 5: CH5 6: CH6 7: CH7 */
#define AUD_I2S_1_DATA_CH7_SEL_RX(x)         ((u32)(((x) & 0x00000007) << 21))
#define AUD_GET_I2S_1_DATA_CH7_SEL_RX(x)     ((u32)(((x >> 21) & 0x00000007)))
#define AUD_BIT_I2S_1_DATA_CH0_RX_DISABLE    ((u32)0x00000001 << 24)          /*!<R/W 0  I2S 1 Rx channel data channel 0 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH1_RX_DISABLE    ((u32)0x00000001 << 25)          /*!<R/W 0  I2S 1 Rx channel data channel 1 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH2_RX_DISABLE    ((u32)0x00000001 << 26)          /*!<R/W 0  I2S 1 Rx channel data channel 2 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH3_RX_DISABLE    ((u32)0x00000001 << 27)          /*!<R/W 0  I2S 1 Rx channel data channel 3 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH4_RX_DISABLE    ((u32)0x00000001 << 28)          /*!<R/W 0  I2S 1 Rx channel data channel 4 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH5_RX_DISABLE    ((u32)0x00000001 << 29)          /*!<R/W 0  I2S 1 Rx channel data channel 5 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH6_RX_DISABLE    ((u32)0x00000001 << 30)          /*!<R/W 0  I2S 1 Rx channel data channel 6 1: disabe 0: enable */
#define AUD_BIT_I2S_1_DATA_CH7_RX_DISABLE    ((u32)0x00000001 << 31)          /*!<R/W 0  I2S 1 Rx channel data channel 7 1: disabe 0: enable */
/** @} */

/** @defgroup CODEC_i2s_ad_sel_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_0_I2S_SRC_SEL            ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 0 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_1_I2S_SRC_SEL            ((u32)0x00000001 << 1)          /*!<R/W 0  Channel 1 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_2_I2S_SRC_SEL            ((u32)0x00000001 << 2)          /*!<R/W 1  Channel 2 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_3_I2S_SRC_SEL            ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 3 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_4_I2S_SRC_SEL            ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 4 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_5_I2S_SRC_SEL            ((u32)0x00000001 << 5)          /*!<R/W 1  Channel 5 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_6_I2S_SRC_SEL            ((u32)0x00000001 << 6)          /*!<R/W 1  Channel 6 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_ADC_7_I2S_SRC_SEL            ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 7 ADC path I2S selection 1: I2S 1 0: I2S 0 */
#define AUD_BIT_DAC_I2S_SRC_SEL              ((u32)0x00000001 << 16)          /*!<R/W 0  DAC path I2S selection 1: I2S 1 0: I2S 0 */
/** @} */

/** @defgroup CODEC_adc_0_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 0 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_0_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_0_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_0_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 0 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 0 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 0 DMIC path SRC 1st LPF FC */
#define AUD_ADC_0_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_0_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_0_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 0 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_0_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 0 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_0_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_0_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_0_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 0 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 0 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 0 ADC path SRC 1st LPF FC */
#define AUD_ADC_0_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_0_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_0_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 0 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_0_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 0 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_0_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_0_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_0_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 0 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_0_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_0_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_0_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 0 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_0_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 0 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_0_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_0_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_0_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 0 ADC path high pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_0_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 0 ADC path high pass filter Fc (Fc: -3dB) 0: 5e10^-3 Fs 1: 2.5e10^-3 Fs 2: 1.125e10^-3 Fs 3: 6.25e10^-4 Fs 4: 3.125e10^-4 Fs 5: 1.5625e10^-4 Fs 6: 7.8125e10^-5 Fs 7: 3.90625e10^-5 Fs */
#define AUD_ADC_0_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_0_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
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
#define AUD_MASK_ADC_0_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_0_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_0_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_0_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 0 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_0_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_0_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_adc_1_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 1 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_1_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_1_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_1_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 1 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 1 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 1 DMIC path SRC 1st LPF FC */
#define AUD_ADC_1_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_1_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_1_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 1 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_1_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 1 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_1_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_1_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_1_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 1 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 1 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 1 ADC path SRC 1st LPF FC */
#define AUD_ADC_1_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_1_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_1_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 1 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_1_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 1 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_1_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_1_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_1_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 1 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_1_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_1_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_1_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 1 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_1_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 1 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_1_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_1_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_1_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 1 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_1_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 1 ADC path High pass filter FC */
#define AUD_ADC_1_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_1_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
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
/** @} */

/** @defgroup CODEC_adc_2_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 2 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_2_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_2_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_2_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 2 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 2 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 2 DMIC path SRC 1st LPF FC */
#define AUD_ADC_2_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_2_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_2_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 2 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_2_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 2 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_2_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_2_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_2_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 2 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 2 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 2 ADC path SRC 1st LPF FC */
#define AUD_ADC_2_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_2_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_2_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 2 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_2_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 2 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_2_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_2_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_2_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 2 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_2_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_2_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_2_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 2 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_2_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 2 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_2_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_2_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_2_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 2 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_2_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 2 ADC path High pass filter FC */
#define AUD_ADC_2_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_2_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
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
/** @} */

/** @defgroup CODEC_adc_3_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 3 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_3_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_3_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_3_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 3 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 3 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 3 DMIC path SRC 1st LPF FC */
#define AUD_ADC_3_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_3_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_3_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 3 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_3_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 3 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_3_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_3_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_3_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 3 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 3 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 3 ADC path SRC 1st LPF FC */
#define AUD_ADC_3_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_3_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_3_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 3 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_3_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 3 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_3_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_3_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_3_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 3 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_3_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_3_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_3_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 3 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_3_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 3 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_3_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_3_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_3_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 3 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_3_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 3 ADC path High pass filter FC */
#define AUD_ADC_3_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_3_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
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
/** @} */

/** @defgroup CODEC_adc_4_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 4 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_4_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_4_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_4_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 4 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_4_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 4 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_4_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 4 DMIC path SRC 1st LPF FC */
#define AUD_ADC_4_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_4_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_4_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 4 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_4_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 4 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_4_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_4_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_4_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 4 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_4_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 4 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_4_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 4 ADC path SRC 1st LPF FC */
#define AUD_ADC_4_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_4_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_4_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 4 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_4_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 4 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_4_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_4_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_4_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 4 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_4_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_4_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_4_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 4 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_4_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 4 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_4_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_4_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_4_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 4 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_4_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 4 ADC path High pass filter FC */
#define AUD_ADC_4_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_4_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_4_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 4 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_4_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_4_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_4_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 4 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_4_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_4_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_4_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_4_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_4_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_4_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 4 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_4_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_4_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_adc_5_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 5 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_5_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_5_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_5_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 5 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_5_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 5 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_5_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 5 DMIC path SRC 1st LPF FC */
#define AUD_ADC_5_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_5_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_5_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 5 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_5_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 5 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_5_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_5_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_5_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 5 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_5_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 5 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_5_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 5 ADC path SRC 1st LPF FC */
#define AUD_ADC_5_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_5_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_5_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 5 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_5_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 5 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_5_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_5_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_5_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 5 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_5_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_5_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_5_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 5 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_5_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 5 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_5_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_5_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_5_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 5 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_5_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 5 ADC path High pass filter FC */
#define AUD_ADC_5_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_5_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_5_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 5 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_5_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_5_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_5_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 5 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_5_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_5_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_5_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_5_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_5_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_5_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 5 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_5_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_5_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_adc_6_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_6_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 6 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_6_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_6_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_6_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 6 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_6_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 6 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_6_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 6 DMIC path SRC 1st LPF FC */
#define AUD_ADC_6_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_6_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_6_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 6 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_6_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 6 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_6_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_6_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_6_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 6 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_6_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 6 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_6_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 6 ADC path SRC 1st LPF FC */
#define AUD_ADC_6_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_6_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_6_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 6 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_6_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 6 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_6_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_6_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_6_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 6 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_6_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_6_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_6_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 6 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_6_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 6 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_6_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_6_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_6_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 6 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_6_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 6 ADC path High pass filter FC */
#define AUD_ADC_6_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_6_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_6_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_6_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 6 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_6_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_6_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_6_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 6 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_6_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_6_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_6_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_6_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_6_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_6_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 6 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_6_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_6_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_adc_7_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_7_DMIC_SRC_SEL          ((u32)0x00000007 << 0)          /*!<R/W 0  Channel 7 DMIC source selection 3'b000: MIC 1 rising 3'b001: MIC 1 falling 3'b010: MIC 2 rising 3'b011: MIC 2 falling 3'b100: MIC 3 rising 3'b101: MIC 3 falling 3'b110: MIC 4 rising 3'b111: MIC 4 falling */
#define AUD_ADC_7_DMIC_SRC_SEL(x)            ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_ADC_7_DMIC_SRC_SEL(x)        ((u32)(((x >> 0) & 0x00000007)))
#define AUD_BIT_ADC_7_DMIC_LPF2ND_EN         ((u32)0x00000001 << 3)          /*!<R/W 1  Channel 7 DMIC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_7_DMIC_LPF1ST_EN         ((u32)0x00000001 << 4)          /*!<R/W 1  Channel 7 DMIC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_7_DMIC_LPF1ST_FC_SEL    ((u32)0x00000003 << 5)          /*!<R/W 0  Channel 7 DMIC path SRC 1st LPF FC */
#define AUD_ADC_7_DMIC_LPF1ST_FC_SEL(x)      ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_ADC_7_DMIC_LPF1ST_FC_SEL(x)  ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_ADC_7_DMIC_MIX_MUTE          ((u32)0x00000001 << 7)          /*!<R/W 1  Channel 7 DMIC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_7_AD_SRC_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Channel 7 ANA ADC source selection 3'd0: ADC 1 3'd1: ADC 2 3'd2: ADC 3 3'd3: ADC 4 3'd4: ADC 5 */
#define AUD_ADC_7_AD_SRC_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_ADC_7_AD_SRC_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define AUD_BIT_ADC_7_AD_LPF2ND_EN           ((u32)0x00000001 << 11)          /*!<R/W 1  Channel 7 ADC path SRC 2nd LPF control 0: Disable 1: Enable */
#define AUD_BIT_ADC_7_AD_LPF1ST_EN           ((u32)0x00000001 << 12)          /*!<R/W 1  Channel 7 ADC path SRC 1st LPF control 0: Disable 1: Enable */
#define AUD_MASK_ADC_7_AD_LPF1ST_FC_SEL      ((u32)0x00000003 << 13)          /*!<R/W 0  Channel 7 ADC path SRC 1st LPF FC */
#define AUD_ADC_7_AD_LPF1ST_FC_SEL(x)        ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_ADC_7_AD_LPF1ST_FC_SEL(x)    ((u32)(((x >> 13) & 0x00000003)))
#define AUD_BIT_ADC_7_AD_MIX_MUTE            ((u32)0x00000001 << 15)          /*!<R/W 1  Channel 7 ADC input path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_7_AD_ZDET_FUNC          ((u32)0x00000003 << 16)          /*!<R/W 0x2  Channel 7 ADC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_ADC_7_AD_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ADC_7_AD_ZDET_FUNC(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ADC_7_AD_ZDET_TOUT          ((u32)0x00000003 << 18)          /*!<R/W 0  Channel 7 ADC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ADC_7_AD_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ADC_7_AD_ZDET_TOUT(x)        ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ADC_7_AD_MUTE                ((u32)0x00000001 << 20)          /*!<R/W 0  Channel 7 ADC path mute 0: Un-Mute 1: Mute */
#define AUD_MASK_ADC_7_DECI_SRC_SEL          ((u32)0x00000003 << 22)          /*!<R/W 0  Channel 7 ADC path decimation source select 0: ADC 1: ANC 2: MUSIC 3: ANC + MUSIC */
#define AUD_ADC_7_DECI_SRC_SEL(x)            ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ADC_7_DECI_SRC_SEL(x)        ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ADC_7_DCHPF_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  Channel 7 ADC path High pass filter enable control (filter DC) 0: Disable 1: Enable */
#define AUD_MASK_ADC_7_DCHPF_FC_SEL          ((u32)0x00000007 << 25)          /*!<R/W 0x5  Channel 7 ADC path High pass filter FC */
#define AUD_ADC_7_DCHPF_FC_SEL(x)            ((u32)(((x) & 0x00000007) << 25))
#define AUD_GET_ADC_7_DCHPF_FC_SEL(x)        ((u32)(((x >> 25) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_7_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_7_AD_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0x2f  Channel 7 ADC digital volume -17.625dB ~ 48dB in 0.375dB step 8'h00: -17.625dB ... 8'h2f: 0dB 8'h30: 0.375dB ... 8'haf: 48dB */
#define AUD_ADC_7_AD_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_ADC_7_AD_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_ADC_7_BOOST_GAIN            ((u32)0x00000003 << 8)          /*!<R/W 0  Channel 7 ADC path boost gain control 00: 0dB 01: 12dB 10: 24dB 11: 36dB */
#define AUD_ADC_7_BOOST_GAIN(x)              ((u32)(((x) & 0x00000003) << 8))
#define AUD_GET_ADC_7_BOOST_GAIN(x)          ((u32)(((x >> 8) & 0x00000003)))
#define AUD_MASK_ADC_7_RSVD                  ((u32)0x00000003 << 10)          /*!<R/W 0  Dummy */
#define AUD_ADC_7_RSVD(x)                    ((u32)(((x) & 0x00000003) << 10))
#define AUD_GET_ADC_7_RSVD(x)                ((u32)(((x >> 10) & 0x00000003)))
#define AUD_MASK_ADC_7_RPTR_HOLD             ((u32)0x0000000F << 12)          /*!<R/W 0  Channel 7 I2S read point hold number rptr_hold = (I2S_fs/ad_fs) - 1 */
#define AUD_ADC_7_RPTR_HOLD(x)               ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_ADC_7_RPTR_HOLD(x)           ((u32)(((x >> 12) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_dac_l_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_DA_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0xaf  Channel L DAC path dvol gain control (0.375dB/step) 8'hAF: 0dB 8'h00: -65.625dB */
#define AUD_DAC_L_DA_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_DAC_L_DA_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_DAC_L_DA_SRC_SEL            ((u32)0x00000003 << 16)          /*!<R/W 0  Channel L DAC path input select 0: I2S L channel 1: I2S R channel 2: I2S L+R channel 3: Test tone */
#define AUD_DAC_L_DA_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_L_DA_SRC_SEL(x)          ((u32)(((x >> 16) & 0x00000003)))
#define AUD_BIT_DAC_L_TEST_TONE_EN           ((u32)0x00000001 << 20)          /*!<R/W 0  Channel L DAC path test tone enable */
#define AUD_MASK_DAC_L_TEST_FC_SEL           ((u32)0x0000007F << 21)          /*!<R/W 0x0  Channel L DAC path test tone frquency: (fs/192)*(tone_fc_sel+1)Hz */
#define AUD_DAC_L_TEST_FC_SEL(x)             ((u32)(((x) & 0x0000007F) << 21))
#define AUD_GET_DAC_L_TEST_FC_SEL(x)         ((u32)(((x >> 21) & 0x0000007F)))
#define AUD_MASK_DAC_L_TEST_GAIN_SEL         ((u32)0x0000000F << 28)          /*!<R/W 0  Channel L DAC path test tone gain control 0 -6.02*(gain_sel)dB */
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
#define AUD_BIT_DAC_L_DMIX_MUTE_SIDETONE     ((u32)0x00000001 << 9)          /*!<R/W 0  Channel L DAC path 128fs-domain mixer sidetone path mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_DMIX_MUTE_DC           ((u32)0x00000001 << 10)          /*!<R/W 0  Channel L DAC path 128fs-domain mixer a DC from dac_l_tone_compensation_offset 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_MUSIC_MUTE_EN          ((u32)0x00000001 << 11)          /*!<R/W 0  Channel L DAC path music output mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_ANC_MUTE_EN            ((u32)0x00000001 << 12)          /*!<R/W 0  Channel L DAC path ANC output mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_L_PDM_EN                 ((u32)0x00000001 << 13)          /*!<R/W 1  Channel L DAC path PDM 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_SDM_EXTEND_FB_EN       ((u32)0x00000001 << 14)          /*!<R/W 1  Channel L DAC path feedback extend (for debug purpose; default need turn on) 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_SDM_EF_EN              ((u32)0x00000001 << 15)          /*!<R/W 1  Channel L DAC path 2nd order SDM enable 0: Disable 1: Enable */
#define AUD_MASK_DAC_L_SDM_DITHER_SEL        ((u32)0x00000003 << 16)          /*!<R/W 0  Channel L DAC path SDM dither select 2'b00: Disable 2'b01: LSB 2'b10: LSB+1 2'b11: LSB+2 */
#define AUD_DAC_L_SDM_DITHER_SEL(x)          ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_L_SDM_DITHER_SEL(x)      ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_DAC_L_DA_FLT_TYPE           ((u32)0x00000003 << 18)          /*!<R/W 0  Channel L DAC path interpolation filter type */
#define AUD_DAC_L_DA_FLT_TYPE(x)             ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_DAC_L_DA_FLT_TYPE(x)         ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_DAC_L_IDWA_BYPASS            ((u32)0x00000001 << 20)          /*!<R/W 0  Channel L DAC path IDWA BYPASS mode 0: Disable 1: Enable (debug purpose) */
#define AUD_BIT_DAC_L_OB_TONE_EN             ((u32)0x00000001 << 21)          /*!<R/W 0  Channel L DAC path Out Band tone enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_dac_l_control_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_DC_OFFSET             ((u32)0x0000FFFF << 0)          /*!<R/W 0  Channel L DAC path 128fs-domain mixe a DC compensation for spur issue 2's complemet */
#define AUD_DAC_L_DC_OFFSET(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define AUD_GET_DAC_L_DC_OFFSET(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
#define AUD_MASK_DAC_L_OB_FC_SEL             ((u32)0x0000000F << 16)          /*!<R/W 0  Channel L DAC path out Band tone frequency 100kHz * (dac_l_ob_fc_sel+1) */
#define AUD_DAC_L_OB_FC_SEL(x)               ((u32)(((x) & 0x0000000F) << 16))
#define AUD_GET_DAC_L_OB_FC_SEL(x)           ((u32)(((x >> 16) & 0x0000000F)))
#define AUD_MASK_DAC_L_OB_GAIN               ((u32)0x0000000F << 20)          /*!<R/W 0x7  Channel L DAC path out Band tone gain -20dB - (6 * dac_l_ob_gain ) */
#define AUD_DAC_L_OB_GAIN(x)                 ((u32)(((x) & 0x0000000F) << 20))
#define AUD_GET_DAC_L_OB_GAIN(x)             ((u32)(((x >> 20) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_dac_r_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_DA_GAIN               ((u32)0x000000FF << 0)          /*!<R/W 0xaf  Channel R DAC path dvol gain control (0.375dB/step) 8'hAF: 0dB 8'h00: -65.625dB */
#define AUD_DAC_R_DA_GAIN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define AUD_GET_DAC_R_DA_GAIN(x)             ((u32)(((x >> 0) & 0x000000FF)))
#define AUD_MASK_DAC_R_DA_SRC_SEL            ((u32)0x00000003 << 16)          /*!<R/W 0x1  Channel R DAC path input select 0: I2S L channel 1: I2S R channel 2: I2S L+R channel 3: Test tone */
#define AUD_DAC_R_DA_SRC_SEL(x)              ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_R_DA_SRC_SEL(x)          ((u32)(((x >> 16) & 0x00000003)))
#define AUD_BIT_DAC_R_TEST_TONE_EN           ((u32)0x00000001 << 20)          /*!<R/W 0  Channel R DAC path test tone enable */
#define AUD_MASK_DAC_R_TEST_FC_SEL           ((u32)0x0000007F << 21)          /*!<R/W 0x0  Channel R DAC path test tone frquency: (fs/192)*(tone_fc_sel+1)Hz */
#define AUD_DAC_R_TEST_FC_SEL(x)             ((u32)(((x) & 0x0000007F) << 21))
#define AUD_GET_DAC_R_TEST_FC_SEL(x)         ((u32)(((x >> 21) & 0x0000007F)))
#define AUD_MASK_DAC_R_TEST_GAIN_SEL         ((u32)0x0000000F << 28)          /*!<R/W 0  Channel R DAC path test tone gain control 0 -6.02*(gain_sel)dB */
#define AUD_DAC_R_TEST_GAIN_SEL(x)           ((u32)(((x) & 0x0000000F) << 28))
#define AUD_GET_DAC_R_TEST_GAIN_SEL(x)       ((u32)(((x >> 28) & 0x0000000F)))
/** @} */

/** @defgroup CODEC_dac_r_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_DA_ZDET_FUNC          ((u32)0x00000003 << 0)          /*!<R/W 0x2  Channel R DAC path zero detection function select 2'b00: immediate change 2'b01: zero detection & immediate change 2'b10: zdet & step 2'b11: zdet & timeout step */
#define AUD_DAC_R_DA_ZDET_FUNC(x)            ((u32)(((x) & 0x00000003) << 0))
#define AUD_GET_DAC_R_DA_ZDET_FUNC(x)        ((u32)(((x >> 0) & 0x00000003)))
#define AUD_MASK_DAC_R_DA_ZDET_TOUT          ((u32)0x00000003 << 2)          /*!<R/W 0  Channel R DAC path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_DAC_R_DA_ZDET_TOUT(x)            ((u32)(((x) & 0x00000003) << 2))
#define AUD_GET_DAC_R_DA_ZDET_TOUT(x)        ((u32)(((x >> 2) & 0x00000003)))
#define AUD_BIT_DAC_R_DA_MUTE                ((u32)0x00000001 << 4)          /*!<R/W 0  Channel R DAC path dvol mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_R_DAHPF_EN               ((u32)0x00000001 << 5)          /*!<R/W 0  Channel R DAC path Narrow-band 1st HPF enable control 0: Disable 1: Enable */
#define AUD_MASK_DAC_R_DA_DITHER_SEL         ((u32)0x00000003 << 6)          /*!<R/W 0  Channel R DAC path dither select 2'b00: Disable 2'b01: LSB 2'b10: LSB+1 2'b11: LSB+2 */
#define AUD_DAC_R_DA_DITHER_SEL(x)           ((u32)(((x) & 0x00000003) << 6))
#define AUD_GET_DAC_R_DA_DITHER_SEL(x)       ((u32)(((x >> 6) & 0x00000003)))
#define AUD_BIT_DAC_R_DMIX_MUTE_DA           ((u32)0x00000001 << 8)          /*!<R/W 0  Channel R DAC path 128fs-domain mixer da path mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_R_DMIX_MUTE_SIDETONE     ((u32)0x00000001 << 9)          /*!<R/W 0  Channel R DAC path 128fs-domain mixer sidetone path mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_R_DMIX_MUTE_DC           ((u32)0x00000001 << 10)          /*!<R/W 0  Channel R DAC path 128fs-domain mixer a DC from dac_r_tone_compensation_offset 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_R_MUSIC_MUTE_EN          ((u32)0x00000001 << 11)          /*!<R/W 0  Channel R DAC path music output mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_R_ANC_MUTE_EN            ((u32)0x00000001 << 12)          /*!<R/W 0  Channel R DAC path ANC output mute enable 1'b0: unmute 1'b1: mute */
#define AUD_BIT_DAC_R_PDM_EN                 ((u32)0x00000001 << 13)          /*!<R/W 1  Channel R DAC path PDM 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_SDM_EXTEND_FB_EN       ((u32)0x00000001 << 14)          /*!<R/W 1  Channel R DAC path feedback extend (for debug purpose; default need turn on) 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_SDM_EF_EN              ((u32)0x00000001 << 15)          /*!<R/W 1  Channel R DAC path 2nd order SDM enable 0: Disable 1: Enable */
#define AUD_MASK_DAC_R_SDM_DITHER_SEL        ((u32)0x00000003 << 16)          /*!<R/W 0  Channel R DAC path SDM dither select 2'b00: Disable 2'b01: LSB 2'b10: LSB+1 2'b11: LSB+2 */
#define AUD_DAC_R_SDM_DITHER_SEL(x)          ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_DAC_R_SDM_DITHER_SEL(x)      ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_DAC_R_DA_FLT_TYPE           ((u32)0x00000003 << 18)          /*!<R/W 0  Channel R DAC path interpolation filter type */
#define AUD_DAC_R_DA_FLT_TYPE(x)             ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_DAC_R_DA_FLT_TYPE(x)         ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_DAC_R_IDWA_BYPASS            ((u32)0x00000001 << 20)          /*!<R/W 0  Channel R DAC path IDWA BYPASS mode 0: Disable 1: Enable (debug purpose) */
#define AUD_BIT_DAC_R_OB_TONE_EN             ((u32)0x00000001 << 21)          /*!<R/W 0  Channel R DAC path Out Band tone enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_dac_r_control_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_DC_OFFSET             ((u32)0x0000FFFF << 0)          /*!<R/W 0  Channel R DAC path 128fs-domain mixe a DC compensation for spur issue 2's complemet */
#define AUD_DAC_R_DC_OFFSET(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define AUD_GET_DAC_R_DC_OFFSET(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
#define AUD_MASK_DAC_R_OB_FC_SEL             ((u32)0x0000000F << 16)          /*!<R/W 0  Channel R DAC path out Band tone frequency 100kHz * (dac_l_ob_fc_sel+1) */
#define AUD_DAC_R_OB_FC_SEL(x)               ((u32)(((x) & 0x0000000F) << 16))
#define AUD_GET_DAC_R_OB_FC_SEL(x)           ((u32)(((x >> 16) & 0x0000000F)))
#define AUD_MASK_DAC_R_OB_GAIN               ((u32)0x0000000F << 20)          /*!<R/W 0x7  Channel R DAC path out Band tone gain -20dB - (6 * dac_l_ob_gain ) */
#define AUD_DAC_R_OB_GAIN(x)                 ((u32)(((x) & 0x0000000F) << 20))
#define AUD_GET_DAC_R_OB_GAIN(x)             ((u32)(((x >> 20) & 0x0000000F)))
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

/** @defgroup CODEC_adc_4_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_4_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 4 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_4_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 4 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_4_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_4_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_4_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 4 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_4_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_4_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_5_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_5_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 5 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_5_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 5 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_5_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_5_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_5_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 5 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_5_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_5_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_6_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_6_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 6 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_6_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 6 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_6_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_6_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_6_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 6 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_6_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_6_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_adc_7_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_7_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel 7 ADC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_ADC_7_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel 7 ADC path silence detection threshold 3'b000: -54db 3'b001: -60db 3'b010: -66db 3'b011: -72db 3'b100: -78db 3'b101: -84db 3'b110: -90db 3'b111: -96db */
#define AUD_ADC_7_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_ADC_7_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_ADC_7_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel 7 ADC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_ADC_7_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_ADC_7_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
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

/** @defgroup CODEC_dac_r_silence_control
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_R_SILENCE_DET_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  Channel R DAC path silence detection enable 0: Disable 1: Enable */
#define AUD_MASK_DAC_R_SILENCE_LEVEL_SEL     ((u32)0x00000007 << 1)          /*!<R/W 1  Channel R DAC path silence detection threshold 3'b000: -78db 3'b001: -84db 3'b010: -90db 3'b011: -102db 3'b100: -108db 3'b101: -114db 3'b110: -126db 3'b111: -132db */
#define AUD_DAC_R_SILENCE_LEVEL_SEL(x)       ((u32)(((x) & 0x00000007) << 1))
#define AUD_GET_DAC_R_SILENCE_LEVEL_SEL(x)   ((u32)(((x >> 1) & 0x00000007)))
#define AUD_MASK_DAC_R_SILENCE_DEBOUNCE_SEL  ((u32)0x00000007 << 4)          /*!<R/W 0x3  Channel R DAC path silence detection debounce (48K) 3'b000: 80ms 3'b001: 160ms 3'b010: 320ms 3'b011: 640ms 3'b100: 1.28s 3'b101: 2.56s 3'b110: 5.12s 3'b111: 0.16ms */
#define AUD_DAC_R_SILENCE_DEBOUNCE_SEL(x)    ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_DAC_R_SILENCE_DEBOUNCE_SEL(x) ((u32)(((x >> 4) & 0x00000007)))
/** @} */

/** @defgroup CODEC_st_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_L_MIC_SRC_SEL            ((u32)0x0000000F << 0)          /*!<R/W 0  Sidetone L path MIC source selection 4'd0: adc_0_lpf 4'd1: adc_1_lpf 4'd2: adc_2_lpf 4'd3: adc_3_lpf 4'd4: adc_4_lpf 4'd5: adc_5_lpf 4'd8: dmic_0_lpf 4'd9: dmic_1_lpf 4'd10: dmic_2_lpf 4'd11: dmic_3_lpf 4'd12: dmic_4_lpf 4'd13: dmic_5_lpf */
#define AUD_ST_L_MIC_SRC_SEL(x)              ((u32)(((x) & 0x0000000F) << 0))
#define AUD_GET_ST_L_MIC_SRC_SEL(x)          ((u32)(((x >> 0) & 0x0000000F)))
#define AUD_BIT_ST_L_HPF_EN                  ((u32)0x00000001 << 4)          /*!<R/W 0  Sidetone L path HPF enable 0: Disable 1: Enable */
#define AUD_MASK_ST_L_HPF_FC_SEL             ((u32)0x00000007 << 5)          /*!<R/W 0  Sidetone L path HPF cut-off frequency select (-6dB) */
#define AUD_ST_L_HPF_FC_SEL(x)               ((u32)(((x) & 0x00000007) << 5))
#define AUD_GET_ST_L_HPF_FC_SEL(x)           ((u32)(((x >> 5) & 0x00000007)))
#define AUD_MASK_ST_L_VOL_SEL                ((u32)0x000000FF << 8)          /*!<R/W 0  Sidetone L path volume select (0.375db/step) 8'hff: 30dB ~ 7'h00: -65.625db */
#define AUD_ST_L_VOL_SEL(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define AUD_GET_ST_L_VOL_SEL(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define AUD_MASK_ST_L_ZDET_TOUT              ((u32)0x00000003 << 16)          /*!<R/W 0  Sidetone L path zero detection time out select 2'b00: 1024*16*128 samples 2'b01: 1024*32*128 samples 2'b10: 1024*64*128 samples 2'b11: 64*128 samples */
#define AUD_ST_L_ZDET_TOUT(x)                ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ST_L_ZDET_TOUT(x)            ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ST_L_ZDET_FUNC              ((u32)0x00000003 << 18)          /*!<R/W 0  Sidetone L path zero detection function select 2'b0: immediate change 2'b01: zero detection 2'b10: zdet step 2'b11: zdet & timeout with step change */
#define AUD_ST_L_ZDET_FUNC(x)                ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ST_L_ZDET_FUNC(x)            ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ST_L_BOOST_SEL               ((u32)0x00000001 << 20)          /*!<R/W 0  Sidetone L path boost gain select 0: 0dB 1: 12.04dB */
#define AUD_BIT_ST_CONTROL_0_RSVD_1          ((u32)0x00000001 << 21)          /*!<R/W 0  Reserved */
#define AUD_MASK_ST_L_IN_SEL                 ((u32)0x00000003 << 22)          /*!<R/W 0  DAC L path sidetone source select 2'b00: sidetone_l 2'b01: sidetone_r 2'b10: (sidetone_l+sidetone_r )/2 */
#define AUD_ST_L_IN_SEL(x)                   ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ST_L_IN_SEL(x)               ((u32)(((x >> 22) & 0x00000003)))
/** @} */

/** @defgroup CODEC_st_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_R_MIC_SRC_SEL            ((u32)0x0000000F << 0)          /*!<R/W 0  Sidetone R path MIC source seRection 4'd0: adc_0_Rpf 4'd1: adc_1_Rpf 4'd2: adc_2_Rpf 4'd3: adc_3_Rpf 4'd4: adc_4_Rpf 4'd5: adc_5_Rpf 4'd8: dmic_0_Rpf 4'd9: dmic_1_Rpf 4'd10: dmic_2_Rpf 4'd11: dmic_3_Rpf 4'd12: dmic_4_Rpf 4'd13: dmic_5_Rpf */
#define AUD_ST_R_MIC_SRC_SEL(x)              ((u32)(((x) & 0x0000000F) << 0))
#define AUD_GET_ST_R_MIC_SRC_SEL(x)          ((u32)(((x >> 0) & 0x0000000F)))
#define AUD_BIT_ST_R_HPF_EN                  ((u32)0x00000001 << 4)          /*!<R/W 0  Sidetone R path HPF enabRe 0: disabRe 1: enabRe */
#define AUD_MASK_ST_R_HPF_FC_SEL             ((u32)0x00000007 << 5)          /*!<R/W 0  Sidetone R path HPF cut-off frequency seRect (-6dB) */
#define AUD_ST_R_HPF_FC_SEL(x)               ((u32)(((x) & 0x00000007) << 5))
#define AUD_GET_ST_R_HPF_FC_SEL(x)           ((u32)(((x >> 5) & 0x00000007)))
#define AUD_MASK_ST_R_VOL_SEL                ((u32)0x000000FF << 8)          /*!<R/W 0  Sidetone R path voRume seRect (0.375db/step) 8'hff: 30dB ~ 7'h00: -65.625db */
#define AUD_ST_R_VOL_SEL(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define AUD_GET_ST_R_VOL_SEL(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define AUD_MASK_ST_R_ZDET_TOUT              ((u32)0x00000003 << 16)          /*!<R/W 0  Sidetone R path zero detection time out seRect 2'b00: 1024*16*128 sampRes 2'b01: 1024*32*128 sampRes 2'b10: 1024*64*128 sampRes 2'b11: 64*128 sampRes */
#define AUD_ST_R_ZDET_TOUT(x)                ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_ST_R_ZDET_TOUT(x)            ((u32)(((x >> 16) & 0x00000003)))
#define AUD_MASK_ST_R_ZDET_FUNC              ((u32)0x00000003 << 18)          /*!<R/W 0  Sidetone R path zero detection function seRect 2'b0: immediate change 2'b01: zero detection 2'b10: zdet step 2'b11: zdet & timeout with step change */
#define AUD_ST_R_ZDET_FUNC(x)                ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ST_R_ZDET_FUNC(x)            ((u32)(((x >> 18) & 0x00000003)))
#define AUD_BIT_ST_R_BOOST_SEL               ((u32)0x00000001 << 20)          /*!<R/W 0  Sidetone R path boost gain seRect 0: 0dB 1: 12.04dB */
#define AUD_BIT_ST_CONTROL_0_RSVD_2          ((u32)0x00000001 << 21)          /*!<R/W 0  Reserved */
#define AUD_MASK_ST_R_IN_SEL                 ((u32)0x00000003 << 22)          /*!<R/W 0  DAC R path sidetone source seRect 2'b00: sidetone_l 2'b01: sidetone_r 2'b10: (sidetone_l+sidetone_r )/2 */
#define AUD_ST_R_IN_SEL(x)                   ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ST_R_IN_SEL(x)               ((u32)(((x >> 22) & 0x00000003)))
/** @} */

/** @defgroup CODEC_st_ds_control_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_SRC_SEL             ((u32)0x0000000F << 0)          /*!<R/W 0  Sidetone L downsample path source selection 4'd0: adc_0_eq_out 4'd1: adc_1_eq_out 4'd2: adc_2_eq_out 4'd3: adc_3_eq_out 4'd4: adc_4_eq_out 4'd5: adc_5_eq_out 4'd8: adc_0_dvol_out 3'd9: adc_1_dvol_out 4'd10: adc_2_dvol_out 3'd11: adc_3_dvol_out 4'd12: adc_4_dvol_out 3'd13: adc_5_dvol_out */
#define AUD_ST_DS_L_SRC_SEL(x)               ((u32)(((x) & 0x0000000F) << 0))
#define AUD_GET_ST_DS_L_SRC_SEL(x)           ((u32)(((x >> 0) & 0x0000000F)))
#define AUD_MASK_ST_DS_CONTROL_0_RSVD_0      ((u32)0x0000000F << 4)          /*!<R/W 0  Reserved */
#define AUD_ST_DS_CONTROL_0_RSVD_0(x)        ((u32)(((x) & 0x0000000F) << 4))
#define AUD_GET_ST_DS_CONTROL_0_RSVD_0(x)    ((u32)(((x >> 4) & 0x0000000F)))
#define AUD_MASK_ST_DS_L_VOL_SEL             ((u32)0x000000FF << 8)          /*!<R/W 0  Sidetone L downsample path volume select(0.375db/step) 7'hbf: 6dB ~ 7'h00: -65.625db */
#define AUD_ST_DS_L_VOL_SEL(x)               ((u32)(((x) & 0x000000FF) << 8))
#define AUD_GET_ST_DS_L_VOL_SEL(x)           ((u32)(((x >> 8) & 0x000000FF)))
#define AUD_BIT_ST_DS_L_BOOST_SEL            ((u32)0x00000001 << 16)          /*!<R/W 0  Sidetone L downsample path boost gain select 0: 0dB 1: 12.04dB */
#define AUD_BIT_ST_DS_L_MUTE_ZDET_EN         ((u32)0x00000001 << 17)          /*!<R/W 0  Sidetone L downsample path zero detection function for MUTE 1'b0: immediate change 1'b1: zero detection */
#define AUD_MASK_ST_DS_L_ZDET_TOUT           ((u32)0x00000003 << 18)          /*!<R/W 0  Sidetone L downsample path zero detection time out select 2'b00: 1024*16 samples 2'b01: 1024*32 samples 2'b10: 1024*64 samples 2'b11: 64 samples */
#define AUD_ST_DS_L_ZDET_TOUT(x)             ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ST_DS_L_ZDET_TOUT(x)         ((u32)(((x >> 18) & 0x00000003)))
#define AUD_MASK_ST_DS_L_ZDET_FUNC           ((u32)0x00000003 << 20)          /*!<R/W 0  Sidetone L downsample path zero detection function for DVOL 1'b0: immediate change 1'b1: zero detection */
#define AUD_ST_DS_L_ZDET_FUNC(x)             ((u32)(((x) & 0x00000003) << 20))
#define AUD_GET_ST_DS_L_ZDET_FUNC(x)         ((u32)(((x >> 20) & 0x00000003)))
#define AUD_MASK_ST_DS_L_IN_SEL              ((u32)0x00000003 << 22)          /*!<R/W 0  DAC L path downsample sidetone source select 2'b00: sidetone_ds_l 2'b01: sidetone_ds_r 2'b10: (sidetone_ds_l+sidetone_ds_r )/2 */
#define AUD_ST_DS_L_IN_SEL(x)                ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ST_DS_L_IN_SEL(x)            ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ST_DS_L_0_MUTE_EN            ((u32)0x00000001 << 24)          /*!<R/W 1  DAC L path downsample sidetone Mute enable (before EQ) 1'b0: unmute 1'b1: mute */
#define AUD_BIT_ST_DS_L_1_MUTE_EN            ((u32)0x00000001 << 25)          /*!<R/W 1  DAC L path downsample sidetone Mute enable (during EQ) 1'b0: unmute 1'b1: mute */
/** @} */

/** @defgroup CODEC_st_ds_control_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_SRC_SEL             ((u32)0x0000000F << 0)          /*!<R/W 0  Sidetone R downsampRe path source seRection 4'd0: adc_0_eq_out 4'd1: adc_1_eq_out 4'd2: adc_2_eq_out 4'd3: adc_3_eq_out 4'd4: adc_4_eq_out 4'd5: adc_5_eq_out 4'd8: adc_0_dvoR_out 3'd9: adc_1_dvoR_out 4'd10: adc_2_dvoR_out 3'd11: adc_3_dvoR_out 4'd12: adc_4_dvoR_out 3'd13: adc_5_dvoR_out */
#define AUD_ST_DS_R_SRC_SEL(x)               ((u32)(((x) & 0x0000000F) << 0))
#define AUD_GET_ST_DS_R_SRC_SEL(x)           ((u32)(((x >> 0) & 0x0000000F)))
#define AUD_MASK_ST_DS_CONTROL_0_RSVD_1      ((u32)0x0000000F << 4)          /*!<R/W 0  Reserved */
#define AUD_ST_DS_CONTROL_0_RSVD_1(x)        ((u32)(((x) & 0x0000000F) << 4))
#define AUD_GET_ST_DS_CONTROL_0_RSVD_1(x)    ((u32)(((x >> 4) & 0x0000000F)))
#define AUD_MASK_ST_DS_R_VOL_SEL             ((u32)0x000000FF << 8)          /*!<R/W 0  Sidetone R downsampRe path voRume seRect(0.375db/step) 7'hbf: 6dB ~ 7'h00: -65.625db */
#define AUD_ST_DS_R_VOL_SEL(x)               ((u32)(((x) & 0x000000FF) << 8))
#define AUD_GET_ST_DS_R_VOL_SEL(x)           ((u32)(((x >> 8) & 0x000000FF)))
#define AUD_BIT_ST_DS_R_BOOST_SEL            ((u32)0x00000001 << 16)          /*!<R/W 0  Sidetone R downsampRe path boost gain seRect 0: 0dB 1: 12.04dB */
#define AUD_BIT_ST_DS_R_MUTE_ZDET_EN         ((u32)0x00000001 << 17)          /*!<R/W 0  Sidetone R downsampRe path zero detection function for MUTE 1'b0: immediate change 1'b1: zero detection */
#define AUD_MASK_ST_DS_R_ZDET_TOUT           ((u32)0x00000003 << 18)          /*!<R/W 0  Sidetone R downsampRe path zero detection time out seRect 2'b00: 1024*16 sampRes 2'b01: 1024*32 sampRes 2'b10: 1024*64 sampRes 2'b11: 64 sampRes */
#define AUD_ST_DS_R_ZDET_TOUT(x)             ((u32)(((x) & 0x00000003) << 18))
#define AUD_GET_ST_DS_R_ZDET_TOUT(x)         ((u32)(((x >> 18) & 0x00000003)))
#define AUD_MASK_ST_DS_R_ZDET_FUNC           ((u32)0x00000003 << 20)          /*!<R/W 0  Sidetone R downsampRe path zero detection function for DVOR 1'b0: immediate change 1'b1: zero detection */
#define AUD_ST_DS_R_ZDET_FUNC(x)             ((u32)(((x) & 0x00000003) << 20))
#define AUD_GET_ST_DS_R_ZDET_FUNC(x)         ((u32)(((x >> 20) & 0x00000003)))
#define AUD_MASK_ST_DS_R_IN_SEL              ((u32)0x00000003 << 22)          /*!<R/W 0  DAC R path downsampRe sidetone source seRect 2'b00: sidetone_ds_l 2'b01: sidetone_ds_r 2'b10: (sidetone_ds_l+sidetone_ds_r )/2 */
#define AUD_ST_DS_R_IN_SEL(x)                ((u32)(((x) & 0x00000003) << 22))
#define AUD_GET_ST_DS_R_IN_SEL(x)            ((u32)(((x >> 22) & 0x00000003)))
#define AUD_BIT_ST_DS_R_0_MUTE_EN            ((u32)0x00000001 << 24)          /*!<R/W 1  DAC R path downsampRe sidetone Mute enabRe (before EQ) 1'b0: unmute 1'b1: mute */
#define AUD_BIT_ST_DS_R_1_MUTE_EN            ((u32)0x00000001 << 25)          /*!<R/W 1  DAC R path downsampRe sidetone Mute enabRe (during EQ) 1'b0: unmute 1'b1: mute */
/** @} */

/** @defgroup CODEC_st_ds_l_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ST_DS_L_BIQUAD_EN_0          ((u32)0x00000001 << 0)          /*!<R/W 0  Sidetone L channel EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ST_DS_L_BIQUAD_EN_1          ((u32)0x00000001 << 1)          /*!<R/W 0  Sidetone L channel EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ST_DS_L_BIQUAD_EN_2          ((u32)0x00000001 << 2)          /*!<R/W 0  Sidetone L channel EQ 2-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_H0_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  Sidetone downsample Lch EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_H0_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_H0_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_B1_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_B1_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_B1_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_B2_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_B2_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_B2_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_A1_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_A1_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_A1_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_A2_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_A2_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_A2_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_H0_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  Sidetone downsample Lch EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_H0_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_H0_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_B1_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_B1_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_B1_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_B2_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_B2_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_B2_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_A1_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_A1_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_A1_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_A2_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_A2_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_A2_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_H0_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  Sidetone downsample Lch EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_H0_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_H0_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_B1_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_B1_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_B1_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_B2_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_B2_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_B2_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_A1_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_A1_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_A1_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_l_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_L_BIQUAD_A2_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Lch EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_L_BIQUAD_A2_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_L_BIQUAD_A2_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ST_DS_R_BIQUAD_EN_0          ((u32)0x00000001 << 0)          /*!<R/W 0  Sidetone R channeR EQ 0-band biquad enabRe 0: disabRe 1: enabRe */
#define AUD_BIT_ST_DS_R_BIQUAD_EN_1          ((u32)0x00000001 << 1)          /*!<R/W 0  Sidetone R channeR EQ 1-band biquad enabRe 0: disabRe 1: enabRe */
#define AUD_BIT_ST_DS_R_BIQUAD_EN_2          ((u32)0x00000001 << 2)          /*!<R/W 0  Sidetone R channeR EQ 2-band biquad enabRe 0: disabRe 1: enabRe */
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_H0_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  Sidetone downsample Rch EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_H0_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_H0_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_B1_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_B1_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_B1_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_B2_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_B2_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_B2_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_A1_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_A1_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_A1_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_A2_0         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_A2_0(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_A2_0(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_H0_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  Sidetone downsample Rch EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_H0_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_H0_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_B1_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_B1_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_B1_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_B2_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_B2_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_B2_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_A1_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_A1_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_A1_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_A2_1         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_A2_1(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_A2_1(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_H0_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  Sidetone downsample Rch EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_H0_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_H0_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_B1_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_B1_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_B1_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_B2_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_B2_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_B2_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_A1_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_A1_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_A1_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_st_ds_r_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ST_DS_R_BIQUAD_A2_2         ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  Sidetone downsample Rch EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ST_DS_R_BIQUAD_A2_2(x)           ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ST_DS_R_BIQUAD_A2_2(x)       ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_L_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  DAC Lch EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  DAC Lch EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  DAC Lch EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  DAC Lch EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  DAC Lch EQ 4-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_5            ((u32)0x00000001 << 5)          /*!<R/W 0  DAC Lch EQ 5-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_6            ((u32)0x00000001 << 6)          /*!<R/W 0  DAC Lch EQ 6-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_7            ((u32)0x00000001 << 7)          /*!<R/W 0  DAC Lch EQ 7-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_8            ((u32)0x00000001 << 8)          /*!<R/W 0  DAC Lch EQ 8-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_L_BIQUAD_EN_9            ((u32)0x00000001 << 9)          /*!<R/W 0  DAC Lch EQ 9-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 5-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 5-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 5-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 5-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 5-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 6-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 6-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 6-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 6-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 6-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 7-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 7-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 7-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 7-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 7-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 8-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 8-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 8-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 8-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 8-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_h0_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_H0_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Lch EQ 9-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_H0_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_H0_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b1_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B1_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 9-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B1_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B1_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_b2_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_B2_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 9-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_B2_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_B2_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a1_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A1_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 9-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A1_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A1_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_l_biquad_a2_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_L_BIQUAD_A2_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Lch EQ 9-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_L_BIQUAD_A2_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_L_BIQUAD_A2_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_R_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  DAC Rch EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  DAC Rch EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  DAC Rch EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  DAC Rch EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  DAC Rch EQ 4-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_5            ((u32)0x00000001 << 5)          /*!<R/W 0  DAC Rch EQ 5-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_6            ((u32)0x00000001 << 6)          /*!<R/W 0  DAC Rch EQ 6-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_7            ((u32)0x00000001 << 7)          /*!<R/W 0  DAC Rch EQ 7-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_8            ((u32)0x00000001 << 8)          /*!<R/W 0  DAC Rch EQ 8-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_DAC_R_BIQUAD_EN_9            ((u32)0x00000001 << 9)          /*!<R/W 0  DAC Rch EQ 9-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 5-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 5-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 5-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 5-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_5
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_5           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 5-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_5(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_5(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 6-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 6-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 6-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 6-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_6
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_6           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 6-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_6(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_6(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 7-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 7-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 7-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 7-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_7
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_7           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 7-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_7(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_7(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 8-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 8-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 8-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 8-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_8
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_8           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 8-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_8(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_8(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_h0_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_H0_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  DAC Rch EQ 9-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_H0_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_H0_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b1_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B1_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 9-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B1_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B1_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_b2_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_B2_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 9-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_B2_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_B2_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a1_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A1_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 9-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A1_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A1_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_dac_r_biquad_a2_9
 * @brief
 * @{
 **/
#define AUD_MASK_DAC_R_BIQUAD_A2_9           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  DAC Rch EQ 9-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_DAC_R_BIQUAD_A2_9(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_DAC_R_BIQUAD_A2_9(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_0_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  ADC channel 0 EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  ADC channel 0 EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  ADC channel 0 EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  ADC channel 0 EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_0_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  ADC channel 0 EQ 4-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_adc_0_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 0 EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 0 EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 0 EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 0 EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 0 EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_0_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_0_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 0 EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_0_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_0_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_1_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  ADC channel 1 EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  ADC channel 1 EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  ADC channel 1 EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  ADC channel 1 EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_1_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  ADC channel 1 EQ 4-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_adc_1_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 1 EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 1 EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 1 EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 1 EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 1 EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_1_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_1_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 1 EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_1_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_1_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_2_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  ADC channel 2 EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  ADC channel 2 EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  ADC channel 2 EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  ADC channel 2 EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_2_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  ADC channel 2 EQ 4-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_adc_2_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 2 EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 2 EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 2 EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 2 EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 2 EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_2_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_2_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 2 EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_2_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_2_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_3_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  ADC channel 3 EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  ADC channel 3 EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  ADC channel 3 EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  ADC channel 3 EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_3_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  ADC channel 3 EQ 4-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_adc_3_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 3 EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 3 EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 3 EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 3 EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 3 EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_3_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_3_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 3 EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_3_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_3_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_4_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  ADC channel 4 EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_4_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  ADC channel 4 EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_4_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  ADC channel 4 EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_4_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  ADC channel 4 EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_4_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  ADC channel 4 EQ 4-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_adc_4_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 4 EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 4 EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 4 EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 4 EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 4 EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_4_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 4 EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_4_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_4_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_eq_ctrl
 * @brief
 * @{
 **/
#define AUD_BIT_ADC_5_BIQUAD_EN_0            ((u32)0x00000001 << 0)          /*!<R/W 0  ADC channel 5 EQ 0-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_5_BIQUAD_EN_1            ((u32)0x00000001 << 1)          /*!<R/W 0  ADC channel 5 EQ 1-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_5_BIQUAD_EN_2            ((u32)0x00000001 << 2)          /*!<R/W 0  ADC channel 5 EQ 2-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_5_BIQUAD_EN_3            ((u32)0x00000001 << 3)          /*!<R/W 0  ADC channel 5 EQ 3-band biquad enable 0: Disable 1: Enable */
#define AUD_BIT_ADC_5_BIQUAD_EN_4            ((u32)0x00000001 << 4)          /*!<R/W 0  ADC channel 5 EQ 4-band biquad enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CODEC_adc_5_biquad_h0_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_H0_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 5 EQ 0-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_H0_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_H0_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 0-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 0-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a1_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A1_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 0-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A1_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A1_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a2_0
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A2_0           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 0-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A2_0(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A2_0(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_h0_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_H0_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 5 EQ 1-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_H0_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_H0_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 1-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 1-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a1_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A1_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 1-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A1_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A1_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a2_1
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A2_1           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 1-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A2_1(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A2_1(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_h0_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_H0_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 5 EQ 2-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_H0_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_H0_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 2-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 2-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a1_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A1_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 2-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A1_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A1_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a2_2
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A2_2           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 2-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A2_2(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A2_2(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_h0_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_H0_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 5 EQ 3-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_H0_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_H0_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 3-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 3-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a1_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A1_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 3-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A1_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A1_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a2_3
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A2_3           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 3-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A2_3(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A2_3(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_h0_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_H0_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x2000000  ADC channel 5 EQ 4-band coef. h0 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_H0_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_H0_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 4-band coef. b1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_b2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_B2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 4-band coef. b2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_B2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_B2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a1_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A1_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 4-band coef. a1 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A1_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A1_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup CODEC_adc_5_biquad_a2_4
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_BIQUAD_A2_4           ((u32)0x1FFFFFFF << 0)          /*!<R/W 0  ADC channel 5 EQ 4-band coef. a2 2's complement in 4.25 format, i. e. the range is from -8~7.99. */
#define AUD_ADC_5_BIQUAD_A2_4(x)             ((u32)(((x) & 0x1FFFFFFF) << 0))
#define AUD_GET_ADC_5_BIQUAD_A2_4(x)         ((u32)(((x >> 0) & 0x1FFFFFFF)))
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

/** @defgroup CODEC_adc_4_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_4_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 4 ADC LPF out valus */
#define AUD_ADC_4_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_4_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_adc_5_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_5_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 5 ADC LPF out valus */
#define AUD_ADC_5_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_5_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_adc_6_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_6_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 6 ADC LPF out valus */
#define AUD_ADC_6_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_6_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_adc_7_lpf_rd
 * @brief
 * @{
 **/
#define AUD_MASK_ADC_7_LPF_RD                ((u32)0x0007FFFF << 0)          /*!<R   CH 7 ADC LPF out valus */
#define AUD_ADC_7_LPF_RD(x)                  ((u32)(((x) & 0x0007FFFF) << 0))
#define AUD_GET_ADC_7_LPF_RD(x)              ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup CODEC_silence_inform
 * @brief
 * @{
 **/
#define AUD_BIT_DAC_L_SILENCE_DET_O          ((u32)0x00000001 << 0)          /*!<R   dac_l_silence data status (result of silence detection) 1'b0: not dac_l_silence data 1'b1: dac_l_silence is detected */
#define AUD_BIT_DAC_L_SILENCE_DET_STATUS     ((u32)0x00000001 << 1)          /*!<R   Ongoing status of dac_l_silence detection 1'b0: dac_l_silence detection is resting (clock is gating) 1'b1: dac_l_silence detection is working */
#define AUD_BIT_DAC_R_SILENCE_DET_O          ((u32)0x00000001 << 2)          /*!<R   dac_r_silence data status (result of silence detection) 1'b0: not dac_r_silence data 1'b1: dac_r_silence is detected */
#define AUD_BIT_DAC_R_SILENCE_DET_STATUS     ((u32)0x00000001 << 3)          /*!<R   Ongoing status of dac_r_silence detection 1'b0: dac_r_silence detection is resting (clock is gating) 1'b1: dac_r_silence detection is working */
#define AUD_BIT_ADC_0_SILENCE_DET_O          ((u32)0x00000001 << 4)          /*!<R   adc_0_silencedata status (result of silence detection) 1'b0: not adc_0_silencedata 1'b1: adc_0_silenceis detected */
#define AUD_BIT_ADC_0_SILENCE_DET_STATUS     ((u32)0x00000001 << 5)          /*!<R   Ongoing status of adc_0_silencedetection 1'b0: adc_0_silencedetection is resting (clock is gating) 1'b1: adc_0_silencedetection is working */
#define AUD_BIT_ADC_1_SILENCE_DET_O          ((u32)0x00000001 << 6)          /*!<R   adc_1_silencedata status (result of silence detection) 1'b0: not adc_1_silencedata 1'b1: adc_1_silenceis detected */
#define AUD_BIT_ADC_1_SILENCE_DET_STATUS     ((u32)0x00000001 << 7)          /*!<R   Ongoing status of adc_1_silencedetection 1'b0: adc_1_silencedetection is resting (clock is gating) 1'b1: adc_1_silencedetection is working */
#define AUD_BIT_ADC_2_SILENCE_DET_O          ((u32)0x00000001 << 8)          /*!<R   adc_2_silencedata status (result of silence detection) 1'b0: not adc_2_silencedata 1'b1: adc_2_silenceis detected */
#define AUD_BIT_ADC_2_SILENCE_DET_STATUS     ((u32)0x00000001 << 9)          /*!<R   Ongoing status of adc_2_silencedetection 1'b0: adc_2_silencedetection is resting (clock is gating) 1'b1: adc_2_silencedetection is working */
#define AUD_BIT_ADC_3_SILENCE_DET_O          ((u32)0x00000001 << 10)          /*!<R   adc_3_silencedata status (result of silence detection) 1'b0: not adc_3_silencedata 1'b1: adc_3_silenceis detected */
#define AUD_BIT_ADC_3_SILENCE_DET_STATUS     ((u32)0x00000001 << 11)          /*!<R   Ongoing status of adc_3_silencedetection 1'b0: adc_3_silencedetection is resting (clock is gating) 1'b1: adc_3_silencedetection is working */
#define AUD_BIT_ADC_4_SILENCE_DET_O          ((u32)0x00000001 << 12)          /*!<R   adc_4_silencedata status (result of silence detection) 1'b0: not adc_4_silencedata 1'b1: adc_4_silenceis detected */
#define AUD_BIT_ADC_4_SILENCE_DET_STATUS     ((u32)0x00000001 << 13)          /*!<R   Ongoing status of adc_4_silencedetection 1'b0: adc_4_silencedetection is resting (clock is gating) 1'b1: adc_4_silencedetection is working */
#define AUD_BIT_ADC_5_SILENCE_DET_O          ((u32)0x00000001 << 14)          /*!<R   adc_5_silencedata status (result of silence detection) 1'b0: not adc_5_silencedata 1'b1: adc_5_silenceis detected */
#define AUD_BIT_ADC_5_SILENCE_DET_STATUS     ((u32)0x00000001 << 15)          /*!<R   Ongoing status of adc_5_silencedetection 1'b0: adc_5_silencedetection is resting (clock is gating) 1'b1: adc_5_silencedetection is working */
#define AUD_BIT_ADC_6_SILENCE_DET_O          ((u32)0x00000001 << 16)          /*!<R   adc_6_silencedata status (result of silence detection) 1'b0: not adc_6_silencedata 1'b1: adc_6_silenceis detected */
#define AUD_BIT_ADC_6_SILENCE_DET_STATUS     ((u32)0x00000001 << 17)          /*!<R   Ongoing status of adc_6_silencedetection 1'b0: adc_6_silencedetection is resting (clock is gating) 1'b1: adc_6_silencedetection is working */
#define AUD_BIT_ADC_7_SILENCE_DET_O          ((u32)0x00000001 << 18)          /*!<R   adc_7_silencedata status (result of silence detection) 1'b0: not adc_7_silencedata 1'b1: adc_7_silenceis detected */
#define AUD_BIT_ADC_7_SILENCE_DET_STATUS     ((u32)0x00000001 << 19)          /*!<R   Ongoing status of adc_7_silence detection 1'b0: adc_7_silencedetection is resting (clock is gating) 1'b1: adc_7_silencedetection is working */
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
 * @brief AUDIO_CODEC I2S Init Structure Definition
 */
typedef struct {

	u32 CODEC_SelI2STxSR;				/*!< Specifies the AUDIO CODEC TX Sample rate
										This parameter can be a value of @ref AUDIO_CODEC_Sample_Rate */

	u32 CODEC_SelI2STxWordLen; 			/*!< Specifies the AUDIO CODEC TX Word length
										This parameter can be a value of @ref AUDIO_CODEC_Word_Length */

	u32 CODEC_SelI2STxCHLen;			/*!< Specifies the AUDIO SPORT TX Channel length
										This parameter can be a value of @ref AUDIO_CODEC_Channel_Length  */

	u32 CODEC_SelI2STxCH;				/*!< Specifies the AUDIO SPORT TX Channel L/R
										This parameter can be a value of @ref AUDIO_CODEC_Tx_Channel  */

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

} I2S_InitTypeDef ;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
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

/** @defgroup AUDIO_CODEC_Tx_Channel
  * @{
  */
#define CH_LR						((u32)0x00000000)
#define CH_RL						((u32)0x00000001)
#define CH_LL						((u32)0x00000002)
#define CH_RR						((u32)0x00000003)

#define IS_CODEC_TXCHN_SEL(SEL) (((SEL) == CH_LR) || \
							((SEL) == CH_RL) || \
							((SEL) == CH_LL) || \
							((SEL) == CH_RR))
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

/** @defgroup AUDIO_CODEC_I2S
  * @{
  */
#define I2S0						    ((u32)0x00000000)
#define I2S1							((u32)0x00000001)

#define IS_CODEC_I2S_SEL(SEL) (((SEL) == I2S0) || \
								((SEL) == I2S1))
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_I2S_TDM_Mode
  * @{
  */
#define I2S_NOTDM						    ((u32)0x00000000)
#define I2S_TDM4							((u32)0x00000001)
#define I2S_TDM6							((u32)0x00000002)
#define I2S_TDM8							((u32)0x00000003)

#define IS_CODEC_I2S_TDM_SEL(SEL) (((SEL) == I2S_NOTDM) || \
							((SEL) == I2S_TDM4) || \
							((SEL) == I2S_TDM6) || \
							((SEL) == I2S_TDM8))
/**
  * @}
  */


/** @defgroup AUDIO_CODEC_AMIC
  * @{
  */
#define AMIC1							((u32)0x00000001)
#define AMIC2							((u32)0x00000002)
#define AMIC3							((u32)0x00000003)
#define AMIC4							((u32)0x00000004)
#define AMIC5							((u32)0x00000005)
#define IS_CODEC_AMIC_SEL(SEL) (((SEL) == AMIC1) || \
							((SEL) == AMIC2) || \
							((SEL) == AMIC3) || \
							((SEL) == AMIC4) || \
							((SEL) == AMIC5))
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
#define ADC5							((u32)0x00000005)
#define ADC6							((u32)0x00000006)
#define ADC7							((u32)0x00000007)
#define ADC8							((u32)0x00000008)
#define IS_CODEC_ADC_SEL(SEL) (((SEL) == ADC1) || \
							((SEL) == ADC2) || \
							((SEL) == ADC3) || \
							((SEL) == ADC4) || \
							((SEL) == ADC5) || \
							((SEL) == ADC6) || \
							((SEL) == ADC7) || \
							((SEL) == ADC8))
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
#define ADCHN5							((u32)0x00000005)
#define ADCHN6							((u32)0x00000006)
#define ADCHN7							((u32)0x00000007)
#define ADCHN8							((u32)0x00000008)
#define IS_CODEC_ADCHN_SEL(SEL) (((SEL) == ADCHN1) || \
								((SEL) == ADCHN2) || \
								((SEL) == ADCHN3) || \
								((SEL) == ADCHN4) || \
								((SEL) == ADCHN5) || \
								((SEL) == ADCHN6) || \
								((SEL) == ADCHN7) || \
								((SEL) == ADCHN8))
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
#define LOWPOWER						((u32)0x00000002)
#define SHUTDOWN						((u32)0x00000003)
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
#define DMIC5							((u32)0x00000005)
#define DMIC6							((u32)0x00000006)
#define DMIC7							((u32)0x00000007)
#define DMIC8							((u32)0x00000008)
#define IS_CODEC_DMIC_SEL(SEL) (((SEL) == DMIC1) || \
							((SEL) == DMIC2) || \
							((SEL) == DMIC3) || \
							((SEL) == DMIC4) || \
							((SEL) == DMIC5) || \
							((SEL) == DMIC6) || \
							((SEL) == DMIC7) || \
							((SEL) == DMIC8))
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

/** @defgroup AUDIO_CODEC_I2S_Master_Source
  * @{
  */
#define INTERNAL_SPORT			((u32)0x00000000)
#define EXTERNAL_I2S			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Power_Mode
  * @{
  */
#define POWER_ON			((u32)0x00000000)
#define POWER_DOWN			((u32)0x00000001)
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

/** @defgroup AUDIO_CODEC_ADC_MUTE_Control
  * @{
  */
#define LINEIN				((u32)0x00000001)
#define MICIN				((u32)0x00000002)
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

/** @defgroup AUDIO_CODEC_Mute
  * @{
  */
#define UNMUTE			((u32)0x00000000)
#define MUTE			((u32)0x00000001)
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

/** @defgroup AUDIO_CODEC_DAC_Channel
  * @{
  */
#define DAC_L			((u32)0x00000000)
#define DAC_R			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_Vref_Value
  * @{
  */
#define VOL_AUDIO_VREF_1V8_F		0x7DE
#define VOL_AUDIO_VREF_1V8_S		0x7DF
#define AUDIO_CAL_5BIT_VALUE(x)		((u8)(((x) & 0x1F) >> 0))
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

/** @defgroup AUDIO_CODEC_PATH
  * @{
  */

#define I2SL					((u32)0x00000000)
#define I2SR					((u32)0x00000001)
#define I2SLR					((u32)0x00000002)
#define TESTTONE				((u32)0x00000003)
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

/** @defgroup AUDIO_CODEC_DAC_MUTE_Control
  * @{
  */
#define ANALOGIN			((u32)0x00000001)
#define DACIN				((u32)0x00000002)
/**s
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
#define APP_HPO_OUT		((u32)0x00000005)
#define APP_DAAD_LPBK	((u32)0x00000006)
#define APP_ADDA_LPBK	((u32)0x00000007)
/**
  * @}
  */

/** @defgroup AUDIO_CODEC_PAD
  * @{
  */
#define PAD_DACL ((u32)0x00000000)
#define PAD_DACR ((u32)0x00000001)
#define PAD_MIC1 ((u32)0x00000002)
#define PAD_MIC2 ((u32)0x00000003)
#define PAD_MIC3 ((u32)0x00000004)
#define PAD_MIC4 ((u32)0x00000005)
#define PAD_MIC5 ((u32)0x00000006)
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

/** @defgroup AUDIO_CODEC_LineOutOrHPO_Channel
  * @{
  */
#define CHN_L			((u32)0x00000000)
#define CHN_R			((u32)0x00000001)
#define CHN_LR			((u32)0x00000002)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
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
_LONG_CALL_ void AUDIO_CODEC_SetADCASRC(u32 i2s_sel, u32 adc_sel, u32 sr, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCMute(u32 adc_sel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCMixMute(u32 adc_num, u32 type,  			      u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetADCANASrc(u32 ad_chn, u32 amic_num);
_LONG_CALL_ void AUDIO_CODEC_SetADCANAFilter(u32 adc_num, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetMicBiasPowerMode(u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstPowerMode(u32 amic_num, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBiasPCUTMode(u32 amic_num, u32 pcut_mode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstChnMute(u32 amic_sel, u32 type, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstInputMode(u32 amic_num, u32 mode);
_LONG_CALL_ void AUDIO_CODEC_SetMicBstGain(u32 amic_sel, u32 gain);
_LONG_CALL_ void AUDIO_CODEC_SetADCDmicFilter(u32 adc_num, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDmicClk(u32 clk, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDmicSrc(u32 ad_chn, u32 dmic_num);
_LONG_CALL_ void AUDIO_CODEC_SetDACSrc(u32 i2s_sel, u32 dac_l_src, u32 dac_r_src);
_LONG_CALL_ void AUDIO_CODEC_SetADCSrc(u32 i2s_sel, u32 adc_num);
_LONG_CALL_ void AUDIO_CODEC_EnableDAC(u32 channel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACMute(u32 channel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACVolume(u32 channel, u32 gain);
_LONG_CALL_ void AUDIO_CODEC_SetDACHPF(u32 channel, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACASRC(u32 sr, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACPowerMode(u32 channel, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetLineOutPowerMode(u32 channel, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetHPOPowerMode(u32 channel, u32 powermode);
_LONG_CALL_ void AUDIO_CODEC_SetLineOutMode(u32 channel, u32 mode);
_LONG_CALL_ void AUDIO_CODEC_SetHPOMode(u32 channel, u32 mode);
_LONG_CALL_ void AUDIO_CODEC_SetLineOutMute(u32 channel, u32 type, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetHPOMute(u32 channel, u32 type, u32 newstate);
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
_LONG_CALL_ void AUDIO_CODEC_EnableADCFifo(u32 ad_chn, u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_EnableDACFifo(u32 newstate);
_LONG_CALL_ void AUDIO_CODEC_SetDACSilenceDet(u32 channel, u32 status);
_LONG_CALL_ void AUDIO_CODEC_SelDACSilenceLevel(u32 channel, u32 level);
_LONG_CALL_ void AUDIO_CODEC_SetADCSilenceDet(u32 channel, u32 status);
_LONG_CALL_ void AUDIO_CODEC_SelADCSilenceLevel(u32 channel, u32 level);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif
/** @} */

/** @} */

/** @} */
