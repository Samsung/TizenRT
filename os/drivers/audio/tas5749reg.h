/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __DRIVERS_AUDIO_TAS5749REG_H
#define __DRIVERS_AUDIO_TAS5749REG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#ifdef CONFIG_AUDIO_TAS5749

#define TAS5749_TI_DEVICE_ID_VAL 0x88
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
typedef enum {
	TAS5749_REG_D_1BYTE,
	TAS5749_REG_D_4BYTE,
	TAS5749_REG_DATA_TYPE_MAX
} TAS5749_REG_DATA_TYPE;

/* Registers Addresses 1byte mode ******************************************************/
#define TAS5749_REG_1BYTE_MODE
#define TAS5749_CLOCK_CTRL					0x00
#define TAS5749_GENERAL_STATUS					0x01
#define TAS5749_SOFT_RESET					0x02
#define TAS5749_MUTE_TIME					0x03
#define TAS5749_MASTER_VOLUME					0x04
#define TAS5749_FINE_VOLUME					0x05
#define TAS5749_VOLUME_CH1					0x06
#define TAS5749_VOLUME_CH2					0x07
#define TAS5749_AMP_SCALE					0x08
#define TAS5749_OPERATION					0x09
#define TAS5749_FUNCTION					0x0A
#define TAS5749_IC_DELAY_CH1					0x0B
#define TAS5749_IC_DELAY_CH2					0x0C
#define TAS5749_IC_DELAY_CH3					0x0D
#define TAS5749_IC_DELAY_CH4					0x0E
#define TAS5749_DC_THRESHOLD_TIMER				0x0F
#define TAS5749_MODULATION_LIMIT				0x10
#define TAS5749_LEGACY_SYSCTL1					0x14
#define TAS5749_LEGACY_SYSCTL2					0x15
#define TAS5749_IC_OFFET					0x17 // (reserved)
#define TAS5749_SURROUND_REGISTER				0x19
#define TAS5749_SPLITCAPT_REGISTER				0x1A // (start-stop preiod)
#define TAS5749_EFUSE_CMD					0x1B // (osc-trim)
#define TAS5749_BACK_END_ERROR					0x1C
#define TAS5749_PWM_TEST_REGISTER				0x1D
#define TAS5749_TEST_REGISTER0					0x1E
#define TAS5749_TEST_REGISTER1					0x1F
#define TAS5749_COEF_FORMAT					0x22
#define TAS5749_PWM_LEVEL_METER_AND_SDOUT_MUX			0x23
#define TAS5749_TI_DEVICE_ID					0x25
#define TAS5749_LEGACY_CLOCK_ERROR_REGISTER			0x26
#define TAS5749_I2C_MODE					0x7E

/* Registers Addresses 4byte mode******************************************************/
#define TAS5749_REG_4BYTE_MODE
#define TAS5749_CH_1_2_BIQUAD_1_B0				0x00
#define TAS5749_CH_1_2_BIQUAD_1_B1				0x01
#define TAS5749_CH_1_2_BIQUAD_1_B2				0x02
#define TAS5749_CH_1_2_BIQUAD_1_A1				0x03
#define TAS5749_CH_1_2_BIQUAD_1_A2				0x04
#define TAS5749_CH_1_2_BIQUAD_2_B0				0x05
#define TAS5749_CH_1_2_BIQUAD_2_B1				0x06
#define TAS5749_CH_1_2_BIQUAD_2_B2				0x07
#define TAS5749_CH_1_2_BIQUAD_2_A1				0x08
#define TAS5749_CH_1_2_BIQUAD_2_A2				0x09
#define TAS5749_CH_1_2_BIQUAD_3_B0				0x0A
#define TAS5749_CH_1_2_BIQUAD_3_B1				0x0B
#define TAS5749_CH_1_2_BIQUAD_3_B2				0x0C
#define TAS5749_CH_1_2_BIQUAD_3_A1				0x0D
#define TAS5749_CH_1_2_BIQUAD_3_A2				0x0E
#define TAS5749_CH_1_2_BIQUAD_4_B0				0x0F
#define TAS5749_CH_1_2_BIQUAD_4_B1				0x10
#define TAS5749_CH_1_2_BIQUAD_4_B2				0x11
#define TAS5749_CH_1_2_BIQUAD_4_A1				0x12
#define TAS5749_CH_1_2_BIQUAD_4_A2				0x13
#define TAS5749_CH_1_2_BIQUAD_5_B0				0x14
#define TAS5749_CH_1_2_BIQUAD_5_B1				0x15
#define TAS5749_CH_1_2_BIQUAD_5_B2				0x16
#define TAS5749_CH_1_2_BIQUAD_5_A1				0x17
#define TAS5749_CH_1_2_BIQUAD_5_A2				0x18
#define TAS5749_CH_1_2_BIQUAD_6_B0				0x19
#define TAS5749_CH_1_2_BIQUAD_6_B1				0x1A
#define TAS5749_CH_1_2_BIQUAD_6_B2				0x1B
#define TAS5749_CH_1_2_BIQUAD_6_A1				0x1C
#define TAS5749_CH_1_2_BIQUAD_6_A2				0x1D
#define TAS5749_CH_1_2_BIQUAD_7_B0				0x1E
#define TAS5749_CH_1_2_BIQUAD_7_B1				0x1F
#define TAS5749_CH_1_2_BIQUAD_7_B2				0x20
#define TAS5749_CH_1_2_BIQUAD_7_A1				0x21
#define TAS5749_CH_1_2_BIQUAD_7_A2				0x22
#define TAS5749_CH_1_2_BIQUAD_8_B0				0x23
#define TAS5749_CH_1_2_BIQUAD_8_B1				0x24
#define TAS5749_CH_1_2_BIQUAD_8_B2				0x25
#define TAS5749_CH_1_2_BIQUAD_8_A1				0x26
#define TAS5749_CH_1_2_BIQUAD_8_A2				0x27
#define TAS5749_PEQ_CHECKSUM					0x28 // (XOR)
#define TAS5749_AGL_CHECKSUM					0x29 // (XOR)
#define TAS5749_CH_1_2_CROSSOVER_BIQUAD_B0			0x2A
#define TAS5749_CH_1_2_CROSSOVER_BIQUAD_B1			0x2B
#define TAS5749_CH_1_2_CROSSOVER_BIQUAD_B2			0x2C
#define TAS5749_CH_1_2_CROSSOVER_BIQUAD_A0			0x2D
#define TAS5749_CH_1_2_CROSSOVER_BIQUAD_A1			0x2E
#define TAS5749_DC_CRC_A_GL_CONTROL_REG				0x2F /* Reg 0x2F<23:16> = CRC latency target (default 0x01) (0 to 1359 ms in 5.3 ms steps at 48)
									0 to 1480 ms in 5.8 ms steps at 44.1) (30% tolerance + volume / HP / bankswitch / shutdown delays)
									Reg 0x2F<10> = Duty cycle error detect enable
									Reg 0x2F<9> = EQ CRC error detect enable
									Reg 0x2F<8> = AGL CRC error detect enable
									Reg 0x2F<5> = Enable (1-H) complementary low-pass filter generation
									Reg 0x2F<1> = AGL2 enable
									Reg 0x2F<0> = AGL1 enable */
#define TAS5749_INPUT_MUX_REGISTER				0x30
#define TAS5749_PWM_MUX_REGISTER				0x32
#define TAS5749_AGL1_THRESHOLD					0x35
#define TAS5749_AGL1_SOFTENING_ALPHA				0x36
#define TAS5749_AGL1_SOFTENING_OMEGA				0x37
#define TAS5749_AGL1_ATTACK_RATE				0x38
#define TAS5749_AGL1_RELEASE_RATE				0x39
#define TAS5749_AGL2_THRESHOLD					0x3A
#define TAS5749_AGL2_SOFTENING_ALPHA				0x3B
#define TAS5749_AGL_SOFTENING_OMEGA				0x3C
#define TAS5749_AGL2_ATTACK_RATE				0x3D
#define TAS5749_AGL2_RELEASE_RATE				0x3E
#define TAS5749_CH_1_OUTPUT_MIXER_LEFT				0x41
#define TAS5749_CH_1_OUTPUT_MIXER_RIGHT				0x42
#define TAS5749_CH_2_OUTPUT_MIXER_LEFT				0x43
#define TAS5749_CH_2_OUTPUT_MIXER_RIGHT				0x44
#define TAS5749_CH_1_INPUT_MIXER_RIGHT_PRE_EQ			0x45
#define TAS5749_CH_1_INPUT_MIXER_LEFT_PRE_EQ			0x46
#define TAS5749_CH_1_INPUT_MIXER_LEFT_PRE_AGL			0x47
#define TAS5749_CH_1_INPUT_MIXER_RIGHT_PRE_AGL			0x48
#define TAS5749_CH_2_INPUT_MIXER_RIGHT_PRE_EQ			0x49
#define TAS5749_CH_2_INPUT_MIXER_LEFT_PRE_EQ			0x4A
#define TAS5749_CH_2_INPUT_MIXER_LEFT_PRE_AGL			0x4B
#define TAS5749_CH_2_INPUT_MIXER_RIGHT_PRE_AGL			0x4C
#define TAS5749_OUTPUT_POST_SCALE				0x4D // (3.23)
#define TAS5749_OUTPUT_PRESCALE					0x4E // (9.17)
#define TAS5749_AMP_SCALE_2					0x4F // (3.23) Used in conjunction with 1-byte mode register 0x08(TAS5749_AMP_SCALE)
#define TAS5749_CH_1_2_BIQUAD_9_B0				0x51
#define TAS5749_CH_1_2_BIQUAD_9_B1				0x52
#define TAS5749_CH_1_2_BIQUAD_9_B2				0x53
#define TAS5749_CH_1_2_BIQUAD_9_A1				0x54
#define TAS5749_CH_1_2_BIQUAD_9_A2				0x55
#define TAS5749_CH_1_2_BIQUAD_A_B0				0x56
#define TAS5749_CH_1_2_BIQUAD_A_B1				0x57
#define TAS5749_CH_1_2_BIQUAD_A_B2				0x58
#define TAS5749_CH_1_2_BIQUAD_A_A1				0x59
#define TAS5749_CH_1_2_BIQUAD_A_A2				0x5A
#define TAS5749_AGL_ADVANC_ED_MODE_GAIN				0x5B
#define TAS5749_EFUSE1_RESERVED					0x5D
#define TAS5749_EFUSE2_RESERVED					0x5E
#define TAS5749_LVLMTR_ALPHA					0x5F // Level meter alpha
#define TAS5749_LVLMTR_OMEGA					0x60 // ED Level meter omega
#define TAS5749_LVLMTR_1_32B					0x61 // Read only 32-bit level meter output 1
#define TAS5749_LVLMTR_2_32B					0x62 // Read only 32-bit level meter output 2
#define TAS5749_LVLMTR_8B					0x67 /* Level meter 1 8 bit: inverted bit <27:20> of V1LM8
									Level meter 2 8 bit: inverted bit <27:20> of V2LM8 */
#define TAS5749_DBE_CTRL_COEFF_1				0x6A
#define TAS5749_DBE_CTRL_COEFF_2				0x6B
#define TAS5749_CH_1_2_DBE_BQ_B0				0x6C
#define TAS5749_CH_1_2_DBE_BQ_B1				0x6D
#define TAS5749_CH_1_2_DBE_BQ_B2				0x6E
#define TAS5749_CH_1_2_DBE_BQ_A1				0x6F
#define TAS5749_CH_1_2_DBE_BQ_A2				0x70
#define TAS5749_REPEAT_SUBADDRESS				0x7C	//Reserved 0x7D
#define TAS5749_REG_4BYTE_MODE_MAX
#endif	/* CONFIG_AUDIO_TAS5749 */
#endif	/* __DRIVERS_AUDIO_TAS5749REG_H */
