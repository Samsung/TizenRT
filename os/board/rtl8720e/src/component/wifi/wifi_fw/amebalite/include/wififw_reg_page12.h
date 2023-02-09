#ifndef __WIFIFW_REG_PAGE12_H__
#define __WIFIFW_REG_PAGE12_H__

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH0SA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH0_SA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Address: the source SRAM address that DDMA will read from. This address bus support byte alignment. */
#define BIT_DDMACH0_SA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH0_SA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH0DA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH0_DA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Address: This address bus support byte alignment. */
#define BIT_DDMACH0_DA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH0_DA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH0CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH0_OWN                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set ‘1’ by firmware/driver to denote this DMA channel info is ready for DMA engine to begin DMA activity; after DMA complete, DMA engine will clear it to ‘0’ to denote for firmware that this channel is standby again. */
#define BIT_DDMACH0_IDMEM_ERR            ((u32)0x00000001 << 30)          /*!<R 0  1: Ddma I/D MEM fail. Please check 0x80[0]=1 or DMEM protection is release 0: No error */
#define BIT_DDMACH0_CHKSUM_EN            ((u32)0x00000001 << 29)          /*!<R/W 0  16bits Exclusive OR checksum enable */
#define BIT_DDMACH0_DA_W_DISABLE         ((u32)0x00000001 << 28)          /*!<R/W 0  DA write disable When DDMACH0_CHKSUM_EN=1, DDMACH0_DA_W_DISABLE = 1 means that DMA engine doesn’t move pkt to destination but executes checksum only and report status to DDMACH0_CHKSUM_STS. */
#define BIT_DDMACH0_CHKSUM_STS           ((u32)0x00000001 << 27)          /*!<R/W1C 0  0: Exclusive OR checksum ok 1: Exclusive OR checksum fail */
#define BIT_DDMACH0_CHKSUM_CONT          ((u32)0x00000001 << 24)          /*!<R/W 0  Set1, hw will continuously check sum in several ddma bursts */
#define BIT_MASK_DDMACH0_DLEN            ((u32)0x0003FFFF << 0)          /*!<R/W 0  DMA Length: Length count served by DMA engine, unit in 1 bytes. The range is from 1 to 256K. */
#define BIT_DDMACH0_DLEN(x)              ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_DDMACH0_DLEN(x)          ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH1SA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH1_SA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Address: the source SRAM address that DDMA will read from. This address bus support byte alignment. */
#define BIT_DDMACH1_SA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH1_SA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH1DA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH1_DA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Address: This address bus support byte alignment. */
#define BIT_DDMACH1_DA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH1_DA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH1CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH1_OWN                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set ‘1’ by firmware/driver to denote this DMA channel info is ready for DMA engine to begin DMA activity; after DMA complete, DMA engine will clear it to ‘0’ to denote for firmware that this channel is standby again. */
#define BIT_DDMACH1_IDMEM_ERR            ((u32)0x00000001 << 30)          /*!<R 0  1: Ddma I/D MEM fail. Please check 0x80[0]=1 or DMEM protection is release 0: No error */
#define BIT_DDMACH1_CHKSUM_EN            ((u32)0x00000001 << 29)          /*!<R/W 0  16bits Exclusive OR checksum enable */
#define BIT_DDMACH1_DA_W_DISABLE         ((u32)0x00000001 << 28)          /*!<R/W 0  DA write disable When DDMACH1_CHKSUM_EN=1, DDMACH1_DA_W_DISABLE = 1 means that DMA engine doesn’t move pkt to destination but executes checksum only and report status to DDMACH1_CHKSUM_STS. */
#define BIT_DDMACH1_CHKSUM_STS           ((u32)0x00000001 << 27)          /*!<R/W1C 0  0: Exclusive OR checksum ok 1: Exclusive OR checksum fail */
#define BIT_DDMACH1_CHKSUM_CONT          ((u32)0x00000001 << 24)          /*!<R/W 0  Set1, hw will continuously check sum in several ddma bursts */
#define BIT_MASK_DDMACH1_DLEN            ((u32)0x0003FFFF << 0)          /*!<R/W 0  DMA Length: Length count served by DMA engine, unit in 1 bytes. The range is from 1 to 256K. */
#define BIT_DDMACH1_DLEN(x)              ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_DDMACH1_DLEN(x)          ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH2SA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH2_SA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Address: the source SRAM address that DDMA will read from. This address bus support byte alignment. */
#define BIT_DDMACH2_SA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH2_SA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH2DA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH2_DA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Address: This address bus support byte alignment. */
#define BIT_DDMACH2_DA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH2_DA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH2CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH2_OWN                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set ‘1’ by firmware/driver to denote this DMA channel info is ready for DMA engine to begin DMA activity; after DMA complete, DMA engine will clear it to ‘0’ to denote for firmware that this channel is standby again. */
#define BIT_DDMACH2_IDMEM_ERR            ((u32)0x00000001 << 30)          /*!<R 0  1: Ddma I/D MEM fail. Please check 0x80[0]=1 or DMEM protection is release 0: No error */
#define BIT_DDMACH2_CHKSUM_EN            ((u32)0x00000001 << 29)          /*!<R/W 0  16bits Exclusive OR checksum enable */
#define BIT_DDMACH2_DA_W_DISABLE         ((u32)0x00000001 << 28)          /*!<R/W 0  DA write disable When DDMACH2_CHKSUM_EN=1, DDMACH2_DA_W_DISABLE = 1 means that DMA engine doesn’t move pkt to destination but executes checksum only and report status to DDMACH2_CHKSUM_STS. */
#define BIT_DDMACH2_CHKSUM_STS           ((u32)0x00000001 << 27)          /*!<R/W1C 0  0: Exclusive OR checksum ok 1: Exclusive OR checksum fail */
#define BIT_DDMACH2_DDMA_MODE            ((u32)0x00000001 << 26)          /*!<R/W 0  0: access DMEM/IMEM using cpu idle time slot 1: access DMEM/IMEM block cpu */
#define BIT_MASK_DDMACH2_DLEN            ((u32)0x0003FFFF << 0)          /*!<R/W 0  DMA Length: Length count served by DMA engine, unit in 1 bytes. The range is from 1 to 256K. */
#define BIT_DDMACH2_DLEN(x)              ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_DDMACH2_DLEN(x)          ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH3SA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH3_SA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Address: the source SRAM address that DDMA will read from. This address bus support byte alignment. */
#define BIT_DDMACH3_SA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH3_SA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH3DA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH3_DA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Address: This address bus support byte alignment. */
#define BIT_DDMACH3_DA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH3_DA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH3CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH3_OWN                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set ‘1’ by firmware/driver to denote this DMA channel info is ready for DMA engine to begin DMA activity; after DMA complete, DMA engine will clear it to ‘0’ to denote for firmware that this channel is standby again. */
#define BIT_DDMACH3_IDMEM_ERR            ((u32)0x00000001 << 30)          /*!<R 0  1: Ddma I/D MEM fail. Please check 0x80[0]=1 or DMEM protection is release 0: No error */
#define BIT_DDMACH3_CHKSUM_EN            ((u32)0x00000001 << 29)          /*!<R/W 0  16bits Exclusive OR checksum enable */
#define BIT_DDMACH3_DA_W_DISABLE         ((u32)0x00000001 << 28)          /*!<R/W 0  DA write disable When DDMACH3_CHKSUM_EN=1, DDMACH3_DA_W_DISABLE = 1 means that DMA engine doesn’t move pkt to destination but executes checksum only and report status to DDMACH3_CHKSUM_STS. */
#define BIT_DDMACH3_CHKSUM_STS           ((u32)0x00000001 << 27)          /*!<R/W1C 0  0: Exclusive OR checksum ok 1: Exclusive OR checksum fail */
#define BIT_DDMACH3_CHKSUM_CONT          ((u32)0x00000001 << 24)          /*!<R/W 0  Set1, hw will continuously check sum in several ddma bursts */
#define BIT_MASK_DDMACH3_DLEN            ((u32)0x0003FFFF << 0)          /*!<R/W 0  DMA Length: Length count served by DMA engine, unit in 1 bytes. The range is from 1 to 256K. */
#define BIT_DDMACH3_DLEN(x)              ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_DDMACH3_DLEN(x)          ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH4SA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH4_SA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Address: the source SRAM address that DDMA will read from. This address bus support byte alignment. */
#define BIT_DDMACH4_SA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH4_SA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH4DA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH4_DA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Address: This address bus support byte alignment. */
#define BIT_DDMACH4_DA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH4_DA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH4CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH4_OWN                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set ‘1’ by firmware/driver to denote this DMA channel info is ready for DMA engine to begin DMA activity; after DMA complete, DMA engine will clear it to ‘0’ to denote for firmware that this channel is standby again. */
#define BIT_DDMACH4_IDMEM_ERR            ((u32)0x00000001 << 30)          /*!<R 0  1: Ddma I/D MEM fail. Please check 0x80[0]=1 or DMEM protection is release 0: No error */
#define BIT_DDMACH4_CHKSUM_EN            ((u32)0x00000001 << 29)          /*!<R/W 0  16bits Exclusive OR checksum enable */
#define BIT_DDMACH4_DA_W_DISABLE         ((u32)0x00000001 << 28)          /*!<R/W 0  DA write disable When DDMACH4_CHKSUM_EN=1, DDMACH4_DA_W_DISABLE = 1 means that DMA engine doesn’t move pkt to destination but executes checksum only and report status to DDMACH4_CHKSUM_STS. */
#define BIT_DDMACH4_CHKSUM_STS           ((u32)0x00000001 << 27)          /*!<R/W1C 0  0: Exclusive OR checksum ok 1: Exclusive OR checksum fail */
#define BIT_DDMACH4_CHKSUM_CONT          ((u32)0x00000001 << 24)          /*!<R/W 0  Set1, hw will continuously check sum in several ddma bursts */
#define BIT_MASK_DDMACH4_DLEN            ((u32)0x0003FFFF << 0)          /*!<R/W 0  DMA Length: Length count served by DMA engine, unit in 1 bytes. The range is from 1 to 256K. */
#define BIT_DDMACH4_DLEN(x)              ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_DDMACH4_DLEN(x)          ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH5SA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH5_SA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Address: the source SRAM address that DDMA will read from. This address bus support byte alignment. */
#define BIT_DDMACH5_SA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH5_SA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH5DA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMACH5_DA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Address: This address bus support byte alignment. */
#define BIT_DDMACH5_DA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DDMACH5_DA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CH5CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH5_OWN                  ((u32)0x00000001 << 31)          /*!<R/W 0  Set ‘1’ by firmware/driver to denote this DMA channel info is ready for DMA engine to begin DMA activity; after DMA complete, DMA engine will clear it to ‘0’ to denote for firmware that this channel is standby again. */
#define BIT_DDMACH5_IDMEM_ERR            ((u32)0x00000001 << 30)          /*!<R 0  1: Ddma I/D MEM fail. Please check 0x80[0]=1 or DMEM protection is release 0: No error */
#define BIT_DDMACH5_CHKSUM_EN            ((u32)0x00000001 << 29)          /*!<R/W 0  16bits Exclusive OR checksum enable */
#define BIT_DDMACH5_DA_W_DISABLE         ((u32)0x00000001 << 28)          /*!<R/W 0  DA write disable When DDMACH5_CHKSUM_EN=1, DDMACH5_DA_W_DISABLE = 1 means that DMA engine doesn’t move pkt to destination but executes checksum only and report status to DDMACH5_CHKSUM_STS. */
#define BIT_DDMACH5_CHKSUM_STS           ((u32)0x00000001 << 27)          /*!<R/W1C 0  0: Exclusive OR checksum ok 1: Exclusive OR checksum fail */
#define BIT_DDMACH5_CHKSUM_CONT          ((u32)0x00000001 << 24)          /*!<R/W 0  Set1, hw will continuously check sum in several ddma bursts */
#define BIT_MASK_DDMACH5_DLEN            ((u32)0x0003FFFF << 0)          /*!<R/W 0  DMA Length: Length count served by DMA engine, unit in 1 bytes. The range is from 1 to 256K. */
#define BIT_DDMACH5_DLEN(x)              ((u32)(((x) & 0x0003FFFF) << 0))
#define BIT_GET_DDMACH5_DLEN(x)          ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_INT_MSK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH5_MSK                  ((u32)0x00000001 << 5)          /*!<R/W 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. FW may check the DDMA_CHSTATUS register to determine the DDMA status. */
#define BIT_DDMACH4_MSK                  ((u32)0x00000001 << 4)          /*!<R/W 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. FW may check the DDMA_CHSTATUS register to determine the DDMA status. */
#define BIT_DDMACH3_MSK                  ((u32)0x00000001 << 3)          /*!<R/W 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. FW may check the DDMA_CHSTATUS register to determine the DDMA status. */
#define BIT_DDMACH2_MSK                  ((u32)0x00000001 << 2)          /*!<R/W 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. FW may check the DDMA_CHSTATUS register to determine the DDMA status. */
#define BIT_DDMACH1_MSK                  ((u32)0x00000001 << 1)          /*!<R/W 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. FW may check the DDMA_CHSTATUS register to determine the DDMA status. */
#define BIT_DDMACH0_MSK                  ((u32)0x00000001 << 0)          /*!<R/W 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. FW may check the DDMA_CHSTATUS register to determine the DDMA status. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_INT_STS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH5_DDMA_DONE            ((u32)0x00000001 << 5)          /*!<R/W1C 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. */
#define BIT_DDMACH4_DDMA_DONE            ((u32)0x00000001 << 4)          /*!<R/W1C 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. */
#define BIT_DDMACH3_DDMA_DONE            ((u32)0x00000001 << 3)          /*!<R/W1C 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. */
#define BIT_DDMACH2_DDMA_DONE            ((u32)0x00000001 << 2)          /*!<R/W1C 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. */
#define BIT_DDMACH1_DDMA_DONE            ((u32)0x00000001 << 1)          /*!<R/W1C 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. */
#define BIT_DDMACH0_DDMA_DONE            ((u32)0x00000001 << 0)          /*!<R/W1C 0  When this bit is set, this DDMA channel will generate interrupt when the DMA done. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CHSTATUS
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DDMACH5_REQ                  ((u32)0x00000001 << 5)          /*!<R 0  After CH5CTRL.OWN bit is polled, DDMA starts and asserts this bit. After the DMA transactions finish, this bit is de-asserted. */
#define BIT_DDMACH4_REQ                  ((u32)0x00000001 << 4)          /*!<R 0  After CH4CTRL.OWN bit is polled, DDMA starts and asserts this bit. After the DMA transactions finish, this bit is de-asserted. */
#define BIT_DDMACH3_REQ                  ((u32)0x00000001 << 3)          /*!<R 0  After CH3CTRL.OWN bit is polled, DDMA starts and asserts this bit. After the DMA transactions finish, this bit is de-asserted. */
#define BIT_DDMACH2_REQ                  ((u32)0x00000001 << 2)          /*!<R 0  After CH2CTRL.OWN bit is polled, DDMA starts and asserts this bit. After the DMA transactions finish, this bit is de-asserted. */
#define BIT_DDMACH1_REQ                  ((u32)0x00000001 << 1)          /*!<R 0  After CH1CTRL.OWN bit is polled, DDMA starts and asserts this bit. After the DMA transactions finish, this bit is de-asserted. */
#define BIT_DDMACH0_REQ                  ((u32)0x00000001 << 0)          /*!<R 0  After CH0CTRL.OWN bit is polled, DDMA starts and asserts this bit. After the DMA transactions finish, this bit is de-asserted. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_CHKSUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DDMA_DST_CS             ((u32)0x00000003 << 22)          /*!<R 0  ddma_dst_ctl current status */
#define BIT_DDMA_DST_CS(x)               ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_DDMA_DST_CS(x)           ((u32)(((x >> 22) & 0x00000003)))
#define BIT_MASK_DDMA_SRC_CS             ((u32)0x00000003 << 20)          /*!<R 0  ddma_src_ctl current status */
#define BIT_DDMA_SRC_CS(x)               ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_DDMA_SRC_CS(x)           ((u32)(((x >> 20) & 0x00000003)))
#define BIT_MASK_DDMA_ARB_CS             ((u32)0x0000000F << 16)          /*!<R 0  ddma_arb_ctl current status */
#define BIT_DDMA_ARB_CS(x)               ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_DDMA_ARB_CS(x)           ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_IDDMA0_CHKSUM           ((u32)0x0000FFFF << 0)          /*!<R 0  Iddma0 Check sum Result */
#define BIT_IDDMA0_CHKSUM(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_IDDMA0_CHKSUM(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DDMA_MONITOR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_IDDMA0_PERMU_UNDERFLOW       ((u32)0x00000001 << 14)          /*!<R/W1C 0  IDDMA0 permu underflow,, write clear */
#define BIT_IDDMA0_FIFO_UNDERFLOW        ((u32)0x00000001 << 13)          /*!<R/W1C 0  IDDMA0 fifo underflow, write clear */
#define BIT_IDDMA0_FIFO_OVERFLOW         ((u32)0x00000001 << 12)          /*!<R/W1C 0  IDDMA0 fifo overflow, write clear */
#define BIT_CH5_ERR                      ((u32)0x00000001 << 5)          /*!<R/W1C 0  Ch5 is busy when it is set again, write clear */
#define BIT_CH4_ERR                      ((u32)0x00000001 << 4)          /*!<R/W1C 0  Ch4 is busy when it is set again, write clear */
#define BIT_CH3_ERR                      ((u32)0x00000001 << 3)          /*!<R/W1C 0  Ch3 is busy when it is set again, write clear */
#define BIT_CH2_ERR                      ((u32)0x00000001 << 2)          /*!<R/W1C 0  Ch2 is busy when it is set again, write clear */
#define BIT_CH1_ERR                      ((u32)0x00000001 << 1)          /*!<R/W1C 0  Ch1 is busy when it is set again, write clear */
#define BIT_CH0_ERR                      ((u32)0x00000001 << 0)          /*!<R/W1C 0  Ch0 is busy when it is set again, write clear */
/** @} */


/*==========PAGE12 Register Address Definition==========*/
#define REG_DDMA_CH0SA                               0x1200
#define REG_DDMA_CH0DA                               0x1204
#define REG_DDMA_CH0CTRL                             0x1208
//#define REG_                                         0x120C
#define REG_DDMA_CH1SA                               0x1210
#define REG_DDMA_CH1DA                               0x1214
#define REG_DDMA_CH1CTRL                             0x1218
//#define REG_                                         0x121C
#define REG_DDMA_CH2SA                               0x1220
#define REG_DDMA_CH2DA                               0x1224
#define REG_DDMA_CH2CTRL                             0x1228
//#define REG_                                         0x122C
#define REG_DDMA_CH3SA                               0x1230
#define REG_DDMA_CH3DA                               0x1234
#define REG_DDMA_CH3CTRL                             0x1238
//#define REG_                                         0x123C
#define REG_DDMA_CH4SA                               0x1240
#define REG_DDMA_CH4DA                               0x1244
#define REG_DDMA_CH4CTRL                             0x1248
//#define REG_                                         0x124C
#define REG_DDMA_CH5SA                               0x1250
#define REG_DDMA_CH5DA                               0x1254
#define REG_DDMA_CH5CTRL                             0x1258
//#define REG_                                         0x125C
//#define REG_                                         0x1260
//#define REG_                                         0x1264
//#define REG_                                         0x1268
//#define REG_                                         0x126C
//#define REG_                                         0x1270
//#define REG_                                         0x1274
//#define REG_                                         0x1278
//#define REG_                                         0x127C
//#define REG_                                         0x1280
//#define REG_                                         0x1284
//#define REG_                                         0x1288
//#define REG_                                         0x128C
//#define REG_                                         0x1290
//#define REG_                                         0x1294
//#define REG_                                         0x1298
//#define REG_                                         0x129C
//#define REG_                                         0x12A0
//#define REG_                                         0x12A4
//#define REG_                                         0x12A8
//#define REG_                                         0x12AC
//#define REG_                                         0x12B0
//#define REG_                                         0x12B4
//#define REG_                                         0x12B8
//#define REG_                                         0x12BC
//#define REG_                                         0x12C0
//#define REG_                                         0x12C4
//#define REG_                                         0x12C8
//#define REG_                                         0x12CC
//#define REG_                                         0x12D0
//#define REG_                                         0x12D4
//#define REG_                                         0x12D8
//#define REG_                                         0x12DC
#define REG_DDMA_INT_MSK                             0x12E0
#define REG_DDMA_INT_STS                             0x12E4
#define REG_DDMA_CHSTATUS                            0x12E8
//#define REG_                                         0x12EC
#define REG_DDMA_CHKSUM                              0x12F0
//#define REG_DUMMY                                    0x12F4
//#define REG_                                         0x12F8
#define REG_DDMA_MONITOR                             0x12FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
