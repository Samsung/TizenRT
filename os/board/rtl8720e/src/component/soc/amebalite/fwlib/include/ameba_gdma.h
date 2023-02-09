/**
  ******************************************************************************
  * @file    ameba_gdma.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the GDMA firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_GDMA_H_
#define _AMEBA_GDMA_H_

/** @addtogroup GDMA
  * @verbatim
  *******************************************************************************************
  * Introduction
  *******************************************************************************************
  * Ameba supports KM4 and AP access GDMA, but KM0 does not support GDMA. GDMA in KM4 or AP has eight channels,
  * The FIFO size of channel 0 and channel is 128bytes, and that of other channels is 64bytes.
  *
  * GDMA0 in AP:
  *		- Base Address: GDMA0_REG_BASE
  *
  *		- Channel index: 0~7
  *
  *		- Block size range: 1~65535
  *
  *		- Transfer Type and Flow Control:
  *                  TTFCMemToMem	(Memory to Memory)
  *                  TTFCMemToPeri	(Memory to Peripheral)
  *                  TTFCPeriToMem	(Peripheral to Memory)
  *                  TTFCPeriToPeri	(Peripheral to Peripheral)
  *
  *		- Source and destination data width:
  *                  TrWidthOneByte
  *                  TrWidthTwoBytes
  *                  TrWidthFourBytes
  *
  *		- Source and destination burst transaction length:
  *                  MsizeOne		(One Byte)
  *                  MsizeFour		(Four Bytes)
  *                  MsizeEight		(Eight Bytes)
  *                  MsizeSixteen	(sixteen Bytes)
  *
  *		- IRQ:
  *                  GDMA0_CHANNEL0_IRQ_AP,
  *                  GDMA0_CHANNEL1_IRQ_AP,
  *                  GDMA0_CHANNEL2_IRQ_AP,
  *                  GDMA0_CHANNEL3_IRQ_AP,
  *                  GDMA0_CHANNEL4_IRQ_AP,
  *                  GDMA0_CHANNEL5_IRQ_AP,
  *                  GDMA0_CHANNEL6_IRQ_AP,
  *                  GDMA0_CHANNEL7_IRQ_AP,
  *
  *
  * GDMA0 in KM4:
  *		- Base Address: GDMA0_REG_BASE
  *
  *		- Channel index: 0~7
  *
  *		- Block size range: 1~65535
  *
  *		- Transfer Type and Flow Control:
  *                  TTFCMemToMem	(Memory to Memory)
  *                  TTFCMemToPeri	(Memory to Peripheral)
  *                  TTFCPeriToMem	(Peripheral to Memory)
  *                  TTFCPeriToPeri	(Peripheral to Peripheral)
  *
  *		- Source and destination data width:
  *                  TrWidthOneByte
  *                  TrWidthTwoBytes
  *                  TrWidthFourBytes
  *
  *		- Source and destination burst transaction length:
  *                  MsizeOne		(One Byte)
  *                  MsizeFour		(Four Bytes)
  *                  MsizeEight		(Eight Bytes)
  *                  MsizeSixteen	(sixteen Bytes)
  *		- IRQ:
  *                  GDMA0_CHANNEL0_IRQ,
  *                  GDMA0_CHANNEL1_IRQ
  *                  GDMA0_CHANNEL2_IRQ
  *                  GDMA0_CHANNEL3_IRQ
  *                  GDMA0_CHANNEL4_IRQ
  *                  GDMA0_CHANNEL5_IRQ
  *                  GDMA0_CHANNEL6_IRQ
  *                  GDMA0_CHANNEL7_IRQ
  *
  *
  *****************************************************************************************
  * how to use GDMA
  *****************************************************************************************
  *      To use the GDMA, the following steps are mandatory:
  *
  *      1. Allocate a GDMA channel using the follwoing function.
  *			GDMA_ChnlAlloc(u32 GDMA_Index, IRQ_FUN IrqFun, u32 IrqData, u32 IrqPriority)
  *
  *			@note  This function also includes the following operation:
  *				- register irq handler if use interrupt mode
  *				- enable NVIC interrupt
  *				- register the GDMA channel to use
  *				- enable GDMA peripheral clock
  *
  *      2. Program GDMA index, GDMA channel, data width, Msize, transfer direction, address increment mode,
  *          hardware handshake interface, reload control, interrupt type, block size, multi-block configuration
  *          and the source and destination address using the GDMA_Init() function.
  *
  *      3. Enable the corresponding interrupt using the function.
  *			GDMA_INTConfig() and register the uart irq handler if you need to use interrupt mode.
  *
  *			@note  This step is included in the "step 2"(GDMA_Init()).
  *
  *      4. Enable GDMA using function GDMA_Cmd().
  *
  *
  * @endverbatim
  */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup GDMA
  * @brief  GDMA driver modules
  * @{
  */

/* Registers Definitions ----------------------------------------------------------------*/
/** @defgroup GDMA_Register_Definitions GDMA Register Definitions
  * @{
  */
/**************************************************************************//**
 * @defgroup GDMA_CTL
 * @{
 *****************************************************************************/
/********************  Bits definition for CTL register  *******************/
#define BIT_CTLX_LO_INT_EN					((u32)(0x00000001 << 0))		/*Lower word Bit[0].Interrupt Enable Bit.*/
#define BIT_CTLX_LO_LLP_DST_EN				((u32)(0x00000001 << 27))		/*Lower word Bit[27].Block chaining is enabled on the destination side only*/
#define BIT_CTLX_LO_LLP_SRC_EN				((u32)(0x00000001 << 28))		/*Lower word Bit[28].Block chaining is enabled on the source side only*/

#define BIT_CTLX_LO_DST_TR_WIDTH			((u32)(0x00000007 << 1))		/*Lower word Bit[3:1].Destination Transfer Width*/
#define BIT_CTLX_LO_SRC_TR_WIDTH			((u32)(0x00000007 << 4))		/*Lower word Bit[6:4].Source Transfer Width*/

#define BIT_CTLX_LO_DINC					((u32)(0x00000003 << 7))		/*Lower word Bit[8:7].Destination Address Increment*/
#define BIT_CTLX_LO_SINC					((u32)(0x00000003 << 9))		/*Lower word Bit[10:9].Source Address Increment*/

#define BIT_CTLX_LO_DEST_MSIZE				((u32)(0x00000007 << 11))		/*Lower word Bit[13:11].Destination Burst Transaction Length*/
#define BIT_CTLX_LO_SRC_MSIZE				((u32)(0x00000007 << 14))		/*Lower word Bit[16:14].Source Burst Transaction Length*/

#define BIT_CTLX_LO_SRC_GATHER_EN		((u32)(0x00000001 << 17))		/*Lower word Bit[17].Source gather enable bit*/
#define BIT_CTLX_LO_DST_SCATTER_EN		((u32)(0x00000001 << 18))		/*Lower word Bit[18].Destination gather enable bit*/

#define BIT_CTLX_LO_TT_FC				((u32)(0x00000007 << 20))		/*Lower word Bit[22:20].Transfer Type and Flow Control*/

#define BIT_CTLX_LO_DMS					((u32)(0x00000003 << 23))		/*Lower word Bit[24:23].Destination Master Select*/
#define BIT_CTLX_LO_SMS					((u32)(0x00000003 << 25))		/*Lower word Bit[26:25].Source Master Select*/

//#define BIT_CTLX_UP_DONE				((u32)(0x00000001 << 12))		/*Upper word Bit[12].Done bit, RTK DMAC dont have this bit */
#define BIT_CTLX_UP_BLOCK_BS			((u32)(0x0000FFFF << 0))		/*Upper word Bit[15:0].Block Transfer Size.*/
/** @} */

/**************************************************************************//**
 * @defgroup GDMA_CFG
 * @{
 *****************************************************************************/
/********************  Bits definition for CFG register  *******************/
#define BIT_CFGX_CH_PRIOR				((u32)(0x0000000F<< 4))			/*Lower word Bit[4:7]. channel priority,  0 is the highest priority value, (DMAC_NUM_CHANNELS-1) is the lowest priroty value. If more than one channel are set with the same priroty value, the channel with small channel number index has higher priority.*/
#define SET_CFGX_CH_PRIOR(x)			((u32)(((x) & 0x0000000F) << 4))
#define BIT_CFGX_LO_RELOAD_SRC			((u32)(0x00000001 << 30))		/*Lower word Bit[30].Automatic Source Reload bit*/
#define BIT_CFGX_LO_RELOAD_DST			((u32)(0x00000001 << 31))		/*Lower word Bit[31].Automatic Destination Reload bit*/

#define BIT_CFGX_UP_SEC_DISABLE			((u32)(0x00000001 << 3))		/*Upper word Bit[10:7]. write 0 to enable secure transfer, default is 0 */
#define BIT_CFGX_UP_SRC_PER				((u32)(0x0000000F << 7))		/*Upper word Bit[10:7].hardware handshaking interface for source peripheral*/
#define BIT_CFGX_UP_DEST_PER			((u32)(0x0000000F << 11))		/*Upper word Bit[14:11].hardware handshaking interface for destination peripheral*/
#define BIT_CFGX_UP_FIFO_MODE			((u32)(0x00000001<<1 ))		/*Upper word Bit[1].hardware FIFO Mode Select, write 1 to enable*/
/** @} */
/** @} */

/* Exported Types --------------------------------------------------------*/

/** @defgroup GDMA_Exported_Types GDMA Exported Types
  * @{
  */

/** @brief GDMA Channel Register Declaration
  */
typedef struct {
	__IO uint32_t SAR;					/*!< Channel x Source Address Register,			Address offset: 0x0000 */
	__O uint32_t RSAR;					/*!< Channel x Source Address Read Back Register,			Address offset: 0x0004 */
	__IO uint32_t DAR;					/*!< Channel x Destination Address Register,		Address offset: 0x0008 */
	__O uint32_t RDAR;					/*!< Channel x Destination Address Read Back Register,		Address offset: 0x000C */
	__IO uint32_t LLP;						/*!< Channel x Linked List Pointer Register,		Address offset: 0x0010 */
	uint32_t RSVD2;
	__IO uint32_t CTL_LOW;				/*!< Channel x Control Register,					Address offset: 0x0018 */
	__IO uint32_t CTL_HIGH;				/*!< Channel x Control Register,					Address offset: 0x001C */
	__IO uint32_t SSTAT;					/*!< Channel x Source Status Register,			Address offset: 0x0020 */
	uint32_t RSVD4;
	__IO uint32_t DSTAT;					/*!< Channel x Destination Status Register,		Address offset: 0x0028 */
	uint32_t RSVD5;
	__IO uint32_t SSTATAR;				/*!< Channel x Source Status Address Register,	Address offset: 0x0030 */
	uint32_t RSVD6;
	__IO uint32_t DSTATAR;				/*!< Channel x Destination Status Address Register,	Address offset: 0x0038 */
	uint32_t RSVD7;
	__IO uint32_t CFG_LOW;				/*!< Channel x Configuration Register,			Address offset: 0x0040 */
	__IO uint32_t CFG_HIGH;				/*!< Channel x Configuration Register,			Address offset: 0x0044 */
	__IO uint32_t SGR;					/*!< Channel x Source Gather Register,			Address offset: 0x0048 */
	uint32_t RSVD9;
	__IO uint32_t DSR;					/*!< Channel x Source Gather Register,			Address offset: 0x0050 */
	uint32_t RSVD10;						/*!< Channel x Last Register,					Address offset: 0x0054 */
} GDMA_ChannelTypeDef;

/** @brief GDMA Register Declaration
  */
typedef struct {
	GDMA_ChannelTypeDef CH[8];		/*!< GDMA IP have 8 channels,				Address offset: 0x00-0x2BC */

	__I uint32_t RAW_TFR;			/*!< Raw Status for IntTfr Interrupt,			Address offset: 0x02C0 */
	uint32_t RSVD0;
	__I uint32_t RAW_BLOCK;			/*!< Raw Status for IntBlock Interrupt,		Address offset: 0x02C8 */
	uint32_t RSVD1;
	__I uint32_t RAW_SRC_TRAN;		/*!< Raw Status for IntSrcTran Interrupt,		Address offset: 0x02D0 */
	uint32_t RSVD2;
	__I uint32_t RAW_DST_TRAN;		/*!< Raw Status for IntDstTran Interrupt,		Address offset: 0x02D0 */
	uint32_t RSVD3;
	__I uint32_t RAW_ERR;			/*!< Raw Status for IntErr Interrupt,			Address offset: 0x02E0 */
	uint32_t RSVD4;

	__I uint32_t STATUS_TFR;		/*!< Status for IntTfr Interrupt,				Address offset: 0x02E8 */
	uint32_t RSVD5;
	__I uint32_t STATUS_BLOCK;		/*!< Status for IntBlock Interrupt,			Address offset: 0x02F0 */
	uint32_t RSVD6;
	__I uint32_t STATUS_SRC_TRAN;	/*!< Status for IntSrcTran Interrupt,			Address offset: 0x02F8 */
	uint32_t RSVD7;
	__I uint32_t STATUS_DST_TRAN;	/*!< Status for IntDstTran Interrupt,			Address offset: 0x0300 */
	uint32_t RSVD8;
	__I uint32_t STATUS_ERR;		/*!< Status for IntErr Interrupt,				Address offset: 0x0308 */
	uint32_t RSVD9;

	__IO uint32_t MASK_TFR;			/*!< Mask for IntTfr Interrupt,				Address offset: 0x0310 */
	uint32_t RSVD10;
	__IO uint32_t MASK_BLOCK;		/*!< Mask for IntBlock Interrupt,				Address offset: 0x0318 */
	uint32_t RSVD11;
	__IO uint32_t MASK_SRC_TRAN;	/*!< Mask for IntSrcTran Interrupt,			Address offset: 0x0320 */
	uint32_t RSVD12;
	__IO uint32_t MASK_DST_TRAN;	/*!< Mask for IntDstTran Interrupt,			Address offset: 0x0328 */
	uint32_t RSVD13;
	__IO uint32_t MASK_ERR;			/*!< Mask for IntErr Interrupt,				Address offset: 0x0330 */
	uint32_t RSVD14;

	__O uint32_t CLEAR_TFR;			/*!< Clear for IntTfr Interrupt,				Address offset: 0x0338 */
	uint32_t RSVD15;
	__O uint32_t CLEAR_BLOCK;		/*!< Clear for IntBlock Interrupt,				Address offset: 0x0340 */
	uint32_t RSVD16;
	__O uint32_t CLEAR_SRC_TRAN;	/*!< Clear for IntSrcTran Interrupt,			Address offset: 0x0348 */
	uint32_t RSVD17;
	__O uint32_t CLEAR_DST_TRAN;	/*!< Clear for IntDstTran Interrupt,			Address offset: 0x0350 */
	uint32_t RSVD18;
	__O uint32_t CLEAR_ERR;			/*!< Clear for IntErr Interrupt,				Address offset: 0x0358 */
	uint32_t RSVD19;
	__O uint32_t StatusInt;			/*!< Status for each interrupt type,			Address offset: 0x0360 */
	uint32_t RSVD191;

	__IO uint32_t ReqSrcReg;		/*!< Source Software Transaction Request Register,			Address offset: 0x0368 */
	uint32_t RSVD20;
	__IO uint32_t ReqDstReg;		/*!< Destination Software Transaction Request Register,		Address offset: 0x0370 */
	uint32_t RSVD21;
	__IO uint32_t SglReqSrcReg;		/*!< Single Source Transaction Request Register,			Address offset: 0x0378 */
	uint32_t RSVD22;
	__IO uint32_t SglReqDstReg;		/*!< Single Destination Transaction Request Register,		Address offset: 0x0380 */
	uint32_t RSVD23;
	__IO uint32_t LstSrcReg;		/*!< Last Source Transaction Request Register,				Address offset: 0x0388 */
	uint32_t RSVD24;
	__IO uint32_t LstDstReg;		/*!< Last Destination Transaction Request Register,			Address offset: 0x0390 */
	uint32_t RSVD25;

	__IO uint32_t DmaCfgReg;		/*!< DMA Configuration Register,			Address offset: 0x0398 */
	uint32_t RSVD26;
	__IO uint32_t ChEnReg;			/*!< DMA Channel Enable Register,			Address offset: 0x03A0 */
	uint32_t RSVD27;
	__I uint32_t DmaIdReg;			/*!< DMA ID Register,						Address offset: 0x03A8 */
	uint32_t RSVD28;
	__IO uint32_t DmaTestReg;		/*!< DMA Test Register,					Address offset: 0x03B0 */
	uint32_t RSVD29;
} GDMA_TypeDef;

/**
  * @brief  GDMA Init Structure Definition
  */
typedef struct {
	u8	GDMA_Index;      /*!< Specifies the GDMA index.
					          This parameter can be the value 0.*/

	u8	GDMA_ChNum;   /*!< Specifies the GDMA channel number.
					          This parameter can be the value 0 ~ 7.*/

	u8	GDMA_ChPrior;   /*!< Specifies the GDMA channel priority.
					          This parameter can be the value 0 ~ 7, while 0 for highset priority.
	                          If more than one channel are set with the same priroty value,
							  the channel with small channel number index has higher priority.*/

	u32	GDMA_DIR;	     /*!< Specifies the GDMA transmission direction.
					          This parameter can be a value of @ref GDMA_Data_Transfer_Direction */

	u32	GDMA_DstDataWidth;    /*!< Specifies the GDMA destination transfer width.
					                This parameter can be a value of @ref GDMA_Data_Size */

	u32	GDMA_SrcDataWidth;    /*!< Specifies the GDMA transfer width.
					                This parameter can be a value of @ref GDMA_Data_Size */

	u32	GDMA_DstInc;		  /*!< Specifies the GDMA destination address increment mode.
					                This parameter can be a value of @ref GDMA_Increment_Mode */

	u32	GDMA_SrcInc;              /*!< Specifies the GDMA source address increment mode.
					                This parameter can be a value of @ref GDMA_Increment_Mode */

	u32	GDMA_DstMsize;          /*!< Specifies the GDMA destination burst transaction length.
					                This parameter can be a value of @ref GDMA_Msize */

	u32	GDMA_SrcMsize;          /*!< Specifies the GDMA source burst transaction length.
					                This parameter can be a value of @ref GDMA_Msize */

	u32	GDMA_SrcAddr;           /*!< Specifies the GDMA source address.
					                This parameter can be a value of the memory or peripheral space address,
					                depending on the GDMA data transfer direction.If this address is configured,
					                GDMA will move data from here to the destination address space*/

	u32	GDMA_DstAddr;          /*!< Specifies the GDMA destination address.
					                This parameter can be a value of the memory or peripheral space address,
					                depending on the GDMA data transfer direction.If this address is configured,
					                GDMA will move data here from source address space*/

	u16	GDMA_BlockSize; 	      /*!< Specifies the GDMA block transfer size.
	                                           This parameter can be a value between 0 ~ 65535.
					                @note This parameter indicates the total number of single transactions for
					                  every block transfer. The field for this parameter locates in CTLx[47:32], so
					                  the value of this parameter must be no more than 0xffff.*/

	u32	GDMA_IsrType;        /*!< Specifies the GDMA interrupt types.
					                This parameter can be a value of @ref GDMA_Interrupt_Type */

	u32	GDMA_ReloadSrc;     /*!< Specifies the GDMA automatic source reload .
					                This parameter can be the 0 or 1.(0 : disable / 1 : enable).
					                @note if this value is setted 1, source address register can be automatically
					                reloaded from its initial value at the end of every block for multi-block transfers.
					                this parameter is only valid in multi block transmission mode*/

	u32	GDMA_ReloadDst;    /*!< Specifies the GDMA automatic destination reload .
					                This parameter can be the 0 or 1.(0 : disable / 1 : enable).
					                @note if this parameter is set 1, destination address register can be automatically
					                reloaded from its initial value at the end of every block for multi-block transfers.
					                this parameter is only valid in multi block transmission mode*/

	u32	GDMA_LlpDstEn;    /*!< Specifies the GDMA whether block chaining is enabled or disabled on the destination
	                                           side only.
					                @note this parameter is only valid in multi-block transmission mode*/

	u32	GDMA_LlpSrcEn;     /*!< Specifies the GDMA whether block chaining is enabled or disabled on the source
	                                           side only.
					                @note this parameter is only valid in multi-block transmission mode*/

	u32	GDMA_SrcHandshakeInterface;    /*!< Specifies the GDMA hardware handshaking interface for the source
							                       peripheral of a GDMA channel.
							                       This parameter can be a value of @ref GDMA0_HS_HandShake_Interface */

	u32	GDMA_DstHandshakeInterface;   /*!< Specifies the GDMA hardware handshaking interface for the destination
							                       peripheral of a GDMA channel.
							                       This parameter can be a value of @ref GDMA0_HS_HandShake_Interface */

	u32	MuliBlockCunt;                /*!< Specifies the GDMA Multi-block counter.
					                This parameter is used in multi-block transmission.*/

	u32	MaxMuliBlock;                /*!< Specifies the GDMA Max block number in Multi-block transmission.
					                This parameter is used in multi-block transmission.*/
	u32	SecureTransfer;		/*!< Specifies the GDMA secure transmission.
								This parameter is used in secure world of trustzone.*/
} GDMA_InitTypeDef, *PGDMA_InitTypeDef;

/**
  * @brief  GDMA LLI ELE Structure Definition
  */
typedef struct {
	u32 Sarx;             /*!< Specifies the GDMA channel x Source Address Register (SARx) value field of a block descriptor
	                                    in block chaining.
					         This parameter stores the source address of the current block transfer.*/

	u32 Darx;            /*!< Specifies the GDMA channel x Destination Address Register(DARx) value field of a block descriptor
	                                    in block chaining.
					         This parameter stores the destination address of the current block transfer.*/

	u32 Llpx;              /*!< Specifies the GDMA channel x Linked List Pointer Register(LLPx) value field of a block descriptor
	                                    in block chaining.
					         This parameter is a address, which points to the next block descriptor.*/

	u32 CtlxLow;        /*!< Specifies the GDMA channel x Control Register(CTRx) Low 32 bit value field of a block descriptor
	                                    in block chaining.
					         This parameter stores the DMA control parameters of the current block transfer.*/

	u32 CtlxUp;          /*!< Specifies the GDMA channel x Control Register(CTRx) High 32 bit value field of a block descriptor
	                                    in block chaining.
					         This parameter stores the DMA control parameters of the current block transfer.*/

	u32 Temp;           /*!< Specifies the reserved GDMA channel x register value field of a block descriptor
	                                    in block chaining.*/
} GDMA_CH_LLI_ELE, *PGDMA_CH_LLI_ELE;

/**
  * @brief  GDMA CH LLI Structure Definition
  */
struct GDMA_CH_LLI {
	GDMA_CH_LLI_ELE	 LliEle;     /*!< Specifies the GDMA Linked List Item Element structure field of Linked List Item
	                                              in block chaining.
					                   This structure variable stores the necessary parameters of a block descriptor.*/

	u32 BlockSize;                    /*!< Specifies the GDMA block size of one block in block chaining.
					                   This parameter indicates the block size of the current block transfer.*/

	struct GDMA_CH_LLI *pNextLli;          /*!< Specifies the GDMA Linked List Item pointer.
					                               This parameter stores the address pointing to the next Linked List Item
					                               in block chaining.*/
};

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup GDMA_Exported_Constants GDMA Exported Constants
  * @{
  */

/** @defgroup GDMA_Index_Channel_Definition
  * @{
  */
#define MAX_GDMA_INDX		(0)
#define MAX_GDMA_CHNL		(7)

#define IS_GDMA_ChannelNum(NUM) ((NUM) <= MAX_GDMA_CHNL)
#define IS_GDMA_Index(NUM) ((NUM) <= MAX_GDMA_INDX)
/**
  * @}
  */

/** @defgroup GDMA_Data_Transfer_Direction
  * @{
  */
#define TTFCMemToMem						((u32)0x00000000)
#define TTFCMemToPeri						((u32)0x00000001)
#define TTFCPeriToMem						((u32)0x00000002)
#define TTFCPeriToPeri						((u32)0x00000003)
#define TTFCPeriToMem_PerCtrl				((u32)0x00000004)
#define TTFCPeriToPeri_SrcPerCtrl			((u32)0x00000005)
#define TTFCMemToPeri_PerCtrl				((u32)0x00000006)
#define TTFCPeriToPeri_DstPerCtrl			((u32)0x00000007)

#define IS_GDMA_DIR(DIR) (((DIR) == TTFCMemToMem) || \
                          ((DIR) == TTFCMemToPeri) || \
                          ((DIR) == TTFCPeriToMem) ||\
                          ((DIR) == TTFCPeriToPeri) ||\
                          ((DIR) == TTFCPeriToMem_PerCtrl) || \
                          ((DIR) == TTFCPeriToPeri_SrcPerCtrl) || \
                          ((DIR) == TTFCPeriToPeri_DstPerCtrl) || \
                          ((DIR) == TTFCMemToPeri_PerCtrl))
/**
  * @}
  */

/** @defgroup GDMA_Data_Size
  * @{
  */
#define TrWidthOneByte						((u32)0x00000000)
#define TrWidthTwoBytes						((u32)0x00000001)
#define TrWidthFourBytes						((u32)0x00000002)
#define IS_GDMA_DATA_SIZE(SIZE) (((SIZE) == TrWidthOneByte) || \
                                       ((SIZE) == TrWidthTwoBytes) || \
                                       ((SIZE) == TrWidthFourBytes))
/**
  * @}
  */

/** @defgroup GDMA_Msize
  * @{
  */
#define MsizeOne							((u32)0x00000000)
#define MsizeFour							((u32)0x00000001)
#define MsizeEight							((u32)0x00000002)
#define MsizeSixteen						((u32)0x00000003)
#define Msize32								((u32)0x00000004)
#define Msize64								((u32)0x00000005)
#define Msize128							((u32)0x00000006)
#define Msize256							((u32)0x00000007)


#define IS_GDMA_MSIZE(SIZE) (((SIZE) == MsizeOne) || \
                                ((SIZE) == MsizeFour) || \
                                ((SIZE) == MsizeEight)||\
                                ((SIZE) == MsizeSixteen))
/**
  * @}
  */

/** @defgroup GDMA_Increment_Mode
  * @{
  */
#define IncType								((u32)0x00000000)
#define DecType								((u32)0x00000001)
#define NoChange							((u32)0x00000002)
#define IS_GDMA_IncMode(STATE) (((STATE) == IncType) || \
                                            ((STATE) == DecType) || \
                                            ((STATE) == NoChange))
/**
  * @}
  */

/** @defgroup GDMA_Interrupt_Type
  * @{
  */
#define TransferType							((u32)0x00000001)
#define BlockType							((u32)0x00000002)
#define SrcTransferType						((u32)0x00000004)
#define DstTransferType						((u32)0x00000008)
#define ErrType								((u32)0x000000010)

#define IS_GDMA_CONFIG_IT(IT) ((((IT) & 0xFFFFFFE0) == 0x00) && ((IT) != 0x00))
/**
  * @}
  */

/** @defgroup GDMA_Reload_Definition
  * @{
  */
#define CLEAN_RELOAD_SRC					((u32)0x00000001)
#define CLEAN_RELOAD_DST					((u32)0x00000002)
#define CLEAN_RELOAD_SRC_DST				((u32)0x00000003)
/**
  * @}
  */

/** @defgroup GDMA0_HS_HandShake_Interface
  * @{
  */
#define GDMA_HANDSHAKE_INTERFACE_UART0_TX		(0)
#define GDMA_HANDSHAKE_INTERFACE_UART0_RX		(1)
#define GDMA_HANDSHAKE_INTERFACE_UART1_TX		(2)
#define GDMA_HANDSHAKE_INTERFACE_UART1_RX		(3)
#define GDMA_HANDSHAKE_INTERFACE_UART2_TX		(4)
#define GDMA_HANDSHAKE_INTERFACE_UART2_RX		(5)
#define GDMA_HANDSHAKE_INTERFACE_UART3_TX		(6)
#define GDMA_HANDSHAKE_INTERFACE_UART3_RX		(7)

#define GDMA_HANDSHAKE_INTERFACE_SPI0_TX		(8)
#define GDMA_HANDSHAKE_INTERFACE_SPI0_RX		(9)
#define GDMA_HANDSHAKE_INTERFACE_SPI1_TX		(10)
#define GDMA_HANDSHAKE_INTERFACE_SPI1_RX		(11)

#define GDMA_HANDSHAKE_INTERFACE_SPIC_TX		(12)
#define GDMA_HANDSHAKE_INTERFACE_SPIC_RX		(13)

#define GDMA_HANDSHAKE_INTERFACE_SPORT0F0_TX	(14)
#define GDMA_HANDSHAKE_INTERFACE_SPORT0F1_TX	(15)
#define GDMA_HANDSHAKE_INTERFACE_SPORT0F0_RX	(16)
#define GDMA_HANDSHAKE_INTERFACE_SPORT0F1_RX	(17)

#define GDMA_HANDSHAKE_INTERFACE_SPORT1F0_TX	(18)
#define GDMA_HANDSHAKE_INTERFACE_SPORT1F1_TX	(19)
#define GDMA_HANDSHAKE_INTERFACE_SPORT1F0_RX	(20)
#define GDMA_HANDSHAKE_INTERFACE_SPORT1F1_RX	(21)


#define GDMA_HANDSHAKE_INTERFACE_LEDC_TX		(22)
#define GDMA_HANDSHAKE_INTERFACE_Zigbee_TX		(23)
#define GDMA_HANDSHAKE_INTERFACE_Zigbee_RX		(24)

#define GDMA_HANDSHAKE_INTERFACE_I2C0_TX		(NULL)
#define GDMA_HANDSHAKE_INTERFACE_I2C0_RX		(NULL)
#define GDMA_HANDSHAKE_INTERFACE_ADC_RX			(NULL)
#define GDMA_HANDSHAKE_INTERFACE_AUDIO_TX		(NULL)
#define GDMA_HANDSHAKE_INTERFACE_AUDIO_RX		(NULL)
/**
  * @}
  */

/** @defgroup GDMA1_HS_HandShake_Interface
  * @brief  Not supported in amebaD.
  * @{
  */
#define GDMA_HANDSHAKE_TIMER_CAPTURE_UP		(0)
#define GDMA_HANDSHAKE_TIMER_CAPTURE_CH0	(1)
#define GDMA_HANDSHAKE_TIMER_PWM_UP			(2)
#define GDMA_HANDSHAKE_TIMER_PWM_CH0		(3)
#define GDMA_HANDSHAKE_TIMER_PWM_CH1		(4)
#define GDMA_HANDSHAKE_TIMER_PWM_CH2		(5)
#define GDMA_HANDSHAKE_TIMER_PWM_CH3		(6)
#define GDMA_HANDSHAKE_TIMER_PWM_CH4		(7)
#define GDMA_HANDSHAKE_TIMER_PWM_CH5		(8)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup GDMA_Exported_Functions GDMA Exported Functions
  * @{
  */

_LONG_CALL_ void GDMA_StructInit(PGDMA_InitTypeDef GDMA_InitStruct);
_LONG_CALL_ void GDMA_Init(u8 GDMA_Index, u8 GDMA_ChNum, PGDMA_InitTypeDef GDMA_InitStruct);
_LONG_CALL_ void GDMA_SetLLP(u8 GDMA_Index, u8 GDMA_ChNum, u32 MultiBlockCount, struct GDMA_CH_LLI *pGdmaChLli, u32 round);
_LONG_CALL_ void GDMA_Cmd(u8 GDMA_Index, u8 GDMA_ChNum, u32 NewState);
_LONG_CALL_ void GDMA_INTConfig(u8 GDMA_Index, u8 GDMA_ChNum, u32 GDMA_IT, u32 NewState);
_LONG_CALL_ u32	 GDMA_ClearINTPendingBit(u8 GDMA_Index, u8 GDMA_ChNum, u32 GDMA_IT);
_LONG_CALL_ u32	 GDMA_ClearINT(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ void GDMA_ChCleanAutoReload(u8 GDMA_Index, u8 GDMA_ChNum, u32 CleanType);

_LONG_CALL_ void GDMA_SetSrcAddr(u8 GDMA_Index, u8 GDMA_ChNum, u32 SrcAddr);
_LONG_CALL_ u32	 GDMA_GetSrcAddr(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ u32	 GDMA_GetDstAddr(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ void GDMA_SetDstAddr(u8 GDMA_Index, u8 GDMA_ChNum, u32 DstAddr);
_LONG_CALL_ void GDMA_SetBlkSize(u8 GDMA_Index, u8 GDMA_ChNum, u32 BlkSize);
_LONG_CALL_ u32	 GDMA_GetBlkSize(u8 GDMA_Index, u8 GDMA_ChNum);

_LONG_CALL_ BOOL GDMA_ChnlRegister(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ void GDMA_ChnlUnRegister(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ u8	 GDMA_ChnlAlloc(u32 GDMA_Index, IRQ_FUN IrqFun, u32 IrqData, u32 IrqPriority);
_LONG_CALL_ void GDMA_ChnlFree(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ u8	 GDMA_GetIrqNum(u8 GDMA_Index, u8 GDMA_ChNum);
_LONG_CALL_ void GDMA_SetChnlPriority(u8 GDMA_Index, u8 GDMA_ChNum, u32 ChnlPriority);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Other Definitions -------------------------------------------------------------------*/

/* Register Address Offset Definitions --------------------------------------------------------*/
/********************  Address Offset Definition for GDMA Registers  *******************/
#define REG_GDMA_CH_OFF					(0x058)		/*address space value between two DMA channels*/
#define REG_GDMA_CH_SAR					(0x000)		/*Source Address Register(SAR) address offset*/
#define REG_GDMA_CH_DAR					(0x008)		/*Destination Address Register(DAR) address offset*/
#define REG_GDMA_CH_LLP					(0x010)		/*Linked List Pointer(LLP) Register address offset*/
#define REG_GDMA_CH_CTL					(0x018)		/*Control Register(CTR) address offset*/
#define REG_GDMA_CH_SSTAT				(0x020)		/*Source Status(SSTAT) Register address offset*/
#define REG_GDMA_CH_DSTAT				(0x028)		/*Destination Status(DSTAT) Register address offset*/
#define REG_GDMA_CH_SSTATAR				(0x030)		/*Source Status Address(SSTATA) Register address offset*/
#define REG_GDMA_CH_DSTATAR				(0x038)		/*Destination Status Address(DSTATA) Register address offset*/
#define REG_GDMA_CH_CFG					(0x040)		/*Configuration(CFG) Register address offset*/
#define REG_GDMA_CH_SGR					(0x048)		/*Source Gather Register(SGR) address offset*/
#define REG_GDMA_CH_DSR					(0x050)		/*Destination Scatter Register(DSR) address offset*/

/**********************  Address Offset Definition for Interrupt Raw Status Registers  *******************/
#define REG_GDMA_RAW_INT_BASE			(0x2C0)		/*Base address for Interrupt Raw Status Registers*/
#define REG_GDMA_RAW_INT_TFR			(0x2C0)		/*address offset for DMA Transfer Complete Interrupt Raw Status Register(RawTfr)*/
#define REG_GDMA_RAW_INT_BLOCK			(0x2c8)		/*address offset for Block Transfer Complete Interrupt Raw Status Register(RawBlock)*/
#define REG_GDMA_RAW_INT_SRC_TRAN		(0x2D0)		/*address offset for Source Transaction Complete Interrupt Raw Status Register(RawSrcTran)*/
#define REG_GDMA_RAW_INT_DST_TRAN		(0x2D8)		/*address offset for Destination Transaction Complete Interrupt Raw Status Register(RawDstTran)*/
#define REG_GDMA_RAW_INT_ERR			(0x2E0)		/*address offset for Error Interrupt Raw Status Register(RawDstTran)*/

/**********************  Address Offset Definition for Interrupt Status Registers  *******************/
#define REG_GDMA_STATUS_INT_BASE		(0x2E8)		/*Base address for Interrupt Status Registers*/
#define REG_GDMA_STATUS_INT_TFR			(0x2E8)		/*address offset for DMA Transfer Complete Interrupt Status Register(StatusTfr)*/
#define REG_GDMA_STATUS_INT_BLOCK		(0x2F0)		/*address offset for Block Transfer Complete Interrupt Status Register(StatusBlock)*/
#define REG_GDMA_STATUS_INT_SRC_TRAN	(0x2F8)		/*address offset for Source Transaction Complete Interrupt Status Register(StatusSrcTran)*/
#define REG_GDMA_STATUS_INT_DST_TRAN	(0x300)		/*address offset for Destination Transaction Complete Interrupt Status Register(StatusDstTran)*/
#define REG_GDMA_STATUS_INT_ERR			(0x308)		/*address offset for Error Interrupt Status Register(StatusErr)*/

/**********************  Address Offset Definition for Interrupt Mask Registers  *******************/
#define REG_GDMA_MASK_INT_BASE			(0x310)		/*Base address for Interrupt Mask Registers*/
#define REG_GDMA_MASK_INT_TFR			(0x310)		/*address offset for DMA Transfer Complete Interrupt Mask Register(MaskTfr)*/
#define REG_GDMA_MASK_INT_BLOCK			(0x318)		/*address offset for Block Transfer Complete Interrupt Mask Register(MaskBlock)*/
#define REG_GDMA_MASK_INT_SRC_TRAN		(0x320)		/*address offset for Source Transaction Complete Interrupt Mask Register(MaskSrcTran)*/
#define REG_GDMA_MASK_INT_DST_TRAN		(0x328)		/*address offset for Destination Transaction Complete Interrupt Mask Register(MaskDstTran)*/
#define REG_GDMA_MASK_INT_INT_ERR		(0x330)		/*address offset for Error Interrupt Mask Register(MaskErr)*/

/**********************  Address Offset Definition for Interrupt Clear Registers  *******************/
#define REG_GDMA_CLEAR_INT_BASE			(0x338)		/*Base address for Interrupt Clear Registers*/
#define REG_GDMA_CLEAR_INT_TFR			(0x338)		/*address offset for DMA Transfer Complete Interrupt Clear Register(ClearTfr)*/
#define REG_GDMA_CLEAR_INT_BLOCK		(0x340)		/*address offset for Block Transfer Complete Interrupt Clear Register(ClearBlock)*/
#define REG_GDMA_CLEAR_INT_SRC_TRAN		(0x348)		/*address offset for Source Transaction Complete Interrupt Clear Register(ClearSrcTran)*/
#define REG_GDMA_CLEAR_INT_DST_TRAN		(0x350)		/*address offset for Destination Transaction Complete Interrupt Clear Register(ClearDstTran)*/
#define REG_GDMA_CLEAR_INT_ERR			(0x358)		/*address offset for Error Interrupt Clear Register(ClearErr)*/

/*********************  Address Offset Definition for Combined Interrupt Status Register  ***********/
#define REG_GDMA_STATUS_INT				(0x360)		/*address offset for Combined Interrupt Status Register*/

/**********************  Address Offset Definition for Software Handshaking Registers  *************/
#define REG_GDMA_REQ_SRC					(0x368)		/*address offset for Source Software Transaction Request Register(ReqSrcReg)*/
#define REG_GDMA_REQ_DST					(0x370)		/*address offset for Destination Software Transaction Request Register(ReqDstReg)*/
#define REG_GDMA_REQ_SGL_REQ				(0x378)		/*address offset for Single Source Transaction Request Register(SglReqSrcReg)*/
#define REG_GDMA_REQ_DST_REQ				(0x380)		/*address offset for Single Destination Transaction Request Register(SglReqDstReg)*/
#define REG_GDMA_REQ_LST_SRC				(0x388)		/*address offset for Last Source Transaction Request Register(LstSrcReg)*/
#define REG_GDMA_REQ_LST_DST				(0x390)		/*address offset for Last Destination Transaction Request Register(LstDstReg)*/

/**********************  Address Offset Definition for Miscellaneous Registers  *************/
#define REG_GDMA_DMAC_CFG               	(0x398)		/*address offset for DMA Configuration Register(DmaCfgReg)*/
#define REG_GDMA_CH_EN                  	(0x3A0)		/*address offset for DMA Channel Enable Register(ChEnReg)*/
#define REG_GDMA_DMA_ID                 	(0x3A8)		/*address offset for DMA ID Register(DmaIdReg)*/
#define REG_GDMA_DMA_TEST               	(0x3B0)		/*address offset for DMA Test Register(DmaTestReg)*/
#define REG_GDMA_DMA_COM_PARAMS6        	(0x3C8)		/*address offset for DMA Component Parameters Register 6(DMA_COMP_PARAMS_6)*/
#define REG_GDMA_DMA_COM_PARAMS5        	(0x3D0)		/*address offset for DMA Component Parameters Register 5(DMA_COMP_PARAMS_5)*/
#define REG_GDMA_DMA_COM_PARAMS4        	(0x3D8)		/*address offset for DMA Component Parameters Register 4(DMA_COMP_PARAMS_4)*/
#define REG_GDMA_DMA_COM_PARAMS3        	(0x3E0)		/*address offset for DMA Component Parameters Register 3(DMA_COMP_PARAMS_3)*/
#define REG_GDMA_DMA_COM_PARAMS2        	(0x3E8)		/*address offset for DMA Component Parameters Register 2(DMA_COMP_PARAMS_2)*/
#define REG_GDMA_DMA_COM_PARAMS1        	(0x3F0)		/*address offset for DMA Component Parameters Register 1(DMA_COMP_PARAMS_1)*/
#define REG_GDMA_DMA_COM_PARAMS0        	(0x3F8)		/*address offset for DMA Component ID Register. Bit[63:32]: DMA_COMP_VERSION
															 Bit[31:0]:DMA_COMP_TYPE*/

#define GDMA_SHIFT_ExtendedSRC_PER1                  15
#define GDMA_BIT_ExtendedSRC_PER1                    ((u32)0x00000001 << GDMA_SHIFT_ExtendedSRC_PER1)
#define GDMA_SHIFT_ExtendedSRC_PER2                  17
#define GDMA_BIT_ExtendedSRC_PER2                    ((u32)0x00000001 << GDMA_SHIFT_ExtendedSRC_PER2)

#define GDMA_SHIFT_ExtendedDEST_PER1                  16
#define GDMA_BIT_ExtendedDEST_PER1                   ((u32)0x00000001 << GDMA_SHIFT_ExtendedDEST_PER1)
#define GDMA_SHIFT_ExtendedDEST_PER2                  18
#define GDMA_BIT_ExtendedDEST_PER2                   ((u32)0x00000001 << GDMA_SHIFT_ExtendedDEST_PER2)

#endif //_AMEBA_GDMA_H_

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
