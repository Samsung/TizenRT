#ifndef _AMEBAD2_AUD_H_
#define _AMEBAD2_AUD_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup AUD
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup AUD_Register_Definitions AUD Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup AUD_ADDA_CTL
 * @brief
 * @{
 **/
#define AUD_BIT_POWADDACK                   ((u32)0x00000001 << 0)          /*!<R/W 1'b0  AD/DA clock power down control (0: power down, 1: power on) */
#define AUD_BIT_DAC_CKXEN                   ((u32)0x00000001 << 1)          /*!<R/W 1'b1  DAC chopper clock enable control (0: disable, 1: enable) */
#define AUD_BIT_DAC_CKXSEL                  ((u32)0x00000001 << 2)          /*!<R/W 1'b0  DAC chopper clock selection (0: ckx2/4, 1: ckx2/8) */
#define AUD_BIT_DAC_POW                     ((u32)0x00000001 << 3)          /*!<R/W 1'b0  DAC power down control (0: power down, 1: power on) */
#define AUD_MASK_DPRAMP_CSEL                ((u32)0x00000003 << 4)          /*!<R/W 2'b11  Depop C size selection (00: 1x, 01: 2x, 10: 3x, 11: 4x) */
#define AUD_DPRAMP_CSEL(x)                  ((u32)(((x) & 0x00000003) << 4))
#define AUD_GET_DPRAMP_CSEL(x)              ((u32)(((x >> 4) & 0x00000003)))
#define AUD_BIT_DPRAMP_ENRAMP               ((u32)0x00000001 << 6)          /*!<R/W 1'b0  DPRAMP enable ramp control (0: disable, 1: enable) */
#define AUD_BIT_DPRAMP_POW                  ((u32)0x00000001 << 7)          /*!<R/W 1'b0  DPRAMP power down control (0: power down, 1: power on) */
#define AUD_BIT_DTSDM_CKXEN                 ((u32)0x00000001 << 8)          /*!<R/W 1'b1  ADC integrater 1 OP chopper enable: 0 : disable 1 : enable (default) */
#define AUD_BIT_DTSDM_POW_L                 ((u32)0x00000001 << 9)          /*!<R/W 1'b0  Left channel ADC power on control: 0: power down 1: power on */
#define AUD_BIT_DTSDM_POW_R                 ((u32)0x00000001 << 10)          /*!<R/W 1'b0  Right channel ADC power on control: 0: power down 1: power on */
/** @} */

/** @defgroup AUD_LO_CTL
 * @brief
 * @{
 **/
#define AUD_BIT_LO_CAL                      ((u32)0x00000001 << 0)          /*!<R/W 1'b0  Lineout output offset calibration mode selection (1: calibration mode, 0: normal mode) */
#define AUD_BIT_LO_ENDP                     ((u32)0x00000001 << 1)          /*!<R/W 1'b1  Lineout enable depop control (0: disable, 1: enable) */
#define AUD_BIT_LO_POW                      ((u32)0x00000001 << 2)          /*!<R/W 1'b0  Lineout power down control (0: power down, 1: power on) */
#define AUD_BIT_LO_MDP                      ((u32)0x00000001 << 3)          /*!<R/W 1'b0  Lineout mute depop mode control (0: no depop, 1: depop) */
#define AUD_BIT_LO_SE                       ((u32)0x00000001 << 4)          /*!<R/W 1'b1  Lineout single-end mode control (0: differential, 1: single-end) */
#define AUD_MASK_LO_M                       ((u32)0x00000003 << 5)          /*!<R/W 2'b11  Lineout mute control (0: un-mute, 1: mute), <0>: DAC, <1>: Analog in */
#define AUD_LO_M(x)                         ((u32)(((x) & 0x00000003) << 5))
#define AUD_GET_LO_M(x)                     ((u32)(((x >> 5) & 0x00000003)))
#define AUD_BIT_LO_OPNDP                    ((u32)0x00000001 << 7)          /*!<R/W 1'b0  Lineout op negative depop mode control (0: no depop, 1: depop) */
#define AUD_BIT_LO_OPPDP                    ((u32)0x00000001 << 8)          /*!<R/W 1'b0  Lineout op positive depop mode control (0: no depop, 1: depop) */
#define AUD_MASK_LO_GSEL                    ((u32)0x00000003 << 9)          /*!<R/W 2'b00  Lineout gain selection (00: 0dB, 01: -3dB, 10: -6dB, 11: -9dB) */
#define AUD_LO_GSEL(x)                      ((u32)(((x) & 0x00000003) << 9))
#define AUD_GET_LO_GSEL(x)                  ((u32)(((x >> 9) & 0x00000003)))
#define AUD_MASK_LO_BIAS                    ((u32)0x00000003 << 11)          /*!<R/W 2'b00  Lineout mode output MOS quiescent curremt tuning (00: 120uA) */
#define AUD_LO_BIAS(x)                      ((u32)(((x) & 0x00000003) << 11))
#define AUD_GET_LO_BIAS(x)                  ((u32)(((x >> 11) & 0x00000003)))
#define AUD_MASK_LO_CCSEL                   ((u32)0x00000003 << 13)          /*!<R/W 2'b11  Lineout op 1st stage miller cap size (00: 20pF, 01: 24pF, 10: 28pF, 11: 32pF) */
#define AUD_LO_CCSEL(x)                     ((u32)(((x) & 0x00000003) << 13))
#define AUD_GET_LO_CCSEL(x)                 ((u32)(((x >> 13) & 0x00000003)))
/** @} */

/** @defgroup AUD_MICBIAS_CTL0
 * @brief
 * @{
 **/
#define AUD_BIT_MBIAS_POW                   ((u32)0x00000001 << 0)          /*!<R/W 1'b0  MBIAS power control 0: power down 1: power on */
#define AUD_BIT_MICBIAS1_ENCHX              ((u32)0x00000001 << 1)          /*!<R/W 1'b1  MICBIAS enable chopper clock 0:disable 1:enable */
#define AUD_BIT_MICBIAS1_POW                ((u32)0x00000001 << 2)          /*!<R/W 1'b0  MICBIAS power control 0:power down 1:power on */
#define AUD_MASK_MICBIAS1_VSET              ((u32)0x0000000F << 3)          /*!<R/W 4'b0011  MICBIAS select output voltage level 0.1V per step 0000:1.15V 0001:1.25V 0010:1.35V 0011:1.45V 0100:1.55V 0101:1.65V 0110:1.75V 0111:1.8V */
#define AUD_MICBIAS1_VSET(x)                ((u32)(((x) & 0x0000000F) << 3))
#define AUD_GET_MICBIAS1_VSET(x)            ((u32)(((x >> 3) & 0x0000000F)))
#define AUD_MASK_MICBIAS1_OCSEL             ((u32)0x00000003 << 7)          /*!<R/W 2'b01  OCP current selection 00: 2.5mA 01: 5mA 10: 7.5mA 11: 10mA */
#define AUD_MICBIAS1_OCSEL(x)               ((u32)(((x) & 0x00000003) << 7))
#define AUD_GET_MICBIAS1_OCSEL(x)           ((u32)(((x >> 7) & 0x00000003)))
#define AUD_MASK_MICBIAS1_COUNT             ((u32)0x00000003 << 9)          /*!<R/W 2'b01  when OCP happen disable time x312.5kHz 00: 819.2us 01: 1638.4us 10: 3276.8us 11: 6553.6us */
#define AUD_MICBIAS1_COUNT(x)               ((u32)(((x) & 0x00000003) << 9))
#define AUD_GET_MICBIAS1_COUNT(x)           ((u32)(((x >> 9) & 0x00000003)))
#define AUD_BIT_MICBIAS1_POWSHDT            ((u32)0x00000001 << 11)          /*!<R/W 1'b0  MICBIAS OCP power control 0: disable OCP 1: enable OCP */
#define AUD_MASK_MICBIAS1_COMP              ((u32)0x0000000F << 12)          /*!<R/W 4'b0000  MICBIAS1_COMP<0> 0: disable internal miller cap 1: enable internal miller cap MICBIAS1_COMP<3:1> reserved */
#define AUD_MICBIAS1_COMP(x)                ((u32)(((x) & 0x0000000F) << 12))
#define AUD_GET_MICBIAS1_COMP(x)            ((u32)(((x >> 12) & 0x0000000F)))
#define AUD_BIT_MICBIAS1_OC                 ((u32)0x00000001 << 16)          /*!<R   OCP happen output signal */
/** @} */

/** @defgroup AUD_MICBST_CTL0
 * @brief
 * @{
 **/
#define AUD_BIT_MICBST_POWL                 ((u32)0x00000001 << 0)          /*!<R/W 1'b0  MICBST power control left channel 0:power down 1:power on */
#define AUD_BIT_MICBST_POWR                 ((u32)0x00000001 << 1)          /*!<R/W 1'b0  MICBST power control right channel 0:power down 1:power on */
#define AUD_BIT_MICBST_ENDFL                ((u32)0x00000001 << 2)          /*!<R/W 1'b0  MICBST left channel enable differential 0:single to differential 1:differential to differential */
#define AUD_BIT_MICBST_ENDFR                ((u32)0x00000001 << 3)          /*!<R/W 1'b0  MICBST right channel enable differential 0:single to differential 1:differential to differential */
#define AUD_BIT_MICBST_ENCALL               ((u32)0x00000001 << 4)          /*!<R/W 1'b0  MICBST left channel enable calibration path 0:disable 1:enable */
#define AUD_BIT_MICBST_ENCAL_SWAPL          ((u32)0x00000001 << 5)          /*!<R/W 1'b0  MICBST left channel swap calibration path 0:disable 1:enable */
#define AUD_MASK_MICBST_GSELL               ((u32)0x0000000F << 6)          /*!<R/W 4'b0000  MICBST left channel gain select 0000: 0dB 0001:5dB 0010:10dB 0011:15dB 0100:20dB 0101:25dB 0110:30dB 0111:35dB 1XXX:40dB */
#define AUD_MICBST_GSELL(x)                 ((u32)(((x) & 0x0000000F) << 6))
#define AUD_GET_MICBST_GSELL(x)             ((u32)(((x >> 6) & 0x0000000F)))
#define AUD_MASK_MICBST_GSELR               ((u32)0x0000000F << 10)          /*!<R/W 4'b0000  MICBST right channel gain select 0000: 0dB 0001:5dB 0010:10dB 0011:15dB 0100:20dB 0101:25dB 0110:30dB 0111:35dB 1XXX:40dB */
#define AUD_MICBST_GSELR(x)                 ((u32)(((x) & 0x0000000F) << 10))
#define AUD_GET_MICBST_GSELR(x)             ((u32)(((x >> 10) & 0x0000000F)))
#define AUD_MASK_MICBST_MUTE_L              ((u32)0x00000003 << 14)          /*!<R/W 2'b11  MICBST left channel mute control mute<0>: mic in mute<1>: line in 0:unmute 1:mute */
#define AUD_MICBST_MUTE_L(x)                ((u32)(((x) & 0x00000003) << 14))
#define AUD_GET_MICBST_MUTE_L(x)            ((u32)(((x >> 14) & 0x00000003)))
#define AUD_MASK_MICBST_MUTE_R              ((u32)0x00000003 << 16)          /*!<R/W 2'b11  MICBST right channel mute control mute<0>: mic in mute<1>: line in 0:unmute 1:mute */
#define AUD_MICBST_MUTE_R(x)                ((u32)(((x) & 0x00000003) << 16))
#define AUD_GET_MICBST_MUTE_R(x)            ((u32)(((x >> 16) & 0x00000003)))
/** @} */

/** @defgroup AUD_MICBST_CTL1
 * @brief
 * @{
 **/
#define AUD_BIT_MICBST3_POW                 ((u32)0x00000001 << 0)          /*!<R/W 1'b0  MICBST3 power control left channel 0:power down 1:power on */
#define AUD_BIT_MICBST3_ENDF                ((u32)0x00000001 << 1)          /*!<R/W 1'b0  MICBST3 channel enable differential 0:single to differential 1:differential to differential */
#define AUD_BIT_MICBST3_ENCAL               ((u32)0x00000001 << 2)          /*!<R/W 1'b0  MICBST3 enable calibration path 0:disable 1:enable */
#define AUD_BIT_MICBST3_ENCAL_SWAP          ((u32)0x00000001 << 3)          /*!<R/W 1'b0  MICBST3 swap calibration path 0:disable 1:enable */
#define AUD_MASK_MICBST3_GSEL               ((u32)0x0000000F << 4)          /*!<R/W 4'b0000  MICBST3 channel gain select if CODEC_RESERVE<3>=0(MICBST3_GAIN_OPT=0) 0000: 3.52 dB 0001:7.23 dB 0010:11.36 dB 0011:15.85 dB 0100:20.42 dB 0101:25.34 dB 0110:31.25 dB 0111:35.19 dB 1XXX:40.04 dB if CODEC_RESERVE<3>=1 (MICBST3_GAIN_OPT=1) 0000: 3.52 dB 0001:7.23 dB 0010:11.36 dB 0011:15.85 dB 0100:20.42 dB 0101:25.34 dB 0110:25.34 dB 0111:29.25 dB 1XXX:34.07 dB */
#define AUD_MICBST3_GSEL(x)                 ((u32)(((x) & 0x0000000F) << 4))
#define AUD_GET_MICBST3_GSEL(x)             ((u32)(((x >> 4) & 0x0000000F)))
#define AUD_BIT_MICBST3_MUTE                ((u32)0x00000001 << 8)          /*!<R/W 1'b1  MICBST3 mute control mute: mic in 0:unmute 1:mute */
#define AUD_BIT_LDO_POW                     ((u32)0x00000001 << 12)          /*!<R/W 1'b0  LDO power control 0: disable LDO 1: enable LDO */
#define AUD_MASK_LDO_TUNE                   ((u32)0x0000001F << 13)          /*!<R/W 5'b10000  LDO voltage control 10000 Vref 0.9V LDO 1.8V */
#define AUD_LDO_TUNE(x)                     ((u32)(((x) & 0x0000001F) << 13))
#define AUD_GET_LDO_TUNE(x)                 ((u32)(((x >> 13) & 0x0000001F)))
#define AUD_BIT_LDO_COMP_INT                ((u32)0x00000001 << 18)          /*!<R/W 1'b0  LDO Miller Compensation 0:close 1:short */
#define AUD_BIT_LDO_POW_0P9V                ((u32)0x00000001 << 19)          /*!<R/W 1'b0  LDO Precharge 0: LDO 1: Unity Gain buffer */
#define AUD_BIT_LDO_PREC                    ((u32)0x00000001 << 20)          /*!<R/W 1'b0  REG_LOD_PREC */
#define AUD_BIT_MICBSTL_ENERGY_METER_MODE   ((u32)0x00000001 << 0)          /*!<R/W 0  MICBSTL_ENERGY_METER_MODE 0: turn off energy-meter mode of MICBST_L 1: turn on energy-meter mode of MICBST_L */
#define AUD_BIT_MICBSTR_ENERGY_METER_MODE   ((u32)0x00000001 << 1)          /*!<R/W 0  MICBSTR_ENERGY_METER_MODE 0: turn off energy-meter mode of MICBST_R 1: turn on energy-meter mode of MICBST_R */
#define AUD_BIT_MICBST3_ENERGY_METER_MODE   ((u32)0x00000001 << 2)          /*!<R/W 0  MICBST3_ENERGY_METER_MODE 0: turn off energy-meter mode of MICBST3 1: turn on energy-meter mode of MICBST3 */
#define AUD_BIT_MICBST3_GAIN_OPT            ((u32)0x00000001 << 3)          /*!<R/W 0  MICBST3_GAIN_OPT influence gain of MICBST3, please see MICBST3_GSEL */
#define AUD_BIT_LDO_CODEC_DRIVING_PATH      ((u32)0x00000001 << 4)          /*!<R/W 1  LDO_CODEC_DRIVING_PATH selection for driving path of LDO_CODEC 0:select strong driving path, PMOS w=240um, L=0.44um 1:select 1.8kohm driving path(default) */
#define AUD_BIT_LDO_CODEC_DISCHARGE_PATH    ((u32)0x00000001 << 5)          /*!<R/W 1  LDO_CODEC_DISCHARGE_PATH selection for discharge path of LDO_CODEC 0:discharge path is turned off (default) 1:1.8kohm discharge path is turned on */
#define AUD_MASK_LDO_CUR_SEL                ((u32)0x00000007 << 6)          /*!<R/W 0  LDO current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_LDO_CUR_SEL(x)                  ((u32)(((x) & 0x00000007) << 6))
#define AUD_GET_LDO_CUR_SEL(x)              ((u32)(((x >> 6) & 0x00000007)))
#define AUD_MASK_DPRAMP_CUR_SEL             ((u32)0x00000007 << 9)          /*!<R/W 0  DPRAMP current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_DPRAMP_CUR_SEL(x)               ((u32)(((x) & 0x00000007) << 9))
#define AUD_GET_DPRAMP_CUR_SEL(x)           ((u32)(((x >> 9) & 0x00000007)))
#define AUD_MASK_VCM_CUR_SEL                ((u32)0x00000007 << 12)          /*!<R/W 0  DAC VCM buffer current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_VCM_CUR_SEL(x)                  ((u32)(((x) & 0x00000007) << 12))
#define AUD_GET_VCM_CUR_SEL(x)              ((u32)(((x >> 12) & 0x00000007)))
#define AUD_BIT_CODEC_RESERVE               ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define AUD_MASK_CODEC_RESERVE1             ((u32)0x0000FFFF << 16)          /*!<R/W 0   */
#define AUD_CODEC_RESERVE1(x)               ((u32)(((x) & 0x0000FFFF) << 16))
#define AUD_GET_CODEC_RESERVE1(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
/** @} */

/** @defgroup AUD_DTS_CTL
 * @brief
 * @{
 **/
#define AUD_BIT_DTSDM3_CKXEN                ((u32)0x00000001 << 3)          /*!<R/W 1'b1  ADC integrater 1 OP chopper enable: 0 : disable 1 : enable (default) */
#define AUD_BIT_DTSDM3_POW                  ((u32)0x00000001 << 4)          /*!<R/W 1'b0  Left channel ADC power on control: 0: power down 1: power on */
#define AUD_BIT_LPMODE_EN                   ((u32)0x00000001 << 5)          /*!<R/W 1'b0  low power mode en 0: disable , current use 12.5uA 1: enable , current use 1uA */
/** @} */

/** @defgroup AUD_MBIAS_CTL0
 * @brief
 * @{
 **/
#define AUD_MASK_MBIAS_ISEL_DAC             ((u32)0x00000007 << 0)          /*!<R/W 3'b110  DAC current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_DAC(x)               ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_MBIAS_ISEL_DAC(x)           ((u32)(((x >> 0) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_LO              ((u32)0x00000007 << 4)          /*!<R/W 3'b110  Lineout current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_LO(x)                ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_MBIAS_ISEL_LO(x)            ((u32)(((x >> 4) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_DTSDM3          ((u32)0x00000007 << 8)          /*!<R/W 3'b110  DTSDM3 current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_DTSDM3(x)            ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_MBIAS_ISEL_DTSDM3(x)        ((u32)(((x >> 8) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_DTSDM3_INT1     ((u32)0x00000007 << 12)          /*!<R/W 3'b110  DTSDM3 integrator current control: if LPMODE_EN=1 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA if LPMODE_EN=0 000:4uA, 001:6uA, 010:8uA, 011:10uA, 100:12uA, 101:14uA, 110:16uA, 111:18uA */
#define AUD_MBIAS_ISEL_DTSDM3_INT1(x)       ((u32)(((x) & 0x00000007) << 12))
#define AUD_GET_MBIAS_ISEL_DTSDM3_INT1(x)   ((u32)(((x >> 12) & 0x00000007)))
/** @} */

/** @defgroup AUD_MBIAS_CTL1
 * @brief
 * @{
 **/
#define AUD_MASK_MBIAS_ISEL_DTSDM_L         ((u32)0x00000007 << 0)          /*!<R/W 3'b110  DTSDM1_L current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_DTSDM_L(x)           ((u32)(((x) & 0x00000007) << 0))
#define AUD_GET_MBIAS_ISEL_DTSDM_L(x)       ((u32)(((x >> 0) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_DTSDM_INT1_L    ((u32)0x00000007 << 4)          /*!<R/W 3'b110  DTSDM1_L integrator current control: if LPMODE_EN=1 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA if LPMODE_EN=0 000:4uA, 001:6uA, 010:8uA, 011:10uA, 100:12uA, 101:14uA, 110:16uA, 111:18uA */
#define AUD_MBIAS_ISEL_DTSDM_INT1_L(x)      ((u32)(((x) & 0x00000007) << 4))
#define AUD_GET_MBIAS_ISEL_DTSDM_INT1_L(x)  ((u32)(((x >> 4) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_DTSDM_R         ((u32)0x00000007 << 8)          /*!<R/W 3'b110  DTSDM1_R current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_DTSDM_R(x)           ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_MBIAS_ISEL_DTSDM_R(x)       ((u32)(((x >> 8) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_DTSDM_INT1_R    ((u32)0x00000007 << 12)          /*!<R/W 3'b110  DTSDM1_R integrator current control: if LPMODE_EN=1 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA if LPMODE_EN=0 000:4uA, 001:6uA, 010:8uA, 011:10uA, 100:12uA, 101:14uA, 110:16uA, 111:18uA */
#define AUD_MBIAS_ISEL_DTSDM_INT1_R(x)      ((u32)(((x) & 0x00000007) << 12))
#define AUD_GET_MBIAS_ISEL_DTSDM_INT1_R(x)  ((u32)(((x >> 12) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_MICBIAS1        ((u32)0x00000007 << 16)          /*!<R/W 3'b110  MICBIAS1 current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_MICBIAS1(x)          ((u32)(((x) & 0x00000007) << 16))
#define AUD_GET_MBIAS_ISEL_MICBIAS1(x)      ((u32)(((x >> 16) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_MICBST_L        ((u32)0x00000007 << 24)          /*!<R/W 3'b110  MICBST_L current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_MICBST_L(x)          ((u32)(((x) & 0x00000007) << 24))
#define AUD_GET_MBIAS_ISEL_MICBST_L(x)      ((u32)(((x >> 24) & 0x00000007)))
#define AUD_MASK_MBIAS_ISEL_MICBST_R        ((u32)0x00000007 << 28)          /*!<R/W 3'b110  MICBST_R current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_MICBST_R(x)          ((u32)(((x) & 0x00000007) << 28))
#define AUD_GET_MBIAS_ISEL_MICBST_R(x)      ((u32)(((x >> 28) & 0x00000007)))
/** @} */

/** @defgroup AUD_MBIAS_CTL2
 * @brief
 * @{
 **/
#define AUD_MASK_MBIAS_ISEL_MICBST3         ((u32)0x00000007 << 8)          /*!<R/W 3'b110  MICBST3 current control: if LPMODE_EN=1 000:1uA, 001:1.5uA, 010:2uA, 011:2.5uA, 100:3uA, 101:3.5uA, 110:4uA, 111:4.5uA if LPMODE_EN=0 000:2uA, 001:3uA, 010:4uA, 011:5uA, 100:6uA, 101:7uA, 110:8uA, 111:9uA */
#define AUD_MBIAS_ISEL_MICBST3(x)           ((u32)(((x) & 0x00000007) << 8))
#define AUD_GET_MBIAS_ISEL_MICBST3(x)       ((u32)(((x >> 8) & 0x00000007)))
#define AUD_MASK_LO_RESERVED                ((u32)0x0000003F << 2)          /*!<R/W 0  LO_RESERVED reserved */
#define AUD_LO_RESERVED(x)                  ((u32)(((x) & 0x0000003F) << 2))
#define AUD_GET_LO_RESERVED(x)              ((u32)(((x >> 2) & 0x0000003F)))
#define AUD_BIT_LO_OP_SECOND_USE_SMALL_GM   ((u32)0x00000001 << 1)          /*!<R/W 0  LO_OP_SECOND_USE_SMALL_GM 0: enlarge 2nd stage gm of LO opamp to reduce voltage stress 1: 2nd stage gm of LO opamp use default small gm */
#define AUD_BIT_VB0_USE_SMALL_SW            ((u32)0x00000001 << 0)          /*!<R/W 0  VB0_USE_SMALL_SW 0: vb<0> use large switch to get better THD performance of Lineout 1: vb<0> use small switch */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup AUD_Exported_Types AUD Exported Types
  * @{
*****************************************************************************/


/**
 * @brief AUD Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t AUD_ADDA_CTL;                           /*!<  Register,  Address offset: 0x00 */
	__IO uint32_t AUD_LO_CTL;                             /*!<  Register,  Address offset: 0x04 */
	__IO uint32_t AUD_MICBIAS_CTL0;                       /*!<  Register,  Address offset: 0x08 */
	__IO uint32_t AUD_MICBST_CTL0;                        /*!<  Register,  Address offset: 0x00C */
	__IO uint32_t AUD_MICBST_CTL1;                        /*!<  Register,  Address offset: 0x010 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x14 */
	__IO uint32_t AUD_DTS_CTL;                            /*!<  Register,  Address offset: 0x018 */
	__IO uint32_t AUD_MBIAS_CTL0;                         /*!<  Register,  Address offset: 0x01C */
	__IO uint32_t AUD_MBIAS_CTL1;                         /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t AUD_MBIAS_CTL2;                         /*!<  Register,  Address offset: 0x024 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x28 */
	__IO uint32_t AUD_DUMMY;                              /*!<  Register,  Address offset: 0x02C */
} AUD_TypeDef;

#define AUD_SYS_BASE ((AUD_TypeDef *) (0x41022100))
#define AUD_SYS_BASE_S ((AUD_TypeDef *) (0x51022100))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
