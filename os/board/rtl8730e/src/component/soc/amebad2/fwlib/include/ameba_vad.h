/**
  ******************************************************************************
  * @file    ameba_vad.h
  * @author  Hank_Li
  * @version V1.0.0
  * @date    2021-02-03
  * @brief   This file contains all the macros and functions prototypes for the VAD
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBAD2_VAD_API_
#define _AMEBAD2_VAD_API_

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @addtogroup VAD
* @{
*/

/* Exported constants --------------------------------------------------------*/
/** @defgroup VAD_Exported_Constants VAD Exported Constants
  * @{
  */

/** @defgroup VAD_ADC_Index
  * @{
  */
#define VAD_ADC_ZERO ((u32)0x00000000)
#define VAD_ADC_ONE ((u32)0x00000001)
#define VAD_ADC_TWO ((u32)0x00000002)
#define VAD_ADC_THREE ((u32)0x00000003)
#define VAD_ADC_FOUR ((u32)0x00000004)
/**
  * @}
  */

/** @defgroup VAD_CODEC_Index
  * @{
  */
#define VAD_CODEC_ZERO ((u32)0x00000000)
#define VAD_CODEC_ONE ((u32)0x00000001)
#define VAD_CODEC_TWO ((u32)0x00000002)
#define VAD_CODEC_THREE ((u32)0x00000003)
/**
  * @}
  */

/** @defgroup VAD_DMIC_Index
  * @{
  */
#define VAD_DMIC_ZERO ((u32)0x00000000)
#define VAD_DMIC_ONE ((u32)0x00000001)
#define VAD_DMIC_TWO ((u32)0x00000002)
#define VAD_DMIC_THREE ((u32)0x00000003)
#define VAD_DMIC_FOUR ((u32)0x00000004)
#define VAD_DMIC_FIVE ((u32)0x00000005)
#define VAD_DMIC_SIX ((u32)0x00000006)
#define VAD_DMIC_SEVEN ((u32)0x00000007)
/**
  * @}
  */

/** @defgroup VAD_DMIC_Clock
  * @{
  */
#define VAD_DMIC_CLK_2M ((u32)0x00000000)
#define VAD_DMIC_CLK_1M ((u32)0x00000001)
#define VAD_DMIC_CLK_500K ((u32)0x00000002)
#define VAD_DMIC_CLK_250K ((u32)0x00000003)
#define VAD_DMIC_CLK_125K ((u32)0x00000004)
/**
  * @}
  */

/** @defgroup VAD_HPF_Level
  * @{
  */
#define VAD_HPF_ZERO ((u32)0x00000000)
#define VAD_HPF_ONE ((u32)0x00000001)
#define VAD_HPF_TWO ((u32)0x00000002)
#define VAD_HPF_THREE ((u32)0x00000003)
#define VAD_HPF_FOUR ((u32)0x00000004)
#define VAD_HPF_FIVE ((u32)0x00000005)
#define VAD_HPF_SIX ((u32)0x00000006)
#define VAD_HPF_SEVEN ((u32)0x00000007)
/**
  * @}
  */

/** @defgroup VAD_Interrupt_Source
  * @{
  */
#define VAD_INTERRUPT_KM0 ((u32)0x00000000)
#define VAD_INTERRUPT_KM4 ((u32)0x00000001)
#define VAD_INTERRUPT_CA7 ((u32)0x00000002)
/**
  * @}
  */

/** @defgroup VAD_Buffer_Single_Block_Index
  * @{
  */
#define VAD_BLOCK_A ((u32)0x00000000)
#define VAD_BLOCK_B ((u32)0x00000001)
#define VAD_BLOCK_C ((u32)0x00000002)
#define VAD_BLOCK_D ((u32)0x00000003)
/**
  * @}
  */

/** @defgroup VAD_Buffer_Double_Block_Index
  * @{
  */
#define VAD_BLOCK_AB ((u32)0x00000000)
#define VAD_BLOCK_BC ((u32)0x00000001)
#define VAD_BLOCK_CD ((u32)0x00000002)
/**
  * @}
  */

/** @defgroup VAD_Buffer_Triple_Block_Index
  * @{
  */
#define VAD_BLOCK_ABC ((u32)0x00000000)
#define VAD_BLOCK_BCD ((u32)0x00000001)
/**
  * @}
  */

/** @defgroup VAD_Buffer_Quad_Block_Index
  * @{
  */
#define VAD_BLOCK_ABCD ((u32)0x00000000)
/**
  * @}
  */

/** @defgroup VAD_Buffer_Transfer_Mode
  * @{
  */
#define VAD_BLOCK_MODE ((u32)0x00000000)
#define VAD_INTERLEAVE_MODE ((u32)0x00000001)
/**
  * @}
  */

/** @defgroup VAD_Judge_Type
  * @{
  */
#define VAD_HOLD ((u32)0x00000000)
#define VAD_INSTANT ((u32)0x00000001)
/**
  * @}
  */

/** @defgroup VAD_Clock_Source
  * @{
  */
#define VAD_OSC ((u32)0x00000000)
#define VAD_XTAL ((u32)0x00000001)
/**
  * @}
  */

/** @defgroup VAD_Interrupt_Type
  * @{
  */
#define VAD_INTERRUPT_FALLING ((u32)0x00000000)
#define VAD_INTERRUPT_RISING ((u32)0x00000001)
/**
  * @}
  */

/** @defgroup VAD_ADC_ANALOG_CLK_Select
  * @{
  */
#define CLK_FOR_AUDIO_CODEC ((u32)0x00000000)
#define CLK_FOR_PC_VAD_CODEC ((u32)0x00000001)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup VAD_Exported_Functions VAD Exported Functions
  * @{
  */


void VAD_Power_Init(u32 clock_source);
void VAD_Set_Bit(void);
void VAD_ADC_Codec_Init(u32 codec_index);
void VAD_ADC_Init(u32 ADC_index);
void VAD_DMIC_Codec_Init(u32 codec_index);
void VAD_Delay(u32 time_ms);
void VAD_DMIC_Codec_Enable(u32 codec_index);
void VAD_ADC_Select(u32 codec_index, u32 adc_index);
void VAD_DMIC_Select(u32 codec_index, u32 dmic_index);
void VAD_Source_Select(u32 codec_index);
void VAD_HPF_Init(u32 codec_index, u32 cut_fre_select);
void VAD_Interrupt_set(u32 vad_interrupt_source, IRQ_FUN CallbackFunc, u32 IrqPriority);
void VAD_Buf_move_one(u32 codec_index, u32 block_index, u32 transfer_mode);
void VAD_Buf_move_two(u32 codec_index_0, u32 codec_index_1, u32 block_index, u32 transfer_mode);
void VAD_Buf_move_three(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 block_index, u32 transfer_mode);
void VAD_Buf_move_four(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 codec_index_3, u32 block_index, u32 transfer_mode);
void VAD_Codec_Select(u8 codec_index);
void VAD_Start(void);
void VAD_Pitch_Set(u32 type);
void VAD_Energy_Set(u32 type);
void VAD_Abs_Set(u32 type);
void VAD_Abs_thre(u16 thre);
void VAD_Energy_keep_Thre(u16 thr);
void VAD_Energy_Search_Thre(u16 thr1, u16 thr2);
void VAD_Det_OD_Default_Thre(u16 thr);
void VAD_Det_MV_Thre(u16 thr);
void VAD_Det_Energy_CHK_Thre(u16 thr);
void VAD_Det_OD_Energy_Thre(u16 thr);
void VAD_Det_Thr_ST_Valid(u16 thr);
void VAD_ADC_Clock_Enable(u32 clock_sel);
void BT_VAD_Start(void);
void BT_VAD_SelSrc(u32 codec_index);
void BT_VAD_SetBit(void);
void BT_VAD_ENIRQ(u32 mode);
void BT_VAD_DISIRQ(u32 mode);
void BT_VAD_SetDetThr(u32 threshold);
void BT_VAD_SetVoice(u32 voice_level);
void BT_VAD_Buf_move_one(u32 codec_index, u32 block_index, u32 transfer_mode);
void BT_VAD_Buf_move_two(u32 codec_index_0, u32 codec_index_1, u32 block_index, u32 transfer_mode);
void BT_VAD_Buf_move_three(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 block_index, u32 transfer_mode);
void BT_VAD_Buf_move_four(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 codec_index_3, u32 block_index, u32 transfer_mode);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif
