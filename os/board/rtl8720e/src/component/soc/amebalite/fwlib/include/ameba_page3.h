#ifndef _AMEBAD2_PAGE3_H_
#define _AMEBAD2_PAGE3_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_AXI_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MAX_RXDMA                ((u32)0x00000003 << 28)          /*!<R/W 3h  RXDMA Burst Size selection default 3. 0: 4bytes; 1: 16bytes; 2: 32bytes; 3: 64bytes */
#define BIT_MAX_RXDMA(x)                  ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_MAX_RXDMA(x)              ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_MAX_TXDMA                ((u32)0x00000003 << 24)          /*!<R/W 3h  TXDMA Burst Size selection default 3. 0: 4bytes; 1: 16bytes; 2: 32bytes; 3: 64bytes. */
#define BIT_MAX_TXDMA(x)                  ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_MAX_TXDMA(x)              ((u32)(((x >> 24) & 0x00000003)))
#define BIT_REG_TXDMA_FAIL_PS             ((u32)0x00000001 << 21)          /*!<W 0  Write 1 to cause a txdma fail ps */
#define BIT_MBSSID_ENSWBCN_BACKDOOR       ((u32)0x00000001 << 16)          /*!<R/W 0  MBSSID enable software becon */
#define BIT_EN_WT_RXTAG                   ((u32)0x00000001 << 15)          /*!<R/W 1h  Display Dword0[32:16] type of Rx buffer descriptor 0: TotalLen 1: Rxtag */
#define BIT_STOPBCN                       ((u32)0x00000001 << 14)          /*!<R/W 0  BCNQ DMA STOP */
#define BIT_STOPMG                        ((u32)0x00000001 << 13)          /*!<R/W 0  MGQ DMA STOP */
#define BIT_STOPVO                        ((u32)0x00000001 << 12)          /*!<R/W 0  VOQ DMA STOP */
#define BIT_STOPVI                        ((u32)0x00000001 << 11)          /*!<R/W 0  VIQ DMA STOP */
#define BIT_STOPBE                        ((u32)0x00000001 << 10)          /*!<R/W 0  BEQ DMA STOP */
#define BIT_STOPBK                        ((u32)0x00000001 << 9)          /*!<R/W 0  BKQ DMA STOP */
#define BIT_STOPRX                        ((u32)0x00000001 << 8)          /*!<R/W 0  RX DMA STOP */
#define BIT_MASK_MQ_STOP                  ((u32)0x000000FF << 0)          /*!<R/W 0  Stop MQ DMA */
#define BIT_MQ_STOP(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_MQ_STOP(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_INT_MIG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_TTMRMIT                  ((u32)0x0000000F << 28)          /*!<R/W 0  When the number of timer matched condition exceeds this setting, then interrupt is issued. */
#define BIT_TTMRMIT(x)                    ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_TTMRMIT(x)                ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_TNUMMIT                  ((u32)0x0000000F << 24)          /*!<R/W 0  When the buffered TX packet number is matched, the interrupt is then issued */
#define BIT_TNUMMIT(x)                    ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_TNUMMIT(x)                ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_RTMRMIT                  ((u32)0x0000000F << 20)          /*!<R/W 0  When the number of timer matched condition exceeds this setting, then interrupt is issued. */
#define BIT_RTMRMIT(x)                    ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_RTMRMIT(x)                ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_RNUMMIT                  ((u32)0x0000000F << 16)          /*!<R/W 0  When the buffered RX packet number is matched, the interrupt is then issued */
#define BIT_RNUMMIT(x)                    ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_RNUMMIT(x)                ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_MIGRATE_TIMER            ((u32)0x0000FFFF << 0)          /*!<R/W 0  Timer for setting Interrupt Migration time, only use [2:0] 3’b000: 64 us 3’b001: 128 us 3’b010: 256 us 3’b011: 512 us 3’b100: 1024 us 3’b101: 2048 us 3’b110: 4096 us 3’b111: 8192 us */
#define BIT_MIGRATE_TIMER(x)              ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MIGRATE_TIMER(x)          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BCNQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BPQDESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for Beacon Queue */
#define BIT_BPQDESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BPQDESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MGQDESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for Manage Queue */
#define BIT_MGQDESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MGQDESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_VOQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_VOQDESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for VO Queue */
#define BIT_VOQDESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_VOQDESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_VIQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_VIQDESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for VI Queue */
#define BIT_VIQDESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_VIQDESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BEQDESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for BE Queue */
#define BIT_BEQDESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BEQDESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BKQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BKQDESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for BK Queue */
#define BIT_BKQDESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BKQDESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXQ_RXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXNRML_DESA              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for RX */
#define BIT_RXNRML_DESA(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_RXNRML_DESA(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ0_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ0DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M0 Queue */
#define BIT_MQ0DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ0DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ1_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ1DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M1 Queue */
#define BIT_MQ1DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ1DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ2_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ2DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M2 Queue */
#define BIT_MQ2DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ2DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ3_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ3DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M3 Queue */
#define BIT_MQ3DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ3DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ4_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ4DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M4 Queue */
#define BIT_MQ4DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ4DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ5_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ5DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M5 Queue */
#define BIT_MQ5DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ5DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ6_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ6DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M6 Queue */
#define BIT_MQ6DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ6DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ7_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ7DESA                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for M7 Queue */
#define BIT_MQ7DESA(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MQ7DESA(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_TXBD_RX_RXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BCNQ_DESC_MODE           ((u32)0x00000003 << 29)          /*!<R/W 0  BCNQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_BCNQ_DESC_MODE(x)             ((u32)(((x) & 0x00000003) << 29))
#define BIT_GET_BCNQ_DESC_MODE(x)         ((u32)(((x >> 29) & 0x00000003)))
#define BIT_BCNQ_FLAG                     ((u32)0x00000001 << 28)          /*!<R/W 0  BCNQ FLAGing bit Read : BCNQ work Write : BCNQ Polling */
#define BIT_MASK_RXDES_NUM                ((u32)0x00000FFF << 16)          /*!<R/W 100h  RX queue descriptor's number */
#define BIT_RXDES_NUM(x)                  ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_RXDES_NUM(x)              ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MGQ_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  MGQ FLAGing bit, read result will be bpqflag */
#define BIT_MASK_MGQ_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  MGQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MGQ_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_MGQ_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_MGQDES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  MG queue descriptor's number */
#define BIT_MGQDES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MGQDES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_VOQ_VIQ_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_VIQ_FLAG                      ((u32)0x00000001 << 30)          /*!<R 0  VIQ, read result will be viqflag */
#define BIT_MASK_VIQ_DESC_MODE            ((u32)0x00000003 << 28)          /*!<R/W 0  VIQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_VIQ_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_VIQ_DESC_MODE(x)          ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_VIQDES_NUM               ((u32)0x00000FFF << 16)          /*!<R/W 100h  VI queue descriptor's number */
#define BIT_VIQDES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_VIQDES_NUM(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_VOQ_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  VOQ, read result will be voqflag */
#define BIT_MASK_VOQ_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  VOQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_VOQ_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_VOQ_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_VOQDES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  VO queue descriptor's number */
#define BIT_VOQDES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_VOQDES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEQ_BKQ_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_BKQ_FLAG                      ((u32)0x00000001 << 30)          /*!<R 0  BKQ, read result will be bkqflag */
#define BIT_MASK_BKQ_DESC_MODE            ((u32)0x00000003 << 28)          /*!<R/W 0  BKQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_BKQ_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_BKQ_DESC_MODE(x)          ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_BKQDES_NUM               ((u32)0x00000FFF << 16)          /*!<R/W 100h  BK queue descriptor's number */
#define BIT_BKQDES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_BKQDES_NUM(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_BEQ_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  BEQ, read result will be beqflag */
#define BIT_MASK_BEQ_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  BEQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_BEQ_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_BEQ_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_BEQDES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  BE queue descriptor's number */
#define BIT_BEQDES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_BEQDES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ0_MQ1_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MQ1_FLAG                      ((u32)0x00000001 << 30)          /*!<R 0  MQ1, read result will be MQ1flag */
#define BIT_MASK_MQ1_DESC_MODE            ((u32)0x00000003 << 28)          /*!<R/W 0  MQ1 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ1_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_MQ1_DESC_MODE(x)          ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_MQ1DES_NUM               ((u32)0x00000FFF << 16)          /*!<R/W 100h  M1 queue descriptor's number */
#define BIT_MQ1DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ1DES_NUM(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MQ0_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  MQ0, read result will be MQ0flag */
#define BIT_MASK_MQ0_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  MQ0 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ0_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_MQ0_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_MQ0DES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  M0 queue descriptor's number */
#define BIT_MQ0DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ0DES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ2_MQ3_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MQ3_FLAG                      ((u32)0x00000001 << 30)          /*!<R 0  MQ3, read result will be MQ3flag */
#define BIT_MASK_MQ3_DESC_MODE            ((u32)0x00000003 << 28)          /*!<R/W 0  MQ3 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ3_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_MQ3_DESC_MODE(x)          ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_MQ3DES_NUM               ((u32)0x00000FFF << 16)          /*!<R/W 100h  M3 queue descriptor's number */
#define BIT_MQ3DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ3DES_NUM(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MQ2_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  MQ2, read result will be MQ2flag */
#define BIT_MASK_MQ2_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  MQ2 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ2_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_MQ2_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_MQ2DES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  M2 queue descriptor's number */
#define BIT_MQ2DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ2DES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ4_MQ5_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MQ5_FLAG                      ((u32)0x00000001 << 30)          /*!<R 0  MQ5, read result will be MQ5flag */
#define BIT_MASK_MQ5_DESC_MODE            ((u32)0x00000003 << 28)          /*!<R/W 0  MQ5 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ5_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_MQ5_DESC_MODE(x)          ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_MQ5DES_NUM               ((u32)0x00000FFF << 16)          /*!<R/W 100h  M5 queue descriptor's number */
#define BIT_MQ5DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ5DES_NUM(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MQ4_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  MQ4, read result will be MQ4flag */
#define BIT_MASK_MQ4_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  MQ4 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ4_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_MQ4_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_MQ4DES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  M4 queue descriptor's number */
#define BIT_MQ4DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ4DES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ6_MQ7_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MQ7_FLAG                      ((u32)0x00000001 << 30)          /*!<R 0  MQ7, read result will be MQ7flag */
#define BIT_MASK_MQ7_DESC_MODE            ((u32)0x00000003 << 28)          /*!<R/W 0  MQ7 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ7_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_MQ7_DESC_MODE(x)          ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_MQ7DES_NUM               ((u32)0x00000FFF << 16)          /*!<R/W 100h  M7 queue descriptor's number */
#define BIT_MQ7DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ7DES_NUM(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MQ6_FLAG                      ((u32)0x00000001 << 14)          /*!<R 0  MQ6, read result will be MQ6flag */
#define BIT_MASK_MQ6_DESC_MODE            ((u32)0x00000003 << 12)          /*!<R/W 0  MQ6 descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_MQ6_DESC_MODE(x)              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_MQ6_DESC_MODE(x)          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_MQ6DES_NUM               ((u32)0x00000FFF << 0)          /*!<R/W 100h  M6 queue descriptor's number */
#define BIT_MQ6DES_NUM(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ6DES_NUM(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BD_RWPTR_CLR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_CLR_MQ7_HW_IDX                ((u32)0x00000001 << 29)          /*!<W 0  Clear MQ7 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ6_HW_IDX                ((u32)0x00000001 << 28)          /*!<W 0  Clear MQ6 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ5_HW_IDX                ((u32)0x00000001 << 27)          /*!<W 0  Clear MQ5 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ4_HW_IDX                ((u32)0x00000001 << 26)          /*!<W 0  Clear MQ4 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ3_HW_IDX                ((u32)0x00000001 << 25)          /*!<W 0  Clear MQ3 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ2_HW_IDX                ((u32)0x00000001 << 24)          /*!<W 0  Clear MQ2 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ1_HW_IDX                ((u32)0x00000001 << 23)          /*!<W 0  Clear MQ1 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MQ0_HW_IDX                ((u32)0x00000001 << 22)          /*!<W 0  Clear MQ0 hardware index, write 1 and hardware auto clear */
#define BIT_CLR_BKQ_HW_IDX                ((u32)0x00000001 << 21)          /*!<W 0  Clear BKQ hardware index, write 1 and hardware auto clear */
#define BIT_CLR_BEQ_HW_IDX                ((u32)0x00000001 << 20)          /*!<W 0  Clear BEQ hardware index, write 1 and hardware auto clear */
#define BIT_CLR_VIQ_HW_IDX                ((u32)0x00000001 << 19)          /*!<W 0  Clear VIQ hardware index, write 1 and hardware auto clear */
#define BIT_CLR_VOQ_HW_IDX                ((u32)0x00000001 << 18)          /*!<W 0  Clear VOQ hardware index, write 1 and hardware auto clear */
#define BIT_CLR_MGQ_HW_IDX                ((u32)0x00000001 << 17)          /*!<W 0  Clear MGQ hardware index, write 1 and hardware auto clear */
#define BIT_CLR_RXQ_HW_IDX                ((u32)0x00000001 << 16)          /*!<W 0  Clear RXQ hardware index, write 1 and hardware auto clear */
#define BIT_SRST_TX_N                     ((u32)0x00000001 << 15)          /*!<W 1h  Reset tx dma */
#define BIT_SRST_RX_N                     ((u32)0x00000001 << 14)          /*!<W 1h  Reset rx dma */
#define BIT_CLR_MQ7_HOST_IDX              ((u32)0x00000001 << 13)          /*!<W 0  Clear MQ7 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ6_HOST_IDX              ((u32)0x00000001 << 12)          /*!<W 0  Clear MQ6 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ5_HOST_IDX              ((u32)0x00000001 << 11)          /*!<W 0  Clear MQ5 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ4_HOST_IDX              ((u32)0x00000001 << 10)          /*!<W 0  Clear MQ4 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ3_HOST_IDX              ((u32)0x00000001 << 9)          /*!<W 0  Clear MQ3 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ2_HOST_IDX              ((u32)0x00000001 << 8)          /*!<W 0  Clear MQ2 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ1_HOST_IDX              ((u32)0x00000001 << 7)          /*!<W 0  Clear MQ1 host index, write 1 and hardware auto clear */
#define BIT_CLR_MQ0_HOST_IDX              ((u32)0x00000001 << 6)          /*!<W 0  Clear MQ0 host index, write 1 and hardware auto clear */
#define BIT_CLR_BKQ_HOST_IDX              ((u32)0x00000001 << 5)          /*!<W 0  Clear MQ host index, write 1 and hardware auto clear */
#define BIT_CLR_BEQ_HOST_IDX              ((u32)0x00000001 << 4)          /*!<W 0  Clear BEQ host index, write 1 and hardware auto clear */
#define BIT_CLR_VIQ_HOST_IDX              ((u32)0x00000001 << 3)          /*!<W 0  Clear VIQ host index, write 1 and hardware auto clear */
#define BIT_CLR_VOQ_HOST_IDX              ((u32)0x00000001 << 2)          /*!<W 0  Clear VOQ host index, write 1 and hardware auto clear */
#define BIT_CLR_MGQ_HOST_IDX              ((u32)0x00000001 << 1)          /*!<W 0  Clear MGQ host index, write 1 and hardware auto clear */
#define BIT_CLR_RXQ_HOST_IDX              ((u32)0x00000001 << 0)          /*!<W 0  Clear RXQ host index, write 1 and hardware auto clear */
/** @} */

/**************************************************************************//**
 * @defgroup REG_VOQ_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_VOQ_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  VO queue descriptor’s hardware index */
#define BIT_VOQ_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_VOQ_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_VOQ_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  VO queue descriptor’s host index */
#define BIT_VOQ_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_VOQ_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_VIQ_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_VIQ_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  VI queue descriptor’s hardware index */
#define BIT_VIQ_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_VIQ_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_VIQ_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  VI queue descriptor’s host index */
#define BIT_VIQ_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_VIQ_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BEQ_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BEQ_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  BE queue descriptor’s hardware index */
#define BIT_BEQ_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_BEQ_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_BEQ_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  BE queue descriptor’s host index */
#define BIT_BEQ_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_BEQ_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BKQ_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BKQ_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  BK queue descriptor’s hardware index */
#define BIT_BKQ_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_BKQ_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_BKQ_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  BK queue descriptor’s host index */
#define BIT_BKQ_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_BKQ_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MGQ_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MGQ_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MG queue descriptor’s hardware index */
#define BIT_MGQ_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MGQ_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MGQ_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MG queue descriptor’s host index */
#define BIT_MGQ_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MGQ_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_RXQ_RXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_RXNRML_HW_IDX            ((u32)0x00000FFF << 16)          /*!<R 0  RX queue descriptor’s hardware index */
#define BIT_RXNRML_HW_IDX(x)              ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_RXNRML_HW_IDX(x)          ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_RXNRML_HOST_IDX          ((u32)0x00000FFF << 0)          /*!<R/W 0  RX queue descriptor’s host index */
#define BIT_RXNRML_HOST_IDX(x)            ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_RXNRML_HOST_IDX(x)        ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ0_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ0_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ0 queue descriptor’s hardware index */
#define BIT_MQ0_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ0_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ0_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ0 queue descriptor’s host index */
#define BIT_MQ0_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ0_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ1_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ1_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ1 queue descriptor’s hardware index */
#define BIT_MQ1_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ1_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ1_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ1 queue descriptor’s host index */
#define BIT_MQ1_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ1_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ2_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ2_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ2 queue descriptor’s hardware index */
#define BIT_MQ2_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ2_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ2_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ2 queue descriptor’s host index */
#define BIT_MQ2_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ2_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ3_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ3_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ3 queue descriptor’s hardware index */
#define BIT_MQ3_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ3_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ3_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ3 queue descriptor’s host index */
#define BIT_MQ3_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ3_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ4_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ4_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ4 queue descriptor’s hardware index */
#define BIT_MQ4_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ4_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ4_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ4 queue descriptor’s host index */
#define BIT_MQ4_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ4_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ5_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ5_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ5 queue descriptor’s hardware index */
#define BIT_MQ5_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ5_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ5_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ5 queue descriptor’s host index */
#define BIT_MQ5_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ5_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ6_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ6_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ6 queue descriptor’s hardware index */
#define BIT_MQ6_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ6_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ6_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ6 queue descriptor’s host index */
#define BIT_MQ6_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ6_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MQ7_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MQ7_HW_IDX               ((u32)0x00000FFF << 16)          /*!<R 0  MQ7 queue descriptor’s hardware index */
#define BIT_MQ7_HW_IDX(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_MQ7_HW_IDX(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_MQ7_HOST_IDX             ((u32)0x00000FFF << 0)          /*!<R/W 0  MQ7 queue descriptor’s host index */
#define BIT_MQ7_HOST_IDX(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_MQ7_HOST_IDX(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AXI_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_DEBUG_REG2                    ((u32)0x00000001 << 31)          /*!<R/W 0  Debug Selection Register */
#define BIT_DEBUG_REG1                    ((u32)0x00000001 << 30)          /*!<R/W 0  Write：hps_clkr_ahb[3] Read:Debug Selection Register */
#define BIT_MASK_HPS_CLKR_AHB             ((u32)0x00000003 << 28)          /*!<R/W 0  The Registers are used to prevent the deadlock whenever HW or FW turn off ANACLK or SYSCLK, which the HW or FW could not resume by themself. Currently they are not used in normal control flow. They might be helpful when Driver would like to handle whole Suspned & Resume flow by itself hps_clkr_ahb[2:1] */
#define BIT_HPS_CLKR_AHB(x)               ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_HPS_CLKR_AHB(x)           ((u32)(((x >> 28) & 0x00000003)))
#define BIT_INT_WLMAC                     ((u32)0x00000001 << 27)          /*!<R/W 0  Write:hps_clkr_ahb[0] Read:Interrupt.Write clear. */
#define BIT_MASK_DEBUG_REG0               ((u32)0x00000007 << 24)          /*!<R/W 0  Write:Debug Selection Register Read:3'b000 */
#define BIT_DEBUG_REG0(x)                 ((u32)(((x) & 0x00000007) << 24))
#define BIT_GET_DEBUG_REG0(x)             ((u32)(((x >> 24) & 0x00000007)))
#define BIT_MASK_DEBUG_REG3               ((u32)0x00FFFFFF << 0)          /*!<R/W 4h  Debug Selection Register */
#define BIT_DEBUG_REG3(x)                 ((u32)(((x) & 0x00FFFFFF) << 0))
#define BIT_GET_DEBUG_REG3(x)             ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DRVIER_TO_FIRMWARE_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_DRV2FW_AHB               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  DRIVER TO FIRMWARE MESSAGE */
#define BIT_DRV2FW_AHB(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_DRV2FW_AHB(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_FIRMWARE_TO_DRIVER_INFO
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_FW2DRV_AHB               ((u32)0xFFFFFFFF << 0)          /*!<R 0  FIRMWARE TO DRIVER MESSAGE */
#define BIT_FW2DRV_AHB(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_FW2DRV_AHB(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AXI_INTERRUPT
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_AHB_ISR                  ((u32)0xFFFFFFFF << 0)          /*!<RW1C 0  [0]: setrxok, rx dma ok [1]: set_rdu, rx descriptor unavailable [2]: setvodok, vo queue ok [3]: setvidok, vi queue ok [4]: setbedok, be queue ok [5]: setbkdok, bk queue ok [6]: setmgdok, manage queue ok [7]: seth2cdok, h2c queue ok [8]: sethighdok, high queue ok [15:9]: resvered [16]: setbcndok0, beacon queue ok, mbssid=0 [17]: setbcndok1, beacon queue ok, mbssid=1 [18]: setbcndok2, beacon queue ok, mbssid=2 [19]: setbcndok3, beacon queue ok, mbssid=3 [20]: setbcndok4, beacon queue ok, mbssid=4 [21]: setbcndok5, beacon queue ok, mbssid=5 [22]: setbcndok6, beacon queue ok, mbssid=6 [23]: setbcndok7, beacon queue ok, mbssid=7 [24]: setbcnderr0, beacon queue error, mbssid=0 [25]: setbcnderr1, beacon queue error, mbssid=1 [26]: setbcnderr2, beacon queue error, mbssid=2 [27]: setbcnderr3, beacon queue error, mbssid=3 [28]: setbcnderr4, beacon queue error, mbssid=4 [29]: setbcnderr5, beacon queue error, mbssid=5 [30]: setbcnderr6, beacon queue error, mbssid=6 [31]: setbcnderrk7, beacon queue error, mbssid=7 */
#define BIT_AHB_ISR(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_AHB_ISR(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AXI_INTERRUPT_MASK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_AHB_IMR                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  interrupt mask of axi_top */
#define BIT_AHB_IMR(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_AHB_IMR(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_TRXDMA_STUCK
 * @brief
 * @{
 *****************************************************************************/
#define BIT_RXDMA_STUCK                   ((u32)0x00000001 << 9)          /*!<R 0  rx dma stuck */
#define BIT_TXDMA_STUCK                   ((u32)0x00000001 << 8)          /*!<R 0  tx dma stuck */
#define BIT_MST_BUSY                      ((u32)0x00000001 << 3)          /*!<R 0  axi master write/read busy */
#define BIT_RXBUF_ZERO_ERR                ((u32)0x00000001 << 2)          /*!<RW1C 0  rx pkt size equal to 0 when caculate buffer size */
#define BIT_HD_SIZE_ERR                   ((u32)0x00000001 << 1)          /*!<RW1C 0  tx dma header size less than TXDESC_SIZE(40d) */
#define BIT_EN_DBG_STUCK                  ((u32)0x00000001 << 0)          /*!<R/W 0  enable trx dma stuck */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LBC_WATCHDOG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_WATCH_DOG_RECORD         ((u32)0x00003FFF << 10)          /*!<R 0  Time out register address */
#define BIT_WATCH_DOG_RECORD(x)           ((u32)(((x) & 0x00003FFF) << 10))
#define BIT_GET_WATCH_DOG_RECORD(x)       ((u32)(((x >> 10) & 0x00003FFF)))
#define BIT_R_IO_TIMEOUT_FLAG             ((u32)0x00000001 << 9)          /*!<RW1C 0  Lbc timeout flag.Write “1” to clear. */
#define BIT_EN_WATCH_DOG                  ((u32)0x00000001 << 8)          /*!<R/W 0  Enable lbc timeout watchdog */
/** @} */

/**************************************************************************//**
 * @defgroup REG_ENABLE_DUMMY_READ
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_DEBUG                  ((u32)0x00000003 << 22)          /*!<W 0  Debug Selection Register */
#define BIT_R_DEBUG(x)                    ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_R_DEBUG(x)                ((u32)(((x >> 22) & 0x00000003)))
#define BIT_EN_OUT_STD                    ((u32)0x00000001 << 6)          /*!<W 1h  cpl multi tag enable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_H2CQ_TXBD_DESA
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_H2CQDESA                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Buffer Descriptor Base Address for H2C Queue */
#define BIT_H2CQDESA(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_H2CQDESA(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_H2CQ_TXBD_NUM
 * @brief
 * @{
 *****************************************************************************/
#define BIT_H2CQ_FLAG                     ((u32)0x00000001 << 14)          /*!<R 0  H2CQ, read result will be H2CQflag */
#define BIT_MASK_H2CQ_DESC_MODE           ((u32)0x00000003 << 12)          /*!<R/W 0  H2CQ descriptor mode : 0 : 2 segments each descriptor 1 : 4 segments each descriptor 2 : 8 segments each descriptor */
#define BIT_H2CQ_DESC_MODE(x)             ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_H2CQ_DESC_MODE(x)         ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_H2CQDES_NUM              ((u32)0x00000FFF << 0)          /*!<R/W 100h  H2C queue descriptor's number */
#define BIT_H2CQDES_NUM(x)                ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_H2CQDES_NUM(x)            ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_H2CQ_TXBD_IDX
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_H2CQ_HW_IDX              ((u32)0x00000FFF << 16)          /*!<R 0  H2C queue descriptor’s hardware index */
#define BIT_H2CQ_HW_IDX(x)                ((u32)(((x) & 0x00000FFF) << 16))
#define BIT_GET_H2CQ_HW_IDX(x)            ((u32)(((x >> 16) & 0x00000FFF)))
#define BIT_MASK_H2CQ_HOST_IDX            ((u32)0x00000FFF << 0)          /*!<R/W 0  H2C queue descriptor’s host index */
#define BIT_H2CQ_HOST_IDX(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_H2CQ_HOST_IDX(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_H2CQ_CSR
 * @brief
 * @{
 *****************************************************************************/
#define BIT_H2CQ_FULL                     ((u32)0x00000001 << 31)          /*!<RW1C 0  Indicate H2CQ full. Write 1 to clear the bit. */
#define BIT_CLR_H2CQ_HOST_IDX             ((u32)0x00000001 << 2)          /*!<W 0  Write 1 to clear H2CQ host index, and then hardware auto clear the bit */
#define BIT_CLR_H2CQ_HW_IDX               ((u32)0x00000001 << 1)          /*!<W 0  Write 1 to clear H2CQ hardware index, and then hardware auto clear the bit */
#define BIT_STOPH2C                       ((u32)0x00000001 << 0)          /*!<R/W 0  Stop h2c queue dma */
/** @} */


/*==========PAGE3 Register Address Definition==========*/
#define REG_AXI_CTRL                                 0x0300
#define REG_INT_MIG                                  0x0304
#define REG_BCNQ_TXBD_DESA                           0x0308
#define REG_MGQ_TXBD_DESA                            0x0310
#define REG_VOQ_TXBD_DESA                            0x0318
#define REG_VIQ_TXBD_DESA                            0x0320
#define REG_BEQ_TXBD_DESA                            0x0328
#define REG_BKQ_TXBD_DESA                            0x0330
#define REG_RXQ_RXBD_DESA                            0x0338
#define REG_MQ0_TXBD_DESA                            0x0340
#define REG_MQ1_TXBD_DESA                            0x0348
#define REG_MQ2_TXBD_DESA                            0x0350
#define REG_MQ3_TXBD_DESA                            0x0358
#define REG_MQ4_TXBD_DESA                            0x0360
#define REG_MQ5_TXBD_DESA                            0x0368
#define REG_MQ6_TXBD_DESA                            0x0370
#define REG_MQ7_TXBD_DESA                            0x0378
#define REG_MGQ_TXBD_RX_RXBD_NUM                     0x0380
#define REG_VOQ_VIQ_TXBD_NUM                         0x0384
#define REG_BEQ_BKQ_TXBD_NUM                         0x0388
#define REG_MQ0_MQ1_TXBD_NUM                         0x038C
#define REG_MQ2_MQ3_TXBD_NUM                         0x0390
#define REG_MQ4_MQ5_TXBD_NUM                         0x0394
#define REG_MQ6_MQ7_TXBD_NUM                         0x0398
#define REG_BD_RWPTR_CLR                             0x039C
#define REG_VOQ_TXBD_IDX                             0x03A0
#define REG_VIQ_TXBD_IDX                             0x03A4
#define REG_BEQ_TXBD_IDX                             0x03A8
#define REG_BKQ_TXBD_IDX                             0x03AC
#define REG_MGQ_TXBD_IDX                             0x03B0
#define REG_RXQ_RXBD_IDX                             0x03B4
#define REG_MQ0_TXBD_IDX                             0x03B8
#define REG_MQ1_TXBD_IDX                             0x03BC
#define REG_MQ2_TXBD_IDX                             0x03C0
#define REG_MQ3_TXBD_IDX                             0x03C4
#define REG_MQ4_TXBD_IDX                             0x03C8
#define REG_MQ5_TXBD_IDX                             0x03CC
#define REG_MQ6_TXBD_IDX                             0x03D0
#define REG_MQ7_TXBD_IDX                             0x03D4
#define REG_AXI_CTRL2                                0x03D8
#define REG_AXI_HRPWM2_CPWM2                         0x03DC
#define REG_DRVIER_TO_FIRMWARE_INFO                  0x03E0
#define REG_FIRMWARE_TO_DRIVER_INFO                  0x03E4
#define REG_AXI_INTERRUPT                            0x03E8
#define REG_AXI_INTERRUPT_MASK                       0x03EC
#define REG_TRXDMA_STUCK                             0x03F0
#define REG_LBC_WATCHDOG                             0x03F8
#define REG_ENABLE_DUMMY_READ                        0x03FC
#define REG_H2CQ_TXBD_DESA                           0x1300
#define REG_H2CQ_TXBD_NUM                            0x1304
#define REG_H2CQ_TXBD_IDX                            0x1308
#define REG_H2CQ_CSR                                 0x130C

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
