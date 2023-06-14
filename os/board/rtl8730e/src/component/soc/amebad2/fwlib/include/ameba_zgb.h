#ifndef _AMEBAD2_ZGB_H_
#define _AMEBAD2_ZGB_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup ZGB
  * @brief ZGB driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup ZGB_Register_Definitions ZGB Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup ZGB_FUN_EN
 * @brief Zigbee Function enable
 * @{
 **/
#define ZGB_BIT_R_ZGB_RX_EN         ((u32)0x00000001 << 1)          /*!<R/W 0  zigbee mac rx enable */
#define ZGB_BIT_R_ZGB_TX_EN         ((u32)0x00000001 << 0)          /*!<R/W 0  zigbee mac tx enable */
/** @} */

/** @defgroup ZGB_TXBUF_CTRL
 * @brief Zigbee Tx buffer control
 * @{
 **/
#define ZGB_MASK_R_TXBUF_EMPTY_TH   ((u32)0x000007FF << 12)          /*!<R/W 140h  when zigbee txbuf empty entry is large than the r_txbuf_empty_th, interrupt is issued to CPU half size of tx buffer:320,unit is 4byte */
#define ZGB_R_TXBUF_EMPTY_TH(x)     ((u32)(((x) & 0x000007FF) << 12))
#define ZGB_GET_R_TXBUF_EMPTY_TH(x) ((u32)(((x >> 12) & 0x000007FF)))
#define ZGB_MASK_R_ZGB_TXDMA_TH     ((u32)0x000007FF << 1)          /*!<R/W 140h  when r_zgb_txdma_en is 1 and zigbee txbuf empty entry is large than the r_zgb_txdma_th, dma_tx_req is raised to GDMA half size of tx buffer:320,unit is 4byte */
#define ZGB_R_ZGB_TXDMA_TH(x)       ((u32)(((x) & 0x000007FF) << 1))
#define ZGB_GET_R_ZGB_TXDMA_TH(x)   ((u32)(((x >> 1) & 0x000007FF)))
#define ZGB_BIT_R_ZGB_TXDMA_EN      ((u32)0x00000001 << 0)          /*!<R/W 0  GDMA enable for Zigbee Txbuf */
/** @} */

/** @defgroup ZGB_RXBUF_CTRL
 * @brief Zigbee Rx buffer control
 * @{
 **/
#define ZGB_MASK_R_RXBUF_FULL_TH    ((u32)0x000003FF << 11)          /*!<R/W C0h  when zigbee rxbuf valid entry is large than the r_rxbuf_full_th, interrupt is issued to CPU half size of tx buffer:192,unit is 4byte */
#define ZGB_R_RXBUF_FULL_TH(x)      ((u32)(((x) & 0x000003FF) << 11))
#define ZGB_GET_R_RXBUF_FULL_TH(x)  ((u32)(((x >> 11) & 0x000003FF)))
#define ZGB_MASK_R_ZGB_RXDMA_TH     ((u32)0x000003FF << 1)          /*!<R/W C0h  when r_zgb_rxdma_en is 1 and zigbee rxbuf valid entry is large than the r_zgb_rxdma_th, dma_rx_req is raised to GDMA half size of tx buffer:192,unit is 4byte */
#define ZGB_R_ZGB_RXDMA_TH(x)       ((u32)(((x) & 0x000003FF) << 1))
#define ZGB_GET_R_ZGB_RXDMA_TH(x)   ((u32)(((x >> 1) & 0x000003FF)))
#define ZGB_BIT_R_ZGB_RXDMA_EN      ((u32)0x00000001 << 0)          /*!<R/W 0  GDMA enable for Zigbee Rxbuf */
/** @} */

/** @defgroup ZGB_ISR0
 * @brief Zigbee Interrupt0 status
 * @{
 **/
#define ZGB_MASK_R_ZGB_ISR0         ((u32)0xFFFFFFFF << 0)          /*!<RW1CB 0  interrupt status register of zigbee [31],WLAN2ZIGBEE_MAILBOX_INT, From Mailbox controller, 指示当前wlan fw 有向 mailbox(wlan->zigbee)写入信息 [30],BT_RX_STOP_INT, From PTA, BIT_BT_TX_START=(pre_gnt_bt_rx==1) &&(cur_gnt_bt_rx==0) [29],BT_RX_START_INT, From PTA, BIT_BT_TX_START=(pre_gnt_bt_rx==0) &&(cur_gnt_bt_rx==1) [28],BT_TX_STOP_INT, From PTA, BIT_BT_TX_START=(pre_gnt_bt_tx==1) &&(cur_gnt_bt_tx==0) [27],BT_TX_START_INT, From PTA, BIT_BT_TX_START=(pre_gnt_bt_tx==0) &&(cur_gnt_bt_tx==1) [26],WLAN_RX_STOP_INT, From wmac, BIT_WLAN_RX_START=(pre_wl_cca==1)&&(cur_wl_cca==0) [25],WLAN_RX_START_INT, From wmac, BIT_WLAN_RX_START=(pre_wl_cca==0)&&(cur_wl_cca==1), Zigbee 收到该中断后,可以选择是否要中断当前封包TX or RX [24],WLAN_TX_STOP_INT, From wmac, BIT_WLAN_TX_START=(pre_wl_mactxen==1)&&(cur_wl_mactxen=0) [23],WLAN_TX_START_INT, From wmac, BIT_WLAN_TX_START=(pre_wl_mactxen==0)&&(cur_wl_mactxen=1), Zigbee 收到该中断后,可以选择是否要中断当前封包TX or RX [22],DIS_GNT_ZIGBEE_TX_INT, From PTA, BIT_DIS_GNT_ZIGBEE_TX=(pre_gnt_zigbee_tx==1)&& (cur_gnt_zigbee_tx==0) [21],GNT_ZIGBEE_TX_INT, From PTA, BIT_GNT_ZIGBEE_TX=(pre_gnt_zigbee_tx==0)&& (cur_gnt_zigbee_tx==1) [20:15],dummy [14],INTER_ZIGBEE_RFGAIN_CHANGED, zigbee_rfgain_changed posedge pulse [13],INTER_ZIGBEE_AAGC_DONE, zigbee_aagcrdy posedge pulse [12],rxbuf_underflow, when read rxbuf and txbuf is empty [11],rxbuf_empty, when rxbuf is empty [10],rxbuf_overflow, when write rxbuf and rxbuf is full [9],rxbuf_full, when rxbuf is full [8],INTER_ZIGBEE_RXBUF_ALMOST_FULL, when zigbee rxbuf valid entry is large than the r_rxbuf_full_th [7:6],dummy [5],INTER_ZIGBEE_TX_STOP, zigbee_rftxen negedege pulse [4],txbuf_overflow, when wirte txbuf and txbuf is full [3],txbuf_full, when txbuf is full [2],txbuf_underflow, when read txbuf and txbuf is empty [1],txbuf_empty, when txbuf is empty [0],INTER_ZIGBEE_TXBUF_ALMOST_EMPTY, when zigbee txbuf empty entry is large than the r_txbuf_empty_th */
#define ZGB_R_ZGB_ISR0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ZGB_GET_R_ZGB_ISR0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ZGB_IMR0
 * @brief Zigbee Interrupt0 mask
 * @{
 **/
#define ZGB_MASK_R_ZGB_IMR0         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  interrupt mask register of zigbee */
#define ZGB_R_ZGB_IMR0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ZGB_GET_R_ZGB_IMR0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ZGB_TRXBUF_INFO
 * @brief Zigbee Tx/Rx buffer information
 * @{
 **/
#define ZGB_MASK_TXBUF_EMPTY_ENTRY  ((u32)0x000007FF << 10)          /*!<R 0  empty entry size of txbuf, unit is 4byte */
#define ZGB_TXBUF_EMPTY_ENTRY(x)    ((u32)(((x) & 0x000007FF) << 10))
#define ZGB_GET_TXBUF_EMPTY_ENTRY(x) ((u32)(((x >> 10) & 0x000007FF)))
#define ZGB_MASK_RXBUF_VALID_ENTRY  ((u32)0x000003FF << 0)          /*!<R 0  valid entry size of rxbuf, unit is 4byte */
#define ZGB_RXBUF_VALID_ENTRY(x)    ((u32)(((x) & 0x000003FF) << 0))
#define ZGB_GET_RXBUF_VALID_ENTRY(x) ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup ZGB_TRX_REQ
 * @brief Zigbee Tx/Rx/pri request
 * @{
 **/
#define ZGB_BIT_TX_REQ              ((u32)0x00000001 << 2)          /*!<R/W 0  zigbee tx request */
#define ZGB_BIT_RX_REQ              ((u32)0x00000001 << 1)          /*!<R/W 0  zigbee rx request */
#define ZGB_BIT_PRI_REQ             ((u32)0x00000001 << 0)          /*!<R/W 0  zigbee priority request */
/** @} */

/** @defgroup ZGB_ISR1
 * @brief Zigbee Interrupt1 status
 * @{
 **/
#define ZGB_MASK_R_ZGB_ISR1         ((u32)0xFFFFFFFF << 0)          /*!<RW1CB 0  interrupt status register of zigbee [31:8],dummy [7:0],INTER_ZIGBEE_INTR_O, zigbee_intr_o[7:0] posedge pulse */
#define ZGB_R_ZGB_ISR1(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ZGB_GET_R_ZGB_ISR1(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup ZGB_IMR1
 * @brief Zigbee Interrupt1 mask
 * @{
 **/
#define ZGB_MASK_R_ZGB_IMR1         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  interrupt mask register of zigbee */
#define ZGB_R_ZGB_IMR1(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define ZGB_GET_R_ZGB_IMR1(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup ZGB_Exported_Types ZGB Exported Types
  * @{
*****************************************************************************/


/**
 * @brief ZGB Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t ZGB_FUN_EN;                             /*!< ZIGBEE FUNCTION ENABLE Register,  Address offset: 0x000 */
	__IO uint32_t ZGB_TXBUF_CTRL;                         /*!< ZIGBEE TX BUFFER CONTROL Register,  Address offset: 0x004 */
	__IO uint32_t ZGB_RXBUF_CTRL;                         /*!< ZIGBEE RX BUFFER CONTROL Register,  Address offset: 0x008 */
	__IO uint32_t ZGB_ISR0;                               /*!< ZIGBEE INTERRUPT0 STATUS Register,  Address offset: 0x00C */
	__IO uint32_t ZGB_IMR0;                               /*!< ZIGBEE INTERRUPT0 MASK Register,  Address offset: 0x010 */
	__IO uint32_t ZGB_TRXBUF_INFO;                        /*!< ZIGBEE TX/RX BUFFER INFORMATION Register,  Address offset: 0x014 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x18 */
	__IO uint32_t ZGB_TRX_REQ;                            /*!< ZIGBEE TX/RX/PRI REQUEST Register,  Address offset: 0x01C */
	__IO uint32_t ZGB_ISR1;                               /*!< ZIGBEE INTERRUPT1 STATUS Register,  Address offset: 0x020 */
	__IO uint32_t ZGB_IMR1;                               /*!< ZIGBEE INTERRUPT1 MASK Register,  Address offset: 0x024 */
} ZGB_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
