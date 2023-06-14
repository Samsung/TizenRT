#ifndef _AMEBAD2_EDDSA_H_
#define _AMEBAD2_EDDSA_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup EDDSA
  * @brief EDDSA driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup EDDSA_Register_Definitions EDDSA Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup ENG_INITR
 * @brief EdDSA Engine Initial Setting Register
 * @{
 **/
#define EDDSA_BIT_ENG_CLK_ENABLE           ((u32)0x00000001 << 8)          /*!<R/W 0  EdDSA engine clock enable signal. */
#define EDDSA_BIT_ENG_RST                  ((u32)0x00000001 << 0)          /*!<R/W 0  EdDSA engine synchronization reset. */
/** @} */

/** @defgroup ENG_CONF_INTR
 * @brief EdDSA Engine Configure Interrupt Register
 * @{
 **/
#define EDDSA_BIT_ENG_DECODE_FAIL          ((u32)0x00000001 << 12)          /*!<R/W 0  Engine decode fail interrupt. Put high to clear fail interrupt. */
#define EDDSA_BIT_MODE3_FINISH             ((u32)0x00000001 << 11)          /*!<R/W 0  Engine mode3 interrupt. Put high to clear mode3 interrupt. */
#define EDDSA_BIT_MODE2_FINISH             ((u32)0x00000001 << 10)          /*!<R/W 0  Engine mode2 interrupt. Put high to clear mode2 interrupt. */
#define EDDSA_BIT_MODE1_FINISH             ((u32)0x00000001 << 9)          /*!<R/W 0  Engine mode1 interrupt. Put high to clear mode1 interrupt. */
#define EDDSA_BIT_MODE0_FINISH             ((u32)0x00000001 << 8)          /*!<R/W 0  Engine mode0 interrupt. Put high to clear mode0 interrupt. */
#define EDDSA_BIT_ENG_IDLE                 ((u32)0x00000001 << 4)          /*!<R/W 0  When engine is busy, this bit will be put high. */
#define EDDSA_BIT_HASH_OK                  ((u32)0x00000001 << 2)          /*!<R/W 0  When Hash digest input value is ready, put hask_ok to high. Only use this bit in some auto-flow setting. When programer set this bit to 1'b1, it means that hash digest input value is ready, engine can contiune processing next auto-flow step. */
#define EDDSA_BIT_ENG_START                ((u32)0x00000001 << 0)          /*!<R/W 0  Start for EdDSA function. It's almost for all EdDSA function, except some auto-flow settings. When programer set this bit to 1'b1, it means that input values of the engine mode are ready, so engine can process the mode. Usually it won't be set with HASH_OK. */
/** @} */

/** @defgroup ENG_CTRLR
 * @brief EdDSA Engine Control Register
 * @{
 **/
#define EDDSA_BIT_YO2_WORD_SWAP            ((u32)0x00000001 << 31)          /*!<R/W 0  Byte swap 1'b1: |7:0 | 15:8 | 23:16 | 31:24| 1'b0: |31:24 | 23:16 | 15:8 |7:0| Word swap 1'b1: | Word0 | Word1 | Word2 | Word3 | Word4 | Word5 | Word6 | Word7 | 1'b0: | Word7 | Word6 | Word5 | Word4 | Word3 | Word2 | Word1 | Word0 | */
#define EDDSA_BIT_YO2_BYTE_SWAP            ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define EDDSA_BIT_XO2_WORD_SWAP            ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define EDDSA_BIT_XO2_BYTE_SWAP            ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define EDDSA_BIT_YO1_WORD_SWAP            ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define EDDSA_BIT_YO1_BYTE_SWAP            ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define EDDSA_BIT_XO1_WORD_SWAP            ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define EDDSA_BIT_XO1_BYTE_SWAP            ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define EDDSA_BIT_Y2_WORD_SWAP             ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define EDDSA_BIT_Y2_BYTE_SWAP             ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define EDDSA_BIT_X2_WORD_SWAP             ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define EDDSA_BIT_X2_BYTE_SWAP             ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define EDDSA_BIT_Y1_WORD_SWAP             ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define EDDSA_BIT_Y1_BYTE_SWAP             ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define EDDSA_BIT_X1_WORD_SWAP             ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define EDDSA_BIT_X1_BYTE_SWAP             ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define EDDSA_BIT_MODE3_F_MASK             ((u32)0x00000001 << 15)          /*!<R/W 0  Mask mode3 finish interrupt */
#define EDDSA_BIT_MODE2_F_MASK             ((u32)0x00000001 << 14)          /*!<R/W 0  Mask mode2 finish interrupt */
#define EDDSA_BIT_MODE1_F_MASK             ((u32)0x00000001 << 13)          /*!<R/W 0  Mask mode1 finish interrupt */
#define EDDSA_BIT_MODE0_F_MASK             ((u32)0x00000001 << 12)          /*!<R/W 0  Mask mode0 finish interrupt */
#define EDDSA_BIT_ENG_DECODE_FAIL_MASK     ((u32)0x00000001 << 11)          /*!<R/W 0  Mask decoding fail interrupt */
#define EDDSA_BIT_RECOVER_X_EN             ((u32)0x00000001 << 10)          /*!<R/W 0  Reocver (decode) point enable contrl. When this bit is set to 1'b1, it means that enable decoding mode. */
#define EDDSA_BIT_SUB_POINT                ((u32)0x00000001 << 9)          /*!<R/W 0  Point subtraction enable control in mode2. */
#define EDDSA_BIT_MOD_Q                    ((u32)0x00000001 << 8)          /*!<R/W 0  Select modular1 or 2 for Hash mod. Modular1: hash[511:0] modular (2^255 - 19) Modular2: hash[511:0] modular (2^252+ 27742317777372353535851937790883648493) */
#define EDDSA_BIT_H512_EN                  ((u32)0x00000001 << 7)          /*!<R/W 0  SHA512 hash enable control. */
#define EDDSA_BIT_BYP_H512                 ((u32)0x00000001 << 6)          /*!<R/W 0  SHA512 hash bypass mode control. */
#define EDDSA_BIT_AUTO_SP                  ((u32)0x00000001 << 5)          /*!<R/W 0  Auto-flow step enable control. Basically, it is used to open one pass flow in mode0/mode1 flow. It won't be set in mode2/mode3. */
#define EDDSA_BIT_AP_EN                    ((u32)0x00000001 << 4)          /*!<R/W 0  Add point enable control. It is only used in mode3. */
#define EDDSA_MASK_FUN_SEL                 ((u32)0x00000003 << 2)          /*!<R/W 0  EdDSA Basic Function selection 2'd0: * Decode point * Add point * Sub mod It depends on ECOVER_X_EN and AP_EN. 2'd1: Div mode 2'd2: Add mode 2'd3: * Mult mod * Hash mod #modular1 * Hash mod #modular2. It depends on H512_EN and MOD_Q. */
#define EDDSA_FUN_SEL(x)                   ((u32)(((x) & 0x00000003) << 2))
#define EDDSA_GET_FUN_SEL(x)               ((u32)(((x >> 2) & 0x00000003)))
#define EDDSA_MASK_MOD_SEL                 ((u32)0x00000003 << 0)          /*!<R/W 0  EdDSA mode selection 2'd0: Mode0 2'd1: Mode1 2'd2: Mode2 2'd3: Mode3 */
#define EDDSA_MOD_SEL(x)                   ((u32)(((x) & 0x00000003) << 0))
#define EDDSA_GET_MOD_SEL(x)               ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup ENG_X1_Px
 * @brief EdDSA Engine X1 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_X1_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  X1 value[31:0] of an input point. */
#define EDDSA_ENG_X1_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_X1_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_Y1_Px
 * @brief EdDSA Engine Y1 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_Y1_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Y1 value[31:0] of an input point. */
#define EDDSA_ENG_Y1_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_Y1_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_X2_Px
 * @brief EdDSA Engine X2 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_X2_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  X2 value[31:0] of an input point. */
#define EDDSA_ENG_X2_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_X2_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_Y2_Px
 * @brief EdDSA Engine Y2 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_Y2_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Y2 value[31:0] of an input point. */
#define EDDSA_ENG_Y2_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_Y2_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_XO1_Px
 * @brief EdDSA Engine X1 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_XO1_Px              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  X1 value[31:0] of an output point. */
#define EDDSA_ENG_XO1_Px(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_XO1_Px(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_YO1_Px
 * @brief EdDSA Engine Y1 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_YO1_Px              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Y1 value[31:0] of an input point. */
#define EDDSA_ENG_YO1_Px(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_YO1_Px(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_XO2_Px
 * @brief EdDSA Engine X2 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_XO2_Px              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  X2 value[31:0] of an output point. */
#define EDDSA_ENG_XO2_Px(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_XO2_Px(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_YO2_Px
 * @brief EdDSA Engine Y2 Point Register x
 * @{
 **/
#define EDDSA_MASK_ENG_YO2_Px              ((u32)0xFFFFFFFF << 0)          /*!<R 0  Y2 value[31:0] of an output point. */
#define EDDSA_ENG_YO2_Px(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define EDDSA_GET_ENG_YO2_Px(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ENG_DBG_OUTPUTR
 * @brief EdDSA Engine Debug Output Register
 * @{
 **/
#define EDDSA_MASK_DBG_DECOMP_CURR_STATE   ((u32)0x0000000F << 28)          /*!<R 0  FSM signal in decoding module */
#define EDDSA_DBG_DECOMP_CURR_STATE(x)     ((u32)(((x) & 0x0000000F) << 28))
#define EDDSA_GET_DBG_DECOMP_CURR_STATE(x) ((u32)(((x >> 28) & 0x0000000F)))
#define EDDSA_BIT_DBG_REG_ENG_RST          ((u32)0x00000001 << 27)          /*!<R 0  Reset signal in top module */
#define EDDSA_BIT_DBG_ENG_FINISH           ((u32)0x00000001 << 26)          /*!<R 0  FSM signal in top module */
#define EDDSA_MASK_DBG_FUNC_CURR_STATE     ((u32)0x0000000F << 22)          /*!<R 0  Function FSM signal in top module */
#define EDDSA_DBG_FUNC_CURR_STATE(x)       ((u32)(((x) & 0x0000000F) << 22))
#define EDDSA_GET_DBG_FUNC_CURR_STATE(x)   ((u32)(((x >> 22) & 0x0000000F)))
#define EDDSA_MASK_DBG_CURR_STATE          ((u32)0x0000000F << 18)          /*!<R 0  FSM signal in top module */
#define EDDSA_DBG_CURR_STATE(x)            ((u32)(((x) & 0x0000000F) << 18))
#define EDDSA_GET_DBG_CURR_STATE(x)        ((u32)(((x >> 18) & 0x0000000F)))
#define EDDSA_BIT_DBG_ADD_1_FINISH         ((u32)0x00000001 << 17)          /*!<R 0  Finish signal in add_1 module */
#define EDDSA_BIT_DBG_ADD_2_FINISH         ((u32)0x00000001 << 16)          /*!<R 0  Finish signal in add_2 module */
#define EDDSA_MASK_DBG_ADD_1_CURR_STATE    ((u32)0x0000000F << 12)          /*!<R 0  FSM signal in add_1module */
#define EDDSA_DBG_ADD_1_CURR_STATE(x)      ((u32)(((x) & 0x0000000F) << 12))
#define EDDSA_GET_DBG_ADD_1_CURR_STATE(x)  ((u32)(((x >> 12) & 0x0000000F)))
#define EDDSA_BIT_DBG_ADD_1_MUL_1_FINISH   ((u32)0x00000001 << 11)          /*!<R 0  mul and inv module finish signal in add_1 module */
#define EDDSA_BIT_DBG_ADD_1_MUL_2_FINISH   ((u32)0x00000001 << 10)          /*!<R 0   */
#define EDDSA_BIT_DBG_ADD_1_INV_1_FINISH   ((u32)0x00000001 << 9)          /*!<R 0   */
#define EDDSA_BIT_DBG_ADD_1_INV_2_FINISH   ((u32)0x00000001 << 8)          /*!<R 0   */
#define EDDSA_MASK_DBG_ADD_2_CURR_STATE    ((u32)0x0000000F << 4)          /*!<R 0  FSM signal in add_2 module */
#define EDDSA_DBG_ADD_2_CURR_STATE(x)      ((u32)(((x) & 0x0000000F) << 4))
#define EDDSA_GET_DBG_ADD_2_CURR_STATE(x)  ((u32)(((x >> 4) & 0x0000000F)))
#define EDDSA_BIT_DBG_ADD_2_MUL_1_FINISH   ((u32)0x00000001 << 3)          /*!<R 0  mul and inv module finish signal in add_2 module */
#define EDDSA_BIT_DBG_ADD_2_MUL_2_FINISH   ((u32)0x00000001 << 2)          /*!<R 0   */
#define EDDSA_BIT_DBG_ADD_2_INV_1_FINISH   ((u32)0x00000001 << 1)          /*!<R 0   */
#define EDDSA_BIT_DBG_ADD_2_INV_2_FINISH   ((u32)0x00000001 << 0)          /*!<R 0   */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup EDDSA_Exported_Types EDDSA Exported Types
  * @{
*****************************************************************************/


/**
 * @brief EDDSA Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t ENG_INITR;                              /*!< EDDSA ENGINE INITIAL SETTING REGISTER,  Address offset: 0x000 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x4 */
	__IO uint32_t ENG_CONF_INTR;                          /*!< EDDSA ENGINE CONFIGURE INTERRUPT REGISTER,  Address offset: 0x008 */
	__IO uint32_t ENG_CTRLR;                              /*!< EDDSA ENGINE CONTROL REGISTER,  Address offset: 0x00C */
	__IO uint32_t ENG_X1_Px[8];                           /*!< EDDSA ENGINE X1 POINT REGISTER x Register,  Address offset: 0x010-0x02C */
	__IO uint32_t ENG_Y1_Px[8];                           /*!< EDDSA ENGINE Y1 POINT REGISTER x Register,  Address offset: 0x030-0x04C */
	__IO uint32_t ENG_X2_Px[8];                           /*!< EDDSA ENGINE X2 POINT REGISTER x Register,  Address offset: 0x050-0x06C */
	__IO uint32_t ENG_Y2_Px[8];                           /*!< EDDSA ENGINE Y2 POINT REGISTER x Register,  Address offset: 0x070-0x08C */
	__IO uint32_t ENG_XO1_Px[8];                          /*!< EDDSA ENGINE X1 POINT REGISTER x Register,  Address offset: 0x090-0x0AC */
	__IO uint32_t ENG_YO1_Px[8];                          /*!< EDDSA ENGINE Y1 POINT REGISTER x Register,  Address offset: 0x0B0-0x0CC */
	__IO uint32_t ENG_XO2_Px[8];                          /*!< EDDSA ENGINE X2 POINT REGISTER x Register,  Address offset: 0x0D0-0x0EC */
	__I  uint32_t ENG_YO2_Px[8];                          /*!< EDDSA ENGINE Y2 POINT REGISTER x Register,  Address offset: 0x0F0-0x10C */
	__IO uint32_t RSVD1[60];                              /*!<  Reserved,  Address offset:0x110-0x1FC */
	__I  uint32_t ENG_DBG_OUTPUTR;                        /*!< EDDSA ENGINE DEBUG OUTPUT REGISTER,  Address offset: 0x200 */
} EDDSA_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported constants --------------------------------------------------------*/
/**
 * @defgroup EDDSA_Exported_Constants EDDSA Exported Constants
 * @{
 */

/** @defgroup EDDSA_Error_Code
  * @{
  */
#define EDDSA_ERR_TIMEOUT_STEP1					-0x2F80  /**< curve not supported in ecdsa engine. */
#define EDDSA_ERR_ERROR_STEP1					-0x2F00  /**< timeout in keygen. */
#define EDDSA_ERR_TIMEOUT_STEP2					-0x2E80  /**< timeout in sign. */
#define EDDSA_ERR_ERROR_STEP2					-0x2E00  /**< error occur in sign. */
#define EDDSA_ERR_TIMEOUT_STEP3					-0x2D80  /**< timeout in verify. */
#define EDDSA_ERR_ERROR_STEP3					-0x2D00  /**< error occur in verify. */
#define EDDSA_ERR_VERIFY_FAIL					-0x2C80  /**< verify fail. */
/**
  * @}
  */

/** @defgroup EDDSA_Engine_Mode
  * @{
  */
#define EDDSA_ENG_MODE0				0
#define EDDSA_ENG_MODE1				1
#define EDDSA_ENG_MODE2				2
#define EDDSA_ENG_MODE3				3
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/**
 * @defgroup EDDSA_Exported_Functions EDDSA Exported Functions
 * @{
 */


_LONG_CALL_ void EDDSA_EngRst(EDDSA_TypeDef *Ed25519);
_LONG_CALL_ void EDDSA_Cmd(EDDSA_TypeDef *Ed25519, u8 NewState);
_LONG_CALL_ void EDDSA_PubRecover_Cmd(EDDSA_TypeDef *Ed25519, u8 NewState);
_LONG_CALL_ void EDDSA_AutoMode(EDDSA_TypeDef *Ed25519, u8 NewState);
_LONG_CALL_ void EDDSA_ModeSel(EDDSA_TypeDef *Ed25519, u32 mode);
_LONG_CALL_ int EDDSA_Verify_StepMode(EDDSA_TypeDef *Ed25519, u8 *pk, u8 *sign, u8 *h);
_LONG_CALL_ int EDDSA_Verify_HalfAutoMode(EDDSA_TypeDef *Ed25519, u8 *pk, u8 *sign, u8 *h);
_LONG_CALL_ int EDDSA_Verify_AutoMode(EDDSA_TypeDef *Ed25519, u8 *pk, u8 *sign, u8 *h);


/**
  * @}
  */
/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
