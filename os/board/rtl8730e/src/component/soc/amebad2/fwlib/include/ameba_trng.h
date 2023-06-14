#ifndef _AMEBAD2_TRNG_H_
#define _AMEBAD2_TRNG_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup TRNG
  * @brief TRNG driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup TRNG_Register_Definitions TRNG Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup TRNG_IER
 * @brief Interrupt Enable Register
 * @{
 **/
#define TRNG_BIT_BUFFER_FULL_INTERRUPT_ENABLE     ((u32)0x00000001 << 4)          /*!<R/W 0x0  RNG buffer full interrupt enable */
#define TRNG_BIT_APT0_FAIL_INTERRUPT_ENABLE       ((u32)0x00000001 << 3)          /*!<R/W 0x1  Adaptive Proportion Test 0 fail interrupt enable */
#define TRNG_BIT_RCT1_FAIL_INTERRUPT_ENABLE       ((u32)0x00000001 << 2)          /*!<R/W 0x1  Repetition Count Test 1 fail interrupt enable */
#define TRNG_BIT_RCT0_FAIL_INTERRUPT_ENABLE       ((u32)0x00000001 << 1)          /*!<R/W 0x1  Repetition Count Test 0 fail interrupt enable */
#define TRNG_BIT_READ_ERROR_INTERRUPT_ENABLE      ((u32)0x00000001 << 0)          /*!<R/W 0x0  Read error interrupt enable. Trigger interrupt when reading RNG data register and RNG valid is not 0x1~0x8. */
/** @} */

/** @defgroup TRNG_ISR
 * @brief Interrupt Status Register
 * @{
 **/
#define TRNG_BIT_BUFFER_FULL_INTERRUPT_ACTIVE     ((u32)0x00000001 << 4)          /*!<RO    */
#define TRNG_BIT_APT0_FAIL_INTERRUPT_ACTIVE       ((u32)0x00000001 << 3)          /*!<RO    */
#define TRNG_BIT_RCT1_FAIL_INTERRUPT_ACTIVE       ((u32)0x00000001 << 2)          /*!<RO    */
#define TRNG_BIT_RCT0_FAIL_INTERRUPT_ACTIVE       ((u32)0x00000001 << 1)          /*!<RO    */
#define TRNG_BIT_READ_ERROR_INTERRUPT_ACTIVE      ((u32)0x00000001 << 0)          /*!<RO    */
/** @} */

/** @defgroup TRNG_ICR
 * @brief Interrupt Clear Register
 * @{
 **/
#define TRNG_BIT_BUFFER_FULL_INTERRUPT_CLEAR      ((u32)0x00000001 << 4)          /*!<WO    */
#define TRNG_BIT_APT0_FAIL_INTERRUPT_CLEAR        ((u32)0x00000001 << 3)          /*!<WO   Clear the this interrupt also clear the error flags of APT0 health tests */
#define TRNG_BIT_RCT1_FAIL_INTERRUPT_CLEAR        ((u32)0x00000001 << 2)          /*!<WO   Clear the this interrupt also clear the error flags of RCT1 health tests */
#define TRNG_BIT_RCT0_FAIL_INTERRUPT_CLEAR        ((u32)0x00000001 << 1)          /*!<WO   Clear the this interrupt also clear the error flags of RCT0 health tests */
#define TRNG_BIT_READ_ERROR_INTERRUPT_CLEAR       ((u32)0x00000001 << 0)          /*!<WO    */
/** @} */

/** @defgroup TRNG_FIFO_STATUS
 * @brief Valid Register
 * @{
 **/
#define TRNG_MASK_RNG_VALID                       ((u32)0x0000000F << 0)          /*!<RO   Encoded status register. 0x0: buffer empty or APT is not done. 0x1~0x8: number of word are valid. 0xF: APT test fail */
#define TRNG_RNG_VALID(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define TRNG_GET_RNG_VALID(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup TRNG_DATAx
 * @brief Data x Register
 * @{
 **/
#define TRNG_MASK_RANDOM_NUMBER                   ((u32)0xFFFFFFFF << 0)          /*!<RO   RNG data register x. Read as 0x0 if RNG valid register is not a ready status. A Read access triggers a buffer pop, so the value is only read once then be discarded. */
#define TRNG_RANDOM_NUMBER(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define TRNG_GET_RANDOM_NUMBER(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup TRNG_STATUS
 * @brief Status Register
 * @{
 **/
#define TRNG_BIT_APT0_FAIL                        ((u32)0x00000001 << 26)          /*!<RO   APT0 error detected */
#define TRNG_BIT_RCT1_FAIL                        ((u32)0x00000001 << 25)          /*!<RO   RCT1 error detected */
#define TRNG_BIT_RCT0_FAIL                        ((u32)0x00000001 << 24)          /*!<RO   RCT0 error detected */
#define TRNG_BIT_APT0_DONE                        ((u32)0x00000001 << 16)          /*!<RO   Adaptive Proportion Test done status. (APT test pass count >= APT done threshold setting) */
#define TRNG_BIT_BUFFER_OVERFLOW                  ((u32)0x00000001 << 8)          /*!<RO   Buffer overflow status. It is clear by RNG read out. */
#define TRNG_MASK_BUFFER_ENTRY_LEVEL              ((u32)0x0000000F << 0)          /*!<RO   Number of 32-bit random number are valid. 0: empty 8: full */
#define TRNG_BUFFER_ENTRY_LEVEL(x)                ((u32)(((x) & 0x0000000F) << 0))
#define TRNG_GET_BUFFER_ENTRY_LEVEL(x)            ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup TRNG_CTRL
 * @brief Control Register
 * @{
 **/
#define TRNG_MASK_SECURITY_CONTROL                ((u32)0x0000000F << 16)          /*!<R/W 0x0  If this field is equal to 0xA, the TRNG become a non-secure slave, and all the registers can be accessed by non-secure transaction. If this field is not equal to 0xA, the non-seucre transaction can only access address 0xC~0x30. */
#define TRNG_SECURITY_CONTROL(x)                  ((u32)(((x) & 0x0000000F) << 16))
#define TRNG_GET_SECURITY_CONTROL(x)              ((u32)(((x >> 16) & 0x0000000F)))
#define TRNG_BIT_LFSR_BYPASS                      ((u32)0x00000001 << 2)          /*!<R/W 0x0  LFSR bypass enable */
#define TRNG_BIT_DEBIAS_CORRECTOR_BYPASS          ((u32)0x00000001 << 1)          /*!<R/W 0x0  Debias corrector bypass enable */
#define TRNG_BIT_NOISE_COLLECTOR_OUTPUT_DISABLE   ((u32)0x00000001 << 0)          /*!<R/W 0x0  Noise collector output disable */
/** @} */

/** @defgroup TRNG_SW_RST
 * @brief Software Reset Register
 * @{
 **/
#define TRNG_BIT_SW_RESET                         ((u32)0x00000001 << 0)          /*!<WO 0x0  Write 1 to trigger software reset. */
/** @} */

/** @defgroup TRNG_RCT0
 * @brief
 * @{
 **/
#define TRNG_MASK_RCT0_CUTOFF_VALUE               ((u32)0x000000FF << 8)          /*!<R/W 0x1F  Cutoff value */
#define TRNG_RCT0_CUTOFF_VALUE(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define TRNG_GET_RCT0_CUTOFF_VALUE(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define TRNG_MASK_RCT0_SAMPLE_SIZE                ((u32)0x00000007 << 1)          /*!<R/W 0x0  Number of bit per test sample. The sample size is the value add 1. 0: 1-bit 1: 2-bit ... 7: 8-bit */
#define TRNG_RCT0_SAMPLE_SIZE(x)                  ((u32)(((x) & 0x00000007) << 1))
#define TRNG_GET_RCT0_SAMPLE_SIZE(x)              ((u32)(((x >> 1) & 0x00000007)))
#define TRNG_BIT_RCT0_ENABLE                      ((u32)0x00000001 << 0)          /*!<R/W 0x1  Test enable */
/** @} */

/** @defgroup TRNG_RCT1
 * @brief
 * @{
 **/
#define TRNG_MASK_RCT1_CUTOFF_VALUE               ((u32)0x000000FF << 8)          /*!<R/W 0x5  Cutoff value */
#define TRNG_RCT1_CUTOFF_VALUE(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define TRNG_GET_RCT1_CUTOFF_VALUE(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define TRNG_MASK_RCT1_SAMPLE_SIZE                ((u32)0x00000007 << 1)          /*!<R/W 0x7  Number of bit per test sample. The sample size is the value add 1. 0: 1-bit 1: 2-bit ... 7: 8-bit */
#define TRNG_RCT1_SAMPLE_SIZE(x)                  ((u32)(((x) & 0x00000007) << 1))
#define TRNG_GET_RCT1_SAMPLE_SIZE(x)              ((u32)(((x >> 1) & 0x00000007)))
#define TRNG_BIT_RCT1_ENABLE                      ((u32)0x00000001 << 0)          /*!<R/W 0x1  Test enable */
/** @} */

/** @defgroup TRNG_APT
 * @brief
 * @{
 **/
#define TRNG_MASK_APT0_DONE_THRESHOLD             ((u32)0x0000000F << 28)          /*!<R/W 0x1  Assert test done after how many test pass */
#define TRNG_APT0_DONE_THRESHOLD(x)               ((u32)(((x) & 0x0000000F) << 28))
#define TRNG_GET_APT0_DONE_THRESHOLD(x)           ((u32)(((x >> 28) & 0x0000000F)))
#define TRNG_MASK_APT0_CUTOFF_VALUE               ((u32)0x000003FF << 16)          /*!<R/W 0x24D  Cutoff value */
#define TRNG_APT0_CUTOFF_VALUE(x)                 ((u32)(((x) & 0x000003FF) << 16))
#define TRNG_GET_APT0_CUTOFF_VALUE(x)             ((u32)(((x >> 16) & 0x000003FF)))
#define TRNG_MASK_APT0_SAMPLE_SIZE                ((u32)0x00000007 << 8)          /*!<R/W 0x0  Number of bit per test sample. The sample size is the value add 1. 0: 1-bit 1: 2-bit ... 7: 8-bit */
#define TRNG_APT0_SAMPLE_SIZE(x)                  ((u32)(((x) & 0x00000007) << 8))
#define TRNG_GET_APT0_SAMPLE_SIZE(x)              ((u32)(((x >> 8) & 0x00000007)))
#define TRNG_BIT_APT0_WINDOW_SIZE                 ((u32)0x00000001 << 1)          /*!<R/W 0x1  0: 512 bits 1: 1024 bits */
#define TRNG_BIT_APT0_ENABLE                      ((u32)0x00000001 << 0)          /*!<R/W 0x1  Test enable */
/** @} */

/** @defgroup TRNG_ROSC_CAL_CTRL
 * @brief ROSC Calibration Control Register
 * @{
 **/
#define TRNG_MASK_SHOT_NUMBER                     ((u32)0x00000FFF << 20)          /*!<R/W 0x10   */
#define TRNG_SHOT_NUMBER(x)                       ((u32)(((x) & 0x00000FFF) << 20))
#define TRNG_GET_SHOT_NUMBER(x)                   ((u32)(((x >> 20) & 0x00000FFF)))
#define TRNG_MASK_CYCLE_NUMBER                    ((u32)0x00000007 << 17)          /*!<R/W 0x6  0x6 = 128 cycles. (this is an encode value). Number of ROSC clock cycle to trigger one calibration */
#define TRNG_CYCLE_NUMBER(x)                      ((u32)(((x) & 0x00000007) << 17))
#define TRNG_GET_CYCLE_NUMBER(x)                  ((u32)(((x >> 17) & 0x00000007)))
#define TRNG_MASK_WAIT_NUMBER                     ((u32)0x0000001F << 12)          /*!<R/W 0x10  After ROSC_D change, the wait cycle number before next calibration procedure. It need to take about 1us to make the OSC stable. */
#define TRNG_WAIT_NUMBER(x)                       ((u32)(((x) & 0x0000001F) << 12))
#define TRNG_GET_WAIT_NUMBER(x)                   ((u32)(((x >> 12) & 0x0000001F)))
#define TRNG_MASK_TRIGGER_NUMBER                  ((u32)0x0000001F << 4)          /*!<R/W 0x10  The value of ROSC_D will be adjusted if CKOUT_ROSC is too high/low more than trig_num times. */
#define TRNG_TRIGGER_NUMBER(x)                    ((u32)(((x) & 0x0000001F) << 4))
#define TRNG_GET_TRIGGER_NUMBER(x)                ((u32)(((x >> 4) & 0x0000001F)))
#define TRNG_BIT_LOCK                             ((u32)0x00000001 << 3)          /*!<R/W 0x0  Force rosc_lock to 1. The NS start to sample when rosc_lock is high. The lock indicate the OCS is stable measure by sensor. */
#define TRNG_BIT_SHOT_MODE                        ((u32)0x00000001 << 2)          /*!<R/W 0x0   */
#define TRNG_BIT_POLARIZATION                     ((u32)0x00000001 << 1)          /*!<R/W 0x0  Polarization of ROSC_D in Calibration. 0: too low -> ROSC_D increases 1: too high -> ROSC_D decreases */
#define TRNG_BIT_HW_CALIB_ENABLE                  ((u32)0x00000001 << 0)          /*!<R/W 0x1  1: control ROSC_D by HW sensor logic. 0: control ROSC_D by SW. */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup TRNG_Exported_Types TRNG Exported Types
  * @{
*****************************************************************************/


/**
 * @brief TRNG Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t TRNG_IER;                               /*!< INTERRUPT ENABLE REGISTER,  Address offset: 0x000 */
	__I  uint32_t TRNG_ISR;                               /*!< INTERRUPT STATUS REGISTER,  Address offset: 0x004 */
	__O  uint32_t TRNG_ICR;                               /*!< INTERRUPT CLEAR REGISTER,  Address offset: 0x008 */
	__I  uint32_t TRNG_FIFO_STATUS;                       /*!< VALID REGISTER,  Address offset: 0x00C */
	__I  uint32_t TRNG_DATAx[8];                          /*!< DATA x REGISTER,  Address offset: 0x010-0x02C */
	__I  uint32_t TRNG_STATUS;                            /*!< STATUS REGISTER,  Address offset: 0x030 */
	__IO uint32_t TRNG_CTRL;                              /*!< CONTROL REGISTER,  Address offset: 0x034 */
	__IO uint32_t TRNG_SW_RST;                            /*!< SOFTWARE RESET REGISTER,  Address offset: 0x038 */
	__IO uint32_t RSVD0[53];                              /*!<  Reserved,  Address offset:0x3C-0x10C */
	__IO uint32_t TRNG_RCT0;                              /*!<  Register,  Address offset: 0x110 */
	__IO uint32_t TRNG_RCT1;                              /*!<  Register,  Address offset: 0x114 */
	__IO uint32_t TRNG_APT;                               /*!<  Register,  Address offset: 0x118 */
	__IO uint32_t RSVD1[35];                              /*!<  Reserved,  Address offset:0x11C-0x1A4 */
	__IO uint32_t TRNG_ROSC_CAL_CTRL;                     /*!< ROSC CALIBRATION CONTROL REGISTER,  Address offset: 0x1A8 */
} TRNG_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported constants --------------------------------------------------------*/
/** @defgroup TRNG_Exported_Constants TRNG Exported Constants
  * @{
  */

/** @defgroup TRNG_Security_Control
  * @{
  */
#define TRNG_NO_SECURITY_EN			0x0A
/**
  * @}
  */

/** @defgroup TRNG_FIFO_Level
  * @{
  */
#define TRNG_FIFO_LEVEL_EMPTY			0x0
#define TRNG_FIFO_LEVEL_INVALID			0xF
/**
  * @}
  */

/** @defgroup TRNG_Polling_Timeout
  * @{
  */
#define TRNG_POLLING_TIMEOUT			0xFFFF
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup TRNG_Exported_Functions TRNG Exported Functions
  * @{
  */

_LONG_CALL_ extern u32 TRNG_Security_Only(BOOL NewState);
_LONG_CALL_ extern void TRNG_INTConfig(u32 TRNG_IT, u32 NewState);
_LONG_CALL_ extern void TRNG_ClearINT(u32 INTrBit);
_LONG_CALL_ extern u32 TRNG_GetINT(void);
_LONG_CALL_ void TRNG_Init(void);
_LONG_CALL_ void TRNG_Random_Delay(void);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
