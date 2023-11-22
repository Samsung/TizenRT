#ifndef _AMEBAD2_UART_H_
#define _AMEBAD2_UART_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup UART
  * @brief UART driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup UART_Register_Definitions UART Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup DLL
 * @brief UART divisor length register
 * @{
 **/
#define RUART_MASK_BAUD                   ((u32)0x000000FF << 0)          /*!<R/W 0x0  The low 8 bits of divisor coefficient (not used) */
#define RUART_BAUD(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define RUART_GET_BAUD(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IER
 * @brief UART interrupt enable register
 * @{
 **/
#define RUART_BIT_ERXNDI                  ((u32)0x00000001 << 6)          /*!<R/W 0x0  Rx noise detected interrupt enable */
#define RUART_BIT_ETOI                    ((u32)0x00000001 << 5)          /*!<R/W 0x0  Rx timeout interrupt enable */
#define RUART_BIT_EMDI                    ((u32)0x00000001 << 4)          /*!<R/W 0x0  Rx path monitor done interrupt enable */
#define RUART_BIT_EDSSI                   ((u32)0x00000001 << 3)          /*!<R/W 0x0  Modem Status Interrupt (EDSSI) (modem status transition) enable */
#define RUART_BIT_ELSI                    ((u32)0x00000001 << 2)          /*!<R/W 0x0  Receiver Line Status Interrupt (ELSI) (receiver line status) enable */
#define RUART_BIT_ETBEI                   ((u32)0x00000001 << 1)          /*!<R/W 0x0  Transmitter FIFO Empty interrupt (ETBEI) (tx fifo empty) enable */
#define RUART_BIT_ERBI                    ((u32)0x00000001 << 0)          /*!<R/W 0x0  Received Data Available Interrupt (ERBFI) (rx trigger) enable */
/** @} */

/** @defgroup IIR
 * @brief UART interrupt identification register
 * @{
 **/
#define RUART_BIT_INT_PEND                ((u32)0x00000001 << 0)          /*!<R 0x1  * 0: pending, and the IIR contents may be used as a pointer to the appropriate interrupt service routine * 1: no interrupt is pending */
/** @} */

/** @defgroup LCR
 * @brief UART line control register
 * @{
 **/
#define RUART_BIT_DLAB                    ((u32)0x00000001 << 7)          /*!<R/W 0x0  Divisor Latch Access bit, and control the accessibility of the DLL, DLM and IER registers * 0: The divisor latches not accessible; IER accessible * 1: The divisor latches accessible; DLL and DLM accessible Note: THR/RBR don't care the value of dlab bit. */
#define RUART_BIT_BRCTL                   ((u32)0x00000001 << 6)          /*!<R/W 0x0  Break Control bit * 0: break signal disable * 1: break signal enable (logic '0') */
#define RUART_BIT_STKP                    ((u32)0x00000001 << 5)          /*!<R/W 0x0  Stick Parity bit * 0: Not stick parity, the parity type is ODD or EVEN (depending on eps) * 1: Stick parity **eps=1: low parity (1'b0) **eps=0: high parity (1'b1) */
#define RUART_BIT_EPS                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  Even Parity selection * 0: Odd parity, the number of Logic '1' is odd * 1: Even parity, the number of Logic '1' is even */
#define RUART_BIT_PEN                     ((u32)0x00000001 << 3)          /*!<R/W 0x0  Parity Enable * 0: No parity bit * 1: Parity bit is generated on each outgoing character and is checked on each incoming one */
#define RUART_BIT_STB                     ((u32)0x00000001 << 2)          /*!<R/W 0x0  Stop bits length selection * 0: 1 stop bit * 1: 2 stop bits Note: The receiver always checks the first stop bit only. */
#define RUART_BIT_WLS0                    ((u32)0x00000001 << 0)          /*!<R/W 0x1  Word length selection * 0: 7 bits data * 1: 8 bits data */
/** @} */

/** @defgroup MCR
 * @brief UART modem control register
 * @{
 **/
#define RUART_BIT_AFE                     ((u32)0x00000001 << 5)          /*!<R/W 0x0  Auto flow control enable */
#define RUART_BIT_LOOP_EN                 ((u32)0x00000001 << 4)          /*!<R/W 0x0  LoopBack mode * 0: Normal operation mode * 1: Loopback mode */
#define RUART_BIT_OUT2                    ((u32)0x00000001 << 3)          /*!<R/W 0x0  Auxiliary output 2 Bit[3] affects the OUT2_ output in a manner identical to that described above set to 0 In loopback mode, connected to Data Carrier Detect (DCD). */
#define RUART_BIT_OUT1                    ((u32)0x00000001 << 2)          /*!<R/W 0x0  Auxiliary output 1 Bit3 affects the OUT1_ output in a manner identical to that described above set to 0 In loopback mode, connected to Ring Indicator (RI) */
#define RUART_BIT_RTS                     ((u32)0x00000001 << 1)          /*!<R/W/ES 0x0  Request to Send (RTS) signal control * 0: rts is logic 1. * 1: rts is logic 0. This bit controls the RTS_ output. Bit[1] affects the RTS_ output in a manner identical to that described above set to 0. */
#define RUART_BIT_DTR                     ((u32)0x00000001 << 0)          /*!<R/W 0x0  * 0: dtr is logic 1 * 1: dtr is logic 0 This bit controls the DTR output */
/** @} */

/** @defgroup LSR
 * @brief UART line status register
 * @{
 **/
#define RUART_BIT_RXND_INT                ((u32)0x00000001 << 12)          /*!<R 0  This bit is only set when RX debouncer has detected noise 1: Rx path noise detected interrupt */
#define RUART_BIT_MODEM_INT               ((u32)0x00000001 << 11)          /*!<R 0  This bit is only set when Modem status pending 1: Modem interrupt */
#define RUART_BIT_MONITOR_DONE_INT        ((u32)0x00000001 << 10)          /*!<R 0  This bit is only set when Rx Path Monitor Done interrupt is enabled(EMDI is set) 1: Rx path monitor done */
#define RUART_BIT_TIMEOUT_INT             ((u32)0x00000001 << 9)          /*!<R 0  This bit is only set when Rx Timeout interrupt is enabled(ETOI is set) 1: at least one character in the FIFO but no character has been input to the FIFO or reading from it for the time duration, which depends on the value in register REG_RX_PATH_CTRL[31:16] */
#define RUART_BIT_RXFIFO_INT              ((u32)0x00000001 << 8)          /*!<R 0  This bit is only set when Received Data Available Interrupt interrupt is enabled(ERBI is set) 1: FIFO Trigger level reached or Rx FIFO full */
#define RUART_BIT_RXFIFO_ERR              ((u32)0x00000001 << 7)          /*!<R 0  Uart_rx_error, depending on the FIFO control register (FCR) bit[0] * 0: clear when the CPU reads the LSR, if there are no subsequent errors in the FIFO. * 1: parity error, overrun error, framing error or break indication in the FIFO. */
#define RUART_BIT_TX_NOT_FULL             ((u32)0x00000001 << 6)          /*!<R 1  TXFIFO not full indicator */
#define RUART_BIT_TX_EMPTY                ((u32)0x00000001 << 5)          /*!<R 1  TXFIFO empty indicator */
#define RUART_BIT_BREAK_INT               ((u32)0x00000001 << 4)          /*!<R   Break Interrupt (BI) indicator * 0: No break condition in the current character * 1: The received data input held in the Spacing ( logic 0) state is longer than a full word transmission time */
#define RUART_BIT_FRM_ERR                 ((u32)0x00000001 << 3)          /*!<R   Framing Error (FE) indicator * 0: No framing error in the current character * 1: The received character does not have a valid stop bit */
#define RUART_BIT_PAR_ERR                 ((u32)0x00000001 << 2)          /*!<R   Parity Error (PE) indicator * 0: No parity error in current character * 1: The received character does not have the correct parity bit */
#define RUART_BIT_OVR_ERR                 ((u32)0x00000001 << 1)          /*!<R   Overrun Error (OE) indicator * 0: No Overrun state * 1: The transmission is continue although the Rx FIFO is full */
#define RUART_BIT_DRDY                    ((u32)0x00000001 << 0)          /*!<R   * 0: No characters in the Receiver FIFO * 1: At least one character has been received and transferred into the FIFO */
/** @} */

/** @defgroup MSR
 * @brief UART modem status register
 * @{
 **/
#define RUART_BIT_R_DCD                   ((u32)0x00000001 << 7)          /*!<R   Complement of the DCD input or equals to Out2 in loopback mode. */
#define RUART_BIT_R_RI                    ((u32)0x00000001 << 6)          /*!<R   Complement of the RI input or equals to Out1 in loopback mode. */
#define RUART_BIT_R_DSR                   ((u32)0x00000001 << 5)          /*!<R   Complement of the DSR input or equals to DTR in loopback mode. */
#define RUART_BIT_R_CTS                   ((u32)0x00000001 << 4)          /*!<R   Complement of the CTS input or equals to RTS in loopback mode. */
#define RUART_BIT_D_DCD                   ((u32)0x00000001 << 3)          /*!<R   * 0: Otherwise * 1: The DCD line has changed its state */
#define RUART_BIT_TERI                    ((u32)0x00000001 << 2)          /*!<R   * 0: Otherwise * 1: The RI line has changed its state from low to high state */
#define RUART_BIT_D_DSR                   ((u32)0x00000001 << 1)          /*!<R   * 0: Otherwise * 1: The DSR line has changed its state */
#define RUART_BIT_D_CTS                   ((u32)0x00000001 << 0)          /*!<R   * 0: Otherwise * 1: The CTS line has changed its state */
/** @} */

/** @defgroup SCR
 * @brief UART scratch pad register
 * @{
 **/
#define RUART_MASK_XFACTOR_ADJ            ((u32)0x000007FF << 16)          /*!<R/W 0x0  One factor of Baud rate calculation for TX path */
#define RUART_XFACTOR_ADJ(x)              ((u32)(((x) & 0x000007FF) << 16))
#define RUART_GET_XFACTOR_ADJ(x)          ((u32)(((x >> 16) & 0x000007FF)))
#define RUART_MASK_DBG_SEL                ((u32)0x0000001F << 8)          /*!<R/W 0x0  Debug port selection */
#define RUART_DBG_SEL(x)                  ((u32)(((x) & 0x0000001F) << 8))
#define RUART_GET_DBG_SEL(x)              ((u32)(((x >> 8) & 0x0000001F)))
#define RUART_BIT_SCRATCH_7               ((u32)0x00000001 << 7)          /*!<R/W/ES 0x0  clear rx break signal interrupt status */
#define RUART_MASK_SCRATCH_6_0            ((u32)0x0000007F << 0)          /*!<R/W 0x30  *Bit[6]: rx break signal interrupt enable *Bit[5]: fl_set_bi_err *Bit[4]: fl_frame_err *Bit[3]: pin_lb_test *Bit[2:0]: reserved */
#define RUART_SCRATCH_6_0(x)              ((u32)(((x) & 0x0000007F) << 0))
#define RUART_GET_SCRATCH_6_0(x)          ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup STS
 * @brief UART STS register
 * @{
 **/
#define RUART_MASK_XFACTOR                ((u32)0x000FFFFF << 4)          /*!<R/W 0x1047  Factor of Baud rate calculation for Tx path */
#define RUART_XFACTOR(x)                  ((u32)(((x) & 0x000FFFFF) << 4))
#define RUART_GET_XFACTOR(x)              ((u32)(((x >> 4) & 0x000FFFFF)))
/** @} */

/** @defgroup RBR_OR_UART_THR
 * @brief UART receiver buffer register/UART transmitter holding register
 * @{
 **/
#define RUART_MASK_DATABIT                ((u32)0x000000FF << 0)          /*!<R/W 0x0  UART_RBR's Rx data bits or UART_THR's Tx data bit[7:0] Note: Bit[0] is the least significant bit. It is the first bit serially received/transmitted. */
#define RUART_DATABIT(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define RUART_GET_DATABIT(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup MISCR
 * @brief UART MISC control register
 * @{
 **/
#define RUART_BIT_CLR_DUMMY_FLAG          ((u32)0x00000001 << 30)          /*!<W 0x0  Indicate the clearing of dummy data */
#define RUART_MASK_DUMMY_DATA             ((u32)0x000000FF << 22)          /*!<R/W 0x0  Store dummy data, when UART is DMA flow controller, dummy data is padded for last RXDMA burst request. */
#define RUART_DUMMY_DATA(x)               ((u32)(((x) & 0x000000FF) << 22))
#define RUART_GET_DUMMY_DATA(x)           ((u32)(((x >> 22) & 0x000000FF)))
#define RUART_BIT_RXDMA_OWNER             ((u32)0x00000001 << 21)          /*!<R/W 0x0  * 0: GDMA * 1: UART, interrupt enable signal other than erpd should be masked */
#define RUART_BIT_IRDA_RX_INV             ((u32)0x00000001 << 20)          /*!<R/W 0x0  irda_rx_i reverse input enable */
#define RUART_BIT_IRDA_TX_INV             ((u32)0x00000001 << 19)          /*!<R/W 0x0  irda_tx_o reverse output enable */
#define RUART_MASK_RXDMA_BURSTSIZE        ((u32)0x000000FF << 11)          /*!<R/W 0x4  RXDMA burstsize, maximum value is 16. */
#define RUART_RXDMA_BURSTSIZE(x)          ((u32)(((x) & 0x000000FF) << 11))
#define RUART_GET_RXDMA_BURSTSIZE(x)      ((u32)(((x >> 11) & 0x000000FF)))
#define RUART_MASK_TXDMA_BURSTSIZE        ((u32)0x000000FF << 3)          /*!<R/W 0x4  TXDMA burstsize, maximum value is 16. */
#define RUART_TXDMA_BURSTSIZE(x)          ((u32)(((x) & 0x000000FF) << 3))
#define RUART_GET_TXDMA_BURSTSIZE(x)      ((u32)(((x >> 3) & 0x000000FF)))
#define RUART_BIT_RXDMA_EN                ((u32)0x00000001 << 2)          /*!<R/W 0x0  RXDMA enable, valid when dma_mode in FCR is 1'b1 */
#define RUART_BIT_TXDMA_EN                ((u32)0x00000001 << 1)          /*!<R/W 0x0  TXDMA enable, valid when dma_mode in FCR is 1'b1 */
#define RUART_BIT_IRDA_ENABLE             ((u32)0x00000001 << 0)          /*!<R/W 0x0  * 0: UART mode only * 1: UART is co-work with IRDA SIR mode */
/** @} */

/** @defgroup SIR_TX_PWC0
 * @brief UART IRDA SIR TX pulse width control 0 register
 * @{
 **/
#define RUART_BIT_UPPERBOUND_SHIFTRIGHT   ((u32)0x00000001 << 31)          /*!<R/W 0x0  * 0: Shift left, minus offset value of upperbound_shiftval * 1: Shift right, plus offset value of upperbound_shiftval */
#define RUART_MASK_UPPERBOUND_SHIFTVAL    ((u32)0x00007FFF << 16)          /*!<R/W 0x0  The shift value of SIR TX pulse's right edge */
#define RUART_UPPERBOUND_SHIFTVAL(x)      ((u32)(((x) & 0x00007FFF) << 16))
#define RUART_GET_UPPERBOUND_SHIFTVAL(x)  ((u32)(((x >> 16) & 0x00007FFF)))
#define RUART_BIT_LOWBOUND_SHIFTRIGHT     ((u32)0x00000001 << 15)          /*!<R/W 0x0  * 0: Shift left, minus offset value of lowbound_shiftval * 1: Shift right, plus offset value of lowbound_shiftval */
#define RUART_MASK_LOWBOUND_SHIFTVAL      ((u32)0x00007FFF << 0)          /*!<R/W 0x0  The shift value of SIR TX pulse's left edge */
#define RUART_LOWBOUND_SHIFTVAL(x)        ((u32)(((x) & 0x00007FFF) << 0))
#define RUART_GET_LOWBOUND_SHIFTVAL(x)    ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/** @defgroup SIR_RX_PFC
 * @brief UART IRDA SIR Rx pulse filter control register
 * @{
 **/
#define RUART_MASK_R_SIR_RX_FILTER_THRS   ((u32)0x00007FFF << 1)          /*!<R/W 0x0  The threshold of SIR Rx filter, Rx pulse is valid only when Rx pulse width is larger than the threshold. */
#define RUART_R_SIR_RX_FILTER_THRS(x)     ((u32)(((x) & 0x00007FFF) << 1))
#define RUART_GET_R_SIR_RX_FILTER_THRS(x) ((u32)(((x >> 1) & 0x00007FFF)))
#define RUART_BIT_R_SIR_RX_FILTER_EN      ((u32)0x00000001 << 0)          /*!<R/W 0x0  IrDA SIR Rx filter enable */
/** @} */

/** @defgroup BAUD_MON
 * @brief UART baud monitor register
 * @{
 **/
#define RUART_BIT_TOGGLE_MON_EN           ((u32)0x00000001 << 31)          /*!<R/W 0x0  Toggle monitor enable */
#define RUART_BIT_MON_DATA_VLD            ((u32)0x00000001 << 30)          /*!<R   Indicate valid monitoring data */
#define RUART_MASK_FALLING_THRESH         ((u32)0x0000003F << 24)          /*!<R/W 0x7  Indicate the threshold of Rx data rising edge */
#define RUART_FALLING_THRESH(x)           ((u32)(((x) & 0x0000003F) << 24))
#define RUART_GET_FALLING_THRESH(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define RUART_MASK_MIN_LOW_PERIOD         ((u32)0x00000FFF << 12)          /*!<R   Minimum space of consecutive logic '0' of Rx data */
#define RUART_MIN_LOW_PERIOD(x)           ((u32)(((x) & 0x00000FFF) << 12))
#define RUART_GET_MIN_LOW_PERIOD(x)       ((u32)(((x >> 12) & 0x00000FFF)))
#define RUART_MASK_MIN_FALL_SPACE         ((u32)0x00000FFF << 0)          /*!<R   Minimum space of adjacent falling edge of Rx data */
#define RUART_MIN_FALL_SPACE(x)           ((u32)(((x) & 0x00000FFF) << 0))
#define RUART_GET_MIN_FALL_SPACE(x)       ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup DBGR
 * @brief UART debug register
 * @{
 **/
#define RUART_MASK_DBG_UART               ((u32)0xFFFFFFFF << 0)          /*!<R   The output value of debug port, depending on dbg_sel value in SCR[2:0] */
#define RUART_DBG_UART(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RUART_GET_DBG_UART(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup RX_PATH_CTRL
 * @brief UART Rx path control register
 * @{
 **/
#define RUART_MASK_R_RXTO_THRS            ((u32)0x0000FFFF << 16)          /*!<R/W 0x40  Indicate the timeout threshold of Rx path, unit in one-bit time */
#define RUART_R_RXTO_THRS(x)              ((u32)(((x) & 0x0000FFFF) << 16))
#define RUART_GET_R_RXTO_THRS(x)          ((u32)(((x >> 16) & 0x0000FFFF)))
#define RUART_MASK_RXBAUD_ADJ_10_0        ((u32)0x000007FF << 3)          /*!<R/W 0x0  One factor of Baud rate calculation for Rx path, similar with xfactor_adj */
#define RUART_RXBAUD_ADJ_10_0(x)          ((u32)(((x) & 0x000007FF) << 3))
#define RUART_GET_RXBAUD_ADJ_10_0(x)      ((u32)(((x >> 3) & 0x000007FF)))
#define RUART_BIT_R_RST_NEWRX_N           ((u32)0x00000001 << 2)          /*!<R/W 0x0  Reset new Rx path, low active */
/** @} */

/** @defgroup MON_BAUD_CTRL
 * @brief UART monitor baud rate control register
 * @{
 **/
#define RUART_BIT_R_UPD_OSC_IN_XTAL       ((u32)0x00000001 << 29)          /*!<R/W 0x0  Update parameter r_cycnum_perbit_osc (during the update of parameter r_cycnum_perbit_xtal) enable */
#define RUART_MASK_R_CYCNUM_PERBIT_OSC    ((u32)0x000FFFFF << 9)          /*!<R/W/ES 0x0  Indicate the average OSC clock cycle number of one bit, for Rx path OSC clk NOTE: SW sets the initial value, HW updates it depending on the monitor result. */
#define RUART_R_CYCNUM_PERBIT_OSC(x)      ((u32)(((x) & 0x000FFFFF) << 9))
#define RUART_GET_R_CYCNUM_PERBIT_OSC(x)  ((u32)(((x >> 9) & 0x000FFFFF)))
#define RUART_MASK_R_BIT_NUM_THRES        ((u32)0x000000FF << 1)          /*!<R/W 0x0  Indicate the bit number threshold of one monitor period, max value is 127 */
#define RUART_R_BIT_NUM_THRES(x)          ((u32)(((x) & 0x000000FF) << 1))
#define RUART_GET_R_BIT_NUM_THRES(x)      ((u32)(((x >> 1) & 0x000000FF)))
#define RUART_BIT_R_MON_BAUD_EN           ((u32)0x00000001 << 0)          /*!<R/W 0x0  Monitor Rx baud enable */
/** @} */

/** @defgroup MON_BAUD_STS
 * @brief UART monitor baud rate status register
 * @{
 **/
#define RUART_MASK_RO_MON_TOTAL_BIT       ((u32)0x000000FF << 21)          /*!<R 0x0  Indicate the number of bits actually monitored */
#define RUART_RO_MON_TOTAL_BIT(x)         ((u32)(((x) & 0x000000FF) << 21))
#define RUART_GET_RO_MON_TOTAL_BIT(x)     ((u32)(((x >> 21) & 0x000000FF)))
#define RUART_BIT_RO_MON_RDY              ((u32)0x00000001 << 20)          /*!<R 0x0  Indicate the finish of calculation of actual cycle number per bit, it's cleared when r_mon_baud_en is 0 */
#define RUART_MASK_R_CYCNUM_PERBIT_XTAL   ((u32)0x000FFFFF << 0)          /*!<R/W/ES 0x0  Indicate the fractional xtal clock cycle number of one bit, for Rx path xtal clk NOTE: SW sets the initial value, HW updates it depending on the monitor result. */
#define RUART_R_CYCNUM_PERBIT_XTAL(x)     ((u32)(((x) & 0x000FFFFF) << 0))
#define RUART_GET_R_CYCNUM_PERBIT_XTAL(x) ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/** @defgroup MON_CYC_NUM
 * @brief UART monitored cycle number register
 * @{
 **/
#define RUART_MASK_RO_MON_TOTAL_CYCLE     ((u32)0x0FFFFFFF << 0)          /*!<R   Indicate the number of clock cycle actually monitored */
#define RUART_RO_MON_TOTAL_CYCLE(x)       ((u32)(((x) & 0x0FFFFFFF) << 0))
#define RUART_GET_RO_MON_TOTAL_CYCLE(x)   ((u32)(((x >> 0) & 0x0FFFFFFF)))
/** @} */

/** @defgroup RX_BYTE_CNT
 * @brief UART Rx data byte count register
 * @{
 **/
#define RUART_BIT_CLR_RX_BYTE_CNT         ((u32)0x00000001 << 16)          /*!<W   Write 1 to clear ro_rx_byte_cnt */
#define RUART_MASK_RO_RX_BYTE_CNT         ((u32)0x0000FFFF << 0)          /*!<R   Count the byte number of data read from rxfifo */
#define RUART_RO_RX_BYTE_CNT(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define RUART_GET_RO_RX_BYTE_CNT(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup FCR
 * @brief UART FIFO control register
 * @{
 **/
#define RUART_MASK_RECVTRG                ((u32)0x00000003 << 6)          /*!<R/W 0x3  Define the 16-entries Receiver FIFO Interrupt trigger level (0~15 bytes) * 00: 1 byte * 01: 16 bytes (FIFO_DEPTH/4) * 10: 32 bytes (FIFO_DEPTH/2) * 11: 62 bytes (FIFO_DEPTH-2) */
#define RUART_RECVTRG(x)                  ((u32)(((x) & 0x00000003) << 6))
#define RUART_GET_RECVTRG(x)              ((u32)(((x >> 6) & 0x00000003)))
#define RUART_BIT_DMA_MODE                ((u32)0x00000001 << 3)          /*!<R/W 0x0  Support dma mode (cooperate with DW DDMA in the data path) */
#define RUART_BIT_XMIT_CLR                ((u32)0x00000001 << 2)          /*!<W 0x0  Clear xmitckt */
#define RUART_BIT_RECV_CLR                ((u32)0x00000001 << 1)          /*!<W 0x0  Clear receiving */
#define RUART_BIT_FIFO_EN                 ((u32)0x00000001 << 0)          /*!<R/W 0x1  Error report of RCVR FIFO field enable, only valid when the LSR bit[7] is set to 1. */
/** @} */

/** @defgroup ICR
 * @brief UART interrupt clear register
 * @{
 **/
#define RUART_BIT_RXNDICF                 ((u32)0x00000001 << 4)          /*!<W 0x0  Rx noise Detected Interrupt Flag */
#define RUART_BIT_MDICF                   ((u32)0x00000001 << 3)          /*!<W 0x0  Monitor Done Interrupt Flag */
#define RUART_BIT_MICF                    ((u32)0x00000001 << 2)          /*!<W 0x0  Modem Interrupt Clear Flag */
#define RUART_BIT_TOICF                   ((u32)0x00000001 << 1)          /*!<W 0x0  Timeout Interrupt Clear Flag */
#define RUART_BIT_RLSICF                  ((u32)0x00000001 << 0)          /*!<W 0x0  Receiver Line Statue Interrupt Clear Flag Writing 1 to this bit clears PARITY_ERR, OVERRUN_ERR, FRAMING_ERR, BREAK_ERR_INT and RXFIFO_ERR in the USART_LSR register. */
/** @} */

/** @defgroup RXDBCR
 * @brief UART RX debounce control register
 * @{
 **/
#define RUART_MASK_DBNC_CYC               ((u32)0x00007FFF << 1)          /*!<R/W 0xF  Debounce cycles in unit of sclk */
#define RUART_DBNC_CYC(x)                 ((u32)(((x) & 0x00007FFF) << 1))
#define RUART_GET_DBNC_CYC(x)             ((u32)(((x >> 1) & 0x00007FFF)))
#define RUART_BIT_DBNC_FEN                ((u32)0x00000001 << 0)          /*!<R/W 0x0  UART RX debounce enable */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup UART_Exported_Types UART Exported Types
  * @{
*****************************************************************************/


/**
 * @brief UART Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t DLL;                                    /*!< UART DIVISOR LENGTH REGISTER,  Address offset: 0x000 */
	__IO uint32_t IER;                                    /*!< UART INTERRUPT ENABLE REGISTER,  Address offset: 0x004 */
	__I  uint32_t IIR;                                    /*!< UART INTERRUPT IDENTIFICATION REGISTER,  Address offset: 0x008 */
	__IO uint32_t LCR;                                    /*!< UART LINE CONTROL REGISTER,  Address offset: 0x00C */
	__IO uint32_t MCR;                                    /*!< UART MODEM CONTROL REGISTER,  Address offset: 0x010 */
	__I  uint32_t LSR;                                    /*!< UART LINE STATUS REGISTER,  Address offset: 0x014 */
	__I  uint32_t MSR;                                    /*!< UART MODEM STATUS REGISTER,  Address offset: 0x018 */
	__IO uint32_t SCR;                                    /*!< UART SCRATCH PAD REGISTER,  Address offset: 0x01C */
	__IO uint32_t STS;                                    /*!< UART STS REGISTER,  Address offset: 0x020 */
	__IO uint32_t RBR_OR_UART_THR;                        /*!< UART RECEIVER BUFFER REGISTER/UART TRANSMITTER HOLDING REGISTER,  Address offset: 0x024 */
	__IO uint32_t MISCR;                                  /*!< UART MISC CONTROL REGISTER,  Address offset: 0x028 */
	__IO uint32_t SIR_TX_PWC0;                            /*!< UART IRDA SIR TX PULSE WIDTH CONTROL 0 REGISTER,  Address offset: 0x02C */
	__IO uint32_t SIR_RX_PFC;                             /*!< UART IRDA SIR RX PULSE FILTER CONTROL REGISTER,  Address offset: 0x030 */
	__IO uint32_t BAUD_MON;                               /*!< UART BAUD MONITOR REGISTER,  Address offset: 0x034 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x38 */
	__I  uint32_t DBGR;                                   /*!< UART DEBUG REGISTER,  Address offset: 0x03C */
	__IO uint32_t RX_PATH_CTRL;                           /*!< UART RX PATH CONTROL REGISTER,  Address offset: 0x040 */
	__IO uint32_t MON_BAUD_CTRL;                          /*!< UART MONITOR BAUD RATE CONTROL REGISTER,  Address offset: 0x044 */
	__IO uint32_t MON_BAUD_STS;                           /*!< UART MONITOR BAUD RATE STATUS REGISTER,  Address offset: 0x048 */
	__I  uint32_t MON_CYC_NUM;                            /*!< UART MONITORED CYCLE NUMBER REGISTER,  Address offset: 0x04C */
	__IO uint32_t RX_BYTE_CNT;                            /*!< UART RX DATA BYTE COUNT REGISTER,  Address offset: 0x050 */
	__IO uint32_t FCR;                                    /*!< UART FIFO CONTROL REGISTER,  Address offset: 0x054 */
	__O  uint32_t ICR;                                    /*!< UART INTERRUPT CLEAR REGISTER,  Address offset: 0x058 */
	__IO uint32_t RXDBCR;                                 /*!< UART RX DEBOUNCE CONTROL REGISTER,  Address offset: 0x05C */
} UART_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup UART_Exported_Types UART Exported Types
  * @{
  */

/**
  * @brief  UART Init Structure Definition
  */
typedef struct {
	u32 DmaModeCtrl;	/*!< Specifies the uart DMA mode state.
							This parameter can be ENABLE or DISABLE. */

	u32 WordLen;		/*!< Specifies the UART word length.
							This parameter can be a value of @ref UART_Word_Length. */

	u32 StopBit;		/*!< Specifies the UART stop bit number.
							This parameter can be a value of @ref UART_Stop_Bit. */

	u32 Parity;			/*!< Specifies the UART parity.
							This parameter can be a value of @ref UART_Parity_Enable. */

	u32 ParityType;		/*!< Specifies the UART parity type.
							This parameter can be a value of @ref UART_Parity_Type. */

	u32 StickParity;	/*!< Specifies the UART stick parity.
							This parameter can be a value of @ref UART_Stick_Parity_Type. */

	u32 FlowControl;	/*!< Specifies the UART auto flow control.
							This parameter can be ENABLE or DISABLE. */

	u32 RxFifoTrigLevel;	/*!< Specifies the UART rx fifo trigger level.
							This parameter can be a value of @ref UART_Rx_FIFO_Trigger_Level. */

	u32 RxErReportCtrl;		/*!< Specifies the UART rx error report control.
							This parameter can be a value of @ref UART_Rx_Err_Report. */

	u32 RxTimeOutCnt;		/*!< Specifies the UART rx time out counter.
							This parameter can be a number between 0x00 and 0xffff. */
} UART_InitTypeDef;

/**
 * @brief UART IRDA Init Structure Definition
 */
typedef struct {
	u32 UART_IrDARxInv;		/*!< Specifies the uart irda rx invert control.
								This parameter can be ENABLE or DISABLE.
								ENABLE: invert the irda input signal.
								DISABLE: does't invert the irda input signal.
								@note This parameter is only used in IrDA mode. */

	u32 UART_IrDATxInv;		/*!< Specifies the uart irda tx invert control.
								This parameter can be ENABLE or DISABLE.
								ENABLE: invert the irda output signal.
								DISABLE: does't invert the irda output signal.
								@note This parameter is only used in IrDA mode. */

	u32 UART_UpperShift;	/*!< Specifies the uart irda tx pulse right edge shift direction.
								This parameter can be a value of @ref UART_IRDA_Pulse_Shift. */

	u32 UART_UpperShiftVal;	/*!< Specifies the uart irda tx pulse right edge shift value in the given direction.
								This parameter can be a number between 0x0000 and 0x7fff. */

	u32 UART_LowShift;		/*!< Specifies the uart irda tx pulse left edge shift direction.
								This parameter can be a value of @ref UART_IRDA_Pulse_Shift. */

	u32 UART_LowShiftVal;	/*!< Specifies the uart irda tx pulse left edge shift value in the given direction.
								This parameter can be a number between 0x0000 and 0x7fff. */

	u32 UART_RxFilterThres;	/*!< Specifies the uart irda rx filter threshold.
								This parameter can be a number between 0x0000 and 0x7fff
								@note This parameter is only used in IrDA mode. */

	u32 UART_RxFilterCmd;	/*!< Specifies the uart irda rx filter control.
								This parameter can be ENABLE or DISABLE.
								ENABLE: uart IrDA rx filter is used.
								DISABLE: uart IrDA rx filter is not used.
								@note This parameter is only used in IrDA mode. */
} IrDA_InitTypeDef;

/**
 * @brief UART CFG Structure Definition
 */
typedef struct {
	u32 LOW_POWER_RX_ENABLE;	/*Enable low power RX*/
} UARTCFG_TypeDef;

/**
 * @brief UART Dev Table Structure Definition
 */
typedef struct {
	UART_TypeDef *UARTx;
	u32 Tx_HandshakeInterface;
	u32 Rx_HandshakeInterface;
	IRQn_Type	IrqNum;
} UART_DevTable;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup UART_Exported_Constants UART Exported Constants
  * @{
  */

/** @defgroup UART_IRDA_Pulse_Shift
  * @{
  */
#define UART_IRDA_PULSE_LEFT_SHIFT		((u32)0x00000000)
#define UART_IRDA_PULSE_RIGHT_SHIFT		((u32)0x00000001)
#define IS_IRDA_PUL_SHIFT(SHIFT) (((SHIFT) == UART_IRDA_PULSE_LEFT_SHIFT) || \
									((SHIFT) == UART_IRDA_PULSE_RIGHT_SHIFT))
/**
  * @}
  */

/** @defgroup UART_Rx_FIFO_Trigger_Level
  * @{
  */
#define UART_RX_FIFOTRIG_LEVEL_1BYTES		((u32)0x00000000)
#define UART_RX_FIFOTRIG_LEVEL_16BYTES		((u32)0x00000040)
#define UART_RX_FIFOTRIG_LEVEL_32BYTES		((u32)0x00000080)
#define UART_RX_FIFOTRIG_LEVEL_62BYTES		((u32)0x000000c0)

#define IS_UART_RXFIFO_LEVEL(LEVEL) (((LEVEL) == UART_RX_FIFOTRIG_LEVEL_1BYTES) || \
								((LEVEL) == UART_RX_FIFOTRIG_LEVEL_16BYTES) || \
								((LEVEL) == UART_RX_FIFOTRIG_LEVEL_32BYTES) || \
								((LEVEL) == UART_RX_FIFOTRIG_LEVEL_62BYTES))
/**
  * @}
  */

/** @defgroup UART_Word_Length
  * @{
  */

#define RUART_WLS_7BITS			((u32)0x00000000)
#define RUART_WLS_8BITS			((u32)0x00000001)

#define IS_UART_WLS(VAL) (((VAL) == RUART_WLS_7BITS) || \
								((VAL) == RUART_WLS_8BITS))
/**
  * @}
  */

/** @defgroup UART_Stop_Bit
  * @{
  */
#define RUART_STOP_BIT_1		((u32)0x00000000)
#define RUART_STOP_BIT_2		((u32)0x00000001)

#define IS_UART_STOP_BIT(VAL) (((VAL) == RUART_STOP_BIT_1) || \
								((VAL) == RUART_STOP_BIT_2))
/**
  * @}
  */

/** @defgroup UART_Parity_Enable
  * @{
  */
#define RUART_PARITY_DISABLE	((u32)0x00000000)
#define RUART_PARITY_ENABLE		((u32)0x00000001)

#define IS_UART_PARITY_ENABLE(VAL) (((VAL) == RUART_PARITY_DISABLE) || \
								((VAL) == RUART_PARITY_ENABLE))
/**
  * @}
  */

/** @defgroup UART_Parity_Type
  * @{
  */
#define RUART_ODD_PARITY		((u32)0x00000000)
#define RUART_EVEN_PARITY		((u32)0x00000001)

#define IS_UART_PARITY_TYPE(VAL) (((VAL) == RUART_ODD_PARITY) || \
								((VAL) == RUART_EVEN_PARITY))
/**
  * @}
  */

/** @defgroup UART_Stick_Parity_Type
  * @{
  */
#define RUART_STICK_PARITY_DISABLE		((u32)0x00000000)
#define RUART_STICK_PARITY_ENABLE		((u32)0x00000001)

#define IS_UART_STICK_PARITY_ENABLE(VAL) (((VAL) == RUART_STICK_PARITY_DISABLE) || \
								((VAL) == RUART_STICK_PARITY_ENABLE))
/**
  * @}
  */

/** @defgroup UART_Interrupt_ID
  * @{
  */
#define RUART_MODEM_STATUS					((u32)0x00000000)
#define RUART_TX_FIFO_EMPTY					((u32)0x00000001)
#define RUART_RECEIVER_DATA_AVAILABLE		((u32)0x00000002)
#define RUART_RECEIVE_LINE_STATUS			((u32)0x00000003)
#define RUART_LP_RX_MONITOR_DONE			((u32)0x00000004)
#define RUART_TIME_OUT_INDICATION			((u32)0x00000006)
/**
  * @}
  */

/** @defgroup UART_Rx_Err_Type
  * @{
  */
#define UART_ALL_RX_ERR		(RUART_BIT_RXFIFO_ERR | \
							RUART_BIT_BREAK_INT | \
							RUART_BIT_FRM_ERR | \
							RUART_BIT_PAR_ERR | \
							RUART_BIT_OVR_ERR)

/**
  * @}
  */

/** @defgroup UART_Rx_Err_Report
  * @{
  */
#define UART_RX_EEROR_REPORT_DISABLE		((u32)0x00000000)
#define UART_RX_EEROR_REPORT_ENABLE			((u32)0x00000001)

#define IS_UART_RX_ERROR_REPORT(REPORT)  (((REPORT) == UART_RX_EEROR_REPORT_DISABLE) || \
							((REPORT) == UART_RX_EEROR_REPORT_ENABLE) )
/**
  * @}
  */

/** @defgroup UART_SoftWare_Status
  * @{
  */
#define STATETX_DMA			1
#define STATETX_INT			2
#define STATETX_POLL		3
#define STATERX_DMA			1
#define STATERX_INT			2
#define STATERX_POLL		3
/**
  * @}
  */

/**
 * @defgroup UART_MAUTO_FLOW_CONTROL_define
 * @{
  */
#define RUART_MCL_FLOW_ENABLE			((u32)(RUART_BIT_AFE | RUART_BIT_RTS)) /*BIT[1],BIT[5],Uart auto flow control enable bit*/
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup UART_Exported_Functions UART Exported Functions
  * @{
  */

/** @defgroup UART_Normal_Functions UART Normal Functions
  * @{
  */
_LONG_CALL_ void UART_DeInit(UART_TypeDef *UARTx);
_LONG_CALL_ void UART_StructInit(UART_InitTypeDef *UART_InitStruct);
_LONG_CALL_ void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
_LONG_CALL_ u32 UART_BaudParaGet(u32 baudrate, u32 *ovsr, u32 *ovsr_adj);
_LONG_CALL_ void UART_BaudParaGetFull(u32 IPclk, u32 baudrate, u32 *ovsr, u32 *ovsr_adj);
_LONG_CALL_ void UART_SetBaudExt(UART_TypeDef *UARTx, u32 Ovsr, u32 Ovsr_adj);
_LONG_CALL_ void UART_SetBaud(UART_TypeDef *UARTx, u32 BaudRate);
_LONG_CALL_ void UART_SetRxLevel(UART_TypeDef *UARTx, u32 FifoLv);
_LONG_CALL_ void UART_RxCmd(UART_TypeDef *UARTx, u32 NewState);
_LONG_CALL_ u32 UART_Writable(UART_TypeDef *UARTx);
_LONG_CALL_ u32 UART_Readable(UART_TypeDef *UARTx);
_LONG_CALL_ void UART_CharPut(UART_TypeDef *UARTx, u8 TxData);
_LONG_CALL_ void UART_CharGet(UART_TypeDef *UARTx, u8  *pRxByte);
_LONG_CALL_ void UART_ReceiveData(UART_TypeDef *UARTx, u8 *OutBuf, u32 Count);
_LONG_CALL_ void UART_SendData(UART_TypeDef *UARTx, u8 *InBuf, u32 Count);
_LONG_CALL_ u32 UART_ReceiveDataTO(UART_TypeDef *UARTx, u8 *OutBuf, u32 Count, u32 Times);
_LONG_CALL_ u32 UART_SendDataTO(UART_TypeDef *UARTx, u8 *InBuf, u32 Count, u32 Times);
_LONG_CALL_ void UART_RxByteCntClear(UART_TypeDef *UARTx);
_LONG_CALL_ u32 UART_RxByteCntGet(UART_TypeDef *UARTx);
_LONG_CALL_ void UART_BreakCtl(UART_TypeDef *UARTx, u32 NewState);
_LONG_CALL_ u32 UART_ClearRxFifo(UART_TypeDef *UARTx);
_LONG_CALL_ void UART_ClearTxFifo(UART_TypeDef *UARTx);
_LONG_CALL_ void UART_INTConfig(UART_TypeDef *UARTx, u32 UART_IT, u32 newState);
_LONG_CALL_ void UART_INT_Clear(UART_TypeDef *UARTx, u32 UART_IT);
_LONG_CALL_ u32 UART_IntStatus(UART_TypeDef *UARTx);
_LONG_CALL_ u32 UART_ModemStatusGet(UART_TypeDef *UARTx);
_LONG_CALL_ u32 UART_LineStatusGet(UART_TypeDef *UARTx);
_LONG_CALL_ void UART_WaitBusy(UART_TypeDef *UARTx, u32 PollTimes);
/**
  * @}
  */

/** @defgroup UART_DMA_functions UART DMA Functions
  * @{
  */
_LONG_CALL_ void UART_TXDMAConfig(UART_TypeDef *UARTx, u32 TxDmaBurstSize);
_LONG_CALL_ void UART_RXDMAConfig(UART_TypeDef *UARTx, u32 RxDmaBurstSize);
_LONG_CALL_ void UART_TXDMACmd(UART_TypeDef *UARTx, u32 NewState);
_LONG_CALL_ void UART_RXDMACmd(UART_TypeDef *UARTx, u32 NewState);
_LONG_CALL_ BOOL UART_TXGDMA_Init(u8 UartIndex, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
								  IRQ_FUN CallbackFunc, u8 *pTxBuf, int TxCount);
_LONG_CALL_ BOOL UART_RXGDMA_Init(u8 UartIndex, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
								  IRQ_FUN CallbackFunc, u8 *pRxBuf, int RxCount);
/**
  * @}
  */

/** @defgroup UART_Low_Power_functions UART Low Power Functions
  * @{
  */
_LONG_CALL_ void UART_MonitorParaConfig(UART_TypeDef *UARTx, u32 BitNumThres, u32 OscPerbitUpdCtrl);
_LONG_CALL_ void UART_LPRxBaudSet(UART_TypeDef *UARTx, u32 BaudRate, u32 RxIPClockHz);
_LONG_CALL_ void UART_RxMonitorCmd(UART_TypeDef *UARTx, u32 NewState);
_LONG_CALL_ u32 UART_RxMonBaudCtrlRegGet(UART_TypeDef *UARTx);
_LONG_CALL_ u32 UART_RxMonitorSatusGet(UART_TypeDef *UARTx);
/**
  * @}
  */

/** @defgroup UART_IRDA_functions UART IRDA Functions
  * @{
  */
_LONG_CALL_ void UART_IrDAStructInit(IrDA_InitTypeDef *IrDA_InitStruct);
_LONG_CALL_ void UART_IrDAInit(UART_TypeDef *UARTx, IrDA_InitTypeDef *IrDA_InitStruct);
_LONG_CALL_ void UART_IrDACmd(UART_TypeDef *UARTx, u32 NewState);
/**
  * @}
  */

/** @defgroup UART_External_functions UART External Functions
  * @{
  */
_LONG_CALL_ void UART_RxDebounceConfig(UART_TypeDef *UARTx, u32 RxDebounceCycle);
_LONG_CALL_ void UART_RxDebounceCmd(UART_TypeDef *UARTx, u32 NewState);
/**
  * @}
  */

/**
  * @}
  */

/** @} */

/** @} */

/* Other Definitions --------------------------------------------------------*/
#define MAX_UART_INDEX			(4)

extern const UART_DevTable UART_DEV_TABLE[MAX_UART_INDEX];
extern const u32 APBPeriph_UARTx[MAX_UART_INDEX];
extern const u32 APBPeriph_UARTx_CLOCK[MAX_UART_INDEX];
extern u32 UART_StateTx[MAX_UART_INDEX];
extern u32 UART_StateRx[MAX_UART_INDEX];

static inline void
UART_SetTxFlag(u32 UartIdx, u32 Flag)
{
	UART_StateTx[UartIdx] = Flag;
}

static inline void
UART_SetRxFlag(u32 UartIdx, u32 Flag)
{
	UART_StateRx[UartIdx] = Flag;
}

static inline u32
UART_GetTxFlag(u32 UartIdx)
{
	return (UART_StateTx[UartIdx]);
}

static inline u32
UART_GetRxFlag(u32 UartIdx)
{
	return (UART_StateRx[UartIdx]);
}

/* MANUAL_GEN_END */

#endif

