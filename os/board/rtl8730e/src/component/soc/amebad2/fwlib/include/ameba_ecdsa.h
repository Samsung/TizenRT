#ifndef _AMEBAD2_ECDSA_H_
#define _AMEBAD2_ECDSA_H_

#include "mbedtls/bignum.h"
#include "mbedtls/ecp.h"

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup ECDSA
  * @brief ECDSA driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup ECDSA_Register_Definitions ECDSA Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup ECR1
 * @brief ECDSA Control Register 1
 * @{
 **/
#define ECDSA_BIT_REG_ENG_CLK_ENABLE      ((u32)0x00000001 << 8)          /*!<R/W 1'b1  ECDSA engine clock enable signal. */
#define ECDSA_BIT_REG_ENG_RST             ((u32)0x00000001 << 0)          /*!<R/W 1'b0  ECDSA reset. Used to reset flip-flop that are clocked by the eng_clk. */
/** @} */

/** @defgroup ESCR
 * @brief ECDSA Swap Control Register
 * @{
 **/
#define ECDSA_BIT_REG_SIGN_S_SWAP         ((u32)0x00000001 << 13)          /*!<R/W 1'b0  Byte swap for eng_sign_s */
#define ECDSA_BIT_REG_SIGN_R_SWAP         ((u32)0x00000001 << 12)          /*!<R/W 1'b0  Byte swap for eng_sign_r */
#define ECDSA_BIT_REG_ORDER_N_SWAP        ((u32)0x00000001 << 11)          /*!<R/W 1'b0  Byte swap for order_n */
#define ECDSA_BIT_REG_H_SWAP              ((u32)0x00000001 << 10)          /*!<R/W 1'b0  Byte swap for eng_h */
#define ECDSA_BIT_REG_PRIME_SWAP          ((u32)0x00000001 << 9)          /*!<R/W 1'b0  Byte swap for prime */
#define ECDSA_BIT_REG_COR_A_SWAP          ((u32)0x00000001 << 8)          /*!<R/W 1'b0  Byte swap for cor_a */
#define ECDSA_BIT_REG_YO_SWAP             ((u32)0x00000001 << 7)          /*!<R/W 1'b0  Byte swap for eng_yo */
#define ECDSA_BIT_REG_XO_SWAP             ((u32)0x00000001 << 6)          /*!<R/W 1'b0  Byte swap for eng_xo */
#define ECDSA_BIT_REG_VERIFY_S_SWAP       ((u32)0x00000001 << 5)          /*!<R/W 1'b0  Byte swap for eng_verify_s */
#define ECDSA_BIT_REG_VERIFY_R_SWAP       ((u32)0x00000001 << 4)          /*!<R/W 1'b0  Byte swap for eng_verify_r */
#define ECDSA_BIT_REG_GY_SWAP             ((u32)0x00000001 << 3)          /*!<R/W 1'b0  Byte swap for eng_gy */
#define ECDSA_BIT_REG_GX_SWAP             ((u32)0x00000001 << 2)          /*!<R/W 1'b0  Byte swap for eng_gx */
#define ECDSA_BIT_REG_Y_SWAP              ((u32)0x00000001 << 1)          /*!<R/W 1'b0  Byte swap for eng_y */
#define ECDSA_BIT_REG_X_SWAP              ((u32)0x00000001 << 0)          /*!<R/W 1'b0  Byte swap for eng_x */
/** @} */

/** @defgroup EISR
 * @brief ECDSA Interrupt Status Register
 * @{
 **/
#define ECDSA_BIT_INFINITY_ERR            ((u32)0x00000001 << 17)          /*!<R 1'b0  This bit indicates that the output coordinates are at infinity. */
#define ECDSA_BIT_SHARE_KEY_ERR           ((u32)0x00000001 << 16)          /*!<R 1'b0  When secret_key < 1, or secret_key > (order_n-1), the bit is set to 1. */
#define ECDSA_MASK_SIGN_ERR               ((u32)0x0000000F << 12)          /*!<R 4'b0  Error message description 0001: private <1 or private > (order_n-1) 0010: (random_k % order_n) == 0 0100: r == 0 1000: s == 0 */
#define ECDSA_SIGN_ERR(x)                 ((u32)(((x) & 0x0000000F) << 12))
#define ECDSA_GET_SIGN_ERR(x)             ((u32)(((x >> 12) & 0x0000000F)))
#define ECDSA_MASK_VERIFY_ERR             ((u32)0x00000003 << 10)          /*!<R 2'b0  Error message description 01: R < 1, or R > (order_n-1) 10: S < 1, or S > (order_n-1) */
#define ECDSA_VERIFY_ERR(x)               ((u32)(((x) & 0x00000003) << 10))
#define ECDSA_GET_VERIFY_ERR(x)           ((u32)(((x >> 10) & 0x00000003)))
#define ECDSA_BIT_VERIFY_PASS             ((u32)0x00000001 << 9)          /*!<R 1'b0  Verification is completed successfully. */
#define ECDSA_BIT_FINISH_INT              ((u32)0x00000001 << 8)          /*!<R/W 1'b0  ECDSA interrupt status. This bit is cleaned by writing 1. If ecdsa_finish_int_mask is configured to 0, it only masks ecdsa_int trigger, ecdsa_finish_int still holds the status. */
#define ECDSA_BIT_ENG_IDLE                ((u32)0x00000001 << 4)          /*!<R 1'b1  When the ECDSA is idle, the bit is set to 1. */
#define ECDSA_BIT_HASH_OK                 ((u32)0x00000001 << 2)          /*!<R/W 1'b0  After the calculation of SHA (m) is completed, this bit will be set to 1 by the software. */
#define ECDSA_MASK_ENG_START              ((u32)0x00000003 << 0)          /*!<W 2'b0  When this bit is set to 1, ECDSA will start the operation according to the corresponding register. */
#define ECDSA_ENG_START(x)                ((u32)(((x) & 0x00000003) << 0))
#define ECDSA_GET_ENG_START(x)            ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup ECR2
 * @brief ECDSA Control Register 2
 * @{
 **/
#define ECDSA_BIT_EXTEND_N                ((u32)0x00000001 << 14)          /*!<R/W 1'b0  When this bit is set to 1, order_n will extend out of 8 bits. 001: 232-bit 010: 200-bit 011: 168-bit 100: 126-bit 101: 120-bit Others: 256-bit */
#define ECDSA_MASK_PRI_KEY_SEL            ((u32)0x00000003 << 12)          /*!<R/W 2'b0  Reserved bits. */
#define ECDSA_PRI_KEY_SEL(x)              ((u32)(((x) & 0x00000003) << 12))
#define ECDSA_GET_PRI_KEY_SEL(x)          ((u32)(((x >> 12) & 0x00000003)))
#define ECDSA_BIT_HASH256_EN              ((u32)0x00000001 << 11)          /*!<R/W 1'b0  If the length of the hash message exceeds 128-bit, use this function to speed up the operation. */
#define ECDSA_MASK_NUM                    ((u32)0x00000007 << 8)          /*!<R/W 3'b0  Select bit_num for correspoinding bit length, when "exten_n" is set to 0. 001: 224-bit 010: 192-bit 011: 160-bit 100: 128-bit 101: 112-bit Others: 256-bit */
#define ECDSA_NUM(x)                      ((u32)(((x) & 0x00000007) << 8))
#define ECDSA_GET_NUM(x)                  ((u32)(((x >> 8) & 0x00000007)))
#define ECDSA_BIT_FINISH_INT_MASK         ((u32)0x00000001 << 7)          /*!<R/W 1'b0  Mask ECDSA engine interrupt. When the ecdsa_finish_int = 1'b1, if ecdsa_finish_int_mask = 1'b1, then the bit masks ecdsa_int. It means that ecdsa_int = 1'b0, but the ecdsa_finish_int status still holds the value(1'b1) in EISR register. */
#define ECDSA_MASK_REG_FUNC_SEL           ((u32)0x00000007 << 4)          /*!<R/W 3'b0  The bit is used to select the operation in basic function mode. The bits are valid when reg_mode_sel[1:0] is set to 2'b11. 000: Field subtraction 001: Field division 010: Field addition 011: Field multiplication 1xx: Point addition */
#define ECDSA_REG_FUNC_SEL(x)             ((u32)(((x) & 0x00000007) << 4))
#define ECDSA_GET_REG_FUNC_SEL(x)         ((u32)(((x >> 4) & 0x00000007)))
#define ECDSA_MASK_REG_MODE_SEL           ((u32)0x00000003 << 2)          /*!<R/W 2'b0  Select operating mode 00: Verification 01: Signature 10: Scalar multiplication 11: Basic function */
#define ECDSA_REG_MODE_SEL(x)             ((u32)(((x) & 0x00000003) << 2))
#define ECDSA_GET_REG_MODE_SEL(x)         ((u32)(((x >> 2) & 0x00000003)))
#define ECDSA_MASK_CURVE_SEL              ((u32)0x00000003 << 0)          /*!<R/W 2'b0  Select the ECC curve 00: secp256k1 01: P-256 10: Curve25519 Weierstrass 11: Others */
#define ECDSA_CURVE_SEL(x)                ((u32)(((x) & 0x00000003) << 0))
#define ECDSA_GET_CURVE_SEL(x)            ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup eng_x_px
 * @brief ENG_X_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_X_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Date register Verification mode: Public key x Signature mode: Private key Scalar multiplication mode: Base point x Basic function mode: Operand */
#define ECDSA_ENG_X_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_X_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_y_px
 * @brief ENG_Y_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_Y_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Date register Verification mode: Public key y Signature mode: Random k Scalar multiplication mode: Base point y Basic function mode: Operand */
#define ECDSA_ENG_Y_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_Y_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_gx_psx
 * @brief ENG_GX_PS Register x
 * @{
 **/
#define ECDSA_MASK_ENG_GX_PSx             ((u32)0xFFFFFFFF << 0)          /*!<R/W 16f8_1798  Because the default cur_sel[1:0] = 2'd0 (secp256k1), so these register initial values are secp256k1 setting values. Verification mode: Base point x Signature mode: Base point x Scalar multiplication mode: Base point x Basic function mode: */
#define ECDSA_ENG_GX_PSx(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_GX_PSx(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_gy_psx
 * @brief ENG_GY_PS Register x
 * @{
 **/
#define ECDSA_MASK_ENG_GY_PSx             ((u32)0xFFFFFFFF << 0)          /*!<R/W fb10_d4b8  Because the default cur_sel[1:0] = 2'd0 (secp256k1), so these register initial values are secp256k1 setting values. Verification mode: Base point y Signature mode: Base point y Scalar multiplication mode: Base point y Basic function mode: */
#define ECDSA_ENG_GY_PSx(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_GY_PSx(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_xo_px
 * @brief ENG_XO_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_XO_Px              ((u32)0xFFFFFFFF << 0)          /*!<R 0  Date register Verification mode: R_x Signature mode: Public x Scalar multiplication mode: Public x Basic function mode: Coordinate x */
#define ECDSA_ENG_XO_Px(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_XO_Px(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_yo_px
 * @brief ENG_YO_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_YO_Px              ((u32)0xFFFFFFFF << 0)          /*!<R 0  Date register Verification mode: R_y Signature mode: Public y Scalar multiplication mode: Public y Basic function mode: Coordinate y */
#define ECDSA_ENG_YO_Px(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_YO_Px(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_sign_r_sx
 * @brief ENG_SIGN_R_S Register x
 * @{
 **/
#define ECDSA_MASK_ENG_SIGN_R_Sx          ((u32)0xFFFFFFFF << 0)          /*!<R 0  Date register Verification mode: Signature mode: R Scalar multiplication mode: Basic function mode: */
#define ECDSA_ENG_SIGN_R_Sx(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_SIGN_R_Sx(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_sign_s_sx
 * @brief ENG_SIGN_S_S Register x
 * @{
 **/
#define ECDSA_MASK_ENG_SIGN_S_Sx          ((u32)0xFFFFFFFF << 0)          /*!<R 0  Date register Verification mode: Signature mode: S Scalar multiplication mode: Basic function mode: */
#define ECDSA_ENG_SIGN_S_Sx(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_SIGN_S_Sx(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_verify_r_px
 * @brief ENG_VERIFY_R_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_VERIFY_R_Px        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Date register Verification mode: R Signature mode: Scalar multiplication mode: Basic function mode: */
#define ECDSA_ENG_VERIFY_R_Px(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_VERIFY_R_Px(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_verify_s_px
 * @brief ENG_VERIFY_S_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_VERIFY_S_Px        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Date register Verification mode: S Signature mode: Scalar multiplication mode: Basic function mode: */
#define ECDSA_ENG_VERIFY_S_Px(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_VERIFY_S_Px(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup eng_h_px
 * @brief ENG_H_P Register x
 * @{
 **/
#define ECDSA_MASK_ENG_H_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  ECDSA "hash" parameter Verification mode: Hash Signature mode: Hash Scalar multiplication mode: Private key Basic function mode: */
#define ECDSA_ENG_H_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ENG_H_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup cor_a_px
 * @brief COR_A_P Register x
 * @{
 **/
#define ECDSA_MASK_COR_A_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  ECDSA "a" parameter Verification mode: cor_a Signature mode: cor_a Scalar multiplication mode: cor_a Basic function mode: cor_a */
#define ECDSA_COR_A_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_COR_A_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup prime_px
 * @brief Prime_P Register x
 * @{
 **/
#define ECDSA_MASK_PRIME_Px               ((u32)0xFFFFFFFF << 0)          /*!<R/W   Because the default cur_sel[1:0] = 2'd0 (secp256k1), so these registers‘ initial values are secp256k1 setting values. ECDSA "prime" parameter Verification mode: Prime Signature mode: Prime Scalar multiplication mode: Prime Basic function mode: Prime */
#define ECDSA_PRIME_Px(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_PRIME_Px(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup order_n_px
 * @brief Order_N_P Register x
 * @{
 **/
#define ECDSA_MASK_ORDER_N_Px             ((u32)0xFFFFFFFF << 0)          /*!<R/W   Because the default cur_sel[1:0] = 2'd0 (secp256k1), so these registers‘ initial values are secp256k1 setting values. ECDSA "prime" parameter Verification mode: order_n Signature mode: order_n Scalar multiplication mode: order_n Basic function mode: order_n */
#define ECDSA_ORDER_N_Px(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_ORDER_N_Px(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ecdsa_dbg_o
 * @brief ECDSA Debug Output Register
 * @{
 **/
#define ECDSA_MASK_DBG_O                  ((u32)0xFFFFFFFF << 0)          /*!<R 32'd0  Debug singnals */
#define ECDSA_DBG_O(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ECDSA_GET_DBG_O(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup ECDSA_Exported_Types ECDSA Exported Types
  * @{
*****************************************************************************/


/**
 * @brief ECDSA Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t ECR1;                                   /*!< ECDSA CONTROL REGISTER 1 Register,  Address offset: 0x000 */
	__IO uint32_t ESCR;                                   /*!< ECDSA SWAP CONTROL REGISTER,  Address offset: 0x004 */
	__IO uint32_t EISR;                                   /*!< ECDSA INTERRUPT STATUS REGISTER,  Address offset: 0x008 */
	__IO uint32_t ECR2;                                   /*!< ECDSA CONTROL REGISTER 2 Register,  Address offset: 0x00C */
	__IO uint32_t ENG_X_Px[8];                            /*!< ENG_X_P REGISTER x Register,  Address offset: 0x010-0x02C */
	__IO uint32_t ENG_Y_Px[8];                            /*!< ENG_Y_P REGISTER x Register,  Address offset: 0x030-0x04C */
	__IO uint32_t ENG_GX_PSx[8];                          /*!< ENG_GX_PS REGISTER x Register,  Address offset: 0x050-0x06C */
	__IO uint32_t ENG_GY_PSx[8];                          /*!< ENG_GY_PS REGISTER x Register,  Address offset: 0x070-0x08C */
	__I  uint32_t ENG_XO_Px[8];                           /*!< ENG_XO_P REGISTER x Register,  Address offset: 0x090-0x0AC */
	__I  uint32_t ENG_YO_Px[8];                           /*!< ENG_YO_P REGISTER x Register,  Address offset: 0x0B0-0x0CC */
	__I  uint32_t ENG_SIGN_R_Sx[8];                       /*!< ENG_SIGN_R_S REGISTER x Register,  Address offset: 0x0D0-0x0EC */
	__I  uint32_t ENG_SIGN_S_Sx[8];                       /*!< ENG_SIGN_S_S REGISTER x Register,  Address offset: 0x0F0-0x10C */
	__IO uint32_t ENG_VERIFY_R_Px[8];                     /*!< ENG_VERIFY_R_P REGISTER x Register,  Address offset: 0x110-0x12C */
	__IO uint32_t ENG_VERIFY_S_Px[8];                     /*!< ENG_VERIFY_S_P REGISTER x Register,  Address offset: 0x130-0x14C */
	__IO uint32_t ENG_H_Px[8];                            /*!< ENG_H_P REGISTER x Register,  Address offset: 0x150-0x16C */
	__IO uint32_t COR_A_Px[8];                            /*!< COR_A_P REGISTER x Register,  Address offset: 0x170-0x18C */
	__IO uint32_t PRIME_Px[8];                            /*!< PRIME_P REGISTER x Register,  Address offset: 0x190-0x1AC */
	__IO uint32_t ORDER_N_Px[8];                          /*!< ORDER_N_P REGISTER x Register,  Address offset: 0x1B0-0x1CC */
	__IO uint32_t RSVD0[12];                              /*!<  Reserved,  Address offset:0x1D0-0x1FC */
	__I  uint32_t DBG_O;                                  /*!< ECDSA DEBUG OUTPUT REGISTER,  Address offset: 0x200 */
} ECDSA_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup ECDSA_Exported_Types ECDSA Exported Types
  * @{
  */

/**
 * @brief ECDSA Curve Structure Definition
 */
typedef struct {
	u32 curvetype;
	u32 *BasePointx;
	u32 *BasePointy;
	u32 *a;
	u32 *P;
	u32 *N;
	u32 *pubx;
	u32 *puby;
	u32 *pri;
	u32 *r;
	u32 *s;
} Curve_Typedefine;

/**
 * @brief ECDSA ECP Group Structure Definition
 */
typedef struct {
	u32 curve_id;	/*!<  internal group identifier		*/
	u32 nbits;		/*!<  bits of N						*/
	u32 curve_bit;	/*!<  curve bits					*/
	u32 *N;			/*!<  N buf pointer					*/
} ecdsa_ecp_group;

/**
 * @brief ECDSA ECP Group ID Definition
 */
typedef enum {
	ECDSA_ECP_NONE = 0,
	ECDSA_ECP_SECP192R1 = 1,      /*!< 192-bits NIST curve  */
	ECDSA_ECP_SECP224R1 = 2,      /*!< 224-bits NIST curve  */
	ECDSA_ECP_SECP256R1 = 3,      /*!< 256-bits NIST curve  */
	ECDSA_ECP_BP256R1 = 6,        /*!< 256-bits Brainpool curve */
	ECDSA_ECP_CURVE25519 = 9,     /*!< Curve25519               */
	ECDSA_ECP_SECP192K1 = 10,     /*!< 192-bits "Koblitz" curve */
	ECDSA_ECP_SECP224K1 = 11,     /*!< 224-bits "Koblitz" curve */
	ECDSA_ECP_SECP256K1 = 12,     /*!< 256-bits "Koblitz" curve */
} ecdsa_ecp_group_id;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup ECDSA_Exported_Constants ECDSA Exported Constants
  * @{
  */

/** @defgroup ECDSA_Curve_Type
  * @{
  */
#define ECDSA_ENG_SECP256K1		0x0
#define ECDSA_ENG_P256			0x1
#define ECDSA_ENG_CURVE25519	0x2
#define ECDSA_ENG_OTHERS		0x3
/**
  * @}
  */

/** @defgroup ECDSA_Mode
  * @{
  */
#define ECDSA_ENG_MODE0		0x0
#define ECDSA_ENG_MODE1		0x1
#define ECDSA_ENG_MODE2		0x2
#define ECDSA_ENG_MODE3		0x3
/**
  * @}
  */

/** @defgroup ECDSA_Basic_Function
  * @{
  */
#define ECDSA_ENG_SUB		0x0
#define ECDSA_ENG_DIV		0x1		/* note: Modular Inverse Algorithm */
#define ECDSA_ENG_ADD		0x2
#define ECDSA_ENG_MUL		0x3
/**
  * @}
  */

/** @defgroup ECDSA_Error_Code
  * @{
  */
#define ECDSA_ERR_CURVE_NOT_SUPPORTED				-0x1F80  /**< curve not supported in ecdsa engine. */
#define ECDSA_ERR_KEYGEN_TIMEOUT					-0x1F00  /**< timeout in keygen. */
#define ECDSA_ERR_SIGN_TIMEOUT						-0x1E80  /**< timeout in sign. */
#define ECDSA_ERR_SIGN_ERROR                     	-0x1E00  /**< error occur in sign. */
#define ECDSA_ERR_VERIFY_TIMEOUT					-0x1D80  /**< timeout in verify. */
#define ECDSA_ERR_VERIFY_ERROR						-0x1D00  /**< error occur in verify. */
#define ECDSA_ERR_VERIFY_FAIL						-0x1C80  /**< verify fail. */
#define ECDSA_ERR_OTP_KEYNUM						-0x1C00  /**< error otp key num. */
#define ECDSA_ERR_BASIC_FUNC_TIMEOUT				-0x1B80  /**< timeout in basic function. */
#define ECDSA_ERR_BASIC_FUNC_ERROR					-0x1B00  /**< error occur in basic function. */
#define ECDSA_ERR_BASIC_FUNC_ERROR_PARA				-0x1A80  /**< paramater larger than 256k1 P  in basic function. */
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup ECDSA_Exported_Functions ECDSA Exported Functions
  * @{
  */

_LONG_CALL_ int ECDSA_CurveSel(ECDSA_TypeDef *ECDSA, ecdsa_ecp_group *curve);
_LONG_CALL_ void ECDSA_EngRst(ECDSA_TypeDef *ECDSA);
_LONG_CALL_ void ECDSA_Cmd(ECDSA_TypeDef *ECDSA, u8 NewState);
_LONG_CALL_ void ECDSA_BufSub(u32 n, u32 *s, u32 *d);
_LONG_CALL_ int ECDSA_BufCmp(u32 *X, u32 sizeofx, u32 *Y);
_LONG_CALL_ int ECDSA_HashBuf_Setting(ecdsa_ecp_group *curve, u32 *x, const u8 *buf, u32 blen);
_LONG_CALL_ int ECDSA_KeyGen(ECDSA_TypeDef *ECDSA, u32 curve_id, u8 *d, u8 *Qx, u8 *Qy, u32 OTPKey);
_LONG_CALL_ int ECDSA_Sign(ECDSA_TypeDef *ECDSA, u32 curve_id, u8 *d, u8 *k, const u8 *hbuf, u32 hlen, u8 *Qx, u8 *Qy, u8 *r, u8 *s, u32 OTPKey);
_LONG_CALL_ int ECDSA_Verify(ECDSA_TypeDef *ECDSA, u32 curve_id, const u8 *hbuf, u32 hlen, u8 *Qx, u8 *Qy, u8 *r, u8 *s);
_LONG_CALL_ int ECDSA_ECDH_Compute_Shared(ECDSA_TypeDef *ECDSA, u32 curve_id, u8 *zx, u8 *zy, u8 *Qx, u8 *Qy, u8 *d, u32 OTPKey);
_LONG_CALL_ int ECDSA_Basic_Func(ECDSA_TypeDef *ECDSA,  u32 X[8], u32 Y[8], u8 R[32], int mode);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
