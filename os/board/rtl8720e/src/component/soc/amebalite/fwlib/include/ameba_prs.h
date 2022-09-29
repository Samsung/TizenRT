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
#define PRS_SRC0_NONE					((u8)0x00)
#define PRS_SRC0_GPIO					((u8)0x01)
#define PRS_SRC0_CMP					((u8)0x02)
#define PRS_SRC0_AUDIO_0				((u8)0x03)
#define PRS_SRC0_AUDIO_1				((u8)0x04)
#define PRS_SRC0_AUDIO_2				((u8)0x05)
#define PRS_SRC0_MASK					((u8)0x0F)
#define PRS_GET_SRC0(sel)				((u8)((sel) & PRS_SRC0_MASK) >> 0)

#define PRS_SRC1_TIM_OF					((u8)0x10)
#define PRS_SRC1_MASK					((u8)0xF0)
#define PRS_GET_SRC1(sel)				((u8)((sel) & PRS_SRC1_MASK) >> 4)


#define IS_PRS_PRODUCER_TYPE(sel)		(((((sel) & PRS_SRC0_MASK) == PRS_SRC0_NONE) || \
										(((sel) & PRS_SRC0_MASK) == PRS_SRC0_GPIO) || \
										(((sel) & PRS_SRC0_MASK) == PRS_SRC0_CMP) || \
										(((sel) & PRS_SRC0_MASK) == PRS_SRC0_AUDIO_0) || \
										(((sel) & PRS_SRC0_MASK) == PRS_SRC0_AUDIO_1) || \
										(((sel) & PRS_SRC0_MASK) == PRS_SRC0_AUDIO_2)) && \
										(((((sel) & PRS_SRC1_MASK) & 0xE0) == 0x00)))

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
_LONG_CALL_ void PRS_ReverseCmd(u32 NewState);
_LONG_CALL_ void PRS_SetDbcCnt(u16 Dbc_Cnt);

/**
  * @}
  */

/* MANUAL_GEN_END */
#endif
