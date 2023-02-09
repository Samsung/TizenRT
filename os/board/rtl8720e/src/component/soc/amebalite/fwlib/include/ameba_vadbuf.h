#ifndef _AMEBA_VADBUF_H_
#define _AMEBA_VADBUF_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup VADBUF
  * @brief VADBUF driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup VADBUF_Register_Definitions VADBUF Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup VADBUF_VAD_BUF_CTRL0
 * @brief
 * @{
 **/
#define VADBUF_BIT_VAD_BIT_SEL                 ((u32)0x00000001 << 28)          /*!<R/W 0x1  0: 16 bits 1: 24 bits */
#define VADBUF_MASK_CODEC3_IN_BUF_ADD_BLOCK    ((u32)0x00000003 << 26)          /*!<R/W 0x3  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VADBUF_CODEC3_IN_BUF_ADD_BLOCK(x)      ((u32)(((x) & 0x00000003) << 26))
#define VADBUF_GET_CODEC3_IN_BUF_ADD_BLOCK(x)  ((u32)(((x >> 26) & 0x00000003)))
#define VADBUF_MASK_CODEC2_IN_BUF_ADD_BLOCK    ((u32)0x00000003 << 24)          /*!<R/W 0x2  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VADBUF_CODEC2_IN_BUF_ADD_BLOCK(x)      ((u32)(((x) & 0x00000003) << 24))
#define VADBUF_GET_CODEC2_IN_BUF_ADD_BLOCK(x)  ((u32)(((x >> 24) & 0x00000003)))
#define VADBUF_MASK_CODEC1_IN_BUF_ADD_BLOCK    ((u32)0x00000003 << 22)          /*!<R/W 0x1  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VADBUF_CODEC1_IN_BUF_ADD_BLOCK(x)      ((u32)(((x) & 0x00000003) << 22))
#define VADBUF_GET_CODEC1_IN_BUF_ADD_BLOCK(x)  ((u32)(((x >> 22) & 0x00000003)))
#define VADBUF_MASK_CODEC0_IN_BUF_ADD_BLOCK    ((u32)0x00000003 << 20)          /*!<R/W 0x0  When block Mode is in use, four coDEC write addresses are configured to be distributed according to specs. If it is greater than 1CH, set the address to be adjacent. At Interleaving Mode, the order in which the CODEC channel writes to SRAM is configured, and it is recommended that it be stored in that order. 0: A 1: B 2: C 3: D */
#define VADBUF_CODEC0_IN_BUF_ADD_BLOCK(x)      ((u32)(((x) & 0x00000003) << 20))
#define VADBUF_GET_CODEC0_IN_BUF_ADD_BLOCK(x)  ((u32)(((x >> 20) & 0x00000003)))
#define VADBUF_BIT_ADC_ANALOG_CLK_SEL          ((u32)0x00000001 << 19)          /*!<R/W 0x0  0: AUDIO CODEC 1: PC_VAD CODEC */
#define VADBUF_BIT_PC_DBGCH_IN_VADBUF_EN       ((u32)0x00000001 << 18)          /*!<R/W 0x0  Vad_pc Debug Channel CODEC data sent to VAD_BUF 2channel, one is data, one is flag, debug mode multiplexes CH0/CH1 write SRAM entry address 0: Disable 1: Enable */
#define VADBUF_BIT_CH3_IN_VADBUF_EN            ((u32)0x00000001 << 13)          /*!<R/W 0x0  Enable VAD CODEC CH3 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VADBUF_BIT_CH2_IN_VADBUF_EN            ((u32)0x00000001 << 12)          /*!<R/W 0x0  Enable VAD CODEC CH2 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VADBUF_BIT_CH1_IN_VADBUF_EN            ((u32)0x00000001 << 11)          /*!<R/W 0x0  Enable VAD CODEC C1 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VADBUF_BIT_CH0_IN_VADBUF_EN            ((u32)0x00000001 << 10)          /*!<R/W 0x0  Enable VAD CODEC CH0 data to be sent to VAD BUF 0: Disable 1: Enable */
#define VADBUF_BIT_VAD_BUF_CTRL_INPUT_SEL      ((u32)0x00000001 << 9)          /*!<R/W 0x1  The data source used to select VAD_BUF_CTRL is used to switch CLK of writing SRAM, 4M at VAD_PC and 40M at VAD_BT through this bit. 0: VAD_PC 1: VAD_BT */
#define VADBUF_MASK_CH_W_VAD_ASSERT            ((u32)0x00000003 << 5)          /*!<R 0x0  The writing channel number when VAD asserts, this status bit is used to record the channel number being written by VAD_BUF_CTRL while VAD is valid. 0: CH0 1: CH1 2: CH2 3: CH3 */
#define VADBUF_CH_W_VAD_ASSERT(x)              ((u32)(((x) & 0x00000003) << 5))
#define VADBUF_GET_CH_W_VAD_ASSERT(x)          ((u32)(((x >> 5) & 0x00000003)))
#define VADBUF_BIT_VAD_W_MODE_SEL              ((u32)0x00000001 << 4)          /*!<R/W 0x0  VAD BUF CTRL the way to write VAD BUF 0: Block 1: Interleaving */
#define VADBUF_BIT_VAD_BUF_SHARE_SRAM_ENABLE   ((u32)0x00000001 << 3)          /*!<R/WE 0x0  Whether to occupy the space control bit of SRAM 0: Disable 1: Enable */
#define VADBUF_BIT_VAD_BUF_LOCK                ((u32)0x00000001 << 2)          /*!<R/WE 0x0  0: Not locked 1: Locked Note: When the bit writes 0, the bit[1:0] can be modified during the working process of the system;When the BIT writes 1, the bit[1:0] cannot be modified during the working process of the system. Only when the whole system is powered on can it be modified again. */
#define VADBUF_MASK_VAD_W_SRAM_ADDRESS_SEL     ((u32)0x00000003 << 0)          /*!<R/WE 0x0  00: 32KB 01: 64KB 10: 96KB 11: 128KB */
#define VADBUF_VAD_W_SRAM_ADDRESS_SEL(x)       ((u32)(((x) & 0x00000003) << 0))
#define VADBUF_GET_VAD_W_SRAM_ADDRESS_SEL(x)   ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup VADBUF_VAD_ASSERT_W_SRAM_ADDRSS
 * @brief
 * @{
 **/
#define VADBUF_MASK_VAD_ASSERT_W_SRAM_ADDRSS   ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  This address is the current address for writing SRAM when VAD asserts */
#define VADBUF_VAD_ASSERT_W_SRAM_ADDRSS(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define VADBUF_GET_VAD_ASSERT_W_SRAM_ADDRSS(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup VADBUF_VAD_SECURE_SEL
 * @brief
 * @{
 **/
#define VADBUF_BIT_VAD_SECURITY_SEL            ((u32)0x00000001 << 0)          /*!<R/W 0x1  Security select for register VAD_BUF_CTRL0 0: Non-secure 1: Secure */
/** @} */

/** @defgroup VADBUF_BUF_CTRL_W_ADDRESS
 * @brief
 * @{
 **/
#define VADBUF_MASK_BUF_CTRL_W_ADDRESS         ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Record VAD_BUF_CTRL write SRAM address. When KM0/KM4 reads VAD_BUF data, refer to this address to avoid reading the wrong data */
#define VADBUF_BUF_CTRL_W_ADDRESS(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define VADBUF_GET_BUF_CTRL_W_ADDRESS(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup VADBUF_AUDIO_TEST
 * @brief
 * @{
 **/
#define VADBUF_MASK_AUDIO_TEST_SUBMOD          ((u32)0x00000003 << 0)          /*!<R/W 0x0  00: audio ADC output test 01: audio DAC input test 1x: audio codec test */
#define VADBUF_AUDIO_TEST_SUBMOD(x)            ((u32)(((x) & 0x00000003) << 0))
#define VADBUF_GET_AUDIO_TEST_SUBMOD(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup VADBUF_Exported_Types VADBUF Exported Types
  * @{
*****************************************************************************/


/**
 * @brief VADBUF Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t VADBUF_VAD_BUF_CTRL0;                   /*!<  Register,  Address offset: 0x50 */
	__I  uint32_t VADBUF_VAD_ASSERT_W_SRAM_ADDRSS;        /*!<  Register,  Address offset: 0x54 */
	__IO uint32_t VADBUF_VAD_SECURE_SEL;                  /*!<  Register,  Address offset: 0x58 */
	__I  uint32_t VADBUF_BUF_CTRL_W_ADDRESS;              /*!<  Register,  Address offset: 0x5C */
	__IO uint32_t VADBUF_AUDIO_TEST;                      /*!<  Register,  Address offset: 0x60 */
} VADBUF_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

/* Exported constants --------------------------------------------------------*/
/** @defgroup VADBUF_Exported_Constants VADBUF Exported Constants
  * @{
  */

/** @defgroup VAD_Reg_Base_Addr
  * @{
  */
#define VAD_REG_BASE ((VADBUF_TypeDef *) (0x41022000 + 0x50))
#define VAD_REG_BASE_S ((VADBUF_TypeDef *) (0x51022000 + 0x50))
/**
  * @}
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

/** @defgroup VAD_Interrupt_Type
  * @{
  */
#define VAD_INTERRUPT_FALLING ((u32)0x00000000)
#define VAD_INTERRUPT_RISING ((u32)0x00000001)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup VADBUF_Exported_Functions VADBUF Exported Functions
  * @{
  */


_LONG_CALL_ void AUDIO_CODEC_VADStart(void);
_LONG_CALL_ void AUDIO_CODEC_SelVADSrc(u32 codec_index);
_LONG_CALL_ void AUDIO_CODEC_ENVADIRQ(u32 mode);
_LONG_CALL_ void AUDIO_CODEC_DISVADIRQ(u32 mode);
_LONG_CALL_ void AUDIO_CODEC_SetVADDetThr(u32 threshold);
_LONG_CALL_ void AUDIO_CODEC_SetVADVoice(u32 voice_level);
_LONG_CALL_ void AUDIO_CODEC_VADBuf_move_one(u32 codec_index, u32 block_index, u32 transfer_mode);
_LONG_CALL_ void AUDIO_CODEC_VADBuf_move_two(u32 codec_index_0, u32 codec_index_1, u32 block_index, u32 transfer_mode);
_LONG_CALL_ void AUDIO_CODEC_VADBuf_move_three(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 block_index, u32 transfer_mode);
_LONG_CALL_ void AUDIO_CODEC_VADBuf_move_four(u32 codec_index_0, u32 codec_index_1, u32 codec_index_2, u32 codec_index_3, u32 block_index, u32 transfer_mode);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
