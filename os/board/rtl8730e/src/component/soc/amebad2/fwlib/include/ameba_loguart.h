#ifndef _AMEBAD2_LOGUART_H_
#define _AMEBAD2_LOGUART_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup LOGUART
  * @brief LOGUART driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup LOGUART_Register_Definitions LOGUART Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup LOGUART_UART_DLL
 * @brief UART divisor length register
 * @{
 **/
#define LOGUART_MASK_BAUD                   ((u32)0x000000FF << 0)          /*!<R/W 0x0  The low 8 bits of divisor coefficient (not used) */
#define LOGUART_BAUD(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define LOGUART_GET_BAUD(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup LOGUART_UART_IER
 * @brief UART interrupt enable register
 * @{
 **/
#define LOGUART_MASK_ETPFEI                 ((u32)0x00000007 << 8)          /*!<R/W 0x0  Tx Path FIFO Empty interrupt (ETPFEI) (Tx Path FIFO empty) enable control *000: All Tx Path FIFO empty interrupt disabled *001: Tx Path1 FIFO Empty interrupt enabled *010: Tx Path2 FIFO Empty interrupt enabled *011: Tx Path3 FIFO Empty interrupt enabled *100: Tx Path4 FIFO Empty interrupt enabled *Others: Reserved */
#define LOGUART_ETPFEI(x)                   ((u32)(((x) & 0x00000007) << 8))
#define LOGUART_GET_ETPFEI(x)               ((u32)(((x >> 8) & 0x00000007)))
#define LOGUART_BIT_ETOI                    ((u32)0x00000001 << 5)          /*!<R/W 0x0  Rx timeout interrupt (ETOI) enable */
#define LOGUART_ETOI(x)                     ((u32)(((x) & 0x00000001) << 5))
#define LOGUART_BIT_EMDI                    ((u32)0x00000001 << 4)          /*!<R/W 0x0  Rx path monitor done interrupt (EMDI) enable */
#define LOGUART_EMDI(x)                     ((u32)(((x) & 0x00000001) << 4))
#define LOGUART_BIT_EDSSI                   ((u32)0x00000001 << 3)          /*!<R/W 0x0  Modem Status Interrupt (EDSSI) (modem status transition) enable */
#define LOGUART_EDSSI(x)                    ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_ELSI                    ((u32)0x00000001 << 2)          /*!<R/W 0x0  Receiver Line Status Interrupt (ELSI) (receiver line status) enable */
#define LOGUART_ELSI(x)                     ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_ERBI                    ((u32)0x00000001 << 0)          /*!<R/W 0x0  Received Data Available Interrupt (ERBFI) (Rx trigger) enable */
#define LOGUART_ERBI(x)                     ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_IIR
 * @brief UART interrupt identification register
 * @{
 **/
#define LOGUART_BIT_INT_PEND                ((u32)0x00000001 << 0)          /*!<R 0x1  Indicate if the interrupt is pending. *0: Pending, and the IIR contents may be used as a pointer to the appropriate interrupt service routine. *1: No interrupt is pending. */
#define LOGUART_INT_PEND(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_LCR
 * @brief UART line control register
 * @{
 **/
#define LOGUART_BIT_DLAB                    ((u32)0x00000001 << 7)          /*!<R/W 0x0  Divisor Latch Access bit, and control the accessibility of the DLL, DLM and IER registers * 0: The divisor latches not accessible; IER accessible * 1: The divisor latches accessible; DLL and DLM accessible Note: THR/RBR don't care the value of dlab bit */
#define LOGUART_DLAB(x)                     ((u32)(((x) & 0x00000001) << 7))
#define LOGUART_BIT_BRCTL                   ((u32)0x00000001 << 6)          /*!<R/W 0x0  Break Control bit * 0: break signal disable * 1: break signal enable (logic '0') */
#define LOGUART_BRCTL(x)                    ((u32)(((x) & 0x00000001) << 6))
#define LOGUART_BIT_STKP                    ((u32)0x00000001 << 5)          /*!<R/W 0x0  Stick Parity bit * 0: Not stick parity, the parity type is ODD or EVEN (depending on eps) * 1: Stick parity, eps=1: low parity (1'b0); eps=0: high parity (1'b1) */
#define LOGUART_STKP(x)                     ((u32)(((x) & 0x00000001) << 5))
#define LOGUART_BIT_EPS                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  Even Parity select * 0: Odd parity, the number of Logic "1" is odd. * 1: Even parity, the number of Logic "1" is even. */
#define LOGUART_EPS(x)                      ((u32)(((x) & 0x00000001) << 4))
#define LOGUART_BIT_PEN                     ((u32)0x00000001 << 3)          /*!<R/W 0x0  Parity Enable * 0: No parity bit * 1: Parity bit is generated on each outgoing character and is checked on each incoming one */
#define LOGUART_PEN(x)                      ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_STB                     ((u32)0x00000001 << 2)          /*!<R/W 0x0  Stop bits length selection * 0: 1 stop bit * 1: 2 stop bits Note: The receiver always checks the first stop bit only. */
#define LOGUART_STB(x)                      ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_WLS0                    ((u32)0x00000001 << 0)          /*!<R/W 0x1  Word length selection * 0: 7 bits data * 1: 8 bits data */
#define LOGUART_WLS0(x)                     ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_MCR
 * @brief UART modem control register
 * @{
 **/
#define LOGUART_BIT_AFE                     ((u32)0x00000001 << 5)          /*!<R/W 0x0  Auto flow control enable */
#define LOGUART_AFE(x)                      ((u32)(((x) & 0x00000001) << 5))
#define LOGUART_BIT_LOOP_EN                 ((u32)0x00000001 << 4)          /*!<R/W 0x0  LoopBack mode * 0: Normal operation mode * 1: Loopback mode */
#define LOGUART_LOOP_EN(x)                  ((u32)(((x) & 0x00000001) << 4))
#define LOGUART_BIT_OUT2                    ((u32)0x00000001 << 3)          /*!<R/W 0x0  Auxiliary output 2 Bit[3] affects the OUT2_ output in a manner identical to that described above set to 0 In loopback mode, connected to Data Carrier Detect (DCD) */
#define LOGUART_OUT2(x)                     ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_OUT1                    ((u32)0x00000001 << 2)          /*!<R/W 0x0  Auxiliary output 1 Bit3 affects the OUT1_ output in a manner identical to that described above set to 0 In loopback mode, connected to Ring Indicator (RI) */
#define LOGUART_OUT1(x)                     ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_RTS                     ((u32)0x00000001 << 1)          /*!<R/W/ES 0x0  Request to Send (RTS) signal control * 0: RTS is logic 1. * 1: RTS is logic 0. This bit controls the RTS_ output, Bit 1 affects the RTS_ output in a manner identical to that described above set to 0. */
#define LOGUART_RTS(x)                      ((u32)(((x) & 0x00000001) << 1))
#define LOGUART_BIT_DTR                     ((u32)0x00000001 << 0)          /*!<R/W 0x0  Data Terminal Ready (DTR) signal control * 0: DTR is logic 1. * 1: DTR is logic 0. This bit controls the DTR output. */
#define LOGUART_DTR(x)                      ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_LSR
 * @brief UART line status register
 * @{
 **/
#define LOGUART_MASK_RX_FIFO_PTR            ((u32)0x000000FF << 24)          /*!<R   Data bytes count in the receive FIFO: the value of rx_fifo_ptr[7:0] indicates the data bytes number received in the FIFO. */
#define LOGUART_RX_FIFO_PTR(x)              ((u32)(((x) & 0x000000FF) << 24))
#define LOGUART_GET_RX_FIFO_PTR(x)          ((u32)(((x >> 24) & 0x000000FF)))
#define LOGUART_BIT_TP4F_NOT_FULL           ((u32)0x00000001 << 23)          /*!<R   Tx Path4 FIFO not full indicator */
#define LOGUART_TP4F_NOT_FULL(x)            ((u32)(((x) & 0x00000001) << 23))
#define LOGUART_BIT_TP3F_NOT_FULL           ((u32)0x00000001 << 22)          /*!<R   Tx Path3 FIFO not full indicator */
#define LOGUART_TP3F_NOT_FULL(x)            ((u32)(((x) & 0x00000001) << 22))
#define LOGUART_BIT_TP2F_NOT_FULL           ((u32)0x00000001 << 21)          /*!<R   Tx Path2 FIFO not full indicator */
#define LOGUART_TP2F_NOT_FULL(x)            ((u32)(((x) & 0x00000001) << 21))
#define LOGUART_BIT_TP1F_NOT_FULL           ((u32)0x00000001 << 20)          /*!<R   Tx Path1 FIFO not full indicator */
#define LOGUART_TP1F_NOT_FULL(x)            ((u32)(((x) & 0x00000001) << 20))
#define LOGUART_BIT_TP4F_EMPTY              ((u32)0x00000001 << 19)          /*!<R   Tx Path4 FIFO empty indicator */
#define LOGUART_TP4F_EMPTY(x)               ((u32)(((x) & 0x00000001) << 19))
#define LOGUART_BIT_TP3F_EMPTY              ((u32)0x00000001 << 18)          /*!<R   Tx Path3 FIFO empty indicator */
#define LOGUART_TP3F_EMPTY(x)               ((u32)(((x) & 0x00000001) << 18))
#define LOGUART_BIT_TP2F_EMPTY              ((u32)0x00000001 << 17)          /*!<R   Tx Path2 FIFO empty indicator */
#define LOGUART_TP2F_EMPTY(x)               ((u32)(((x) & 0x00000001) << 17))
#define LOGUART_BIT_TP1F_EMPTY              ((u32)0x00000001 << 16)          /*!<R   Tx Path1 FIFO empty indicator */
#define LOGUART_TP1F_EMPTY(x)               ((u32)(((x) & 0x00000001) << 16))
#define LOGUART_BIT_RP_RXFIFO_ERR           ((u32)0x00000001 << 15)          /*!<R   Relay Path rx_error,only valid when the FCR[12] set to 1 * 0: Clear when the CPU reads the LSR, if there are no subsequent errors in the relay path FIFO * 1: Parity error, framing error or break indication in the relay path FIFO */
#define LOGUART_RP_RXFIFO_ERR(x)            ((u32)(((x) & 0x00000001) << 15))
#define LOGUART_BIT_TIMEOUT_INT             ((u32)0x00000001 << 14)          /*!<R   This bit is only set when Rx Timeout interrupt is enabled(ETOI is set) 1: at least one character in the FIFO but no character has been input to the FIFO or reading from it for the time duration, which depends on the value in register REG_RX_PATH_CTRL[31:16] */
#define LOGUART_TIMEOUT_INT(x)              ((u32)(((x) & 0x00000001) << 14))
#define LOGUART_BIT_RXFIFO_INT              ((u32)0x00000001 << 13)          /*!<R   This bit is only set when Received Data Available Interrupt interrupt is enabled(ERBI is set) 1: FIFO Trigger level reached or Rx FIFO full */
#define LOGUART_RXFIFO_INT(x)               ((u32)(((x) & 0x00000001) << 13))
#define LOGUART_BIT_RP_FRM_ERR              ((u32)0x00000001 << 12)          /*!<R   Relay Path Framing Error (FE) indicator * 0: No framing error in the current character * 1: The received character does not have a valid stop bit */
#define LOGUART_RP_FRM_ERR(x)               ((u32)(((x) & 0x00000001) << 12))
#define LOGUART_BIT_RP_PAR_ERR              ((u32)0x00000001 << 11)          /*!<R   Relay Path Parity Error (PE) indicator * 0: No parity error in current character * 1: The received character does not have the correct parity bit */
#define LOGUART_RP_PAR_ERR(x)               ((u32)(((x) & 0x00000001) << 11))
#define LOGUART_BIT_RPF_OVR_ERR             ((u32)0x00000001 << 10)          /*!<R   Relay Path FIFO Overrun Error (OE) indicator * 0: No Overrun state * 1: The transmission is continue although the Relay FIFO is full */
#define LOGUART_RPF_OVR_ERR(x)              ((u32)(((x) & 0x00000001) << 10))
#define LOGUART_BIT_RPF_NOT_FULL            ((u32)0x00000001 << 9)          /*!<R 0x1  Relay Path FIFO Not FULL (NF) indicator * 0: Relay path FIFO is full * 1: Relay path FIFO is not full */
#define LOGUART_RPF_NOT_FULL(x)             ((u32)(((x) & 0x00000001) << 9))
#define LOGUART_BIT_RPF_DRDY                ((u32)0x00000001 << 8)          /*!<R   Relay Path FIFO Data Ready (DR) indicator * 0: No characters in the Relay Receiver FIFO * 1: At least one character has been received and transferred into the Relay path FIFO */
#define LOGUART_RPF_DRDY(x)                 ((u32)(((x) & 0x00000001) << 8))
#define LOGUART_BIT_RXFIFO_ERR              ((u32)0x00000001 << 7)          /*!<R   Uart_rx_error, only valid when the FCR bit [0] set to 1 * 0: Clear when the CPU reads the LSR, if there are no subsequent errors in the FIFO * 1: Parity error, overrun error,framing error or break indication in the FIFO */
#define LOGUART_RXFIFO_ERR(x)               ((u32)(((x) & 0x00000001) << 7))
#define LOGUART_BIT_TX_EMPTY                ((u32)0x00000001 << 6)          /*!<R   TX fifo empty indicator */
#define LOGUART_TX_EMPTY(x)                 ((u32)(((x) & 0x00000001) << 6))
#define LOGUART_BIT_MONITOR_DONE_INT        ((u32)0x00000001 << 5)          /*!<R 0  This bit is only set when Rx Path Monitor Done interrupt is enabled(EMDI is set) 1: Rx path monitor done */
#define LOGUART_MONITOR_DONE_INT(x)         ((u32)(((x) & 0x00000001) << 5))
#define LOGUART_BIT_BREAK_INT               ((u32)0x00000001 << 4)          /*!<R   Break Interrupt (BI) indicator * 0: No break condition in the current character * 1: The received data input held in the Spacing ( logic 0) state is longer than a full word transmission time */
#define LOGUART_BREAK_INT(x)                ((u32)(((x) & 0x00000001) << 4))
#define LOGUART_BIT_FRM_ERR                 ((u32)0x00000001 << 3)          /*!<R   Framing Error (FE) indicator * 0: No framing error in the current character * 1: The received character does not have a valid stop bit */
#define LOGUART_FRM_ERR(x)                  ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_PAR_ERR                 ((u32)0x00000001 << 2)          /*!<R   Parity Error (PE) indicator * 0: No parity error in current character * 1: The received character does not have the correct parity bit */
#define LOGUART_PAR_ERR(x)                  ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_OVR_ERR                 ((u32)0x00000001 << 1)          /*!<R   Overrun Error (OE) indicator * 0: No Overrun state * 1: The transmission is continue although the Rx FIFO is full */
#define LOGUART_OVR_ERR(x)                  ((u32)(((x) & 0x00000001) << 1))
#define LOGUART_BIT_DRDY                    ((u32)0x00000001 << 0)          /*!<R   Data Ready (DR) indicator * 0: No characters in the Receiver FIFO * 1: At least one character has been received and transferred into the FIFO */
#define LOGUART_DRDY(x)                     ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_MSR
 * @brief UART modem status register
 * @{
 **/
#define LOGUART_BIT_R_DCD                   ((u32)0x00000001 << 7)          /*!<R 0x0  Complement of the DCD input or equals to Out2 in loopback mode. */
#define LOGUART_R_DCD(x)                    ((u32)(((x) & 0x00000001) << 7))
#define LOGUART_BIT_R_RI                    ((u32)0x00000001 << 6)          /*!<R 0x0  Complement of the RI input or equals to Out1 in loopback mode. */
#define LOGUART_R_RI(x)                     ((u32)(((x) & 0x00000001) << 6))
#define LOGUART_BIT_R_DSR                   ((u32)0x00000001 << 5)          /*!<R 0x0  Complement of the DSR input or equals to DTR in loopback mode. */
#define LOGUART_R_DSR(x)                    ((u32)(((x) & 0x00000001) << 5))
#define LOGUART_BIT_R_CTS                   ((u32)0x00000001 << 4)          /*!<R 0x0  Complement of the CTS input or equals to RTS in loopback mode. */
#define LOGUART_R_CTS(x)                    ((u32)(((x) & 0x00000001) << 4))
#define LOGUART_BIT_D_DCD                   ((u32)0x00000001 << 3)          /*!<R   Delta Data Carrier Detect (DDCD) indicator * 0: Otherwise * 1: The DCD line has changed its state */
#define LOGUART_D_DCD(x)                    ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_TERI                    ((u32)0x00000001 << 2)          /*!<R   Trailing Edge of Ring Indicator (TERI) detector * 0: Otherwise * 1: The RI line has changed its state from low to high state */
#define LOGUART_TERI(x)                     ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_D_DSR                   ((u32)0x00000001 << 1)          /*!<R   Delta Data Set Ready (DDSR) indicator * 0: Otherwise * 1: The DSR line has changed its state */
#define LOGUART_D_DSR(x)                    ((u32)(((x) & 0x00000001) << 1))
#define LOGUART_BIT_D_CTS                   ((u32)0x00000001 << 0)          /*!<R   Delta Clear to Send (DCTS) indicator * 0: Otherwise * 1: The CTS line has changed its state */
#define LOGUART_D_CTS(x)                    ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_SCR
 * @brief UART scratch pad register
 * @{
 **/
#define LOGUART_MASK_XFACTOR_ADJ            ((u32)0x000007FF << 16)          /*!<R/W 0x0  One factor of Baud rate calculation for Tx path */
#define LOGUART_XFACTOR_ADJ(x)              ((u32)(((x) & 0x000007FF) << 16))
#define LOGUART_GET_XFACTOR_ADJ(x)          ((u32)(((x >> 16) & 0x000007FF)))
#define LOGUART_MASK_DBG_SEL                ((u32)0x0000000F << 8)          /*!<R/W 0x0  Debug port selection */
#define LOGUART_DBG_SEL(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define LOGUART_GET_DBG_SEL(x)              ((u32)(((x >> 8) & 0x0000000F)))
#define LOGUART_BIT_SCRATCH_7               ((u32)0x00000001 << 7)          /*!<R/W/ES 0x0  clear Rx break signal interrupt status */
#define LOGUART_SCRATCH_7(x)                ((u32)(((x) & 0x00000001) << 7))
#define LOGUART_MASK_SCRATCH_6_0            ((u32)0x0000007F << 0)          /*!<R/W 0x30  *Bit[6]: Rx break signal interrupt enable *Bit[5]: fl_set_bi_err *Bit[4]: fl_frame_err *Bit[3]: pin_lb_test *Bit[2:0]: reserved */
#define LOGUART_SCRATCH_6_0(x)              ((u32)(((x) & 0x0000007F) << 0))
#define LOGUART_GET_SCRATCH_6_0(x)          ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup LOGUART_UART_STSR
 * @brief UART STS register
 * @{
 **/
#define LOGUART_MASK_XFACTOR                ((u32)0x000FFFFF << 4)          /*!<R/W 0x1047  Factor of Baud rate calculation for Tx path */
#define LOGUART_XFACTOR(x)                  ((u32)(((x) & 0x000FFFFF) << 4))
#define LOGUART_GET_XFACTOR(x)              ((u32)(((x >> 4) & 0x000FFFFF)))
/** @} */

/** @defgroup LOGUART_UART_RBR
 * @brief UART receiver buffer register
 * @{
 **/
#define LOGUART_MASK_RXDATABIT              ((u32)0x000000FF << 0)          /*!<R 0x0  Tx data bit 0~7 Note: Bit[0] is the least significant bit. It is the first bit serially received/transmitted. */
#define LOGUART_RXDATABIT(x)                ((u32)(((x) & 0x000000FF) << 0))
#define LOGUART_GET_RXDATABIT(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup LOGUART_UART_MISCR
 * @brief UART MISC control register
 * @{
 **/
#define LOGUART_BIT_CLR_DUMMY_FLAG          ((u32)0x00000001 << 24)          /*!<W 0x0  Indicate the clearing of dummy data */
#define LOGUART_CLR_DUMMY_FLAG(x)           ((u32)(((x) & 0x00000001) << 24))
#define LOGUART_MASK_DUMMY_DATA             ((u32)0x000000FF << 16)          /*!<R/W 0x0  Store dummy data, when UART is DMA flow controller, dummy data is padded for last RXDMA burst request */
#define LOGUART_DUMMY_DATA(x)               ((u32)(((x) & 0x000000FF) << 16))
#define LOGUART_GET_DUMMY_DATA(x)           ((u32)(((x >> 16) & 0x000000FF)))
#define LOGUART_BIT_RXDMA_OWNER             ((u32)0x00000001 << 15)          /*!<R/W 0x0  Indicate the DMA flow controller for UART Rx DMA1 * 0: GDMA * 1: UART, interrupt enable signal other than erpd should be masked */
#define LOGUART_RXDMA_OWNER(x)              ((u32)(((x) & 0x00000001) << 15))
#define LOGUART_BIT_IRDA_RX_INV             ((u32)0x00000001 << 14)          /*!<R/W 0x0  irda_rx_i reverse input enable */
#define LOGUART_IRDA_RX_INV(x)              ((u32)(((x) & 0x00000001) << 14))
#define LOGUART_BIT_IRDA_TX_INV             ((u32)0x00000001 << 13)          /*!<R/W 0x0  irda_tx_o reverse output enable */
#define LOGUART_IRDA_TX_INV(x)              ((u32)(((x) & 0x00000001) << 13))
#define LOGUART_MASK_RXDMA_BURSTSIZE        ((u32)0x0000001F << 8)          /*!<R/W 0x4  RXDMA burstsize, maximum value is 16 */
#define LOGUART_RXDMA_BURSTSIZE(x)          ((u32)(((x) & 0x0000001F) << 8))
#define LOGUART_GET_RXDMA_BURSTSIZE(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define LOGUART_MASK_TXDMA_BURSTSIZE        ((u32)0x0000001F << 3)          /*!<R/W 0x4  TXDMA burstsize, maximum value is 16 */
#define LOGUART_TXDMA_BURSTSIZE(x)          ((u32)(((x) & 0x0000001F) << 3))
#define LOGUART_GET_TXDMA_BURSTSIZE(x)      ((u32)(((x >> 3) & 0x0000001F)))
#define LOGUART_BIT_RXDMA_EN                ((u32)0x00000001 << 2)          /*!<R/W 0x0  RXDMA enable, valid when dma_mode in FCR is 1'b1 */
#define LOGUART_RXDMA_EN(x)                 ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_TXDMA_EN                ((u32)0x00000001 << 1)          /*!<R/W 0x0  TXDMA enable, valid when dma_mode in FCR is 1'b1 */
#define LOGUART_TXDMA_EN(x)                 ((u32)(((x) & 0x00000001) << 1))
#define LOGUART_BIT_IRDA_ENABLE             ((u32)0x00000001 << 0)          /*!<R/W 0x0  IrDA SIR mode enable * 0: UART mode only * 1: UART is co-work with IRDA SIR mode */
#define LOGUART_IRDA_ENABLE(x)              ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_REG_SIR_TX_PWC0
 * @brief UART IRDA SIR Tx pulse width control 0 register
 * @{
 **/
#define LOGUART_BIT_UPPERBOUND_SHIFTRIGHT   ((u32)0x00000001 << 31)          /*!<R/W 0x0  Indicate the shift type of SIR Tx pulse's right edge * 0: Shift left, minus offset value of upperbound_shiftval * 1: Shift right, plus offset value of upperbound_shiftval */
#define LOGUART_UPPERBOUND_SHIFTRIGHT(x)    ((u32)(((x) & 0x00000001) << 31))
#define LOGUART_MASK_UPPERBOUND_SHIFTVAL    ((u32)0x00007FFF << 16)          /*!<R/W 0x0  The shift value of SIR Tx pulse's right edge */
#define LOGUART_UPPERBOUND_SHIFTVAL(x)      ((u32)(((x) & 0x00007FFF) << 16))
#define LOGUART_GET_UPPERBOUND_SHIFTVAL(x)  ((u32)(((x >> 16) & 0x00007FFF)))
#define LOGUART_BIT_LOWBOUND_SHIFTRIGHT     ((u32)0x00000001 << 15)          /*!<R/W 0x0  Indicate the shift type of SIR Tx pulse's left edge * 0: Shift left, minus offset value of lowbound_shiftval * 1: Shift right, plus offset value of lowbound_shiftval */
#define LOGUART_LOWBOUND_SHIFTRIGHT(x)      ((u32)(((x) & 0x00000001) << 15))
#define LOGUART_MASK_LOWBOUND_SHIFTVAL      ((u32)0x00007FFF << 0)          /*!<R/W 0x0  The shift value of SIR Tx pulse's left edge */
#define LOGUART_LOWBOUND_SHIFTVAL(x)        ((u32)(((x) & 0x00007FFF) << 0))
#define LOGUART_GET_LOWBOUND_SHIFTVAL(x)    ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/** @defgroup LOGUART_UART_REG_SIR_RX_PFC
 * @brief UART IRDA SIR Rx pulse filter control register
 * @{
 **/
#define LOGUART_MASK_R_SIR_RX_FILTER_THRS   ((u32)0x00007FFF << 1)          /*!<R/W 0x0  The threshold of SIR Rx filter. Rx pulse is valid only when Rx pulse width is larger than the threshold. */
#define LOGUART_R_SIR_RX_FILTER_THRS(x)     ((u32)(((x) & 0x00007FFF) << 1))
#define LOGUART_GET_R_SIR_RX_FILTER_THRS(x) ((u32)(((x >> 1) & 0x00007FFF)))
#define LOGUART_BIT_R_SIR_RX_FILTER_EN      ((u32)0x00000001 << 0)          /*!<R/W 0x0  IrDA SIR Rx filter enable */
#define LOGUART_R_SIR_RX_FILTER_EN(x)       ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_REG_BAUD_MON
 * @brief UART baud monitor register
 * @{
 **/
#define LOGUART_BIT_TOGGLE_MON_EN           ((u32)0x00000001 << 31)          /*!<R/W 0x0  Toggle monitor enable */
#define LOGUART_TOGGLE_MON_EN(x)            ((u32)(((x) & 0x00000001) << 31))
#define LOGUART_BIT_MON_DATA_VLD            ((u32)0x00000001 << 30)          /*!<R   Indicate valid monitoring data */
#define LOGUART_MON_DATA_VLD(x)             ((u32)(((x) & 0x00000001) << 30))
#define LOGUART_MASK_FALLING_THRESH         ((u32)0x0000003F << 24)          /*!<R/W 0x7  Indicate the threshold of Rx data rising edge */
#define LOGUART_FALLING_THRESH(x)           ((u32)(((x) & 0x0000003F) << 24))
#define LOGUART_GET_FALLING_THRESH(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define LOGUART_MASK_MIN_LOW_PERIOD         ((u32)0x00000FFF << 12)          /*!<R   Minimum space of consecutive logic “0” of Rx data */
#define LOGUART_MIN_LOW_PERIOD(x)           ((u32)(((x) & 0x00000FFF) << 12))
#define LOGUART_GET_MIN_LOW_PERIOD(x)       ((u32)(((x >> 12) & 0x00000FFF)))
#define LOGUART_MASK_MIN_FALL_SPACE         ((u32)0x00000FFF << 0)          /*!<R   Minimum space of adjacent falling edge of Rx data */
#define LOGUART_MIN_FALL_SPACE(x)           ((u32)(((x) & 0x00000FFF) << 0))
#define LOGUART_GET_MIN_FALL_SPACE(x)       ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup LOGUART_UART_REG_DBG
 * @brief UART debug register
 * @{
 **/
#define LOGUART_MASK_DBG_UART               ((u32)0xFFFFFFFF << 0)          /*!<R   The output value of debug port, depend on dbg_sel value in SCR[2:0] */
#define LOGUART_DBG_UART(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LOGUART_GET_DBG_UART(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup LOGUART_UART_REG_RX_PATH_CTRL
 * @brief UART Rx path control register
 * @{
 **/
#define LOGUART_MASK_R_RXTO_THRS            ((u32)0x0000FFFF << 16)          /*!<R/W 0x40  Indicate the timeout threshold of Rx path, unit in one-bit time */
#define LOGUART_R_RXTO_THRS(x)              ((u32)(((x) & 0x0000FFFF) << 16))
#define LOGUART_GET_R_RXTO_THRS(x)          ((u32)(((x >> 16) & 0x0000FFFF)))
#define LOGUART_MASK_RXBAUD_ADJ_10_0        ((u32)0x000007FF << 3)          /*!<R/W 0x0  One factor of Baud rate calculation for Rx path, similar with xfactor_adj */
#define LOGUART_RXBAUD_ADJ_10_0(x)          ((u32)(((x) & 0x000007FF) << 3))
#define LOGUART_GET_RXBAUD_ADJ_10_0(x)      ((u32)(((x >> 3) & 0x000007FF)))
#define LOGUART_BIT_R_RST_NEWRX_N           ((u32)0x00000001 << 2)          /*!<R/W 0x0  Reset new Rx path, low active */
#define LOGUART_R_RST_NEWRX_N(x)            ((u32)(((x) & 0x00000001) << 2))
/** @} */

/** @defgroup LOGUART_UART_REG_MON_BAUD_CTRL
 * @brief UART monitor baud rate control register
 * @{
 **/
#define LOGUART_BIT_R_UPD_OSC_IN_XTAL       ((u32)0x00000001 << 29)          /*!<R/W 0x0  Update parameter r_cycnum_perbit_osc(during the update of parameter r_cycnum_perbit_xtal) enable */
#define LOGUART_R_UPD_OSC_IN_XTAL(x)        ((u32)(((x) & 0x00000001) << 29))
#define LOGUART_MASK_R_CYCNUM_PERBIT_OSC    ((u32)0x000FFFFF << 9)          /*!<R/W/ES 0x0  Indicate the average OSC clock cycle number of one bit, for Rx path OSC clock Note: Sortware sets the initial value; hardware updates it depending on the monitor result. */
#define LOGUART_R_CYCNUM_PERBIT_OSC(x)      ((u32)(((x) & 0x000FFFFF) << 9))
#define LOGUART_GET_R_CYCNUM_PERBIT_OSC(x)  ((u32)(((x >> 9) & 0x000FFFFF)))
#define LOGUART_MASK_R_BIT_NUM_THRES        ((u32)0x000000FF << 1)          /*!<R/W 0x0  Indicate the bit number threshold of one monitor period, max value is 127. */
#define LOGUART_R_BIT_NUM_THRES(x)          ((u32)(((x) & 0x000000FF) << 1))
#define LOGUART_GET_R_BIT_NUM_THRES(x)      ((u32)(((x >> 1) & 0x000000FF)))
#define LOGUART_BIT_R_MON_BAUD_EN           ((u32)0x00000001 << 0)          /*!<R/W 0x0  Monitor Rx baud enable */
#define LOGUART_R_MON_BAUD_EN(x)            ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_REG_MON_BAUD_STS
 * @brief UART monitor baud rate status register
 * @{
 **/
#define LOGUART_MASK_RO_MON_TOTAL_BIT       ((u32)0x000000FF << 21)          /*!<R 0x0  Indicate the number of bits actually monitored */
#define LOGUART_RO_MON_TOTAL_BIT(x)         ((u32)(((x) & 0x000000FF) << 21))
#define LOGUART_GET_RO_MON_TOTAL_BIT(x)     ((u32)(((x >> 21) & 0x000000FF)))
#define LOGUART_BIT_RO_MON_RDY              ((u32)0x00000001 << 20)          /*!<R 0x0  Indicate the finish of calculation of actual cycle number per bit. It's cleared when r_mon_baud_en is 0. */
#define LOGUART_RO_MON_RDY(x)               ((u32)(((x) & 0x00000001) << 20))
#define LOGUART_MASK_R_CYCNUM_PERBIT_XTAL   ((u32)0x000FFFFF << 0)          /*!<R/W/ES 0x0  Indicate the fractional xtal clock cycle number of one bit, for Rx path xtal clk Note: Sortware sets the initial value; hardware updates it depending on the monitor result. */
#define LOGUART_R_CYCNUM_PERBIT_XTAL(x)     ((u32)(((x) & 0x000FFFFF) << 0))
#define LOGUART_GET_R_CYCNUM_PERBIT_XTAL(x) ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/** @defgroup LOGUART_UART_REG_MON_CYC_NUM
 * @brief UART monitored cycle number register
 * @{
 **/
#define LOGUART_MASK_RO_MON_TOTAL_CYCLE     ((u32)0x0FFFFFFF << 0)          /*!<R   Indicate the number of clock cycle actually monitored */
#define LOGUART_RO_MON_TOTAL_CYCLE(x)       ((u32)(((x) & 0x0FFFFFFF) << 0))
#define LOGUART_GET_RO_MON_TOTAL_CYCLE(x)   ((u32)(((x >> 0) & 0x0FFFFFFF)))
/** @} */

/** @defgroup LOGUART_UART_REG_Rx_BYTE_CNT
 * @brief UART Rx data byte count register
 * @{
 **/
#define LOGUART_BIT_CLR_RX_BYTE_CNT         ((u32)0x00000001 << 16)          /*!<W 0x0  Write 1 to clear ro_rx_byte_cnt */
#define LOGUART_CLR_RX_BYTE_CNT(x)          ((u32)(((x) & 0x00000001) << 16))
#define LOGUART_MASK_RO_RX_BYTE_CNT         ((u32)0x0000FFFF << 0)          /*!<R   Count the byte number of data read from rxfifo */
#define LOGUART_RO_RX_BYTE_CNT(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define LOGUART_GET_RO_RX_BYTE_CNT(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup LOGUART_UART_FCR
 * @brief UART FIFO control register
 * @{
 **/
#define LOGUART_BIT_RP_RECV_CLR             ((u32)0x00000001 << 13)          /*!<W 0x0  Clear Relay Path receiving */
#define LOGUART_RP_RECV_CLR(x)              ((u32)(((x) & 0x00000001) << 13))
#define LOGUART_BIT_RP_FIFO_EN              ((u32)0x00000001 << 12)          /*!<R/W 0x1  Error report of Relay Path RCVR FIFO field enable, */
#define LOGUART_RP_FIFO_EN(x)               ((u32)(((x) & 0x00000001) << 12))
#define LOGUART_MASK_RECVTRG                ((u32)0x00000003 << 6)          /*!<R/W 0x3  Define the 16-entry Receiver FIFO Interrupt trigger level (0~15 bytes) * 00: 1 byte * 01: FIFO_DEPTH/4 * 10: FIFO_DEPTH/2 * 11: FIFO_DEPTH-2 */
#define LOGUART_RECVTRG(x)                  ((u32)(((x) & 0x00000003) << 6))
#define LOGUART_GET_RECVTRG(x)              ((u32)(((x >> 6) & 0x00000003)))
#define LOGUART_BIT_DMA_MODE                ((u32)0x00000001 << 3)          /*!<R/W 0x0  Support DMA mode (cooperating with DW DDMA in the data path) */
#define LOGUART_DMA_MODE(x)                 ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_RECV_CLR                ((u32)0x00000001 << 1)          /*!<W 0x0  Clear receiving */
#define LOGUART_RECV_CLR(x)                 ((u32)(((x) & 0x00000001) << 1))
#define LOGUART_BIT_FIFO_EN                 ((u32)0x00000001 << 0)          /*!<R/W 0x1  Error report of RCVR FIFO field enable */
#define LOGUART_FIFO_EN(x)                  ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_AGGC
 * @brief UART AGG control register
 * @{
 **/
#define LOGUART_BIT_RP_EN                   ((u32)0x00000001 << 31)          /*!<R/W 0x0  Relay Rx Path enable *0: Disable *1: Enable */
#define LOGUART_RP_EN(x)                    ((u32)(((x) & 0x00000001) << 31))
#define LOGUART_BIT_TP4_EN                  ((u32)0x00000001 << 30)          /*!<R/W 0x0  Tx Path4 enable *0: Disable *1: Enable */
#define LOGUART_TP4_EN(x)                   ((u32)(((x) & 0x00000001) << 30))
#define LOGUART_BIT_TP3_EN                  ((u32)0x00000001 << 29)          /*!<R/W 0x0  Tx Path3 enable *0: Disable *1: Enable */
#define LOGUART_TP3_EN(x)                   ((u32)(((x) & 0x00000001) << 29))
#define LOGUART_BIT_TP2_EN                  ((u32)0x00000001 << 28)          /*!<R/W 0x0  Tx Path2 enable *0: Disable *1: Enable */
#define LOGUART_TP2_EN(x)                   ((u32)(((x) & 0x00000001) << 28))
#define LOGUART_BIT_TP1_EN                  ((u32)0x00000001 << 27)          /*!<R/W 0x0  Tx Path1 enable *0: Disable *1: Enable */
#define LOGUART_TP1_EN(x)                   ((u32)(((x) & 0x00000001) << 27))
#define LOGUART_BIT_AGG_EN                  ((u32)0x00000001 << 26)          /*!<R/W 0x0  Tx AGG function enable */
#define LOGUART_AGG_EN(x)                   ((u32)(((x) & 0x00000001) << 26))
#define LOGUART_MASK_AGG_TO                 ((u32)0x00003FFF << 12)          /*!<R/W 0xBB8  AGG timeout Unit is one baudrate period of loguart Tx, default is 1ms if Tx baudrate is 3Mbps. Data should be sent from log FIFO to tx_fifo if idle time > AGG timeout, even if there is just only one byte in log FIFO of the path. */
#define LOGUART_AGG_TO(x)                   ((u32)(((x) & 0x00003FFF) << 12))
#define LOGUART_GET_AGG_TO(x)               ((u32)(((x >> 12) & 0x00003FFF)))
#define LOGUART_MASK_AGG_TH                 ((u32)0x0000000F << 8)          /*!<R/W 0x7  AGG threshold 4'0000: 1 4'0111: 8 Numbers above 4'0111 are reserved. */
#define LOGUART_AGG_TH(x)                   ((u32)(((x) & 0x0000000F) << 8))
#define LOGUART_GET_AGG_TH(x)               ((u32)(((x >> 8) & 0x0000000F)))
#define LOGUART_MASK_AGG_SYNC_PAT           ((u32)0x000000FF << 0)          /*!<R/W 0xFF  AGG packet Start */
#define LOGUART_AGG_SYNC_PAT(x)             ((u32)(((x) & 0x000000FF) << 0))
#define LOGUART_GET_AGG_SYNC_PAT(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup LOGUART_UART_THRx
 * @brief UART transmitter holding register x
 * @{
 **/
#define LOGUART_MASK_TXxDATABIT             ((u32)0x000000FF << 0)          /*!<W 0x0  Tx data bit 0~7 Note: Bit 0 is the least significant bit. It is the first bit serially received/transmitted */
#define LOGUART_TXxDATABIT(x)               ((u32)(((x) & 0x000000FF) << 0))
#define LOGUART_GET_TXxDATABIT(x)           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup LOGUART_UART_RP_LCR
 * @brief UART Relay Rx Path line control register
 * @{
 **/
#define LOGUART_MASK_RP_RXBAUD_19_0         ((u32)0x000FFFFF << 12)          /*!<R/W 0x0  Factor of Baud rate calculation for Relay path */
#define LOGUART_RP_RXBAUD_19_0(x)           ((u32)(((x) & 0x000FFFFF) << 12))
#define LOGUART_GET_RP_RXBAUD_19_0(x)       ((u32)(((x >> 12) & 0x000FFFFF)))
#define LOGUART_BIT_RP_DLAB                 ((u32)0x00000001 << 7)          /*!<R/W 0x0  Divisor Latch Access bit, and control the accessibility of the DLL, DLM and IER registers * 0: The divisor latches not accessible; IER accessible * 1: The divisor latches accessible; DLL and DLM accessible Note: THR/RBR don not care the value of dlab bit. */
#define LOGUART_RP_DLAB(x)                  ((u32)(((x) & 0x00000001) << 7))
#define LOGUART_BIT_RP_STKP                 ((u32)0x00000001 << 5)          /*!<R/W 0x0  Stick Parity bit * 0: Not stick parity, the parity type is ODD or EVEN (depend on eps) * 1: Stick parity, eps=1: low parity (1'b0); eps=0: high parity (1'b1) */
#define LOGUART_RP_STKP(x)                  ((u32)(((x) & 0x00000001) << 5))
#define LOGUART_BIT_RP_EPS                  ((u32)0x00000001 << 4)          /*!<R/W 0x0  Even Parity select * 0: Odd parity, the number of Logic "1" is odd. * 1: Even parity, the number of Logic "1" is even. */
#define LOGUART_RP_EPS(x)                   ((u32)(((x) & 0x00000001) << 4))
#define LOGUART_BIT_RP_PEN                  ((u32)0x00000001 << 3)          /*!<R/W 0x0  Parity Enable * 0: No parity bit * 1: Parity bit is generated on each outgoing character and is checked on each incoming one. */
#define LOGUART_RP_PEN(x)                   ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_RP_STB                  ((u32)0x00000001 << 2)          /*!<R/W 0x0  Stop bits length selection * 0: 1 stop bit * 1: 2 stop bits Note: the receiver always checks the first stop bit only. */
#define LOGUART_RP_STB(x)                   ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_RP_WLS0                 ((u32)0x00000001 << 0)          /*!<R/W 0x1  Word length selection * 0: 7 bits data * 1: 8 bits data */
#define LOGUART_RP_WLS0(x)                  ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup LOGUART_UART_RP_RX_PATH_CTRL
 * @brief UART Relay Rx path control register
 * @{
 **/
#define LOGUART_MASK_RP_RXBAUD_ADJ_10_0     ((u32)0x000007FF << 3)          /*!<R/W 0x0  One factor of Baud rate calculation for Relay path, similar with xfactor_adj */
#define LOGUART_RP_RXBAUD_ADJ_10_0(x)       ((u32)(((x) & 0x000007FF) << 3))
#define LOGUART_GET_RP_RXBAUD_ADJ_10_0(x)   ((u32)(((x >> 3) & 0x000007FF)))
#define LOGUART_BIT_R_RST_RP_RX_N           ((u32)0x00000001 << 2)          /*!<R/W 0x0  Reset Relay Rx path, low active */
#define LOGUART_R_RST_RP_RX_N(x)            ((u32)(((x) & 0x00000001) << 2))
/** @} */

/** @defgroup LOGUART_UART_ICR
 * @brief UART interrupt clear register
 * @{
 **/
#define LOGUART_BIT_RPRLSICF                ((u32)0x00000001 << 3)          /*!<W 0x0  Relay Path Receiver Line Statue Interrupt Clear Flag Writing 1 to this bit clears relay path's PARITY_ERR, OVERRUN_ERR, FRAMING_ERR and RXFIFO_ERR in the USART_LSR register. */
#define LOGUART_RPRLSICF(x)                 ((u32)(((x) & 0x00000001) << 3))
#define LOGUART_BIT_MDICF                   ((u32)0x00000001 << 2)          /*!<W 0x0  Monitor done clear flag */
#define LOGUART_MDICF(x)                    ((u32)(((x) & 0x00000001) << 2))
#define LOGUART_BIT_TOICF                   ((u32)0x00000001 << 1)          /*!<W 0x0  Timeout Interrupt Clear Flag */
#define LOGUART_TOICF(x)                    ((u32)(((x) & 0x00000001) << 1))
#define LOGUART_BIT_RLSICF                  ((u32)0x00000001 << 0)          /*!<W 0x0  Receiver Line Statue Interrupt Clear Flag Writing 1 to this bit clears PARITY_ERR, OVERRUN_ERR, FRAMING_ERR, BREAK_ERR_INT and RXFIFO_ERR in the USART_LSR register. */
#define LOGUART_RLSICF(x)                   ((u32)(((x) & 0x00000001) << 0))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup LOGUART_Exported_Types LOGUART Exported Types
  * @{
*****************************************************************************/


/**
 * @brief LOGUART Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t LOGUART_UART_DLL;                       /*!< UART DIVISOR LENGTH REGISTER,  Address offset: 0x000 */
	__IO uint32_t LOGUART_UART_IER;                       /*!< UART INTERRUPT ENABLE REGISTER,  Address offset: 0x004 */
	__I  uint32_t LOGUART_UART_IIR;                       /*!< UART INTERRUPT IDENTIFICATION REGISTER,  Address offset: 0x008 */
	__IO uint32_t LOGUART_UART_LCR;                       /*!< UART LINE CONTROL REGISTER,  Address offset: 0x00C */
	__IO uint32_t LOGUART_UART_MCR;                       /*!< UART MODEM CONTROL REGISTER,  Address offset: 0x010 */
	__I  uint32_t LOGUART_UART_LSR;                       /*!< UART LINE STATUS REGISTER,  Address offset: 0x014 */
	__I  uint32_t LOGUART_UART_MSR;                       /*!< UART MODEM STATUS REGISTER,  Address offset: 0x018 */
	__IO uint32_t LOGUART_UART_SCR;                       /*!< UART SCRATCH PAD REGISTER,  Address offset: 0x01C */
	__IO uint32_t LOGUART_UART_STSR;                      /*!< UART STS REGISTER,  Address offset: 0x020 */
	__I  uint32_t LOGUART_UART_RBR;                       /*!< UART RECEIVER BUFFER REGISTER,  Address offset: 0x024 */
	__IO uint32_t LOGUART_UART_MISCR;                     /*!< UART MISC CONTROL REGISTER,  Address offset: 0x028 */
	__IO uint32_t LOGUART_UART_REG_SIR_TX_PWC0;           /*!< UART IRDA SIR TX PULSE WIDTH CONTROL 0 REGISTER,  Address offset: 0x02C */
	__IO uint32_t LOGUART_UART_REG_SIR_RX_PFC;            /*!< UART IRDA SIR RX PULSE FILTER CONTROL REGISTER,  Address offset: 0x030 */
	__IO uint32_t LOGUART_UART_REG_BAUD_MON;              /*!< UART BAUD MONITOR REGISTER,  Address offset: 0x034 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x38 */
	__I  uint32_t LOGUART_UART_REG_DBG;                   /*!< UART DEBUG REGISTER,  Address offset: 0x03C */
	__IO uint32_t LOGUART_UART_REG_RX_PATH_CTRL;          /*!< UART RX PATH CONTROL REGISTER,  Address offset: 0x040 */
	__IO uint32_t LOGUART_UART_REG_MON_BAUD_CTRL;         /*!< UART MONITOR BAUD RATE CONTROL REGISTER,  Address offset: 0x044 */
	__IO uint32_t LOGUART_UART_REG_MON_BAUD_STS;          /*!< UART MONITOR BAUD RATE STATUS REGISTER,  Address offset: 0x048 */
	__I  uint32_t LOGUART_UART_REG_MON_CYC_NUM;           /*!< UART MONITORED CYCLE NUMBER REGISTER,  Address offset: 0x04C */
	__IO uint32_t LOGUART_UART_REG_RX_BYTE_CNT;           /*!< UART RX DATA BYTE COUNT REGISTER,  Address offset: 0x050 */
	__IO uint32_t LOGUART_UART_FCR;                       /*!< UART FIFO CONTROL REGISTER,  Address offset: 0x054 */
	__IO uint32_t LOGUART_UART_AGGC;                      /*!< UART AGG CONTROL REGISTER,  Address offset: 0x058 */
	__O  uint32_t LOGUART_UART_THRx[4];                   /*!< UART TRANSMITTER HOLDING REGISTER x Register,  Address offset: 0x05C-0x068 */
	__IO uint32_t LOGUART_UART_RP_LCR;                    /*!< UART RELAY RX PATH LINE CONTROL REGISTER,  Address offset: 0x06C */
	__IO uint32_t LOGUART_UART_RP_RX_PATH_CTRL;           /*!< UART RELAY RX PATH CONTROL REGISTER,  Address offset: 0x070 */
	__O  uint32_t LOGUART_UART_ICR;                       /*!< UART INTERRUPT CLEAR REGISTER,  Address offset: 0x074 */
} LOGUART_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported Types --------------------------------------------------------*/
/** @addtogroup LOGUART_Exported_Types LOGUART Exported Types
  * @{
  */

/**
  * @brief  LOGUART Init Structure Definition
  */
typedef struct {
	u32	DmaModeCtrl;	/*!< Specifies the LOGUART DMA mode state.
										This parameter can be ENABLE or DISABLE. */

	u32	WordLen;		/*!< Specifies the LOGUART word length.
										This parameter can be a value of @ref LOGUART_Word_Length. */

	u32	StopBit;		/*!< Specifies the LOGUART stop bit number.
										This parameter can be a value of @ref LOGUART_Stop_Bit. */

	u32	Parity;			/*!< Specifies the LOGUART parity.
										This parameter can be a value of @ref LOGUART_Parity_Enable. */

	u32	ParityType;		/*!< Specifies the LOGUART parity type.
										This parameter can be a value of @ref LOGUART_Parity_Type. */

	u32	StickParity;	/*!< Specifies the LOGUART stick parity.
										This parameter can be a value of @ref LOGUART_Stick_Parity_Type. */

	u32	FlowControl;	/*!< Specifies the LOGUART auto flow control.
										This parameter can be ENABLE or DISABLE. */

	u32	RxFifoTrigLevel;	/*!< Specifies the LOGUART rx fifo trigger level.
										This parameter can be a value of @ref LOGUART_Rx_FIFO_Trigger_Level. */

	u32	RxErReportCtrl;		/*!< Specifies the LOGUART rx error report control.
										This parameter can be a value of @ref LOGUART_Rx_Err_Report. */

	u32 RxTimeOutCnt;	/*!< Specifies the LOGUART rx time out counter.
										This parameter can be a number between 0x00 and 0xffff. */
	/* AGG function*/
	u32 AGGEn; 		 /*!< Specifies the Tx AGG control.
										This parameter can be ENABLE or DISABLE. */

	u32 P1En;		/*!< Specifies the Tx Path1 control.
										This parameter can be ENABLE or DISABLE. */

	u32 P2En;		/*!< Specifies the Tx Path2 control.
										This parameter can be ENABLE or DISABLE. */

	u32 P3En;		/*!< Specifies the Tx Path3 control.
										This parameter can be ENABLE or DISABLE. */

	u32 P4En;		/*!< Specifies the Tx Path4 control.
										This parameter can be ENABLE or DISABLE. */

	u32 RPEn;		/*!< Specifies the Tx Relay Path control.
										This parameter can be ENABLE or DISABLE. */

	u32 AGGTH;		/*!< Specifies the AGG threshold.
										This parameter can be a value of @LOGUART_AGG_Threshold_Level. */

	u32 AGGTO;		/*!< Specifies the AGG timeout.
										This parameter can be a number between 0x00 and 0x3fff. */

	u32 AGG_SYNC_PAT;		/*!< Specifies the AGG packet start.
										This parameter can be a number between 0x00 and 0xff. */
} LOGUART_InitTypeDef;

/**
  * @brief  LOGUART Relay Path Init Structure Definition
  */
typedef struct {
	u32	WordLen;		/*!< Specifies the LOGUART Relay Path word length.
										This parameter can be a value of @ref LOGUART_Word_Length. */

	u32	StopBit;		/*!< Specifies the LOGUART Relay Path stop bit number.
										This parameter can be a value of @ref LOGUART_Stop_Bit. */

	u32	Parity;			/*!< Specifies the LOGUART Relay Path parity.
										This parameter can be a value of @ref LOGUART_Parity_Enable. */

	u32	ParityType;		/*!< Specifies the LOGUART Relay Path parity type.
										This parameter can be a value of @ref LOGUART_Parity_Type. */

	u32	StickParity;	/*!< Specifies the LOGUART Relay Path stick parity.
										This parameter can be a value of @ref LOGUART_Stick_Parity_Type. */

	u32	RxErReportCtrl;		/*!< Specifies the LOGUART Relay Path rx error report control.
										This parameter can be a value of @ref LOGUART_Rx_Err_Report. */
} LOGUART_Relay_InitTypeDef;

/**
  * @brief  LOGUART Relay Port Structure Definition
  */
typedef struct {
	u32 idx;
	u32 not_full;
	u32 empty;
	u32 tx_timeout; /* caculate under 4800bps, test ok under 9600bps */
	IRQn_Type irq_num;
} LOG_UART_PORT;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup LOGUART_Exported_Constants LOGUART Exported Constants
  * @{
  */

/** @defgroup LOGUART_Rx_FIFO_Trigger_Level
  * @{
  */
#define LOGUART_RX_FIFOTRIG_LEVEL_1BYTES        ((u32)0x00000000)
#define LOGUART_RX_FIFOTRIG_LEVEL_4BYTES        ((u32)0x00000040)
#define LOGUART_RX_FIFOTRIG_LEVEL_8BYTES        ((u32)0x00000080)
#define LOGUART_RX_FIFOTRIG_LEVEL_14BYTES      ((u32)0x000000c0)

#define IS_LOGUART_RXFIFO_LEVEL(LEVEL) (((LEVEL) == LOGUART_RX_FIFOTRIG_LEVEL_1BYTES) || \
								((LEVEL) == LOGUART_RX_FIFOTRIG_LEVEL_4BYTES) || \
								((LEVEL) == LOGUART_RX_FIFOTRIG_LEVEL_8BYTES) || \
								((LEVEL) == LOGUART_RX_FIFOTRIG_LEVEL_14BYTES))
/**
  * @}
  */

/** @defgroup LOGUART_Clock_Source
  * @{
  */
#define LOGUART_CLK_XTAL_40M	((u32)0x00000000)
#define LOGUART_CLK_OSC_LP		((u32)0x00000001)
#define IS_LOGUART_RX_CLK(CLK) (((CLK) == LOGUART_CLK_XTAL_40M) || \
							((CLK) == LOGUART_CLK_OSC_LP))
/**
  * @}
  */

/** @defgroup LOGUART_AGG_Threshold_Level
  * @{
  */
#define LOGUART_AGG_THRESHOLD_LEVEL_1BYTES      ((u32)0x00000000)
#define LOGUART_AGG_THRESHOLD_LEVEL_2BYTES      ((u32)0x00000001)
#define LOGUART_AGG_THRESHOLD_LEVEL_3BYTES      ((u32)0x00000002)
#define LOGUART_AGG_THRESHOLD_LEVEL_4BYTES      ((u32)0x00000003)
#define LOGUART_AGG_THRESHOLD_LEVEL_5BYTES      ((u32)0x00000004)
#define LOGUART_AGG_THRESHOLD_LEVEL_6BYTES      ((u32)0x00000005)
#define LOGUART_AGG_THRESHOLD_LEVEL_7BYTES      ((u32)0x00000006)
#define LOGUART_AGG_THRESHOLD_LEVEL_8BYTES      ((u32)0x00000007)

#define IS_LOGUART_AGG_THRESHOLD_LEVEL(LEVEL) (((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_1BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_2BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_3BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_4BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_5BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_6BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_7BYTES) || \
								((LEVEL) == LOGUART_AGG_THRESHOLD_LEVEL_8BYTES))
/**
  * @}
  */

/** @defgroup LOGUART_Path_Index
  * @{
  */
#define LOGUART_PATH_INDEX_1		((u8)0x01)
#define LOGUART_PATH_INDEX_2		((u8)0x02)
#define LOGUART_PATH_INDEX_3		((u8)0x03)
#define LOGUART_PATH_INDEX_4		((u8)0x04)
#define LOGUART_PATH_INDEX_5		((u8)0x05)

#define IS_LOGUART_PATH_INDEX(PATHINDEX) (((PATHINDEX) == LOGUART_PATH_INDEX_1) || \
								((PATHINDEX) == LOGUART_PATH_INDEX_2) || \
								((PATHINDEX) == LOGUART_PATH_INDEX_3) || \
								((PATHINDEX) == LOGUART_PATH_INDEX_4) || \
								((PATHINDEX) == LOGUART_PATH_INDEX_5))
/**
  * @}
  */

/** @defgroup LOGUART_Word_Length
  * @{
  */
#define LOGUART_WLS_7BITS        ((u32)0x00000000)
#define LOGUART_WLS_8BITS        ((u32)0x00000001)

#define IS_LOGUART_WLS(VAL) (((VAL) == LOGUART_WLS_7BITS) || \
								((VAL) == LOGUART_WLS_8BITS))
/**
  * @}
  */

/** @defgroup LOGUART_Stop_Bit
  * @{
  */
#define LOGUART_STOP_BIT_1        ((u32)0x00000000)
#define LOGUART_STOP_BIT_2        ((u32)0x00000001)

#define IS_LOGUART_STOP_BIT(VAL) (((VAL) == LOGUART_STOP_BIT_1) || \
								((VAL) == LOGUART_STOP_BIT_2))
/**
  * @}
  */

/** @defgroup LOGUART_Parity_Enable
  * @{
  */
#define LOGUART_PARITY_DISABLE		((u32)0x00000000)
#define LOGUART_PARITY_ENABLE		((u32)0x00000001)

#define IS_LOGUART_PARITY_ENABLE(VAL) (((VAL) == LOGUART_PARITY_DISABLE) || \
								((VAL) == LOGUART_PARITY_ENABLE))
/**
  * @}
  */

/** @defgroup LOGUART_Parity_Type
  * @{
  */
#define LOGUART_ODD_PARITY		((u32)0x00000000)
#define LOGUART_EVEN_PARITY		((u32)0x00000001)

#define IS_LOGUART_PARITY_TYPE(VAL) (((VAL) == LOGUART_ODD_PARITY) || \
								((VAL) == LOGUART_EVEN_PARITY))
/**
  * @}
  */

/** @defgroup LOGUART_Stick_Parity_Type
  * @{
  */
#define LOGUART_STICK_PARITY_DISABLE		((u32)0x00000000)
#define LOGUART_STICK_PARITY_ENABLE			((u32)0x00000001)

#define IS_LOGUART_STICK_PARITY_ENABLE(VAL) (((VAL) == LOGUART_STICK_PARITY_DISABLE) || \
								((VAL) == LOGUART_STICK_PARITY_ENABLE))
/**
  * @}
  */

/** @defgroup LOGUART_Rx_Err_Report
  * @{
  */
#define LOGUART_RX_EEROR_REPORT_DISABLE		((u32)0x00000000)
#define LOGUART_RX_EEROR_REPORT_ENABLE			((u32)0x00000001)

#define IS_LOGUART_RX_ERROR_REPORT(REPORT)  (((REPORT) == LOGUART_RX_EEROR_REPORT_DISABLE) || \
							((REPORT) == LOGUART_RX_EEROR_REPORT_ENABLE) )
/**
  * @}
  */

/** @defgroup LOGUART_Tx_Empty_Intr_Enable
  * @{
  */
#define LOGUART_TX_EMPTY_PATH_1_INTR		LOGUART_ETPFEI(0x1)
#define LOGUART_TX_EMPTY_PATH_2_INTR		LOGUART_ETPFEI(0x2)
#define LOGUART_TX_EMPTY_PATH_3_INTR		LOGUART_ETPFEI(0x3)
#define LOGUART_TX_EMPTY_PATH_4_INTR		LOGUART_ETPFEI(0x4)

#define IS_LOGUART_TX_EMPTY_ENABLE(PATHINDEX) (((PATHINDEX) == LOGUART_TX_EMPTY_PATH_1_INTR) || \
								((PATHINDEX) == LOGUART_TX_EMPTY_PATH_2_INTR) || \
								((PATHINDEX) == LOGUART_TX_EMPTY_PATH_3_INTR) || \
								((PATHINDEX) == LOGUART_TX_EMPTY_PATH_4_INTR))
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup LOGUART_Exported_Functions LOGUART Exported Functions
  * @{
  */

_LONG_CALL_ void LOGUART_DiagInit(BOOL InitConsol);

_LONG_CALL_ void LOGUART_AGGCmd(LOGUART_TypeDef *UARTLOG, u32 NewState);
_LONG_CALL_ void LOGUART_AGGPathCmd(LOGUART_TypeDef *UARTLOG, u8 PathIndex, u32 NewState);
_LONG_CALL_ void LOGUART_AGGSetTimeOut(LOGUART_TypeDef *UARTLOG, u32 TimeOut);
_LONG_CALL_ void LOGUART_AGGSetThreshold(LOGUART_TypeDef *UARTLOG, u32 Threshold);
_LONG_CALL_ void LOGUART_AGGSetSyncPattern(LOGUART_TypeDef *UARTLOG, u32 SyncPattern);
_LONG_CALL_ void LOGUART_AGGPathAllCmd(LOGUART_TypeDef *UARTLOG, u32 NewState);
_LONG_CALL_ void LOGUART_PutChar(u8 c);
_LONG_CALL_ u8 LOGUART_GetChar(BOOL PullMode);
_LONG_CALL_ u8 LOGUART_Readable(void);
_LONG_CALL_ u32 LOGUART_GetRxCount(void);
_LONG_CALL_ u32 LOGUART_GetIMR(void);
_LONG_CALL_ void LOGUART_SetIMR(u32 SetValue);

_LONG_CALL_ void LOGUART_INTConfig(LOGUART_TypeDef *UARTLOG, u32 UART_IT, u32 newState);
_LONG_CALL_ void LOGUART_INTClear(LOGUART_TypeDef *UARTLOG, u32 UART_IT);
_LONG_CALL_ u32 LOGUART_GetStatus(LOGUART_TypeDef *UARTLOG);
_LONG_CALL_ u32 LOGUART_ClearRxFifo(LOGUART_TypeDef *UARTLOG);
_LONG_CALL_ void LOGUART_WaitBusy(void);
_LONG_CALL_ void LOGUART_WaitTxComplete(void);

_LONG_CALL_ void LOGUART_StructInit(LOGUART_InitTypeDef *LOGUART_InitStruct);
_LONG_CALL_ void _LOGUART_Init(LOGUART_TypeDef *UARTLOG, LOGUART_InitTypeDef *LOGUART_InitStruct);
_LONG_CALL_ void LOGUART_SetBaud(LOGUART_TypeDef *UARTLOG, u32 BaudRate);
_LONG_CALL_ void LOGUART_RxCmd(LOGUART_TypeDef *UARTLOG, u32 NewState);

_LONG_CALL_ void LOGUART_BT_SendData(u8 *InBuf, u32 Count);

_LONG_CALL_ void LOGUART_Relay_StructInit(LOGUART_Relay_InitTypeDef *LOGUART_Relay_InitStruct);
_LONG_CALL_ void LOGUART_Relay_SetFormat(LOGUART_TypeDef *UARTLOG, LOGUART_Relay_InitTypeDef *LOGUART_Relay_InitStruct);
_LONG_CALL_ void LOGUART_Relay_SetBaud(LOGUART_TypeDef *UARTLOG, u32 RPBaudRate);
_LONG_CALL_ u32 LOGUART_Relay_ClearRxFifo(LOGUART_TypeDef *UARTLOG);
_LONG_CALL_ void LOGUART_Relay_RxCmd(LOGUART_TypeDef *UARTLOG, u32 NewState);

_LONG_CALL_ void LOGUART_MonitorParaConfig(LOGUART_TypeDef *UARTLOG, u32 BitNumThres, u32 OscPerbitUpdCtrl);
_LONG_CALL_ void LOGUART_LPBaudSet(LOGUART_TypeDef *UARTLOG, u32 BaudRate, u32 IPClockHz);
_LONG_CALL_ void LOGUART_Relay_LPSetBaud(LOGUART_TypeDef *UARTLOG, u32 RPBaudRate, u32 IPClockHz);
_LONG_CALL_ void LOGUART_RxMonitorCmd(LOGUART_TypeDef *UARTLOG, u32 NewState);
_LONG_CALL_ u32 LOGUART_RxMonBaudCtrlRegGet(LOGUART_TypeDef *UARTLOG);
_LONG_CALL_ u32 LOGUART_RxMonitorSatusGet(LOGUART_TypeDef *UARTLOG);

/**
  * @}
  */

/** @} */

/** @} */

#define DiagPutChar		LOGUART_PutChar
#define DiagGetChar		LOGUART_GetChar

/* MANUAL_GEN_END */

#endif

