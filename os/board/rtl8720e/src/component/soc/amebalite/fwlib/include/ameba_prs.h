#ifndef _AMEBA_PRS_H_
#define _AMEBA_PRS_H_

/* MANUAL_GEN_START */

//Please add your definition here

/* Exported types ------------------------------------------------------------*/
/** @defgroup PRS_Exported_Types PRS Exported Types
  * @{
  */

/**
  * @brief	PRS Init Structure Definition
  */
typedef struct {
	u8 PRS_ProducerType;		/*!< Specifies PRS producer signal type, which can be a value of
									@ref PRS_Producer_Signal_Type_Definitions */

	u8 PRS_ConsumerType;		/*!< Specifies PRS consumer signal type which can be a value of
									@ref PRS_Consumer_Signal_Type_Definitions */

	u8 PRS_TimerOfIn;			/*!< Specifies whether input timer overflow signal to PRS */

	u8 PRS_ReverseSig;			/*!< Specifies whether to reverse source signal from producer after receiving it */

	u16 PRS_DebounceCnt;		/*!< Specifies debounce count under XTAL40MHz clock */
} PRS_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PRS_Exported_Constants PRS Exported Constants
  * @{
  */

/** @defgroup PRS_Producer_Signal_Type_Definitions
  * @{
  */
#define PRS_SRC_NONE					((u8)0x0)
#define PRS_SRC_GPIO					((u8)0x1)
#define PRS_SRC_CMP						((u8)0x2)
#define PRS_SRC_AUDIO_0					((u8)0x3)
#define PRS_SRC_AUDIO_1					((u8)0x4)
#define PRS_SRC_AUDIO_2					((u8)0x5)

#define IS_PRS_PRODUCER_TYPE(sel)		((sel) < 0x0f)

/**
  * @}
  */

/** @defgroup PRS_Consumer_Signal_Type_Definitions
  * @{
  */
#define PRS_DST_NONE					((u8)0x01)
#define PRS_DST_TIM_EN					((u8)0x02)
#define PRS_DST_PWM_TRIG				((u8)0x04)

#define IS_PRS_CONSUMER_TYPE(sel)		(((sel) & 0x07) != 0 && \
										((sel) & 0xf8) == 0)

/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PRS_Exported_Functions PRS Exported Functions
  * @{
  */

_LONG_CALL_ void PRS_StructInit(PRS_InitTypeDef *PRS_InitStruct);
_LONG_CALL_ void PRS_Init(PRS_InitTypeDef *PRS_InitStruct);
_LONG_CALL_ void PRS_SrcConfig(u8 SrcType);
_LONG_CALL_ void PRS_DstConfig(u8 DstType);
_LONG_CALL_ void PRS_TimOfInCmd(u32 NewState);
_LONG_CALL_ void PRS_ReverseCmd(u32 NewState);
_LONG_CALL_ void PRS_SetDbcCnt(u16 Dbc_Cnt);

/**
  * @}
  */

/* MANUAL_GEN_END */
#endif
