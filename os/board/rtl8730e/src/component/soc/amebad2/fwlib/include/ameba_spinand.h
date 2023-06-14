#ifndef _AMEBAD2_SPINAND_H_
#define _AMEBAD2_SPINAND_H_

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @addtogroup SPIC
* @{
*/

/* Exported constants --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup NAND_Page_Size_Definitions
  * @{
  */
#define NAND_PAGE_SIZE_MAIN_2K_BIT_EXP 		(11)
#define NAND_PAGE_SIZE_MAIN_4K_BIT_EXP 		(12)
/**
 * @}
 */

/** @defgroup NAND_FLASH_Default_CMD
  * @{
  */
#define NAND_CMD_WREN			0x06            //write enable
#define NAND_CMD_WRDI			0x04            //write disable
#define NAND_CMD_RDID			0x9F            //read idenfication
#define NAND_CMD_WRSR			0x1F            //set feature, write status register
#define NAND_CMD_RDSR			0x0F            //get feature, read status register

#define NAND_CMD_READ			0x03            //read data
#define NAND_CMD_FREAD			0x0B            //fast read data
#define NAND_CMD_DREAD			0x3B            //Double Output Mode command	1-1-2
#define NAND_CMD_2READ			0xBB            // 2 x I/O read  command	1-2-2//Not In MXIC
#define NAND_CMD_QREAD			0x6B            // 1I / 4O read command		1-1-4
#define NAND_CMD_4READ			0xEB            // 4 x I/O read  command	1-4-4//Not In MXIC

#define NAND_CMD_RESET			0xFF            //Device Reset
#define NAND_CMD_PAGERD			0x13            //Page Read
#define NAND_CMD_PROMEXEC		0x10            //Program Execute
#define NAND_CMD_BE				0xD8            //128K Block Erase

#define NAND_CMD_PP				0x02            //Page load
#define NAND_CMD_PP_RANDOM		0x84            //Page load random(do not reset buffer)
#define NAND_CMD_QPP			0x32            //quad page load	1-1-4
#define NAND_CMD_QPP_RANDOM		0x34            //quad page load random	1-1-4(do not reset buffer)
/** @} */

/** @defgroup NAND_Dummmy_Cycle_Definitions
  * @{
  */

/*	@define dummy cycle as the sequence below.
*		@arg SpicOneBitMode
*		@arg SpicDualOBitMode
*		@arg SpicDualIOBitMode
*		@arg SpicQuadOBitMode
*		@arg SpicQuadIOBitMode
*/
#define NAND_DM_CYCLE_READ		0x08            //GigaDevice is 0
/*#define NAND_DM_CYCLE_FREAD		0x08*/
#define NAND_DM_CYCLE_2O		0x08
#define NAND_DM_CYCLE_2IO		0x04
#define NAND_DM_CYCLE_4O		0x08
#define NAND_DM_CYCLE_4IO		0x04
/** @} */

/** @defgroup SPIC_ADDR_PHASE_LEN_definitions
  * @{
  * @brief Winbond/MXIC READ/PP CMD ADDR Sequence is CA15-8/CA7-0, so ADDR_LEN is x02
  * GigaDevice is Special:
  * READ/FREAD/DREAD/QREAD CMD ADDR Sequence is Dummy/CA15-8/CA7-0, so ADDR_LEN is x03,
  * 2READ/4READ CMD ADDR Sequence is CA15-8/CA7-0, so ADDR_LEN shall be changed to x02.
  */
#define NAND_COL_ADDR_2_BYTE			0x2
#define NAND_COL_ADDR_3_BYTE			0x3
#define NAND_COL_ADDR_4_BYTE			0x4
/** @} */

/** @defgroup SPI_NAND_Status_Register_group
  * @{
  */
/* feature register */
#define NAND_REG_BLOCK_LOCK				0xA0
#define NAND_BL_ALL_UNLOCKED			0x00

/* configuration register */
#define NAND_REG_CFG					0xB0
#define NAND_CFG_OTP_ENABLE				BIT(6)
#define NAND_CFG_ECC_ENABLE				BIT(4)
#define NAND_CFG_QUAD_ENABLE			BIT(0)	/*Winband is diff with MXIC*/

/* status register */
#define NAND_REG_STATUS					0xC0
#define NAND_STATUS_BUSY				BIT(0)
#define NAND_STATUS_WEL					BIT(1)
#define NAND_STATUS_ERASE_FAILED		BIT(2)
#define NAND_STATUS_PROG_FAILED			BIT(3)
#define NAND_STATUS_ECC_MASK			BIT_OFFSET_LEN_MASK_32(4, 3) /*this is diff with other Spi Nand Flash*/
#define NAND_STATUS_ECC_NO_BITFLIPS		(0 << 4)
#define NAND_STATUS_ECC_HAS_BITFLIPS	(1 << 4)
#define NAND_STATUS_ECC_UNCOR_ERROR		(2 << 4)
/** @} */

/** @defgroup NAND_Page_Size_Main_Definitions
  * @{
  */
#define NAND_PAGE_SIZE_MAIN_BIT_EXP 		(flash_init_para.FLASH_pagesize_main_bit_exp)
#define NAND_PAGE_SIZE_MAIN 				(1U << NAND_PAGE_SIZE_MAIN_BIT_EXP)
#define NAND_ADDR_TO_PAGE_ADDR(addr) 		((addr) >> NAND_PAGE_SIZE_MAIN_BIT_EXP)
#define NAND_ADDR_TO_BYTE_ADDR(addr) 		((addr) & BIT_LEN_MASK_32(NAND_PAGE_SIZE_MAIN_BIT_EXP))
/** @} */

/** @defgroup NAND_Block_Page_Definitions
  * @{
  */
/*Each block has 64 page.*/
#define NAND_BLOCK_PAGE_CNT_BIT_EXP 		(6)
#define NAND_BLOCK_PAGE_CNT 				(1U << NAND_BLOCK_PAGE_CNT_BIT_EXP)
#define NAND_BLOCK_PAGE_MASK 				(NAND_BLOCK_PAGE_CNT - 1)
#define NAND_BLOCK_ID_TO_PAGE_ADDR(block_id)	((block_id) << NAND_BLOCK_PAGE_CNT_BIT_EXP)
#define NAND_PAGE_ADDR_TO_BLOCK_ID(page_addr) 	((page_addr) >> NAND_BLOCK_PAGE_CNT_BIT_EXP)
/** @} */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Functions FLASH Exported Functions
  * @{
  */

/** @defgroup FLASH_NAND_Functions FLASH NAND Functions
  * @{
  */
_LONG_CALL_ void NAND_RxCmd(u8 cmd, u8 addr_len, u8 *paddr, u32 read_len, u8 *read_data);
_LONG_CALL_ void NAND_RxData(u8 cmd, u32 StartAddr, u32 read_len, u8 *read_data);
_LONG_CALL_ u8 NAND_WaitBusy(u32 WaitType);
_LONG_CALL_ void NAND_WriteEn(void);
_LONG_CALL_ void NAND_TxCmd(u8 cmd, u8 AddrLen, u8 *pAddr, u32 DataLen, u8 *pData);
_LONG_CALL_ void NAND_TxData(u8 cmd, u32 StartAddr, u32 ByteLen, u8 *pData);
_LONG_CALL_ u8 NAND_GetStatus(u8 FeatureID);
_LONG_CALL_ void NAND_SetStatus(u8 FeatureID, u8 Status);
_LONG_CALL_ void NAND_SetStatusBits(u8 FeatureID, u8 SetBits, u8 NewState);
_LONG_CALL_ u8 NAND_Erase(u32 PageAddr);
_LONG_CALL_ u8 NAND_Page_Read_ArrayToCache(u32 PageAddr);
_LONG_CALL_ void NAND_Page_Read_FromCache(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData);
_LONG_CALL_ u8 NAND_Page_Read(u32 PageAddr, u32 ByteAddr, u32 ByteLen, u8 *pData);
_LONG_CALL_ void NAND_Page_Write_Data_Xfer(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData);
_LONG_CALL_ u8 NAND_Page_Write_Program_Execute(u32 PageAddr);
_LONG_CALL_ u8 NAND_Page_Write(u32 PageAddr, u32 ByteAddr, u32 ByteLen, u8 *pData);
_LONG_CALL_ void NAND_SetSpiMode(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode);
_LONG_CALL_ void NAND_StructInit_Micron(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ void NAND_StructInit_MXIC(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ void NAND_StructInit_GD(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ void NAND_StructInit(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ u8 NAND_Init(u8 SpicBitMode);
/** @} */

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
