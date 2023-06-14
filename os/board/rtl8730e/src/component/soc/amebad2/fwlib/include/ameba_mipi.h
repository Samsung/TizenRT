#ifndef _AMEBAD2_MIPI_H_
#define _AMEBAD2_MIPI_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup MIPI
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup MIPI_Register_Definitions MIPI Register Definitions
 * @{
 **/

/**
 * @defgroup MIPI_MAIN_CTRL
 * @brief
 * @{
 **/
#define MIPI_BIT_DPHY_ISO_CTRL                      ((u32)0x00000001 << 27)          /*!<R/W ‘h1  DPHY isolation function control 1: isolation mode 0: normal mode Related to DPHY port: BY_PASS_ON_MIPI */
#define MIPI_BIT_APHY_PWDB                          ((u32)0x00000001 << 26)          /*!<R/W ‘h0  APHY core power down signal for isolation */
#define MIPI_BIT_APHY_RSTB                          ((u32)0x00000001 << 25)          /*!<R/W ‘h0  APHY reset signal */
#define MIPI_BIT_DPHY_TX_RSTN                       ((u32)0x00000001 << 24)          /*!<R/W ‘h0  DPHY external reset */
#define MIPI_BIT_RX_ECC_EN                          ((u32)0x00000001 << 22)          /*!<R/W ‘h0  RX ECC check and correction enable */
#define MIPI_BIT_RX_CRC_EN                          ((u32)0x00000001 << 21)          /*!<R/W ‘h0  RX CRC check enable */
#define MIPI_BIT_TURNDISABLE                        ((u32)0x00000001 << 20)          /*!<R/W ‘h0  Disable Bus Turn Around (BTA) function. */
#define MIPI_BIT_TX_CRC_EN                          ((u32)0x00000001 << 16)          /*!<R/W ‘h0  TX CRC calculation enable */
#define MIPI_BIT_EOTP_EN                            ((u32)0x00000001 << 12)          /*!<R/W ‘h0  EOT packet enable */
#define MIPI_MASK_VID_FORMAT                        ((u32)0x00000003 << 8)          /*!<R/W ‘h0  Video stream data format 2’b00: 24bpp (8, 8, 8 RGB) 2’b01: 16bpp (5, 6, 5 RGB) 2’b10: 18bpp (6, 6, 6 RGB) packed 2’b11: 18bpp (6, 6, 6, RGB) loosely packed */
#define MIPI_VID_FORMAT(x)                          ((u32)(((x) & 0x00000003) << 8))
#define MIPI_GET_VID_FORMAT(x)                      ((u32)(((x >> 8) & 0x00000003)))
#define MIPI_MASK_LANE_NUM                          ((u32)0x00000003 << 4)          /*!<R/W ‘h0  Lane number setting 2’b00: 1-lane 2’b01: 2-lane 2’b10: 3-lane 2’b11: 4-lane */
#define MIPI_LANE_NUM(x)                            ((u32)(((x) & 0x00000003) << 4))
#define MIPI_GET_LANE_NUM(x)                        ((u32)(((x >> 4) & 0x00000003)))
#define MIPI_BIT_LPRX_RST                           ((u32)0x00000001 << 3)          /*!<R/W ‘h0  Async SW reset for LPRX function, set 1’b1 to reset LPRX function. */
#define MIPI_BIT_LPTX_RST                           ((u32)0x00000001 << 2)          /*!<R/W ‘h0  Async SW reset for LPTX function, set 1’b1 to reset LPTX function. */
#define MIPI_BIT_HSTX_RST                           ((u32)0x00000001 << 1)          /*!<R/W ‘h0  Async SW reset for HSTX function, set 1’b1 to reset HSTX function. */
#define MIPI_BIT_DSI_MODE                           ((u32)0x00000001 << 0)          /*!<R/W ‘h0  DSI mode selection 0: command mode 1: video mode (shall be set after timing contrl register) */
/** @} */

/**
 * @defgroup MIPI_CKLANE_CTRL
 * @brief
 * @{
 **/
#define MIPI_MASK_FORCETXSTOPMODE                   ((u32)0x0000000F << 12)          /*!<R/W ‘h0  Force TX STOP */
#define MIPI_FORCETXSTOPMODE(x)                     ((u32)(((x) & 0x0000000F) << 12))
#define MIPI_GET_FORCETXSTOPMODE(x)                 ((u32)(((x >> 12) & 0x0000000F)))
#define MIPI_BIT_FORCERXMODE_CH0                    ((u32)0x00000001 << 8)          /*!<R/W ‘h0  Force LPRX mode */
#define MIPI_BIT_TXULPSEXIT_CLK                     ((u32)0x00000001 << 5)          /*!<R/W ‘h0  Clock ULPS mode exit */
#define MIPI_BIT_TXULPS_CLK                         ((u32)0x00000001 << 4)          /*!<R/W ‘h0  Clock ULPS mode enable */
#define MIPI_MASK_TXSKEWCALHS                       ((u32)0x0000000F << 0)          /*!<R/W ‘h0  Skew Calibration */
#define MIPI_TXSKEWCALHS(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define MIPI_GET_TXSKEWCALHS(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup MIPI_LANE_CTRL
 * @brief
 * @{
 **/
#define MIPI_BIT_TXULPSEXIT_CH3                     ((u32)0x00000001 << 29)          /*!<R/W ‘h0   */
#define MIPI_BIT_TXULPSESC_CH3                      ((u32)0x00000001 << 28)          /*!<R/W ‘h0   */
#define MIPI_MASK_TXTRIGGERESC_CH3                  ((u32)0x0000000F << 24)          /*!<R/W ‘h0   */
#define MIPI_TXTRIGGERESC_CH3(x)                    ((u32)(((x) & 0x0000000F) << 24))
#define MIPI_GET_TXTRIGGERESC_CH3(x)                ((u32)(((x >> 24) & 0x0000000F)))
#define MIPI_BIT_TXULPSEXIT_CH2                     ((u32)0x00000001 << 21)          /*!<R/W ‘h0   */
#define MIPI_BIT_TXULPSESC_CH2                      ((u32)0x00000001 << 20)          /*!<R/W ‘h0   */
#define MIPI_MASK_TXTRIGGERESC_CH2                  ((u32)0x0000000F << 16)          /*!<R/W ‘h0   */
#define MIPI_TXTRIGGERESC_CH2(x)                    ((u32)(((x) & 0x0000000F) << 16))
#define MIPI_GET_TXTRIGGERESC_CH2(x)                ((u32)(((x >> 16) & 0x0000000F)))
#define MIPI_BIT_TXULPSEXIT_CH1                     ((u32)0x00000001 << 13)          /*!<R/W ‘h0   */
#define MIPI_BIT_TXULPSESC_CH1                      ((u32)0x00000001 << 12)          /*!<R/W ‘h0   */
#define MIPI_MASK_TXTRIGGERESC_CH1                  ((u32)0x0000000F << 8)          /*!<R/W ‘h0   */
#define MIPI_TXTRIGGERESC_CH1(x)                    ((u32)(((x) & 0x0000000F) << 8))
#define MIPI_GET_TXTRIGGERESC_CH1(x)                ((u32)(((x >> 8) & 0x0000000F)))
#define MIPI_BIT_TXULPSEXIT_CH0                     ((u32)0x00000001 << 5)          /*!<R/W ‘h0   */
#define MIPI_BIT_TXULPSESC_CH0                      ((u32)0x00000001 << 4)          /*!<R/W ‘h0   */
#define MIPI_MASK_TXTRIGGERESC_CH0                  ((u32)0x0000000F << 0)          /*!<R/W ‘h0   */
#define MIPI_TXTRIGGERESC_CH0(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define MIPI_GET_TXTRIGGERESC_CH0(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup MIPI_INTE
 * @brief
 * @{
 **/
#define MIPI_BIT_INTP_EN_ACPU                       ((u32)0x00000001 << 2)          /*!<R/W ‘b0  Set 1’b1 to enable DSI ACPU interrupt */
#define MIPI_BIT_INTP_EN                            ((u32)0x00000001 << 1)          /*!<R/W ‘b0  Set 1’b1 to enable DSI SCPU interrupt */
#define MIPI_BIT_SEL                                ((u32)0x00000001 << 0)          /*!<R/W ‘b0  Select which CPU to interrupt 0: SCPU 1: ACPU */
/** @} */

/**
 * @defgroup MIPI_INTS
 * @brief
 * @{
 **/
#define MIPI_BIT_HSTX_TIMEOUT                       ((u32)0x00000001 << 10)          /*!<R/W ‘b0  HS Tx mode, timeout */
#define MIPI_BIT_LPTX_TIMEOUT                       ((u32)0x00000001 << 9)          /*!<R/W ‘b0  LP Tx mode, timeout */
#define MIPI_BIT_LPRX_TIMEOUT                       ((u32)0x00000001 << 8)          /*!<R/W ‘b0  LP Rx mode, timeout */
#define MIPI_BIT_RX_TRIGGER                         ((u32)0x00000001 << 7)          /*!<R/W ‘b0  Received Rx trigger signal in DPHY_STATUS1 */
#define MIPI_BIT_ERROR                              ((u32)0x00000001 << 6)          /*!<R/W ‘b0  Error report in register DPHY_ERR */
#define MIPI_BIT_RCMD3                              ((u32)0x00000001 << 5)          /*!<R/W ‘b0  Receive command mode response at RCMD3 */
#define MIPI_BIT_RCMD2                              ((u32)0x00000001 << 4)          /*!<R/W ‘b0  Receive command mode response at RCMD2 */
#define MIPI_BIT_RCMD1                              ((u32)0x00000001 << 3)          /*!<R/W ‘b0  Receive command mode response at RCMD1 */
#define MIPI_BIT_CMD_TXDONE                         ((u32)0x00000001 << 2)          /*!<R/W ‘b0  Command mode Tx done */
#define MIPI_BIT_VID_DONE                           ((u32)0x00000001 << 1)          /*!<R/W ‘b0  Video mode frame Tx done */
#define MIPI_BIT_WRITE_DATA                         ((u32)0x00000001 << 0)          /*!<W ‘b0   */
/** @} */

/**
 * @defgroup MIPI_INTS_ACPU
 * @brief
 * @{
 **/
#define MIPI_BIT_FRAME_DONE                         ((u32)0x00000001 << 1)          /*!<R/W ‘b0  Video mode frame done */
#define MIPI_BIT_WRITE_DATA                         ((u32)0x00000001 << 0)          /*!<W ‘b0   */
/** @} */

/**
 * @defgroup MIPI_RCMD_x
 * @brief
 * @{
 **/
#define MIPI_BIT_RCMDx_CRC_CHK_ERR                  ((u32)0x00000001 << 27)          /*!<R ‘h0  RCMD CRC check error */
#define MIPI_BIT_RCMDx_ECC_CHK_ERR                  ((u32)0x00000001 << 26)          /*!<R ‘h0  RCMD ECC check error can’t be fixed. */
#define MIPI_BIT_RCMDx_ECC_ERR_FIX                  ((u32)0x00000001 << 25)          /*!<R ‘h0  RCMD ECC check error but fixed */
#define MIPI_BIT_RCMDx_ECC_NO_ERR                   ((u32)0x00000001 << 24)          /*!<R ‘h0  RCMD ECC check error free */
#define MIPI_MASK_RCMDx_BYTE1                       ((u32)0x000000FF << 16)          /*!<R ‘h0  Short command/long command header */
#define MIPI_RCMDx_BYTE1(x)                         ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_RCMDx_BYTE1(x)                     ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_RCMDx_BYTE0                       ((u32)0x000000FF << 8)          /*!<R ‘h0  Short command/long command header */
#define MIPI_RCMDx_BYTE0(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_RCMDx_BYTE0(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_RCMDx_DATAID                      ((u32)0x000000FF << 0)          /*!<R ‘h0  Short command/long command header */
#define MIPI_RCMDx_DATAID(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_RCMDx_DATAID(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_TC0
 * @brief
 * @{
 **/
#define MIPI_MASK_HSA                               ((u32)0x00001FFF << 16)          /*!<R/W ‘h0  Set Hsync pulse width in bytes */
#define MIPI_HSA(x)                                 ((u32)(((x) & 0x00001FFF) << 16))
#define MIPI_GET_HSA(x)                             ((u32)(((x >> 16) & 0x00001FFF)))
#define MIPI_MASK_HACT                              ((u32)0x00001FFF << 0)          /*!<R/W ‘h0  Set video active width in bytes */
#define MIPI_HACT(x)                                ((u32)(((x) & 0x00001FFF) << 0))
#define MIPI_GET_HACT(x)                            ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**
 * @defgroup MIPI_TC1
 * @brief
 * @{
 **/
#define MIPI_MASK_HBP                               ((u32)0x00001FFF << 16)          /*!<R/W ‘h0  Set Horizontal Back Porch in bytes */
#define MIPI_HBP(x)                                 ((u32)(((x) & 0x00001FFF) << 16))
#define MIPI_GET_HBP(x)                             ((u32)(((x >> 16) & 0x00001FFF)))
#define MIPI_MASK_HFP                               ((u32)0x00001FFF << 0)          /*!<R/W ‘h0  Set Horizontal Front Porch in bytes */
#define MIPI_HFP(x)                                 ((u32)(((x) & 0x00001FFF) << 0))
#define MIPI_GET_HFP(x)                             ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/**
 * @defgroup MIPI_TC2
 * @brief
 * @{
 **/
#define MIPI_MASK_VSA                               ((u32)0x000007FF << 16)          /*!<R/W ‘h0  Set Vsync pulse width in Line */
#define MIPI_VSA(x)                                 ((u32)(((x) & 0x000007FF) << 16))
#define MIPI_GET_VSA(x)                             ((u32)(((x >> 16) & 0x000007FF)))
#define MIPI_MASK_VACT                              ((u32)0x000007FF << 0)          /*!<R/W ‘h0  Set video active scan line numbers */
#define MIPI_VACT(x)                                ((u32)(((x) & 0x000007FF) << 0))
#define MIPI_GET_VACT(x)                            ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**
 * @defgroup MIPI_TC3
 * @brief
 * @{
 **/
#define MIPI_MASK_VBP                               ((u32)0x000007FF << 16)          /*!<R/W ‘h0  Set Vertical Back Porch in Line */
#define MIPI_VBP(x)                                 ((u32)(((x) & 0x000007FF) << 16))
#define MIPI_GET_VBP(x)                             ((u32)(((x >> 16) & 0x000007FF)))
#define MIPI_MASK_VFP                               ((u32)0x000007FF << 0)          /*!<R/W ‘h0  Set Vertical Front Porch in Line */
#define MIPI_VFP(x)                                 ((u32)(((x) & 0x000007FF) << 0))
#define MIPI_GET_VFP(x)                             ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**
 * @defgroup MIPI_TC4
 * @brief
 * @{
 **/
#define MIPI_MASK_CRC_LEN                           ((u32)0x0000000F << 24)          /*!<R/W ‘h2  Set HS mode CRC length in bytes */
#define MIPI_CRC_LEN(x)                             ((u32)(((x) & 0x0000000F) << 24))
#define MIPI_GET_CRC_LEN(x)                         ((u32)(((x >> 24) & 0x0000000F)))
#define MIPI_MASK_HEADER_LEN                        ((u32)0x0000000F << 20)          /*!<R/W ‘h4  Set HS mode Header length in bytes */
#define MIPI_HEADER_LEN(x)                          ((u32)(((x) & 0x0000000F) << 20))
#define MIPI_GET_HEADER_LEN(x)                      ((u32)(((x >> 20) & 0x0000000F)))
#define MIPI_BIT_ACTIVE_BURST_EN                    ((u32)0x00000001 << 17)          /*!<R/W ‘h0  Send active data in burst mode (send EOTP in active lines) */
#define MIPI_BIT_HSE_EN                             ((u32)0x00000001 << 16)          /*!<R/W ‘h0  Enable register for Hsync end event packet */
#define MIPI_MASK_BLLP_LEN                          ((u32)0x0000FFFF << 0)          /*!<R/W ‘h0  Set clock request timing in byteclk cycles */
#define MIPI_BLLP_LEN(x)                            ((u32)(((x) & 0x0000FFFF) << 0))
#define MIPI_GET_BLLP_LEN(x)                        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup MIPI_TC5
 * @brief
 * @{
 **/
#define MIPI_MASK_PIXEL_NUM                         ((u32)0x000007FF << 16)          /*!<R/W ‘h0  Set pixel numbers in pixel for VO PIF */
#define MIPI_PIXEL_NUM(x)                           ((u32)(((x) & 0x000007FF) << 16))
#define MIPI_GET_PIXEL_NUM(x)                       ((u32)(((x >> 16) & 0x000007FF)))
#define MIPI_MASK_LINE_TIME                         ((u32)0x0000FFFF << 0)          /*!<R/W ‘h0  Set HSTX line time in byteclk cycles */
#define MIPI_LINE_TIME(x)                           ((u32)(((x) & 0x0000FFFF) << 0))
#define MIPI_GET_LINE_TIME(x)                       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup MIPI_IDMA0
 * @brief
 * @{
 **/
#define MIPI_BIT_MEM_EN                             ((u32)0x00000001 << 16)          /*!<R/W ‘h0  In-direct memory access enable */
#define MIPI_BIT_MEM_WR                             ((u32)0x00000001 << 12)          /*!<R/W ‘h0  0: memory write 1: memory read */
#define MIPI_MASK_MEM_ADDR                          ((u32)0x000001FF << 0)          /*!<R/W ‘h0  In-direct memory access */
#define MIPI_MEM_ADDR(x)                            ((u32)(((x) & 0x000001FF) << 0))
#define MIPI_GET_MEM_ADDR(x)                        ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**
 * @defgroup MIPI_IDMA1
 * @brief
 * @{
 **/
#define MIPI_MASK_MEM_D0                            ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  Memory data bit[31:0] */
#define MIPI_MEM_D0(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_MEM_D0(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_IDMA2
 * @brief
 * @{
 **/
#define MIPI_MASK_MEM_D1                            ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  Memory data bit[63:32] */
#define MIPI_MEM_D1(x)                              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_MEM_D1(x)                          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_TO1
 * @brief
 * @{
 **/
#define MIPI_BIT_HSTX_TO_EN                         ((u32)0x00000001 << 31)          /*!<R/W ‘h0  HSTX timout enable */
#define MIPI_MASK_HSTX_TO                           ((u32)0x7FFFFFFF << 0)          /*!<R/W ‘h200  HSTX timeout */
#define MIPI_HSTX_TO(x)                             ((u32)(((x) & 0x7FFFFFFF) << 0))
#define MIPI_GET_HSTX_TO(x)                         ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_TO2
 * @brief
 * @{
 **/
#define MIPI_BIT_LPTX_TO_EN                         ((u32)0x00000001 << 31)          /*!<R/W ‘h0  LPTX timout enable */
#define MIPI_MASK_LPTX_TO                           ((u32)0x7FFFFFFF << 0)          /*!<R/W ‘h200  LPTX timeout */
#define MIPI_LPTX_TO(x)                             ((u32)(((x) & 0x7FFFFFFF) << 0))
#define MIPI_GET_LPTX_TO(x)                         ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_TO3
 * @brief
 * @{
 **/
#define MIPI_BIT_LPRX_TO_EN                         ((u32)0x00000001 << 31)          /*!<R/W ‘h0  LPRX timout enable */
#define MIPI_MASK_LPRX_TO                           ((u32)0x7FFFFFFF << 0)          /*!<R/W ‘h200  LPRX timeout */
#define MIPI_LPRX_TO(x)                             ((u32)(((x) & 0x7FFFFFFF) << 0))
#define MIPI_GET_LPRX_TO(x)                         ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_TO4
 * @brief
 * @{
 **/
#define MIPI_MASK_FIFO_RE_DLY                       ((u32)0x0000000F << 0)          /*!<R/W ‘h0  After FIFO_RE_DLY clock cycles delay, start to read FIFO to send HS data */
#define MIPI_FIFO_RE_DLY(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define MIPI_GET_FIFO_RE_DLY(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup MIPI_CMD_GO
 * @brief
 * @{
 **/
#define MIPI_BIT_CMD_GO                             ((u32)0x00000001 << 0)          /*!<R/W ‘h0  Set 1’b1 to start command transmission sequence */
/** @} */

/**
 * @defgroup MIPI_CMD0
 * @brief
 * @{
 **/
#define MIPI_MASK_CMD_BYTE1                         ((u32)0x000000FF << 16)          /*!<R/W ‘h0  Short command/long command header direct write */
#define MIPI_CMD_BYTE1(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_CMD_BYTE1(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_CMD_BYTE0                         ((u32)0x000000FF << 8)          /*!<R/W ‘h0  Short command/long command header direct write */
#define MIPI_CMD_BYTE0(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_CMD_BYTE0(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_CMD_DATAID                        ((u32)0x000000FF << 0)          /*!<R/W ‘h0  Short command/long command header direct write */
#define MIPI_CMD_DATAID(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_CMD_DATAID(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_DPHY_STATUS0
 * @brief
 * @{
 **/
#define MIPI_MASK_ULPSACTIVENOT                     ((u32)0x0000000F << 8)          /*!<R ‘h0  Data lane ULPS mode indicator */
#define MIPI_ULPSACTIVENOT(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define MIPI_GET_ULPSACTIVENOT(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define MIPI_MASK_STOPSTATE                         ((u32)0x0000000F << 4)          /*!<R ‘h0  Data lane Stop State indicator */
#define MIPI_STOPSTATE(x)                           ((u32)(((x) & 0x0000000F) << 4))
#define MIPI_GET_STOPSTATE(x)                       ((u32)(((x >> 4) & 0x0000000F)))
#define MIPI_BIT_ULPSACTIVENOT_CLK                  ((u32)0x00000001 << 3)          /*!<R ‘h0  CLK lane ULPS mode indicator */
#define MIPI_BIT_STOPSTATE_CLK                      ((u32)0x00000001 << 2)          /*!<R ‘h0  CLK lane Stop State indicator */
#define MIPI_BIT_DIRECTION                          ((u32)0x00000001 << 1)          /*!<R ‘h0  Low Power Lane direction */
#define MIPI_BIT_RXULPSESC                          ((u32)0x00000001 << 0)          /*!<R ‘h0  RX ULPS mode indicator */
/** @} */

/**
 * @defgroup MIPI_DPHY_STATUS1
 * @brief
 * @{
 **/
#define MIPI_MASK_RXTRIGGERESC                      ((u32)0x0000000F << 0)          /*!<R ‘h0  Escape mode trigger */
#define MIPI_RXTRIGGERESC(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define MIPI_GET_RXTRIGGERESC(x)                    ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup MIPI_DPHY_ERR
 * @brief
 * @{
 **/
#define MIPI_BIT_ERRCONTECNTIAL_LP1_CH3             ((u32)0x00000001 << 11)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP0_CH3             ((u32)0x00000001 << 10)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP1_CH2             ((u32)0x00000001 << 9)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP0_CH2             ((u32)0x00000001 << 8)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP1_CH1             ((u32)0x00000001 << 7)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP0_CH1             ((u32)0x00000001 << 6)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP1_CH0             ((u32)0x00000001 << 5)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTECNTIAL_LP0_CH0             ((u32)0x00000001 << 4)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRCONTROL                         ((u32)0x00000001 << 3)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRSYNCESC                         ((u32)0x00000001 << 2)          /*!<R/W ‘h0   */
#define MIPI_BIT_ERRESC                             ((u32)0x00000001 << 1)          /*!<R/W ‘h0   */
#define MIPI_BIT_WRITE_DATA                         ((u32)0x00000001 << 0)          /*!<W ‘b0   */
/** @} */

/**
 * @defgroup MIPI_BIST
 * @brief
 * @{
 **/
#define MIPI_BIT_DSI_LS1                            ((u32)0x00000001 << 13)          /*!<R/W ‘h0  LS control 1 */
#define MIPI_BIT_DSI_RME1                           ((u32)0x00000001 << 12)          /*!<R/W ‘h0  REM1 */
#define MIPI_MASK_DSI_RM1                           ((u32)0x0000000F << 8)          /*!<R/W ‘h0  RM1 */
#define MIPI_DSI_RM1(x)                             ((u32)(((x) & 0x0000000F) << 8))
#define MIPI_GET_DSI_RM1(x)                         ((u32)(((x >> 8) & 0x0000000F)))
#define MIPI_BIT_DSI_LS0                            ((u32)0x00000001 << 5)          /*!<R/W ‘h0  LS control 0 */
#define MIPI_BIT_DSI_RME0                           ((u32)0x00000001 << 4)          /*!<R/W ‘h0  REM0 */
#define MIPI_MASK_DSI_RM0                           ((u32)0x0000000F << 0)          /*!<R/W ‘h0  RM0 */
#define MIPI_DSI_RM0(x)                             ((u32)(((x) & 0x0000000F) << 0))
#define MIPI_GET_DSI_RM0(x)                         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup MIPI_PAT_GEN
 * @brief
 * @{
 **/
#define MIPI_BIT_PG_EN                              ((u32)0x00000001 << 27)          /*!<R/W ‘h0  Pattern generator enable */
#define MIPI_MASK_PG_TYPE                           ((u32)0x00000007 << 24)          /*!<R/W ‘h0  3’h0: color bar 3’h1: black 3’h2: white 3’h3: red 3’h4: blue 3’h5: yellow 3’h6: magenta 3’h7: user defined (ud_r, ud_g, ud_b) */
#define MIPI_PG_TYPE(x)                             ((u32)(((x) & 0x00000007) << 24))
#define MIPI_GET_PG_TYPE(x)                         ((u32)(((x >> 24) & 0x00000007)))
#define MIPI_MASK_UD_B                              ((u32)0x000000FF << 16)          /*!<R/W ‘h0  User-defined blue color space */
#define MIPI_UD_B(x)                                ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_UD_B(x)                            ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_UD_G                              ((u32)0x000000FF << 8)          /*!<R/W ‘h0  User-defined green color space */
#define MIPI_UD_G(x)                                ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_UD_G(x)                            ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_UD_R                              ((u32)0x000000FF << 0)          /*!<R/W ‘h0  User-defined red color space */
#define MIPI_UD_R(x)                                ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_UD_R(x)                            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_DUMMY0
 * @brief
 * @{
 **/
#define MIPI_MASK_DUMMY_REG0                        ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  Dummy register */
#define MIPI_DUMMY_REG0(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DUMMY_REG0(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_DUMMY1
 * @brief
 * @{
 **/
#define MIPI_MASK_DUMMY_REG1                        ((u32)0xFFFFFFFF << 0)          /*!<R/W ‘h0  Dummy register */
#define MIPI_DUMMY_REG1(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DUMMY_REG1(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_DBG
 * @brief
 * @{
 **/
#define MIPI_BIT_CLKDET_COUNT_EN                    ((u32)0x00000001 << 17)          /*!<R/W ‘h0  1: start clock frequency detection */
#define MIPI_BIT_CLKDET_RESET                       ((u32)0x00000001 << 16)          /*!<R/W ‘h0  0: reset crt_pll_div clk det function */
#define MIPI_MASK_SEL1                              ((u32)0x0000000F << 12)          /*!<R/W ‘h0  Mux select for di_dbg_out1 */
#define MIPI_SEL1(x)                                ((u32)(((x) & 0x0000000F) << 12))
#define MIPI_GET_SEL1(x)                            ((u32)(((x >> 12) & 0x0000000F)))
#define MIPI_BIT_EN1                                ((u32)0x00000001 << 8)          /*!<R/W ‘h0  Output enable for di_dbg_out1 */
#define MIPI_MASK_SEL0                              ((u32)0x0000000F << 4)          /*!<R/W ‘h0  Mux select for di_dbg_out0 */
#define MIPI_SEL0(x)                                ((u32)(((x) & 0x0000000F) << 4))
#define MIPI_GET_SEL0(x)                            ((u32)(((x >> 4) & 0x0000000F)))
#define MIPI_BIT_EN0                                ((u32)0x00000001 << 0)          /*!<R/W ‘h0  Output enable for di_dbg_out0 */
/** @} */

/**
 * @defgroup MIPI_CLK_DET0
 * @brief
 * @{
 **/
#define MIPI_MASK_COUNT_CLK_DIV8                    ((u32)0x0001FFFF << 12)          /*!<R ‘h0  Counter output */
#define MIPI_COUNT_CLK_DIV8(x)                      ((u32)(((x) & 0x0001FFFF) << 12))
#define MIPI_GET_COUNT_CLK_DIV8(x)                  ((u32)(((x >> 12) & 0x0001FFFF)))
#define MIPI_MASK_COUNT_REFCLK_DIV8                 ((u32)0x000007FF << 1)          /*!<R ‘h0  Counter output of ref clock */
#define MIPI_COUNT_REFCLK_DIV8(x)                   ((u32)(((x) & 0x000007FF) << 1))
#define MIPI_GET_COUNT_REFCLK_DIV8(x)               ((u32)(((x >> 1) & 0x000007FF)))
#define MIPI_BIT_DET_DONE_DIV8                      ((u32)0x00000001 << 0)          /*!<R ‘h0  Clock detect done */
/** @} */

/**
 * @defgroup MIPI_CLK_DET1
 * @brief
 * @{
 **/
#define MIPI_MASK_COUNT_CLK_DIV2                    ((u32)0x0001FFFF << 12)          /*!<R ‘h0  Counter output */
#define MIPI_COUNT_CLK_DIV2(x)                      ((u32)(((x) & 0x0001FFFF) << 12))
#define MIPI_GET_COUNT_CLK_DIV2(x)                  ((u32)(((x >> 12) & 0x0001FFFF)))
#define MIPI_MASK_COUNT_REFCLK_DIV2                 ((u32)0x000007FF << 1)          /*!<R ‘h0  Counter output of ref clock */
#define MIPI_COUNT_REFCLK_DIV2(x)                   ((u32)(((x) & 0x000007FF) << 1))
#define MIPI_GET_COUNT_REFCLK_DIV2(x)               ((u32)(((x >> 1) & 0x000007FF)))
#define MIPI_BIT_DET_DONE_DIV2                      ((u32)0x00000001 << 0)          /*!<R ‘h0  Clock detect done */
/** @} */

/**
 * @defgroup MIPI_DBG_ST0
 * @brief
 * @{
 **/
#define MIPI_MASK_DBG_PIF                           ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Debug status */
#define MIPI_DBG_PIF(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DBG_PIF(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_DBG_ST1
 * @brief
 * @{
 **/
#define MIPI_MASK_DBG_TIMEGEN                       ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Debug status */
#define MIPI_DBG_TIMEGEN(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DBG_TIMEGEN(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_DBG_ST2
 * @brief
 * @{
 **/
#define MIPI_MASK_DBG_PKTGEN                        ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Debug status */
#define MIPI_DBG_PKTGEN(x)                          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DBG_PKTGEN(x)                      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_DBG_ST3
 * @brief
 * @{
 **/
#define MIPI_MASK_DBG_CH_CTRL                       ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Debug status */
#define MIPI_DBG_CH_CTRL(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DBG_CH_CTRL(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_DBG_ST4
 * @brief
 * @{
 **/
#define MIPI_MASK_DBG_CMD                           ((u32)0xFFFFFFFF << 0)          /*!<R ‘h0  Debug status */
#define MIPI_DBG_CMD(x)                             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define MIPI_GET_DBG_CMD(x)                         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_CLOCK_GEN
 * @brief
 * @{
 **/
#define MIPI_BIT_DISABLE_ESCAPE_MODE                ((u32)0x00000001 << 22)          /*!<R/W ‘h0  Disable escape mode */
#define MIPI_BIT_MTX_EN                             ((u32)0x00000001 << 21)          /*!<R/W ‘h1  Register for MTX_EN */
#define MIPI_BIT_PLL_CK_LANE_ENABLE                 ((u32)0x00000001 << 20)          /*!<R/W ‘h1  Enable ulpsactivenot_ck_lane to control PLL enable */
#define MIPI_BIT_PLL_LANE3_ENABLE                   ((u32)0x00000001 << 19)          /*!<R/W ‘h1  Enable ulpsactivenot_ch3 to control PLL enable */
#define MIPI_BIT_PLL_LANE2_ENABLE                   ((u32)0x00000001 << 18)          /*!<R/W ‘h1  Enable ulpsactivenot_ch2 to control PLL enable */
#define MIPI_BIT_PLL_LANE1_ENABLE                   ((u32)0x00000001 << 17)          /*!<R/W ‘h1  Enable ulpsactivenot_ch1 to control PLL enable */
#define MIPI_BIT_PLL_LANE0_ENABLE                   ((u32)0x00000001 << 16)          /*!<R/W ‘h1  Enable ulpsactivenot_ch0 to control PLL enable */
#define MIPI_BIT_BYPASS_CLKRDY                      ((u32)0x00000001 << 15)          /*!<R/W ‘h0  Clock gating bypass clkrdy */
#define MIPI_BIT_CLK_DEGLITCH_PLL_EN                ((u32)0x00000001 << 14)          /*!<R/W ‘h1  PLL_EN clock deglitch 1‘B1: Deglitch enable 1‘B0: deglitch disable */
#define MIPI_BIT_LANE_SYNC_DISABLE                  ((u32)0x00000001 << 13)          /*!<R/W ‘h0  4 lane sync disable */
#define MIPI_BIT_CK_ESCAPE_DISABLE                  ((u32)0x00000001 << 12)          /*!<R/W ‘h0  Disable Tx Ctrl FSM TX_CK_RQST wait ck_escape_entry */
#define MIPI_BIT_TX_ULPS_PLL_ENABLE                 ((u32)0x00000001 << 11)          /*!<R/W ‘h0  No use */
#define MIPI_BIT_TX_RST_N                           ((u32)0x00000001 << 10)          /*!<R/W ‘h0  Tx reset */
#define MIPI_BIT_RX_RST_N                           ((u32)0x00000001 << 9)          /*!<R/W ‘h0  Rx reset */
#define MIPI_BIT_CLK_LANE_EN                        ((u32)0x00000001 << 8)          /*!<R/W ‘h0  LANE clock enable */
#define MIPI_BIT_LANE3_EN                           ((u32)0x00000001 << 7)          /*!<R/W ‘h0  LANE3 enable */
#define MIPI_BIT_LANE2_EN                           ((u32)0x00000001 << 6)          /*!<R/W ‘h0  LANE2 enable */
#define MIPI_BIT_LANE1_EN                           ((u32)0x00000001 << 5)          /*!<R/W ‘h0  LANE1 enable */
#define MIPI_BIT_LANE0_EN                           ((u32)0x00000001 << 4)          /*!<R/W ‘h0  LANE0 enable */
#define MIPI_BIT_TXBYTECLKHS_CKINV                  ((u32)0x00000001 << 3)          /*!<R/W ‘h0  TXBYTECLKHS INV 0: normal 1: INV */
#define MIPI_BIT_TXCLKESC_CKINV                     ((u32)0x00000001 << 2)          /*!<R/W ‘h0  TXCLKESC INV 0: normal 1: INV */
#define MIPI_MASK_TXCLKESC_SEL                      ((u32)0x00000003 << 0)          /*!<R/W ‘h0  TXCLKESC SEL 00: TXCLKESC 01: TXBYTECLKHS 10: XTAL 11: XTAL */
#define MIPI_TXCLKESC_SEL(x)                        ((u32)(((x) & 0x00000003) << 0))
#define MIPI_GET_TXCLKESC_SEL(x)                    ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_CLK_0
 * @brief
 * @{
 **/
#define MIPI_BIT_RQST_LP_MASK_ENABLE                ((u32)0x00000001 << 27)          /*!<R/W ‘h0  Delete 0.5T T lp when Request */
#define MIPI_BIT_LP_CLK_CFG                         ((u32)0x00000001 << 26)          /*!<R/W ‘h0  PN SWAP(connect to escape_tx_clk and tx_ctrl_l*) 1: SWAP 0: normal */
#define MIPI_MASK_ULPS_ENTRY_SEL                    ((u32)0x00000003 << 24)          /*!<R/W ‘h3  00: 01: need txrequest_clk is 0 10: need tx_ctrl FSM is TX_CK_STOP 11: need txrequest_clk is 0 && tx_ctrl FSM is TX_CK_STOP */
#define MIPI_ULPS_ENTRY_SEL(x)                      ((u32)(((x) & 0x00000003) << 24))
#define MIPI_GET_ULPS_ENTRY_SEL(x)                  ((u32)(((x >> 24) & 0x00000003)))
#define MIPI_MASK_CK_TX_10_EXIT_TIME                ((u32)0x0000000F << 20)          /*!<R/W ‘h0  Timer: (FSM ck_tx_ulps_lp_10 ) CK lane LP10 timer */
#define MIPI_CK_TX_10_EXIT_TIME(x)                  ((u32)(((x) & 0x0000000F) << 20))
#define MIPI_GET_CK_TX_10_EXIT_TIME(x)              ((u32)(((x >> 20) & 0x0000000F)))
#define MIPI_MASK_CK_TX_IDLE_EXIT_TIME              ((u32)0x0000000F << 16)          /*!<R/W ‘h0F  Timer: (FSM ck_tx_idle_lp_11) CK lane idle timer */
#define MIPI_CK_TX_IDLE_EXIT_TIME(x)                ((u32)(((x) & 0x0000000F) << 16))
#define MIPI_GET_CK_TX_IDLE_EXIT_TIME(x)            ((u32)(((x >> 16) & 0x0000000F)))
#define MIPI_MASK_CK_ULPS_EXIT_TIME                 ((u32)0x0000FFFF << 0)          /*!<R/W ‘h3F  Timer: (FSM ck_tx_ulps_lp_exit_10) unit is 800ns */
#define MIPI_CK_ULPS_EXIT_TIME(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define MIPI_GET_CK_ULPS_EXIT_TIME(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_0
 * @brief
 * @{
 **/
#define MIPI_BIT_DATA_PRPR_MASK_ENABLE              ((u32)0x00000001 << 31)          /*!<R/W ‘h0  Enable delete 0.125T clock prepare */
#define MIPI_MASK_DATA_PRPR_MASK_COUNTER            ((u32)0x00000007 << 28)          /*!<R/W ‘h3  0: Delete 0.125*7 T 1: Delete 0.125*6 T 2: Delete 0.125*5 T 3: Delete 0.125*4 T 4: Delete 0.125*3 T 5: Delete 0.125*2 T 6: Delete 0.125*1 T 7: Delete 0.125*0 T */
#define MIPI_DATA_PRPR_MASK_COUNTER(x)              ((u32)(((x) & 0x00000007) << 28))
#define MIPI_GET_DATA_PRPR_MASK_COUNTER(x)          ((u32)(((x >> 28) & 0x00000007)))
#define MIPI_BIT_CLK_PRPR_MASK_ENABLE               ((u32)0x00000001 << 26)          /*!<R/W ‘h0  Enable delete 0.125T clock prepare */
#define MIPI_MASK_CLK_PRPR_MASK_COUNTER             ((u32)0x00000007 << 23)          /*!<R/W ‘h3  0: Delete 0.125*7 T 1: Delete 0.125*6 T 2: Delete 0.125*5 T 3: Delete 0.125*4 T 4: Delete 0.125*3 T 5: Delete 0.125*2 T 6: Delete 0.125*1 T 7: Delete 0.125*0 T */
#define MIPI_CLK_PRPR_MASK_COUNTER(x)               ((u32)(((x) & 0x00000007) << 23))
#define MIPI_GET_CLK_PRPR_MASK_COUNTER(x)           ((u32)(((x >> 23) & 0x00000007)))
#define MIPI_BIT_RQST_LP_ONE_QUARTER_MASK_ENABLE    ((u32)0x00000001 << 22)          /*!<R/W ‘h0  Delete 0.125T T lp when Request */
#define MIPI_MASK_RQST_LP_ONE_QUARTER_MASK_COUNTER  ((u32)0x0000000F << 18)          /*!<R/W ‘h0  0: Delete 0.125*1 T 1: Delete 0.125*2 T 2: Delete 0.125*3 T 3: Delete 0.125*4 T 4: Delete 0.125*5 T 5: Delete 0.125*6 T */
#define MIPI_RQST_LP_ONE_QUARTER_MASK_COUNTER(x)    ((u32)(((x) & 0x0000000F) << 18))
#define MIPI_GET_RQST_LP_ONE_QUARTER_MASK_COUNTER(x) ((u32)(((x >> 18) & 0x0000000F)))
#define MIPI_BIT_TAGET_TIMEOUT_DISABLE              ((u32)0x00000001 << 17)          /*!<R/W ‘h0  Disable reg_taget_timeout_disable */
#define MIPI_MASK_TIME_TAGET_TIMEOUT                ((u32)0x0000001F << 12)          /*!<R/W ‘h10  Timer: (FSM wait_RX_TA_ACK) TX2RX, wait slave sending ACK timer */
#define MIPI_TIME_TAGET_TIMEOUT(x)                  ((u32)(((x) & 0x0000001F) << 12))
#define MIPI_GET_TIME_TAGET_TIMEOUT(x)              ((u32)(((x >> 12) & 0x0000001F)))
#define MIPI_MASK_TIME_TAGET                        ((u32)0x0000000F << 8)          /*!<R/W ‘h2  Timer: (FSM master_tx_state_00) T_ta_sure */
#define MIPI_TIME_TAGET(x)                          ((u32)(((x) & 0x0000000F) << 8))
#define MIPI_GET_TIME_TAGET(x)                      ((u32)(((x >> 8) & 0x0000000F)))
#define MIPI_MASK_TIME_TAGO                         ((u32)0x0000000F << 4)          /*!<R/W ‘h3  Timer：(FSM tx_arunaround_00_2) T_tago */
#define MIPI_TIME_TAGO(x)                           ((u32)(((x) & 0x0000000F) << 4))
#define MIPI_GET_TIME_TAGO(x)                       ((u32)(((x >> 4) & 0x0000000F)))
#define MIPI_MASK_TX_IDLE_EXIT_TIME                 ((u32)0x0000000F << 0)          /*!<R/W ‘h4  Timer：(FSM tx_idle_lp_11) */
#define MIPI_TX_IDLE_EXIT_TIME(x)                   ((u32)(((x) & 0x0000000F) << 0))
#define MIPI_GET_TX_IDLE_EXIT_TIME(x)               ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_1
 * @brief
 * @{
 **/
#define MIPI_MASK_TX_LPX_TIME                       ((u32)0x000000FF << 24)          /*!<R/W ‘h06  Timer Clock period needs 48ns – 77ns (FSM TX_CK_RQST/TX_HS_RQST) */
#define MIPI_TX_LPX_TIME(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define MIPI_GET_TX_LPX_TIME(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define MIPI_MASK_HS_EXIT_TIME                      ((u32)0x000000FF << 16)          /*!<R/W ‘h0E  Timer (FSM TX_HS_STOP/TX_CK_STOP) */
#define MIPI_HS_EXIT_TIME(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_HS_EXIT_TIME(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_TX_INIT_TIME                      ((u32)0x0000FFFF << 0)          /*!<R/W ‘h1100  Timer (FSM TX_INIT) */
#define MIPI_TX_INIT_TIME(x)                        ((u32)(((x) & 0x0000FFFF) << 0))
#define MIPI_GET_TX_INIT_TIME(x)                    ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_2
 * @brief
 * @{
 **/
#define MIPI_MASK_CLK_PRE_TIME                      ((u32)0x000000FF << 24)          /*!<R/W ‘h01  Timer (FSM TX_CK_PRE) */
#define MIPI_CLK_PRE_TIME(x)                        ((u32)(((x) & 0x000000FF) << 24))
#define MIPI_GET_CLK_PRE_TIME(x)                    ((u32)(((x >> 24) & 0x000000FF)))
#define MIPI_MASK_CLK_ZERO_TIME                     ((u32)0x000000FF << 16)          /*!<R/W ‘h1D  Timer (FSM TX_CK_ZERO) */
#define MIPI_CLK_ZERO_TIME(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_CLK_ZERO_TIME(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_CLK_PRPR_TIME                     ((u32)0x000000FF << 8)          /*!<R/W ‘h09  Timer (FSM TX_CK_PRPR) */
#define MIPI_CLK_PRPR_TIME(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_CLK_PRPR_TIME(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_CLK_POST_TIME                     ((u32)0x000000FF << 0)          /*!<R/W ‘h0D  Timer (FSM TX_CK_POST) */
#define MIPI_CLK_POST_TIME(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_CLK_POST_TIME(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_3
 * @brief
 * @{
 **/
#define MIPI_MASK_TX_VLD_TIME                       ((u32)0x000000FF << 24)          /*!<R/W ‘h05  Timer */
#define MIPI_TX_VLD_TIME(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define MIPI_GET_TX_VLD_TIME(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define MIPI_MASK_TX_TAIL_TIME                      ((u32)0x000000FF << 16)          /*!<R/W ‘h09  Timer (FSM TX_CK_TRAIL/TX_HS_TRAIL) */
#define MIPI_TX_TAIL_TIME(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_TX_TAIL_TIME(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_HS_ZERO_TIME                      ((u32)0x000000FF << 8)          /*!<R/W ‘h09  Timer (FSM TX_HS_ZERO) */
#define MIPI_HS_ZERO_TIME(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_HS_ZERO_TIME(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_HS_PRPR_TIME                      ((u32)0x000000FF << 0)          /*!<R/W ‘h09  Timer (FSM TX_HS_PRPR) */
#define MIPI_HS_PRPR_TIME(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_HS_PRPR_TIME(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_4
 * @brief
 * @{
 **/
#define MIPI_BIT_CLKEDGE_CFG_3                      ((u32)0x00000001 << 19)          /*!<R/W ‘h00  Skew pattern (lane3) 1: 01010101 0: 10101010 */
#define MIPI_BIT_LP_DAT_CFG_3                       ((u32)0x00000001 << 18)          /*!<R/W ‘h00  PN SWAP (connect to escape_tx and tx_ctrl_l*) (lane3) 1: SWAP 0: normal */
#define MIPI_BIT_HSVLD_DAT_CFG_3                    ((u32)0x00000001 << 17)          /*!<R/W ‘h00  TX_HS_DATA_VALID INV(connect to tx_ctrl_l*)(lane3) 1: INV 0: nornal */
#define MIPI_BIT_CLKEDGE_CFG_2                      ((u32)0x00000001 << 16)          /*!<R/W ‘h00  Skew pattern (lane2) 1: 01010101 0: 10101010 */
#define MIPI_BIT_LP_DAT_CFG_2                       ((u32)0x00000001 << 15)          /*!<R/W ‘h00  PN SWAP (connect to escape_tx and tx_ctrl_l*) (lane2) 1: SWAP 0: normal */
#define MIPI_BIT_HSVLD_DAT_CFG_2                    ((u32)0x00000001 << 14)          /*!<R/W ‘h00  TX_HS_DATA_VALID INV (connect to tx_ctrl_l*) (lane2) 1: INV 0: nornal */
#define MIPI_BIT_CLKEDGE_CFG_1                      ((u32)0x00000001 << 13)          /*!<R/W ‘h00  Skew pattern (lane1) 1: 01010101 0: 10101010 */
#define MIPI_BIT_LP_DAT_CFG_1                       ((u32)0x00000001 << 12)          /*!<R/W ‘h00  PN SWAP (connect to escape_tx and tx_ctrl_l*) (lane1) 1: SWAP 0: normal */
#define MIPI_BIT_HSVLD_DAT_CFG_1                    ((u32)0x00000001 << 11)          /*!<R/W ‘h00  TX_HS_DATA_VALID INV (connect to tx_ctrl_l*) (lane1) 1: INV 0: normal */
#define MIPI_BIT_CLKEDGE_CFG_0                      ((u32)0x00000001 << 10)          /*!<R/W ‘h00  Skew pattern (lane0) 1: 01010101 0: 10101010 */
#define MIPI_BIT_LP_DAT_CFG_0                       ((u32)0x00000001 << 9)          /*!<R/W ‘h00  PN SWAP (connect to escape_tx and tx_ctrl_l*) (lane0) 1: SWAP 0: normal */
#define MIPI_BIT_HSVLD_DAT_CFG_0                    ((u32)0x00000001 << 8)          /*!<R/W ‘h00  TX_HS_DATA_VALID INV (connect to tx_ctrl_l*) (lane0) 1: INV 0: nornal */
#define MIPI_MASK_HS_WKUP_TIME                      ((u32)0x000000FF << 0)          /*!<R/W ‘h00  Timer */
#define MIPI_HS_WKUP_TIME(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_HS_WKUP_TIME(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_5
 * @brief
 * @{
 **/
#define MIPI_MASK_DIG_RESERVED_3                    ((u32)0x000000FF << 24)          /*!<R/W ‘h00  Digital reserved register */
#define MIPI_DIG_RESERVED_3(x)                      ((u32)(((x) & 0x000000FF) << 24))
#define MIPI_GET_DIG_RESERVED_3(x)                  ((u32)(((x >> 24) & 0x000000FF)))
#define MIPI_MASK_DIG_RESERVED_2                    ((u32)0x000000FF << 16)          /*!<R/W ‘h00  Digital reserved register */
#define MIPI_DIG_RESERVED_2(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_DIG_RESERVED_2(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_DIG_RESERVED_1                    ((u32)0x000000FF << 8)          /*!<R/W ‘h00  Bit[1]: reg_hswkp_data_advance 1: reg_hswkp_data raise early 1T 0: normal mode Bit[0]: reg_hswkp_cc_advance 1: reg_hswkp_cc raise early 1T 0: normal mode */
#define MIPI_DIG_RESERVED_1(x)                      ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_DIG_RESERVED_1(x)                  ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_DIG_RESERVED_0                    ((u32)0x000000FF << 0)          /*!<R/W ‘h09  Digital reserved register */
#define MIPI_DIG_RESERVED_0(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_DIG_RESERVED_0(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_TX_DATA_6
 * @brief
 * @{
 **/
#define MIPI_MASK_SKEW_TIME                         ((u32)0x0000FFFF << 16)          /*!<R/W ‘h0010  Skew timer: the minimum time for HS burst */
#define MIPI_SKEW_TIME(x)                           ((u32)(((x) & 0x0000FFFF) << 16))
#define MIPI_GET_SKEW_TIME(x)                       ((u32)(((x >> 16) & 0x0000FFFF)))
#define MIPI_MASK_ULPS_EXIT_TIME                    ((u32)0x0000FFFF << 0)          /*!<R/W ‘h0050  Timer, unit is 50 ns (FSM tx_ulps_exit_10) */
#define MIPI_ULPS_EXIT_TIME(x)                      ((u32)(((x) & 0x0000FFFF) << 0))
#define MIPI_GET_ULPS_EXIT_TIME(x)                  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup MIPI_ESCAPE_RX_DATA_0
 * @brief
 * @{
 **/
#define MIPI_BIT_DISABLE_ERRESC                     ((u32)0x00000001 << 30)          /*!<R/W ‘h0  Disable erresc */
#define MIPI_BIT_DISABLE_ERRSYNCESC                 ((u32)0x00000001 << 29)          /*!<R/W ‘h0  Disable errsyncesc */
#define MIPI_BIT_DISABLE_ERRCONTROL                 ((u32)0x00000001 << 28)          /*!<R/W ‘h0  Disable errcontrol */
#define MIPI_MASK_RX_LP_DELAY                       ((u32)0x0000000F << 24)          /*!<R/W ‘h0  Rx low power signal TX2RX delay */
#define MIPI_RX_LP_DELAY(x)                         ((u32)(((x) & 0x0000000F) << 24))
#define MIPI_GET_RX_LP_DELAY(x)                     ((u32)(((x >> 24) & 0x0000000F)))
#define MIPI_BIT_RX_LP_DEGLITCH                     ((u32)0x00000001 << 23)          /*!<R/W ‘h0  Deglitch */
#define MIPI_MASK_TIME_RX2TX                        ((u32)0x0000001F << 18)          /*!<R/W ‘h10  Timer Extend signal RX2TX FSM from master_rx_state to master_tx_state_00 */
#define MIPI_TIME_RX2TX(x)                          ((u32)(((x) & 0x0000001F) << 18))
#define MIPI_GET_TIME_RX2TX(x)                      ((u32)(((x >> 18) & 0x0000001F)))
#define MIPI_MASK_TIME_RX_TA_ACK                    ((u32)0x0000001F << 13)          /*!<R/W ‘h07  Timer Extend signal rx_ta_ack FSM from wait_RX_TA_ACK to master_rx_state */
#define MIPI_TIME_RX_TA_ACK(x)                      ((u32)(((x) & 0x0000001F) << 13))
#define MIPI_GET_TIME_RX_TA_ACK(x)                  ((u32)(((x >> 13) & 0x0000001F)))
#define MIPI_MASK_RX_CLOCK_PHASE                    ((u32)0x000000FF << 5)          /*!<R/W ‘h00  Rx clock phase [7:6]: lane3 [5:4]: lane2 [3:2]: lane1 [1:0]: lane0 2‘B11: 4T 2‘B10: 3T 2‘B01: 2T 2‘B00: 1T */
#define MIPI_RX_CLOCK_PHASE(x)                      ((u32)(((x) & 0x000000FF) << 5))
#define MIPI_GET_RX_CLOCK_PHASE(x)                  ((u32)(((x >> 5) & 0x000000FF)))
#define MIPI_MASK_DEGLITCH_CFG                      ((u32)0x0000001F << 0)          /*!<R/W ‘h00  Rx stop state deglitch [4]: lane ck [3]: lane3 [2]: lane2 [1]: lane1 [0]: lane0 */
#define MIPI_DEGLITCH_CFG(x)                        ((u32)(((x) & 0x0000001F) << 0))
#define MIPI_GET_DEGLITCH_CFG(x)                    ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**
 * @defgroup MIPI_LANE_SWAP_CTRL
 * @brief
 * @{
 **/
#define MIPI_MASK_LANE3_SEL                         ((u32)0x00000003 << 15)          /*!<R/W ‘h3  Lane swap select */
#define MIPI_LANE3_SEL(x)                           ((u32)(((x) & 0x00000003) << 15))
#define MIPI_GET_LANE3_SEL(x)                       ((u32)(((x >> 15) & 0x00000003)))
#define MIPI_MASK_LANE2_SEL                         ((u32)0x00000003 << 13)          /*!<R/W ‘h2  Lane swap select */
#define MIPI_LANE2_SEL(x)                           ((u32)(((x) & 0x00000003) << 13))
#define MIPI_GET_LANE2_SEL(x)                       ((u32)(((x >> 13) & 0x00000003)))
#define MIPI_MASK_LANE1_SEL                         ((u32)0x00000003 << 11)          /*!<R/W ‘h1  Lane swap select */
#define MIPI_LANE1_SEL(x)                           ((u32)(((x) & 0x00000003) << 11))
#define MIPI_GET_LANE1_SEL(x)                       ((u32)(((x >> 11) & 0x00000003)))
#define MIPI_MASK_LANE0_SEL                         ((u32)0x00000003 << 9)          /*!<R/W ‘h0  Lane swap select */
#define MIPI_LANE0_SEL(x)                           ((u32)(((x) & 0x00000003) << 9))
#define MIPI_GET_LANE0_SEL(x)                       ((u32)(((x >> 9) & 0x00000003)))
#define MIPI_MASK_MUX_DELAY_SEL                     ((u32)0x00000003 << 7)          /*!<R/W ‘h0  Tx_ctrl/tx escape select signal 2‘b11: sync 3T 2‘b10: sync 2T 2‘b01: sync 1T 2‘b00: sync 0T */
#define MIPI_MUX_DELAY_SEL(x)                       ((u32)(((x) & 0x00000003) << 7))
#define MIPI_GET_MUX_DELAY_SEL(x)                   ((u32)(((x >> 7) & 0x00000003)))
#define MIPI_BIT_CLKEDGE_CFG                        ((u32)0x00000001 << 6)          /*!<R/W ‘h0  CK lane HS data 1: 01010101 0: 10101010 */
#define MIPI_BIT_SWP                                ((u32)0x00000001 << 5)          /*!<R/W ‘h0  1: SWAP 0: normal */
#define MIPI_BIT_ENHST_DAT_CFG                      ((u32)0x00000001 << 4)          /*!<R/W ‘h0  1: INV 0: normal */
#define MIPI_BIT_WKUP_DAT_CFG                       ((u32)0x00000001 << 3)          /*!<R/W ‘h0  1: INV 0: normal */
#define MIPI_BIT_HSVLD_CLK_CFG                      ((u32)0x00000001 << 2)          /*!<R/W ‘h0  1: INV 0: normal */
#define MIPI_BIT_ENHST_CLK_CFG                      ((u32)0x00000001 << 1)          /*!<R/W ‘h0  1: INV 0: normal */
#define MIPI_BIT_WKUP_CLK_CFG                       ((u32)0x00000001 << 0)          /*!<R/W ‘h0  1: INV 0: normal */
/** @} */

/**
 * @defgroup MIPI_LANE_LP_BIST_0
 * @brief
 * @{
 **/
#define MIPI_MASK_RX_LP_BIST_PATTERN                ((u32)0x000000FF << 13)          /*!<R/W ‘h0  Rx LP bist pattern */
#define MIPI_RX_LP_BIST_PATTERN(x)                  ((u32)(((x) & 0x000000FF) << 13))
#define MIPI_GET_RX_LP_BIST_PATTERN(x)              ((u32)(((x >> 13) & 0x000000FF)))
#define MIPI_MASK_TX_LP_BIST_PATTERN                ((u32)0x000000FF << 5)          /*!<R/W ‘h1  Tx LP bist pattern */
#define MIPI_TX_LP_BIST_PATTERN(x)                  ((u32)(((x) & 0x000000FF) << 5))
#define MIPI_GET_TX_LP_BIST_PATTERN(x)              ((u32)(((x >> 5) & 0x000000FF)))
#define MIPI_BIT_BIST_PATH                          ((u32)0x00000001 << 4)          /*!<R/W ‘h0  Select BIST path */
#define MIPI_BIT_RX_LP_BIST_ENABLE                  ((u32)0x00000001 << 3)          /*!<R/W ‘h0  BIST Rx enable */
#define MIPI_BIT_TX_LP_BIST_ENABLE                  ((u32)0x00000001 << 2)          /*!<R/W ‘h0  BIST Tx enable */
#define MIPI_MASK_BIST_PATTERN_SEL                  ((u32)0x00000003 << 0)          /*!<R/W ‘h0  2‘b11: cnt 2‘b10: reg_tx_lp_bist_pattern 2‘b01: 8‘b1100_1100 2‘b00: 8‘b0101_1010 */
#define MIPI_BIST_PATTERN_SEL(x)                    ((u32)(((x) & 0x00000003) << 0))
#define MIPI_GET_BIST_PATTERN_SEL(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup MIPI_LANE_HS_BIST
 * @brief
 * @{
 **/
#define MIPI_MASK_PATTERN_SEL                       ((u32)0x00000003 << 30)          /*!<R/W ‘h0  Pattern sel 2‘b11: PRBS 23 2‘b10: PRBS7 2‘b01: CRC32 2‘b00: from pdat and ndat */
#define MIPI_PATTERN_SEL(x)                         ((u32)(((x) & 0x00000003) << 30))
#define MIPI_GET_PATTERN_SEL(x)                     ((u32)(((x >> 30) & 0x00000003)))
#define MIPI_BIT_PRBS_INVS                          ((u32)0x00000001 << 29)          /*!<R/W ‘h0  PRBS inverse */
#define MIPI_BIT_PRBS_ENABLE                        ((u32)0x00000001 << 28)          /*!<R/W ‘h0  PRBS enable */
#define MIPI_BIT_TEST_EN                            ((u32)0x00000001 << 27)          /*!<R/W ‘h0  Enable */
#define MIPI_BIT_TEST_HS_EN                         ((u32)0x00000001 << 26)          /*!<R/W ‘h0  HS enable */
#define MIPI_BIT_TEST_DAT_MODE                      ((u32)0x00000001 << 25)          /*!<R/W ‘h0  No use */
#define MIPI_BIT_TEST_HSDAT_ONLY                    ((u32)0x00000001 << 24)          /*!<R/W ‘h0  Test pattern select */
#define MIPI_MASK_PDAT_TST                          ((u32)0x000000FF << 16)          /*!<R/W ‘h17  Manual data */
#define MIPI_PDAT_TST(x)                            ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_PDAT_TST(x)                        ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_NDAT_TST                          ((u32)0x000000FF << 8)          /*!<R/W ‘h35  Manual data */
#define MIPI_NDAT_TST(x)                            ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_NDAT_TST(x)                        ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_LPX_TST                           ((u32)0x000000FF << 0)          /*!<R/W ‘h6  LP length counter */
#define MIPI_LPX_TST(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_LPX_TST(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_contention_detector_and_stopstate_dt
 * @brief
 * @{
 **/
#define MIPI_MASK_CONTENTION_DETECT_WAY             ((u32)0x0000000F << 22)          /*!<R/W ‘h2  Contention detect way reg_contention_detect_way [1]: 1: LP0 use LDPC_H 0: LP0 use LDPC_L reg_contention_detect_way [0]: 1: LP1 use LDPC_H 0: LP1 use LDPC_L */
#define MIPI_CONTENTION_DETECT_WAY(x)               ((u32)(((x) & 0x0000000F) << 22))
#define MIPI_GET_CONTENTION_DETECT_WAY(x)           ((u32)(((x >> 22) & 0x0000000F)))
#define MIPI_MASK_STOPSTATE_DETECT_WAY_SEL          ((u32)0x00000003 << 20)          /*!<R/W ‘h0  Stop_state_detect_way_sel */
#define MIPI_STOPSTATE_DETECT_WAY_SEL(x)            ((u32)(((x) & 0x00000003) << 20))
#define MIPI_GET_STOPSTATE_DETECT_WAY_SEL(x)        ((u32)(((x >> 20) & 0x00000003)))
#define MIPI_MASK_STOPSTATE_DEGLITCH_SEL            ((u32)0x00000007 << 17)          /*!<R/W ‘h0  Stopstate_deglitch _sel */
#define MIPI_STOPSTATE_DEGLITCH_SEL(x)              ((u32)(((x) & 0x00000007) << 17))
#define MIPI_GET_STOPSTATE_DEGLITCH_SEL(x)          ((u32)(((x >> 17) & 0x00000007)))
#define MIPI_MASK_TX_DETECT_DELAY_SEL               ((u32)0x00000007 << 14)          /*!<R/W ‘h1  Tx delay sel */
#define MIPI_TX_DETECT_DELAY_SEL(x)                 ((u32)(((x) & 0x00000007) << 14))
#define MIPI_GET_TX_DETECT_DELAY_SEL(x)             ((u32)(((x >> 14) & 0x00000007)))
#define MIPI_MASK_RX_DETECT_DELAY_SEL               ((u32)0x00000003 << 12)          /*!<R/W ‘h0  Rx delay sel */
#define MIPI_RX_DETECT_DELAY_SEL(x)                 ((u32)(((x) & 0x00000003) << 12))
#define MIPI_GET_RX_DETECT_DELAY_SEL(x)             ((u32)(((x >> 12) & 0x00000003)))
#define MIPI_MASK_DETECT_ENABLE                     ((u32)0x0000000F << 8)          /*!<R/W ‘hf  [0]: LP0_dp [1]: LP0_dm [2]: LP1_dp [3]: LP1_dm */
#define MIPI_DETECT_ENABLE(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define MIPI_GET_DETECT_ENABLE(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define MIPI_MASK_DETECT_IDLE_EXIT_TIME             ((u32)0x000000FF << 0)          /*!<R/W ‘h0f  Contention detect idle exit time */
#define MIPI_DETECT_IDLE_EXIT_TIME(x)               ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_DETECT_IDLE_EXIT_TIME(x)           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_DEBUG_0
 * @brief
 * @{
 **/
#define MIPI_MASK_DIG_DEBUG_SEL                     ((u32)0x000000FF << 0)          /*!<R/W ‘h00  Debug address */
#define MIPI_DIG_DEBUG_SEL(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_DIG_DEBUG_SEL(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_DEBUG_1
 * @brief
 * @{
 **/
#define MIPI_MASK_DEBUG_OUT                         ((u32)0x3FFFFFFF << 0)          /*!<R ‘h0  DEBUG_OUT signal */
#define MIPI_DEBUG_OUT(x)                           ((u32)(((x) & 0x3FFFFFFF) << 0))
#define MIPI_GET_DEBUG_OUT(x)                       ((u32)(((x >> 0) & 0x3FFFFFFF)))
/** @} */

/**
 * @defgroup MIPI_SSC0
 * @brief
 * @{
 **/
#define MIPI_MASK_OC_DONE_DELAY                     ((u32)0x0000003F << 8)          /*!<R/W ‘h  Delay to setting OC_DONE=b1 after over-clock or under-clock is done by (REG_OC_DONE_delay*Tref) */
#define MIPI_OC_DONE_DELAY(x)                       ((u32)(((x) & 0x0000003F) << 8))
#define MIPI_GET_OC_DONE_DELAY(x)                   ((u32)(((x >> 8) & 0x0000003F)))
#define MIPI_MASK_HS_OC_STOP_DIFF                   ((u32)0x00000003 << 6)          /*!<R/W ‘h1  HS stop control b00: HS OC stop at (Target_freq-2*fref) b01: HS OC stop at (Target_freq-6*fref) b10: HS OC stop at (Target_freq-10*fref) b11: HS OC stop at (Target_freq-14*fref) */
#define MIPI_HS_OC_STOP_DIFF(x)                     ((u32)(((x) & 0x00000003) << 6))
#define MIPI_GET_HS_OC_STOP_DIFF(x)                 ((u32)(((x >> 6) & 0x00000003)))
#define MIPI_MASK_SEL_OC_MODE                       ((u32)0x00000003 << 4)          /*!<R/W ‘h0  Over-clock/Under-clock mode selection b00: Low speed mode b01: High speed mode b10: Adaptive high speed mode */
#define MIPI_SEL_OC_MODE(x)                         ((u32)(((x) & 0x00000003) << 4))
#define MIPI_GET_SEL_OC_MODE(x)                     ((u32)(((x >> 4) & 0x00000003)))
#define MIPI_BIT_DPI_SSC_FLAG_INIT                  ((u32)0x00000001 << 3)          /*!<R/W ‘b0  SSC initial flag */
#define MIPI_BIT_SDM_ORDER                          ((u32)0x00000001 << 2)          /*!<R/W ‘b0  Select 1st-order or 2nd-order SDM */
#define MIPI_BIT_EN_SSC                             ((u32)0x00000001 << 1)          /*!<R/W ‘b0  Enable SSC function 1: Enable 0: Disable */
#define MIPI_BIT_BYPASS_PI                          ((u32)0x00000001 << 0)          /*!<R/W ‘b0  When no fractional divider or SSC, bypass Phase Interpolator for CMU loop 0: including PI inside the loop 1: bypass PI inside the loop */
/** @} */

/**
 * @defgroup MIPI_SSC1
 * @brief
 * @{
 **/
#define MIPI_BIT_GRAN_AUTO_RST                      ((u32)0x00000001 << 23)          /*!<R/W ‘b1  When FMOD transition, reset SDM */
#define MIPI_MASK_DOT_GRAN                          ((u32)0x00000007 << 20)          /*!<R/W ‘h4  Decimal point position of GRAN b000: 4.15 b001: 5.14 b010: 6.13 b011: 7.12 b100: 8.11 */
#define MIPI_DOT_GRAN(x)                            ((u32)(((x) & 0x00000007) << 20))
#define MIPI_GET_DOT_GRAN(x)                        ((u32)(((x >> 20) & 0x00000007)))
#define MIPI_MASK_GRAN_EST                          ((u32)0x0007FFFF << 0)          /*!<R/W ‘h021FB  SSC step SSC step=(Fssc-F) / (0.5*Tssc) */
#define MIPI_GRAN_EST(x)                            ((u32)(((x) & 0x0007FFFF) << 0))
#define MIPI_GET_GRAN_EST(x)                        ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/**
 * @defgroup MIPI_SSC2
 * @brief
 * @{
 **/
#define MIPI_MASK_FCODE_T_SCPU                      ((u32)0x000007FF << 16)          /*!<R/W ‘h4c  Fractional code of loop divider (target Fcode of overclocking) */
#define MIPI_FCODE_T_SCPU(x)                        ((u32)(((x) & 0x000007FF) << 16))
#define MIPI_GET_FCODE_T_SCPU(x)                    ((u32)(((x >> 16) & 0x000007FF)))
#define MIPI_MASK_FCODE_SSC                         ((u32)0x000007FF << 0)          /*!<R/W ‘h4c  Fractional code of loop divider */
#define MIPI_FCODE_SSC(x)                           ((u32)(((x) & 0x000007FF) << 0))
#define MIPI_GET_FCODE_SSC(x)                       ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/**
 * @defgroup MIPI_SSC3
 * @brief
 * @{
 **/
#define MIPI_BIT_OC_EN                              ((u32)0x00000001 << 26)          /*!<R/W ‘b0  Enable over-clock or under-clock */
#define MIPI_MASK_OC_STEP_SET                       ((u32)0x000003FF << 16)          /*!<R/W ‘h28  Over-clock / Under-clock slope control of low speed mode */
#define MIPI_OC_STEP_SET(x)                         ((u32)(((x) & 0x000003FF) << 16))
#define MIPI_GET_OC_STEP_SET(x)                     ((u32)(((x >> 16) & 0x000003FF)))
#define MIPI_MASK_NCODE_T_SCPU                      ((u32)0x000000FF << 8)          /*!<R/W ‘h22  Integer code of loop divider,real divider code is (target Ncode of overclocking) CKREF=27M, REG_PDIV=b00, REG_BYPASS_PI=b0, VCO frequency=27*( REG_N_CODE [7:0] +3+ REG_F_CODE<10:0>/2048)=1.6GHz -> REG_N_CODE [7:0] =56, REG_F_CODE<10:0>=531 */
#define MIPI_NCODE_T_SCPU(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_NCODE_T_SCPU(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_NCODE_SSC                         ((u32)0x000000FF << 0)          /*!<R/W ‘h22  Integer code of loop divider,real divider code is CKREF=27M, REG_PDIV=b00, REG_BYPASS_PI=b0, VCO frequency=27*( REG_N_CODE [7:0] +3+ REG_F_CODE<10:0>/2048)=1.0GHz -> REG_N_CODE [7:0] =34, REG_F_CODE<10:0>=76 */
#define MIPI_NCODE_SSC(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_NCODE_SSC(x)                       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_WATCHDOG
 * @brief
 * @{
 **/
#define MIPI_MASK_TIME2_RST_WIDTH                   ((u32)0x00000003 << 12)          /*!<R/W ‘h1  Reset timing control (REG_TIME_RST_WIDTH x CKREF) d0: 2 d1: 4 d2: 8 d3: 20 */
#define MIPI_TIME2_RST_WIDTH(x)                     ((u32)(((x) & 0x00000003) << 12))
#define MIPI_GET_TIME2_RST_WIDTH(x)                 ((u32)(((x >> 12) & 0x00000003)))
#define MIPI_MASK_DFS_TIME_RDY_CKOUT                ((u32)0x00000003 << 10)          /*!<R/W ‘h1  DFS Clock ready timing control (clock ready time: 2 times the value of REG_TIME_WD x CKREF + REG_TIME_RDY_CKOUT x CKTXPLL) d0: 3 d1: 100 d2: 200 d3: 600 */
#define MIPI_DFS_TIME_RDY_CKOUT(x)                  ((u32)(((x) & 0x00000003) << 10))
#define MIPI_GET_DFS_TIME_RDY_CKOUT(x)              ((u32)(((x >> 10) & 0x00000003)))
#define MIPI_MASK_TIME_RDY_CKOUT                    ((u32)0x00000003 << 8)          /*!<R/W ‘h2  Clock ready timing control (clock ready time: 2 times the value of REG_TIME_WD x CKREF + REG_TIME_RDY_CKOUT x CKTXPLL) d0: 3 d1: 100 d2: 200 d3: 600 */
#define MIPI_TIME_RDY_CKOUT(x)                      ((u32)(((x) & 0x00000003) << 8))
#define MIPI_GET_TIME_RDY_CKOUT(x)                  ((u32)(((x >> 8) & 0x00000003)))
#define MIPI_MASK_TIME0_CK                          ((u32)0x00000007 << 4)          /*!<R/W ‘h3  Watchdog timing control (clock output time: 2 times the value of REG_TIME_WD x CKREF) d0: 2 d1: 200 d2: 600 d3: 1200 d4: 3000 d5: 6400 d6: 12800 d7: 16000 */
#define MIPI_TIME0_CK(x)                            ((u32)(((x) & 0x00000007) << 4))
#define MIPI_GET_TIME0_CK(x)                        ((u32)(((x >> 4) & 0x00000007)))
#define MIPI_BIT_RSTB                               ((u32)0x00000001 << 3)          /*!<R/W ‘b0  Reset SDM */
#define MIPI_MASK_F390K                             ((u32)0x00000003 << 1)          /*!<R/W ‘h1  F390K output delay, not used */
#define MIPI_F390K(x)                               ((u32)(((x) & 0x00000003) << 1))
#define MIPI_GET_F390K(x)                           ((u32)(((x >> 1) & 0x00000003)))
#define MIPI_BIT_EN_WDOG                            ((u32)0x00000001 << 0)          /*!<R/W ‘b0  Enable watch dog function 0: disable Watch Dog function 1: enable Watch Dog function */
/** @} */

/**
 * @defgroup MIPI_RX_ESCAPE_READ_0
 * @brief
 * @{
 **/
#define MIPI_BIT_LANE1_ESC_VLD                      ((u32)0x00000001 << 17)          /*!<R ‘h0  Esc command valid */
#define MIPI_BIT_LANE0_ESC_VLD                      ((u32)0x00000001 << 16)          /*!<R ‘h0  Esc command valid */
#define MIPI_MASK_LANE1_ESC_CMD                     ((u32)0x000000FF << 8)          /*!<R ‘h0  Esc command */
#define MIPI_LANE1_ESC_CMD(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_LANE1_ESC_CMD(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_LANE0_ESC_CMD                     ((u32)0x000000FF << 0)          /*!<R ‘h0  Esc command */
#define MIPI_LANE0_ESC_CMD(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_LANE0_ESC_CMD(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_RX_ESCAPE_READ_1
 * @brief
 * @{
 **/
#define MIPI_BIT_LANE3_ESC_VLD                      ((u32)0x00000001 << 17)          /*!<R ‘h0  Esc command valid */
#define MIPI_BIT_LANE2_ESC_VLD                      ((u32)0x00000001 << 16)          /*!<R ‘h0  Esc command valid */
#define MIPI_MASK_LANE3_ESC_CMD                     ((u32)0x000000FF << 8)          /*!<R ‘h0  Esc command */
#define MIPI_LANE3_ESC_CMD(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_LANE3_ESC_CMD(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_LANE2_ESC_CMD                     ((u32)0x000000FF << 0)          /*!<R ‘h0  Esc command */
#define MIPI_LANE2_ESC_CMD(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_LANE2_ESC_CMD(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_Deglitch_lp
 * @brief
 * @{
 **/
#define MIPI_BIT_LP_SWITCH_DEGLITCH                 ((u32)0x00000001 << 16)          /*!<R/W ‘h0  LP deglitch enable */
#define MIPI_MASK_PH_SEL                            ((u32)0x0000FFFF << 0)          /*!<R/W ‘h00  Delay chain phase select */
#define MIPI_PH_SEL(x)                              ((u32)(((x) & 0x0000FFFF) << 0))
#define MIPI_GET_PH_SEL(x)                          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup MIPI_LP_BIST_1
 * @brief
 * @{
 **/
#define MIPI_MASK_PACKET_CNT_LANE1                  ((u32)0x000000FF << 24)          /*!<R ‘h0  Packet counter lane1 */
#define MIPI_PACKET_CNT_LANE1(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define MIPI_GET_PACKET_CNT_LANE1(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define MIPI_MASK_ERR_CNT_LANE1                     ((u32)0x000000FF << 16)          /*!<R ‘h0  Error counter lane1 */
#define MIPI_ERR_CNT_LANE1(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_ERR_CNT_LANE1(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_PACKET_CNT_LANE0                  ((u32)0x000000FF << 8)          /*!<R ‘h0  Packet counter lane0 */
#define MIPI_PACKET_CNT_LANE0(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_PACKET_CNT_LANE0(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_ERR_CNT_LANE0                     ((u32)0x000000FF << 0)          /*!<R ‘h0  Error counter lane0 */
#define MIPI_ERR_CNT_LANE0(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_ERR_CNT_LANE0(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_LP_BIST_2
 * @brief
 * @{
 **/
#define MIPI_MASK_PACKET_CNT_LANE3                  ((u32)0x000000FF << 24)          /*!<R ‘h0  Packet counter lane3 */
#define MIPI_PACKET_CNT_LANE3(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define MIPI_GET_PACKET_CNT_LANE3(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define MIPI_MASK_ERR_CNT_LANE3                     ((u32)0x000000FF << 16)          /*!<R ‘h0  Error counter lane3 */
#define MIPI_ERR_CNT_LANE3(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define MIPI_GET_ERR_CNT_LANE3(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define MIPI_MASK_PACKET_CNT_LANE2                  ((u32)0x000000FF << 8)          /*!<R ‘h0  Packet counter lane2 */
#define MIPI_PACKET_CNT_LANE2(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define MIPI_GET_PACKET_CNT_LANE2(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define MIPI_MASK_ERR_CNT_LANE2                     ((u32)0x000000FF << 0)          /*!<R ‘h0  Error counter lane2 */
#define MIPI_ERR_CNT_LANE2(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define MIPI_GET_ERR_CNT_LANE2(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup MIPI_lane_pn_swap_1
 * @brief
 * @{
 **/
#define MIPI_BIT_WKUP_INVERSE                       ((u32)0x00000001 << 26)          /*!<R/W ‘h0  HS wakeup signal inverse */
#define MIPI_BIT_ENHS_INVERSE                       ((u32)0x00000001 << 25)          /*!<R/W ‘h0  HS enhs signal inverse */
#define MIPI_BIT_LANE4_PN_SWAP                      ((u32)0x00000001 << 24)          /*!<R/W ‘h0  PN swap in digital lane 4 */
#define MIPI_BIT_LANE3_PN_SWAP                      ((u32)0x00000001 << 23)          /*!<R/W ‘h0  PN swap in digital lane 3 */
#define MIPI_BIT_LANE2_PN_SWAP                      ((u32)0x00000001 << 22)          /*!<R/W ‘h0  PN swap in digital lane 2 */
#define MIPI_BIT_LANE1_PN_SWAP                      ((u32)0x00000001 << 21)          /*!<R/W ‘h0  PN swap in digital lane 1 */
#define MIPI_BIT_LANE0_PN_SWAP                      ((u32)0x00000001 << 20)          /*!<R/W ‘h0  PN swap in digital lane 0 */
#define MIPI_MASK_TX_LANE4_SEL                      ((u32)0x00000007 << 16)          /*!<R/W ‘h4  Tx Lane swap select If digital lane 4 connects to analog lane 4, reg_tx_lane4_sel needs to set to 3‘d4 */
#define MIPI_TX_LANE4_SEL(x)                        ((u32)(((x) & 0x00000007) << 16))
#define MIPI_GET_TX_LANE4_SEL(x)                    ((u32)(((x >> 16) & 0x00000007)))
#define MIPI_MASK_TX_LANE3_SEL                      ((u32)0x00000007 << 12)          /*!<R/W ‘h4  Tx Lane swap select If digital lane 4 connects to analog lane 3, reg_tx_lane3_sel needs to set to 3‘d4 */
#define MIPI_TX_LANE3_SEL(x)                        ((u32)(((x) & 0x00000007) << 12))
#define MIPI_GET_TX_LANE3_SEL(x)                    ((u32)(((x >> 12) & 0x00000007)))
#define MIPI_MASK_TX_LANE2_SEL                      ((u32)0x00000007 << 8)          /*!<R/W ‘h4  Tx Lane swap select If digital lane 4 connects to analog lane 2, reg_tx_lane2_sel needs to set to 3‘d4 */
#define MIPI_TX_LANE2_SEL(x)                        ((u32)(((x) & 0x00000007) << 8))
#define MIPI_GET_TX_LANE2_SEL(x)                    ((u32)(((x >> 8) & 0x00000007)))
#define MIPI_MASK_TX_LANE1_SEL                      ((u32)0x00000007 << 4)          /*!<R/W ‘h1  Tx Lane swap select If digital lane 4 connects to analog lane 1, reg_tx_lane1_sel needs to set to 3‘d4 */
#define MIPI_TX_LANE1_SEL(x)                        ((u32)(((x) & 0x00000007) << 4))
#define MIPI_GET_TX_LANE1_SEL(x)                    ((u32)(((x >> 4) & 0x00000007)))
#define MIPI_MASK_TX_LANE0_SEL                      ((u32)0x00000007 << 0)          /*!<R/W ‘h0  Tx Lane swap select If digital lane 4 connects to analog lane 0, reg_tx_lane0_sel needs to set to 3‘d4 */
#define MIPI_TX_LANE0_SEL(x)                        ((u32)(((x) & 0x00000007) << 0))
#define MIPI_GET_TX_LANE0_SEL(x)                    ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**
 * @defgroup MIPI_lane_pn_swap_2
 * @brief
 * @{
 **/
#define MIPI_MASK_RX_LANE4_SEL                      ((u32)0x00000007 << 16)          /*!<R/W ‘h4  Rx Lane swap select If digital lane 4 connects to analog lane 4, reg_rx_lane4_sel needs to set to 3‘d4 */
#define MIPI_RX_LANE4_SEL(x)                        ((u32)(((x) & 0x00000007) << 16))
#define MIPI_GET_RX_LANE4_SEL(x)                    ((u32)(((x >> 16) & 0x00000007)))
#define MIPI_MASK_RX_LANE3_SEL                      ((u32)0x00000007 << 12)          /*!<R/W ‘h3  Rx Lane swap select If digital lane 3 connects to analog lane 4, reg_rx_lane3_sel needs to set to 3‘d4 */
#define MIPI_RX_LANE3_SEL(x)                        ((u32)(((x) & 0x00000007) << 12))
#define MIPI_GET_RX_LANE3_SEL(x)                    ((u32)(((x >> 12) & 0x00000007)))
#define MIPI_MASK_RX_LANE2_SEL                      ((u32)0x00000007 << 8)          /*!<R/W ‘h2  Rx Lane swap select If digital lane 2 connects to analog lane 4, reg_rx_lane2_sel needs to set to 3‘d4 */
#define MIPI_RX_LANE2_SEL(x)                        ((u32)(((x) & 0x00000007) << 8))
#define MIPI_GET_RX_LANE2_SEL(x)                    ((u32)(((x >> 8) & 0x00000007)))
#define MIPI_MASK_RX_LANE1_SEL                      ((u32)0x00000007 << 4)          /*!<R/W ‘h1  Rx Lane swap select If digital lane 1 connects to analog lane 4, reg_rx_lane1_sel needs to set to 3‘d4 */
#define MIPI_RX_LANE1_SEL(x)                        ((u32)(((x) & 0x00000007) << 4))
#define MIPI_GET_RX_LANE1_SEL(x)                    ((u32)(((x >> 4) & 0x00000007)))
#define MIPI_MASK_RX_LANE0_SEL                      ((u32)0x00000007 << 0)          /*!<R/W ‘h0  Rx Lane swap select If digital lane 0 connects to analog lane 4, reg_rx_lane0_sel needs to set to 3‘d4 */
#define MIPI_RX_LANE0_SEL(x)                        ((u32)(((x) & 0x00000007) << 0))
#define MIPI_GET_RX_LANE0_SEL(x)                    ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**
 * @defgroup MIPI_MPLL
 * @brief
 * @{
 **/
#define MIPI_BIT_BYPASS_8X                          ((u32)0x00000001 << 27)          /*!<R/W ‘h0  CLK1X_DIG select signal, replace TXPLL ouput */
#define MIPI_MASK_TXTST_SEL                         ((u32)0x00000003 << 25)          /*!<R/W ‘h2  P2S debug output select */
#define MIPI_TXTST_SEL(x)                           ((u32)(((x) & 0x00000003) << 25))
#define MIPI_GET_TXTST_SEL(x)                       ((u32)(((x >> 25) & 0x00000003)))
#define MIPI_BIT_TXTST_EN                           ((u32)0x00000001 << 24)          /*!<R/W ‘h0  P2S debug enable */
#define MIPI_BIT_MPLL_CCO_BAND                      ((u32)0x00000001 << 18)          /*!<R/W ‘h0  Tx PLL */
#define MIPI_BIT_EN_TX_LDO                          ((u32)0x00000001 << 17)          /*!<R/W ‘h1  1.2v TXLA LDO enalbe */
#define MIPI_BIT_CMU_POW                            ((u32)0x00000001 << 16)          /*!<R/W ‘h1  TXPLL enable signal */
#define MIPI_BIT_MPLL_TESTEN                        ((u32)0x00000001 << 15)          /*!<R/W ‘h0  TXPLL's debug enable */
#define MIPI_MASK_MPLL_PI_ISEL                      ((u32)0x00000003 << 13)          /*!<R/W ‘h2  TXPLL's PI bias current config */
#define MIPI_MPLL_PI_ISEL(x)                        ((u32)(((x) & 0x00000003) << 13))
#define MIPI_GET_MPLL_PI_ISEL(x)                    ((u32)(((x >> 13) & 0x00000003)))
#define MIPI_BIT_MPLL_PI_C                          ((u32)0x00000001 << 12)          /*!<R/W ‘h1  TXPLL's PI Cap config */
#define MIPI_BIT_MPLL_PI_BYPASS                     ((u32)0x00000001 << 11)          /*!<R/W ‘h1  TXPLL's PI output bypass signal */
#define MIPI_MASK_MPLL_PDIV                         ((u32)0x00000003 << 9)          /*!<R/W ‘h0  TXPLL's pre dividre control */
#define MIPI_MPLL_PDIV(x)                           ((u32)(((x) & 0x00000003) << 9))
#define MIPI_GET_MPLL_PDIV(x)                       ((u32)(((x >> 9) & 0x00000003)))
#define MIPI_MASK_MPLL_LPF_SR                       ((u32)0x00000007 << 6)          /*!<R/W ‘h4  TXPLL's LPF R control */
#define MIPI_MPLL_LPF_SR(x)                         ((u32)(((x) & 0x00000007) << 6))
#define MIPI_GET_MPLL_LPF_SR(x)                     ((u32)(((x >> 6) & 0x00000007)))
#define MIPI_MASK_MPLL_LDO_VREF                     ((u32)0x00000003 << 4)          /*!<R/W ‘h2  TXPLL's LDO reference control */
#define MIPI_MPLL_LDO_VREF(x)                       ((u32)(((x) & 0x00000003) << 4))
#define MIPI_GET_MPLL_LDO_VREF(x)                   ((u32)(((x >> 4) & 0x00000003)))
#define MIPI_MASK_MPLL_ICP                          ((u32)0x00000003 << 2)          /*!<R/W ‘h2  TXPLL's charge pump control */
#define MIPI_MPLL_ICP(x)                            ((u32)(((x) & 0x00000003) << 2))
#define MIPI_GET_MPLL_ICP(x)                        ((u32)(((x >> 2) & 0x00000003)))
#define MIPI_BIT_MPLL_DIV_CYC                       ((u32)0x00000001 << 1)          /*!<R/W ‘h1  TXPLL's loop divider pulse width control */
#define MIPI_BIT_MPLL_BIG_CCO                       ((u32)0x00000001 << 0)          /*!<R/W ‘h1  TXPLL KVCO selection */
/** @} */

/**
 * @defgroup MIPI_LF
 * @brief
 * @{
 **/
#define MIPI_MASK_CK8X_LAG                          ((u32)0x00000007 << 27)          /*!<R/W ‘h10  CK8X lag config */
#define MIPI_CK8X_LAG(x)                            ((u32)(((x) & 0x00000007) << 27))
#define MIPI_GET_CK8X_LAG(x)                        ((u32)(((x >> 27) & 0x00000007)))
#define MIPI_BIT_CK8X_INV                           ((u32)0x00000001 << 26)          /*!<R/W ‘h1  CK8X inverser config */
#define MIPI_BIT_CK1X_INV                           ((u32)0x00000001 << 25)          /*!<R/W ‘h1  CK1X inverse config */
#define MIPI_MASK_DATA2_DLY                         ((u32)0x0000001F << 10)          /*!<R/W ‘h10  DATA2 lane's skew delay control */
#define MIPI_DATA2_DLY(x)                           ((u32)(((x) & 0x0000001F) << 10))
#define MIPI_GET_DATA2_DLY(x)                       ((u32)(((x >> 10) & 0x0000001F)))
#define MIPI_MASK_DATA1_DLY                         ((u32)0x0000001F << 5)          /*!<R/W ‘h10  DATA1 lane's skew delay control */
#define MIPI_DATA1_DLY(x)                           ((u32)(((x) & 0x0000001F) << 5))
#define MIPI_GET_DATA1_DLY(x)                       ((u32)(((x >> 5) & 0x0000001F)))
#define MIPI_MASK_DATA0_DLY                         ((u32)0x0000001F << 0)          /*!<R/W ‘h10  DATA0 lane's skew delay control */
#define MIPI_DATA0_DLY(x)                           ((u32)(((x) & 0x0000001F) << 0))
#define MIPI_GET_DATA0_DLY(x)                       ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**
 * @defgroup MIPI_TXF
 * @brief
 * @{
 **/
#define MIPI_MASK_CK8X_PHS                          ((u32)0x00000003 << 26)          /*!<R/W ‘h2  CK8X phase select, not used here */
#define MIPI_CK8X_PHS(x)                            ((u32)(((x) & 0x00000003) << 26))
#define MIPI_GET_CK8X_PHS(x)                        ((u32)(((x >> 26) & 0x00000003)))
#define MIPI_MASK_Z0_TX                             ((u32)0x0000000F << 22)          /*!<R/W ‘h8  TXLA's term reset control */
#define MIPI_Z0_TX(x)                               ((u32)(((x) & 0x0000000F) << 22))
#define MIPI_GET_Z0_TX(x)                           ((u32)(((x >> 22) & 0x0000000F)))
#define MIPI_MASK_TX_PRERES_SEL                     ((u32)0x00000003 << 20)          /*!<R/W ‘h2  TXLA's pre amp reload control */
#define MIPI_TX_PRERES_SEL(x)                       ((u32)(((x) & 0x00000003) << 20))
#define MIPI_GET_TX_PRERES_SEL(x)                   ((u32)(((x >> 20) & 0x00000003)))
#define MIPI_MASK_TX_PRECUR_SEL                     ((u32)0x00000003 << 18)          /*!<R/W ‘h2  TXLA's pre amp current control */
#define MIPI_TX_PRECUR_SEL(x)                       ((u32)(((x) & 0x00000003) << 18))
#define MIPI_GET_TX_PRECUR_SEL(x)                   ((u32)(((x >> 18) & 0x00000003)))
#define MIPI_MASK_SR_CTRL                           ((u32)0x00000003 << 16)          /*!<R/W ‘h2  Tx PAD GPIO mode slew rate control */
#define MIPI_SR_CTRL(x)                             ((u32)(((x) & 0x00000003) << 16))
#define MIPI_GET_SR_CTRL(x)                         ((u32)(((x >> 16) & 0x00000003)))
#define MIPI_BIT_RANGE_MODE                         ((u32)0x00000001 << 15)          /*!<R/W ‘h0  TXLA's LDO output level config */
#define MIPI_BIT_OVERSHOOT_BP                       ((u32)0x00000001 << 14)          /*!<R/W ‘h0  TXLA's overshoot bypass */
#define MIPI_MASK_LP_DRV                            ((u32)0x00000003 << 12)          /*!<R/W ‘h2  LP mode Tx slew rate control */
#define MIPI_LP_DRV(x)                              ((u32)(((x) & 0x00000003) << 12))
#define MIPI_GET_LP_DRV(x)                          ((u32)(((x >> 12) & 0x00000003)))
#define MIPI_MASK_LPLDO_LVL                         ((u32)0x00000007 << 9)          /*!<R/W ‘h4  TXLA's LDO output level config */
#define MIPI_LPLDO_LVL(x)                           ((u32)(((x) & 0x00000007) << 9))
#define MIPI_GET_LPLDO_LVL(x)                       ((u32)(((x >> 9) & 0x00000007)))
#define MIPI_BIT_IB_EN                              ((u32)0x00000001 << 8)          /*!<R/W ‘h1  HS CML bias current enable */
#define MIPI_BIT_EN_LV_LDO                          ((u32)0x00000001 << 7)          /*!<R/W ‘h1  1.5v TXPLL LDO enalbe */
#define MIPI_MASK_EMP                               ((u32)0x00000007 << 4)          /*!<R/W ‘h4  HS CML driver deemphasis swing config */
#define MIPI_EMP(x)                                 ((u32)(((x) & 0x00000007) << 4))
#define MIPI_GET_EMP(x)                             ((u32)(((x >> 4) & 0x00000007)))
#define MIPI_BIT_DEEMP_EN                           ((u32)0x00000001 << 3)          /*!<R/W ‘h1  HS CML driver deemphasis enalbe signal */
#define MIPI_MASK_AMP                               ((u32)0x00000007 << 0)          /*!<R/W ‘h4  HS CML driver output swing config */
#define MIPI_AMP(x)                                 ((u32)(((x) & 0x00000007) << 0))
#define MIPI_GET_AMP(x)                             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**
 * @defgroup MIPI_DF
 * @brief
 * @{
 **/
#define MIPI_BIT_PWCT                               ((u32)0x00000001 << 25)          /*!<R/W ‘h0  Analog power switch enable */
#define MIPI_BIT_MBIAS_EN                           ((u32)0x00000001 << 24)          /*!<R/W ‘h1  PLL post divider, all div4 selection */
#define MIPI_MASK_LPRX_VTH_SEL                      ((u32)0x00000007 << 21)          /*!<R/W ‘h4  PLL post divider, all div4 selection */
#define MIPI_LPRX_VTH_SEL(x)                        ((u32)(((x) & 0x00000007) << 21))
#define MIPI_GET_LPRX_VTH_SEL(x)                    ((u32)(((x >> 21) & 0x00000007)))
#define MIPI_MASK_LPCD_LEVTH_SEL                    ((u32)0x00000007 << 18)          /*!<R/W ‘h4  PLL post divider, all div4 selection */
#define MIPI_LPCD_LEVTH_SEL(x)                      ((u32)(((x) & 0x00000007) << 18))
#define MIPI_GET_LPCD_LEVTH_SEL(x)                  ((u32)(((x >> 18) & 0x00000007)))
#define MIPI_MASK_LPCD_HEVTH_SEL                    ((u32)0x00000007 << 15)          /*!<R/W ‘h4  PLL post divider, all div4 selection */
#define MIPI_LPCD_HEVTH_SEL(x)                      ((u32)(((x) & 0x00000007) << 15))
#define MIPI_GET_LPCD_HEVTH_SEL(x)                  ((u32)(((x >> 15) & 0x00000007)))
#define MIPI_MASK_EN_LPRX                           ((u32)0x00000007 << 10)          /*!<R/W ‘h7   */
#define MIPI_EN_LPRX(x)                             ((u32)(((x) & 0x00000007) << 10))
#define MIPI_GET_EN_LPRX(x)                         ((u32)(((x >> 10) & 0x00000007)))
#define MIPI_MASK_EN_LPCD                           ((u32)0x00000007 << 5)          /*!<R/W ‘h7   */
#define MIPI_EN_LPCD(x)                             ((u32)(((x) & 0x00000007) << 5))
#define MIPI_GET_EN_LPCD(x)                         ((u32)(((x >> 5) & 0x00000007)))
#define MIPI_BIT_POSTDIV4                           ((u32)0x00000001 << 4)          /*!<R/W ‘h1  PLL post divider, all div4 selection */
#define MIPI_BIT_POSTDIV3                           ((u32)0x00000001 << 3)          /*!<R/W ‘h1  PLL post divider, all div4 selection */
#define MIPI_BIT_POSTDIV2                           ((u32)0x00000001 << 2)          /*!<R/W ‘h1  PLL post divider, all div4 selection */
#define MIPI_BIT_EN_ALL_DIV4                        ((u32)0x00000001 << 1)          /*!<R/W ‘h0  PLL post divider, all div4 selection */
#define MIPI_BIT_EN_ALL_DIV2                        ((u32)0x00000001 << 0)          /*!<R/W ‘h0  PLL post divider, all div2 selection */
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/**
 * @defgroup MIPI_Exported_Types MIPI Exported Types
 * @{
 * @brief AMEBAD2_MIPI Register Declaration
 **/
typedef struct {
	__IO uint32_t MIPI_MAIN_CTRL;                         /*!<  Register,  Address offset: 0x00 */
	__IO uint32_t MIPI_CKLANE_CTRL;                       /*!<  Register,  Address offset: 0x04 */
	__IO uint32_t MIPI_LANE_CTRL;                         /*!<  Register,  Address offset: 0x08 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__IO uint32_t MIPI_INTE;                              /*!<  Register,  Address offset: 0x10 */
	__IO uint32_t MIPI_INTS;                              /*!<  Register,  Address offset: 0x14 */
	__IO uint32_t MIPI_INTS_ACPU;                         /*!<  Register,  Address offset: 0x18 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x1C */
	__I  uint32_t MIPI_RCMD_x[3];                         /*!<  Register,  Address offset: 0x20-0x28 */
	__IO uint32_t RSVD2[53];                              /*!<  Reserved,  Address offset:0x2C-0xFC */
	__IO uint32_t MIPI_TC0;                               /*!<  Register,  Address offset: 0x100 */
	__IO uint32_t MIPI_TC1;                               /*!<  Register,  Address offset: 0x104 */
	__IO uint32_t MIPI_TC2;                               /*!<  Register,  Address offset: 0x108 */
	__IO uint32_t MIPI_TC3;                               /*!<  Register,  Address offset: 0x10C */
	__IO uint32_t MIPI_TC4;                               /*!<  Register,  Address offset: 0x110 */
	__IO uint32_t MIPI_TC5;                               /*!<  Register,  Address offset: 0x114 */
	__IO uint32_t RSVD3[58];                              /*!<  Reserved,  Address offset:0x118-0x1FC */
	__IO uint32_t MIPI_IDMA0;                             /*!<  Register,  Address offset: 0x200 */
	__IO uint32_t MIPI_IDMA1;                             /*!<  Register,  Address offset: 0x204 */
	__IO uint32_t MIPI_IDMA2;                             /*!<  Register,  Address offset: 0x208 */
	__IO uint32_t RSVD4[61];                              /*!<  Reserved,  Address offset:0x20C-0x2FC */
	__IO uint32_t MIPI_TO1;                               /*!<  Register,  Address offset: 0x300 */
	__IO uint32_t MIPI_TO2;                               /*!<  Register,  Address offset: 0x304 */
	__IO uint32_t MIPI_TO3;                               /*!<  Register,  Address offset: 0x308 */
	__IO uint32_t MIPI_TO4;                               /*!<  Register,  Address offset: 0x30C */
	__IO uint32_t RSVD5[60];                              /*!<  Reserved,  Address offset:0x310-0x3FC */
	__IO uint32_t MIPI_CMD_GO;                            /*!<  Register,  Address offset: 0x400 */
	__IO uint32_t MIPI_CMD0;                              /*!<  Register,  Address offset: 0x404 */
	__IO uint32_t RSVD6[62];                              /*!<  Reserved,  Address offset:0x408-0x4FC */
	__I  uint32_t MIPI_DPHY_STATUS0;                      /*!<  Register,  Address offset: 0x500 */
	__I  uint32_t MIPI_DPHY_STATUS1;                      /*!<  Register,  Address offset: 0x504 */
	__IO uint32_t MIPI_DPHY_ERR;                          /*!<  Register,  Address offset: 0x508 */
	__IO uint32_t RSVD7[61];                              /*!<  Reserved,  Address offset:0x50C-0x5FC */
	__IO uint32_t MIPI_BIST;                              /*!<  Register,  Address offset: 0x600 */
	__IO uint32_t RSVD8[3];                               /*!<  Reserved,  Address offset:0x604-0x60C */
	__IO uint32_t MIPI_PAT_GEN;                           /*!<  Register,  Address offset: 0x610 */
	__IO uint32_t RSVD9[59];                              /*!<  Reserved,  Address offset:0x614-0x6FC */
	__IO uint32_t MIPI_DUMMY0;                            /*!<  Register,  Address offset: 0x700 */
	__IO uint32_t MIPI_DUMMY1;                            /*!<  Register,  Address offset: 0x704 */
	__IO uint32_t MIPI_DBG;                               /*!<  Register,  Address offset: 0x708 */
	__I  uint32_t MIPI_CLK_DET0;                          /*!<  Register,  Address offset: 0x70C */
	__I  uint32_t MIPI_CLK_DET1;                          /*!<  Register,  Address offset: 0x710 */
	__I  uint32_t MIPI_DBG_ST0;                           /*!<  Register,  Address offset: 0x714 */
	__I  uint32_t MIPI_DBG_ST1;                           /*!<  Register,  Address offset: 0x718 */
	__I  uint32_t MIPI_DBG_ST2;                           /*!<  Register,  Address offset: 0x71C */
	__I  uint32_t MIPI_DBG_ST3;                           /*!<  Register,  Address offset: 0x720 */
	__I  uint32_t MIPI_DBG_ST4;                           /*!<  Register,  Address offset: 0x724 */
	__IO uint32_t RSVD10[54];                             /*!<  Reserved,  Address offset:0x728-0x7FC */
	__IO uint32_t MIPI_CLOCK_GEN;                         /*!<  Register,  Address offset: 0x800 */
	__IO uint32_t MIPI_ESCAPE_TX_CLK_0;                   /*!<  Register,  Address offset: 0x804 */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_0;                  /*!<  Register,  Address offset: 0x808 */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_1;                  /*!<  Register,  Address offset: 0x80C */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_2;                  /*!<  Register,  Address offset: 0x810 */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_3;                  /*!<  Register,  Address offset: 0x814 */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_4;                  /*!<  Register,  Address offset: 0x818 */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_5;                  /*!<  Register,  Address offset: 0x81C */
	__IO uint32_t MIPI_ESCAPE_TX_DATA_6;                  /*!<  Register,  Address offset: 0x820 */
	__IO uint32_t MIPI_ESCAPE_RX_DATA_0;                  /*!<  Register,  Address offset: 0x824 */
	__IO uint32_t MIPI_LANE_SWAP_CTRL;                    /*!<  Register,  Address offset: 0x828 */
	__IO uint32_t MIPI_LANE_LP_BIST_0;                    /*!<  Register,  Address offset: 0x82C */
	__IO uint32_t MIPI_LANE_HS_BIST;                      /*!<  Register,  Address offset: 0x830 */
	__IO uint32_t MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT;/*!<  Register,  Address offset: 0x834 */
	__IO uint32_t MIPI_DEBUG_0;                           /*!<  Register,  Address offset: 0x838 */
	__I  uint32_t MIPI_DEBUG_1;                           /*!<  Register,  Address offset: 0x83C */
	__IO uint32_t MIPI_SSC0;                              /*!<  Register,  Address offset: 0x840 */
	__IO uint32_t MIPI_SSC1;                              /*!<  Register,  Address offset: 0x844 */
	__IO uint32_t MIPI_SSC2;                              /*!<  Register,  Address offset: 0x848 */
	__IO uint32_t MIPI_SSC3;                              /*!<  Register,  Address offset: 0x84C */
	__IO uint32_t MIPI_WATCHDOG;                          /*!<  Register,  Address offset: 0x850 */
	__I  uint32_t MIPI_RX_ESCAPE_READ_0;                  /*!<  Register,  Address offset: 0x854 */
	__I  uint32_t MIPI_RX_ESCAPE_READ_1;                  /*!<  Register,  Address offset: 0x858 */
	__IO uint32_t MIPI_DEGLITCH_LP;                       /*!<  Register,  Address offset: 0x85C */
	__I  uint32_t MIPI_LP_BIST_1;                         /*!<  Register,  Address offset: 0x860 */
	__I  uint32_t MIPI_LP_BIST_2;                         /*!<  Register,  Address offset: 0x864 */
	__IO uint32_t MIPI_LANE_PN_SWAP_1;                    /*!<  Register,  Address offset: 0x868 */
	__IO uint32_t MIPI_LANE_PN_SWAP_2;                    /*!<  Register,  Address offset: 0x86C */
	__IO uint32_t RSVD11[228];                             /*!<  Reserved,  Address offset:0x870-0xBFC */
	__IO uint32_t MIPI_MPLL;                              /*!<  Register,  Address offset: 0xC00 */
	__IO uint32_t MIPI_LF;                                /*!<  Register,  Address offset: 0xC04 */
	__IO uint32_t MIPI_TXF;                               /*!<  Register,  Address offset: 0xC08 */
	__IO uint32_t MIPI_DF;                                /*!<  Register,  Address offset: 0xC0C */
} MIPI_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */
/** @defgroup MIPI_Exported_Constants MIPI Exported Constants
 * @{
 **/

//Please add your defination here
/** @defgroup MIPI_VIDEO_DATA_FORMAT_define
  * @{
  */
#define MIPI_VIDEO_DATA_FORMAT_RGB888				(0x0)
#define MIPI_VIDEO_DATA_FORMAT_RGB565				(0x1)
#define MIPI_VIDEO_DATA_FORMAT_RGB666_PACKED		(0x2)
#define MIPI_VIDEO_DATA_FORMAT_RGB666_LOOSELY		(0x3)
/** @} */

/** @defgroup MIPI_VIDEO_MODE_INTERFACE_define
  * @{
  */
#define MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_PULSES	(0x0)
#define MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_EVENTS	(0x1)
#define MIPI_VIDEO_BURST_MODE						(0x2)
/** @} */
/** @} */

#define MIPI_LPTX_IS_READ(CmdId)		((CmdId) == 0x04 || (CmdId) == 0x14 || (CmdId) == 0x24 || (CmdId) == 0x06)
#define MIPI_LPRX_IS_LONGRead(CmdId)	((CmdId) == 0x1A || (CmdId) == 0x1C)
#define MIPI_LPRX_IS_LONGWrite(CmdId)	((CmdId) == 0x39 || (CmdId) == 0x29)

/**
 * @defgroup MIPI_Exported_Types MIPI Exported Types
 * @{
 * @brief AMEBAD2_MIPI Register Declaration
 **/

typedef struct {
	u8 MIPI_VideoNCmdMode;
	u8 MIPI_VideoDataFormat;
	u8 MIPI_VideoModeInterface;
	u8 MIPI_TxCrcCalculationEn;

	u8 MIPI_RxEccChkEn;
	u8 MIPI_RxCrcChkEn;
	u8 MIPI_EotpEn;
	u8 MIPI_BTADis;

	u8 MIPI_LaneNum;
	u8 MIPI_FrameRate;
	u16 MIPI_VideDataLaneFreq;	/* in Mbps */

	u16 MIPI_HSA;				/* in bytes */
	u16 MIPI_HBP;
	u16 MIPI_HACT;
	u16 MIPI_HFP;

	u16 MIPI_VSA;				/* in lines */
	u16 MIPI_VBP;
	u16 MIPI_VACT;
	u16 MIPI_VFP;

	u16 MIPI_BllpLen;			/* in byteclk cycles */
	u16 MIPI_LineTime;			/* in byteclk cycles */
} MIPI_InitTypeDef;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup MIPI_Exported_Functions MIPI Exported Functions
  * @{
  */
_LONG_CALL_ u32 MIPI_DSI_INTS_Get(MIPI_TypeDef *MIPIx);
_LONG_CALL_ void MIPI_DSI_INTS_Clr(MIPI_TypeDef *MIPIx, u32 DSI_INTS);
_LONG_CALL_ u32 MIPI_DSI_INTS_ACPU_Get(MIPI_TypeDef *MIPIx);
_LONG_CALL_ void MIPI_DSI_INTS_ACPU_Clr(MIPI_TypeDef *MIPIx, u32 DSI_INTS);
_LONG_CALL_ void MIPI_DSI_INT_Config(MIPI_TypeDef *MIPIx, u8 AcpuEn, u8 ScpuEN, u8 IntSelAnS);

_LONG_CALL_ void MIPI_DSI_TO1_Set(MIPI_TypeDef *MIPIx, u8 NewState, u32 TimeoutValue);
_LONG_CALL_ void MIPI_DSI_TO2_Set(MIPI_TypeDef *MIPIx, u8 NewState, u32 TimeoutValue);
_LONG_CALL_ void MIPI_DSI_TO3_Set(MIPI_TypeDef *MIPIx, u8 NewState, u32 TimeoutValue);

_LONG_CALL_ void MIPI_DSI_Main_Ctrl(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct);
_LONG_CALL_ void MIPI_DSI_CMD_Send(MIPI_TypeDef *MIPIx, u8 DataId, u8 Byte0, u8 Byte1);
_LONG_CALL_ u32 MIPI_DSI_CMD_Rxcv_CMD(MIPI_TypeDef *MIPIx, u8 rcmd_idx);
_LONG_CALL_ void MIPI_DSI_CMD_LongPkt_MemQWordRW(MIPI_TypeDef *MIPIx, u32 Addr, u32 *Word0, u32 *Word1, u8 IsRead);

_LONG_CALL_ void MIPI_DSI_Mode_Switch(MIPI_TypeDef *MIPIx, u32 MIPI_VideoNCmdMode);
_LONG_CALL_ void MIPI_StructInit(MIPI_InitTypeDef *MIPI_InitStruct);
_LONG_CALL_ void MIPI_DSI_init(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct);
_LONG_CALL_ void MIPI_DPHY_init(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct);
_LONG_CALL_ void MIPI_Init(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct);

/**
  * @}
  */
/* MANUAL_GEN_END */

/**
  * @}
  */

/**
  * @}
  */

#endif
