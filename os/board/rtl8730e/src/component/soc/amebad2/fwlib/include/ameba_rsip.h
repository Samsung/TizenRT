#ifndef _AMEBAD2_RSIP_H_
#define _AMEBAD2_RSIP_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup RSIP
  * @brief RSIP driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup RSIP_Register_Definitions RSIP Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup RSIP_SR
 * @brief Status Register
 * @{
 **/
#define RSIP_BIT_LOCK_STATUS          ((u32)0x00000001 << 8)          /*!<R 0x0  Indicates whether the setting is locked. If it is locked, only Clr_intr and Maintenance registers can be set. */
#define RSIP_BIT_REMAP_ADDR_ERROR     ((u32)0x00000001 << 7)          /*!<R 0x0  Indicates the address after adding offset size cross 256MB boundaries */
#define RSIP_BIT_REMAP_REGION_ERROR   ((u32)0x00000001 << 6)          /*!<R 0x0  Indicates the read address hits multiple remapping region. */
#define RSIP_BIT_INTR_ERR_INIT        ((u32)0x00000001 << 5)          /*!<R 0x0  Indicates the transaction was received before the key initialization is completed. */
#define RSIP_BIT_INTR_ERR_APB         ((u32)0x00000001 << 4)          /*!<R 0x0  Indicates whether the setting configuration failed. When a transaction is in progress, all settings except clear interruption are invalid. */
#define RSIP_BIT_INTR_ERR_RESP        ((u32)0x00000001 << 3)          /*!<R 0x0  Indicates the error response is received from slave. */
#define RSIP_BIT_INTR_ERR_TAG         ((u32)0x00000001 << 2)          /*!<R 0x0  Indicates a GCM tag mismatch error occurred. */
#define RSIP_BIT_KEY_INIT_DONE        ((u32)0x00000001 << 1)          /*!<R 0x0  Indicates whether the initialization of the key is completed. Only used in GCM and MIX mode. */
#define RSIP_BIT_IDLE                 ((u32)0x00000001 << 0)          /*!<R 0x1  Indicates whether there is any outstanding transaction. */
/** @} */

/** @defgroup RSIP_CIR
 * @brief Component Identification Register
 * @{
 **/
#define RSIP_MASK_LINE_SIZE           ((u32)0x00000007 << 12)          /*!<R ---  Cache line size 0: 16B = 1 AES block 1: 32B = 2 AES blocks 2: 64B = 4 AES blocks 3: 128B = 8 AES blocks Indicates how many 128-bit blocks are used to calculate the tag cache line size (Byte) = 16 * 2BLOCK_SIZE */
#define RSIP_LINE_SIZE(x)             ((u32)(((x) & 0x00000007) << 12))
#define RSIP_GET_LINE_SIZE(x)         ((u32)(((x >> 12) & 0x00000007)))
#define RSIP_BIT_KEY_SOURCE           ((u32)0x00000001 << 11)          /*!<R ---  Indicates the key source 0: from dedicated key port (only support this configuration currently) 1: from APB register */
#define RSIP_BIT_SUPPORT_GCM          ((u32)0x00000001 << 10)          /*!<R ---  Indicates whether GCM mode is supported. */
#define RSIP_BIT_SUPPORT_MIX          ((u32)0x00000001 << 9)          /*!<R ---  Indicates whether MIX mode is supported. */
#define RSIP_BIT_SUPPORT_CTR          ((u32)0x00000001 << 8)          /*!<R ---  Indicates whether CTR mode is supported. */
#define RSIP_MASK_IV_NUMBER           ((u32)0x0000000F << 4)          /*!<R ---  Number of supported IVs registers */
#define RSIP_IV_NUMBER(x)             ((u32)(((x) & 0x0000000F) << 4))
#define RSIP_GET_IV_NUMBER(x)         ((u32)(((x >> 4) & 0x0000000F)))
#define RSIP_MASK_REGION_NUMBER       ((u32)0x0000000F << 0)          /*!<R ---  Number of supported decryption regions */
#define RSIP_REGION_NUMBER(x)         ((u32)(((x) & 0x0000000F) << 0))
#define RSIP_GET_REGION_NUMBER(x)     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup RSIP_EALR
 * @brief Error Address Log Register
 * @{
 **/
#define RSIP_MASK_ERROR_ADDRESS       ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Recorded address when GCM Tag mismatch error or receiving an error response. */
#define RSIP_ERROR_ADDRESS(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RSIP_GET_ERROR_ADDRESS(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup RSIP_ICR
 * @brief Interrupt Clear Register
 * @{
 **/
#define RSIP_BIT_CLR_INTR             ((u32)0x00000001 << 0)          /*!<W 0x0  Interrupt clear. It has to write 1'b1 to this register every time when the error exception occurs. */
/** @} */

/** @defgroup RSIP_CR
 * @brief Control Register
 * @{
 **/
#define RSIP_BIT_ICG_CTRL_4           ((u32)0x00000001 << 20)          /*!<R/W 0x0  Disable key-reg auto clock gating */
#define RSIP_BIT_ICG_CTRL_3           ((u32)0x00000001 << 19)          /*!<R/W 0x0  Disable apb-ctrl auto clock gating */
#define RSIP_BIT_ICG_CTRL_2           ((u32)0x00000001 << 18)          /*!<R/W 0x0  Disable aes-engine auto clock gating */
#define RSIP_BIT_ICG_CTRL_1           ((u32)0x00000001 << 17)          /*!<R/W 0x0  Disable fetch-unit auto clock gating */
#define RSIP_BIT_ICG_CTRL_0           ((u32)0x00000001 << 16)          /*!<R/W 0x0  Disable line-buffer auto clock gating */
#define RSIP_BIT_AES_EN               ((u32)0x00000001 << 8)          /*!<R/W 0x0  Determine whether to enable the AES engine 0: disable decryption engine (expect remapping function) 1: enable decryption engine and line buffer */
#define RSIP_MASK_KEY_SIZE            ((u32)0x00000003 << 5)          /*!<R/W 0x0  Determine the size of KEY 0: 128 bits 1: 192 bits 2: 256 bits */
#define RSIP_KEY_SIZE(x)              ((u32)(((x) & 0x00000003) << 5))
#define RSIP_GET_KEY_SIZE(x)          ((u32)(((x >> 5) & 0x00000003)))
#define RSIP_BIT_AXI_WORD_SWAP        ((u32)0x00000001 << 4)          /*!<R/W 0x0  Determine the endian of ciphertext in slave memory. If set to 1, can swap word order. */
#define RSIP_BIT_AXI_BYTE_SWAP        ((u32)0x00000001 << 3)          /*!<R/W 0x0  Determine the endian of ciphertext in slave memory. If set to 1, can swap byte order. */
#define RSIP_MASK_TAG_SIZE            ((u32)0x00000003 << 0)          /*!<R/W 0x0  Tag size. The size of the tag will affect the length of each tag command. 0: 32 bits 1: 64 bits 2: 128bits */
#define RSIP_TAG_SIZE(x)              ((u32)(((x) & 0x00000003) << 0))
#define RSIP_GET_TAG_SIZE(x)          ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup RSIP_MOR
 * @brief Maintenance Operation Register
 * @{
 **/
#define RSIP_BIT_CLEAN_CACHE_LINE     ((u32)0x00000001 << 2)          /*!<W 0x0  Manually clear the line buffer. Write 1 to clear all after flash data update. */
#define RSIP_BIT_DEC_KEY_INIT         ((u32)0x00000001 << 1)          /*!<W 0x0  Only used in MIX mode. Whenever the key is changed, write 1 to this bit to initialize the decryption key. When keys are update by external port, sec engine will trigger this bit automatically. The status of completion can be determined by reading bit 1 of the status register. */
#define RSIP_BIT_GCM_KEY_INIT         ((u32)0x00000001 << 0)          /*!<W 0x0  Only used in GCM mode. Whenever the key is changed, write 1 to initialize the GMAC key. When keys are update by external port, sec engine will trigger this bit automatically. The status of completion can be determined by reading bit 1 of the status register. */
/** @} */

/** @defgroup RSIP_LR
 * @brief Lock Register
 * @{
 **/
#define RSIP_MASK_LOCK                ((u32)0x000000FF << 0)          /*!<W 0x0  When set 0xDC to this field, all registers except Clr_intr and Maintenance are not allowed to be written. */
#define RSIP_LOCK(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define RSIP_GET_LOCK(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup RSIP_TBAR
 * @brief GCM Tag Base Address Register
 * @{
 **/
#define RSIP_MASK_TAG_BASE_0          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Tag base address in flash memory. */
#define RSIP_TAG_BASE_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RSIP_GET_TAG_BASE_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup RSIP_GDR
 * @brief Gen Date Register
 * @{
 **/
#define RSIP_MASK_YEAR                ((u32)0x0000FFFF << 16)          /*!<R 0x0  Year of code generation. */
#define RSIP_YEAR(x)                  ((u32)(((x) & 0x0000FFFF) << 16))
#define RSIP_GET_YEAR(x)              ((u32)(((x >> 16) & 0x0000FFFF)))
#define RSIP_MASK_DATE                ((u32)0x0000FFFF << 0)          /*!<R 0x0  Date of code generation. */
#define RSIP_DATE(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define RSIP_GET_DATE(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup RSIP_VR
 * @brief Version Register
 * @{
 **/
#define RSIP_MASK_VERSION             ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Version of code generation. */
#define RSIP_VERSION(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RSIP_GET_VERSION(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup RSIP_RxBAR
 * @brief Region x Base Address Register
 * @{
 **/
#define RSIP_MASK_REGION_x_BASE       ((u32)0x07FFFFFF << 5)          /*!<R/W 0x0  Region x base address register. */
#define RSIP_REGION_x_BASE(x)         ((u32)(((x) & 0x07FFFFFF) << 5))
#define RSIP_GET_REGION_x_BASE(x)     ((u32)(((x >> 5) & 0x07FFFFFF)))
#define RSIP_MASK_REGION_x_IV_NUM     ((u32)0x00000007 << 0)          /*!<R/W 0x0  IV used in region x Apply which IV reigster to this region Number corresponds to IV register */
#define RSIP_REGION_x_IV_NUM(x)       ((u32)(((x) & 0x00000007) << 0))
#define RSIP_GET_REGION_x_IV_NUM(x)   ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup RSIP_RxEAR
 * @brief Region x End Address Register
 * @{
 **/
#define RSIP_MASK_REGION_x_END        ((u32)0x07FFFFFF << 5)          /*!<R/W 0x0  Region x end address register. */
#define RSIP_REGION_x_END(x)          ((u32)(((x) & 0x07FFFFFF) << 5))
#define RSIP_GET_REGION_x_END(x)      ((u32)(((x >> 5) & 0x07FFFFFF)))
#define RSIP_MASK_REGION_x_MODE       ((u32)0x00000003 << 1)          /*!<R/W 0x0  Mode used in region x 2'b00: GCM mode 2'b01: CTR mode 2'b10: MIX mode 2'b11: Reserved */
#define RSIP_REGION_x_MODE(x)         ((u32)(((x) & 0x00000003) << 1))
#define RSIP_GET_REGION_x_MODE(x)     ((u32)(((x >> 1) & 0x00000003)))
#define RSIP_BIT_REGION_x_ENABLE      ((u32)0x00000001 << 0)          /*!<R/W 0x0  Enable region x setting */
/** @} */

/** @defgroup RSIP_IVxL
 * @brief IV x Bit Low Register
 * @{
 **/
#define RSIP_MASK_IV_x_HIGH           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Fixed field of IV value. iv_x[127:96] IV = iv_x_high32 || iv_x_low32 || (address >> (4+(LINE_SIZE/32)-1))32 || (address[4+(LINE_SIZE/32)-2:0] + 2)32 */
#define RSIP_IV_x_HIGH(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RSIP_GET_IV_x_HIGH(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup RSIP_IVxH
 * @brief IV x Bit High Register
 * @{
 **/
#define RSIP_MASK_IV_x_LOW            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Fixed field of IV value. iv_x[95:64] IV = iv_x_high32 || iv_x_low32 || (address >> (4+(LINE_SIZE/32)-1))32 || (address[4+(LINE_SIZE/32)-2:0] + 2)32 */
#define RSIP_IV_x_LOW(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RSIP_GET_IV_x_LOW(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup RSIP_REMAPxBAR
 * @brief Remap x Base Address Register
 * @{
 **/
#define RSIP_MASK_REMAP_BASE_x_ADDR   ((u32)0x007FFFFF << 9)          /*!<R/W 0x0  Remap region x base address [27:5] Region_Base_Addr = {flash_base[31:28], remap_base_x_addr, 5'b0} The region address is 32B aligned, and not cross 256KB */
#define RSIP_REMAP_BASE_x_ADDR(x)     ((u32)(((x) & 0x007FFFFF) << 9))
#define RSIP_GET_REMAP_BASE_x_ADDR(x) ((u32)(((x >> 9) & 0x007FFFFF)))
/** @} */

/** @defgroup RSIP_REMAPxEAR
 * @brief Remap x End Address Register
 * @{
 **/
#define RSIP_MASK_REMAP_END_x_OFFSET  ((u32)0x007FFFFF << 9)          /*!<R/W 0x0  Remap region x end address [27:5] Region_Base_Addr = {flash_base[31:28], remap_end_x_addr, 5'b0} The region address is 32B aligned, and not cross 256KB */
#define RSIP_REMAP_END_x_OFFSET(x)    ((u32)(((x) & 0x007FFFFF) << 9))
#define RSIP_GET_REMAP_END_x_OFFSET(x) ((u32)(((x >> 9) & 0x007FFFFF)))
/** @} */

/** @defgroup RSIP_REMAPxOR
 * @brief Remap x Offset Register
 * @{
 **/
#define RSIP_MASK_REMAP_x_OFFSET      ((u32)0x007FFFFF << 9)          /*!<R/W 0x0  Remap region x offset [27:5] Region_Offset = {4'b0,remap_x_offset, 5'b0} The output address is "Access address - Region_Base_Addr + Region_Offset" */
#define RSIP_REMAP_x_OFFSET(x)        ((u32)(((x) & 0x007FFFFF) << 9))
#define RSIP_GET_REMAP_x_OFFSET(x)    ((u32)(((x >> 9) & 0x007FFFFF)))
#define RSIP_BIT_REMAP_x_ENABLE       ((u32)0x00000001 << 0)          /*!<R/W 0x0  Remap region x enable */
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/** @defgroup RSIP_Exported_Types RSIP Exported Types
  * @{
*****************************************************************************/

/** @brief RSIP port
*****************************************************************************/
typedef struct {

	__IO uint32_t RSIP_RxBAR;                             /*!< REGION x BASE ADDRESS REGISTER */
	__IO uint32_t RSIP_RxEAR;                             /*!< REGION x END ADDRESS REGISTER */
} OTF_Region_TypeDef;


/** @brief RSIP port
*****************************************************************************/
typedef struct {

	__IO uint32_t RSIP_IVxL;                              /*!< IV x BIT LOW REGISTER */
	__IO uint32_t RSIP_IVxH;                              /*!< IV x BIT HIGH REGISTER */
} OTF_IV_TypeDef;


/** @brief RSIP port
*****************************************************************************/
typedef struct {

	__IO uint32_t RSIP_REMAPxBAR;                         /*!< REMAP x BASE ADDRESS REGISTER */
	__IO uint32_t RSIP_REMAPxEAR;                         /*!< REMAP x END ADDRESS REGISTER */
	__IO uint32_t RSIP_REMAPxOR;                          /*!< REMAP x OFFSET REGISTER */
} RSIP_MMU_TypeDef;





/**
 * @brief RSIP Register Declaration
 *****************************************************************************/
typedef struct {
	__I  uint32_t RSIP_SR;                                /*!< STATUS REGISTER,  Address offset: 0x00 */
	__I  uint32_t RSIP_CIR;                               /*!< COMPONENT IDENTIFICATION REGISTER,  Address offset: 0x04 */
	__I  uint32_t RSIP_EALR;                              /*!< ERROR ADDRESS LOG REGISTER,  Address offset: 0x08 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__O  uint32_t RSIP_ICR;                               /*!< INTERRUPT CLEAR REGISTER,  Address offset: 0x10 */
	__IO uint32_t RSIP_CR;                                /*!< CONTROL REGISTER,  Address offset: 0x14 */
	__O  uint32_t RSIP_MOR;                               /*!< MAINTENANCE OPERATION REGISTER,  Address offset: 0x18 */
	__IO uint32_t RSIP_LR;                                /*!< LOCK REGISTER,  Address offset: 0x1C */
	__IO uint32_t RSIP_TBAR;                              /*!< GCM TAG BASE ADDRESS REGISTER,  Address offset: 0x20 */
	__I  uint32_t RSIP_GDR;                               /*!< GEN DATE REGISTER,  Address offset: 0x24 */
	__I  uint32_t RSIP_VR;                                /*!< VERSION REGISTER,  Address offset: 0x28 */
	__IO uint32_t RSVD1[5];                               /*!<  Reserved,  Address offset:0x2C-0x3C */
	OTF_Region_TypeDef OTF_REGION[8];                     /*!< RSIP IP have 8 ports,  Address offset: 0x40-0x7C */
	OTF_IV_TypeDef OTF_IV[8];                             /*!< RSIP IP have 8 ports,  Address offset: 0x80-0xBC */
	__IO uint32_t RSVD2[16];                              /*!<  Reserved,  Address offset:0xC0-0xFC */
	RSIP_MMU_TypeDef FLASH_MMU[4];                        /*!< RSIP IP have 4 ports,  Address offset: 0x100-0x12C */
} RSIP_REG_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported constants --------------------------------------------------------*/
/** @defgroup RSIP_Exported_Constants RSIP Exported Constants
  * @{
  */

/** @defgroup RSIP_Key_Req_Poll_Times
  * @{
  */
#define KEY_REQ_POLL_TIMES			0xFF
/**
  * @}
  */

/** @defgroup RSIP_Addr_Mask
  * @{
  */
#define MMU_ADDR_BIT_MASK			((u32)0x0FFFF000)			/*!<Decoder mask enable for MMU address */
/**
  * @}
  */

/** @defgroup RSIP_MMU_Addr_Shift
  * @{
  */
#define MMU_END_ADDR_SHIFT			12
#define MMU_START_ADDR_SHIFT		4
#define MMU_OFFSET_ADDR_SHIFT		4
/**
  * @}
  */

/** @defgroup RSIP_OTF_Mode
  * @{
  */
#define OTF_CTR_MODE		0x1
#define OTF_XTS_MODE		0x2
#define IS_OTF_MODE(MODE) 	((MODE <= 0x2) && (MODE >= 0x1))
/**
  * @}
  */

/** @defgroup RSIP_OTF_Key_Size
  * @{
  */
#define OTF_KEY_128			0x0
#define OTF_KEY_192			0x1
#define OTF_KEY_256			0x2
#define IS_OTF_KEY(SIZE) 	((SIZE) <= 0x2)
/**
  * @}
  */

/** @defgroup RSIP_OTF_Ivnum
  * @{
  */
#define IS_OTF_IVNUM(NUM) ((NUM) <= 0x7)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RSIP_Exported_Functions RSIP Exported Functions
  * @{
  */

_LONG_CALL_ void RSIP_OTF_init(u32 KeySize);
_LONG_CALL_ void RSIP_IV_Set(u8 index, u8 *IV);
_LONG_CALL_ void RSIP_OTF_Cmd(u32 NewStatus);
_LONG_CALL_ void RSIP_OTF_Enable(u32 EnIdx, u32 BaseAddr, u32 EndAddr, u32 NewStatus, u32 IVNum, u32 Mode);
_LONG_CALL_ void RSIP_MMU_Config(u32 MMUIdx, u32 AddrStart, u32 AddrEnd, u32 AddrOffset);
_LONG_CALL_ void RSIP_MMU_Cmd(u32 MMUIdx, u32 NewStatus);
_LONG_CALL_ void RSIP_MMU_Cache_Clean(void);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
