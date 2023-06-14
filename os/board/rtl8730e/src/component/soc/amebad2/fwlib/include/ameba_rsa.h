#ifndef _AMEBAD2_RSA_H_
#define _AMEBAD2_RSA_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup RSA
  * @brief RSA driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup RSA_Register_Definitions RSA Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup MODE_CTRL
 * @brief Mode Control Register
 * @{
 **/
#define RSA_BIT_MODE_CTRL_ENDIAN         ((u32)0x00000001 << 4)          /*!<RW 0x0  0x0: little 0x1: big */
#define RSA_BIT_MODE_CTRL_BYTE_SWAP      ((u32)0x00000001 << 3)          /*!<RW 0x0  Byte swapping in a word 0x0: little 0x1: big */
#define RSA_MASK_MODE_CTRL_KEY_SIZE_SEL  ((u32)0x00000007 << 0)          /*!<RW 0x0  0x0: 2048 0x2: 1024 0x5: 3072 */
#define RSA_MODE_CTRL_KEY_SIZE_SEL(x)    ((u32)(((x) & 0x00000007) << 0))
#define RSA_GET_MODE_CTRL_KEY_SIZE_SEL(x) ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup START_INT_CTRL
 * @brief Start Interrupt Control Register
 * @{
 **/
#define RSA_BIT_START_INT_CTRL_GO        ((u32)0x00000001 << 31)          /*!<RW 0x0  Write 1 to start engine */
#define RSA_BIT_START_INT_CTRL_INT_EN    ((u32)0x00000001 << 1)          /*!<RW 0x0  Interrupt enable */
#define RSA_BIT_START_INT_CTRL_INT_CLR   ((u32)0x00000001 << 0)          /*!<RW 0x0  0x0: no interrupt 0x1: interrupt active Write 1 to clear interrupt */
/** @} */

/** @defgroup STATUS
 * @brief Status Register
 * @{
 **/
#define RSA_BIT_STATUS_NPINV_ERROR       ((u32)0x00000001 << 12)          /*!<RO 0x0  NPINV generate error (modular is not odd) */
#define RSA_BIT_STATUS_M_RANGE_ERROR     ((u32)0x00000001 << 7)          /*!<RO 0x0  M > N error */
#define RSA_BIT_STATUS_EXP_ERROR         ((u32)0x00000001 << 6)          /*!<RO 0x0  Exponent error (exponent is all zero) */
#define RSA_BIT_STATUS_RSA_FINISH        ((u32)0x00000001 << 4)          /*!<RO 0x0  RSA finish */
/** @} */

/** @defgroup CLK_GATING_CTRL
 * @brief Clock Gating Control Register
 * @{
 **/
#define RSA_BIT_LCLK_GATING_CTRL_EN      ((u32)0x00000001 << 0)          /*!<RW 0x1  Auto clock gating enable */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup RSA_Exported_Types RSA Exported Types
  * @{
*****************************************************************************/


/**
 * @brief RSA Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t MODE_CTRL;                              /*!< MODE CONTROL REGISTER,  Address offset: 0xF00 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xF04 */
	__IO uint32_t START_INT_CTRL;                         /*!< START INTERRUPT CONTROL REGISTER,  Address offset: 0xF08 */
	__I  uint32_t STATUS;                                 /*!< STATUS REGISTER,  Address offset: 0xF0C */
	__IO uint32_t RSVD1[7];                               /*!<  Reserved,  Address offset:0xF10-0xF28 */
	__IO uint32_t CLK_GATING_CTRL;                        /*!< CLOCK GATING CONTROL REGISTER,  Address offset: 0xF2C */
} RSA_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup RSA_Exported_Types RSA Exported Types
 * @{
 */

/**
 * @brief RSA MEM & RSA REG Declaration
 */
typedef struct {
	__IO uint32_t M[96];			/*!< Message mem,					Address offset: 0x0 */
	__IO uint32_t R[96];			/*!< Result mem,					Address offset: 0x180 */
	uint32_t RSVD0[96];
	__IO uint32_t N[96];			/*!< Modular value,					Address offset: 0x480 */
	__IO uint32_t E[96];			/*!< Exponent value,				Address offset: 0x600 */

} RSA_MEM_TypeDef;
/** @} */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RSA_Exported_Constants RSA Exported Constants
 * @{
 */

/** @defgroup RSA_Error_Code
  * @{
 */
#define RSA_ERR_BAD_INPUT_BASE						-0x4080  /**< Bad input parameters of rsa base. */
#define RSA_ERR_BAD_KEYSIZE							-0x4100  /**< keysize not supported. */
#define RSA_ERR_TIMEOUT								-0x4180  /**< rsa calculate timeout. */
#define RSA_ERR_CALCULATE_ERROR						-0x4200  /**< Something failed during calculation. */
#define RSA_ERR_OUTLEN_OUTOFRANGE					-0x4280  /**< out buf out of range. */
/**
  * @}
  */

/** @defgroup RSA_Key_Size
  * @{
  */
#define RSA_KEY_SIZE_2048		((u32)0x00000000)
#define RSA_KEY_SIZE_1024		((u32)0x00000002)
#define RSA_KEY_SIZE_3072		((u32)0x00000005)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RSA_Exported_Functions RSA Exported Functions
 * @{
 */

_LONG_CALL_ int RSA_HW_EXP_MOD(RSA_TypeDef *RSA, char *output, u32 outlen, char *M, u32 MLen, char *Key, u32 KLen, char *N, u32 NLen);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
